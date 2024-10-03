/**
 * @brief Micro-ROS node for the CougUV
 * @author Nelson Durrant
 * @date September 2024
 *
 * This node is designed to run on the CougUV, a small underwater vehicle
 * designed for multi-agent applications by the BYU FRoSt lab. The node is
 * responsible for controlling the vehicle's actuators (fins and thruster),
 * reading sensor data (battery voltage, current, leak sensor, and pressure
 * sensor), and communicating with the micro-ROS agent running on a Raspberry
 * Pi.
 *
 * Subscribes:
 * -
 * Publishes:
 * -
 */

#include "battery_pub.h"
#include "leak_pub.h"
#include "pressure_pub.h"

#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <frost_interfaces/msg/u_command.h>

#define ENABLE_SERVOS
#define ENABLE_THRUSTER
// #define ENABLE_BATTERY
// #define ENABLE_LEAK
#define ENABLE_PRESSURE
#define ENABLE_BT_DEBUG

#define EXECUTE_EVERY_N_MS(MS, X)                                              \
  do {                                                                         \
    static volatile int64_t init = -1;                                         \
    if (init == -1) {                                                          \
      init = uxr_millis();                                                     \
    }                                                                          \
    if (uxr_millis() - init > MS) {                                            \
      X;                                                                       \
      init = uxr_millis();                                                     \
    }                                                                          \
  } while (0)

// micro-ROS config values
#define BAUD_RATE 6000000
#define CALLBACK_TOTAL 1
#define SYNC_TIMEOUT 1000

// hardware pin values
#define BT_MC_RX 34
#define BT_MC_TX 35
#define SERVO_PIN1 9  // top fin
#define SERVO_PIN2 10 // right fin, from front
#define SERVO_PIN3 11 // left fin, from front
#define THRUSTER_PIN 12
#define VOLT_PIN 18
#define CURRENT_PIN 17
#define LEAK_PIN 16
#define LED_PIN 13

// default actuator positions
#define DEFAULT_SERVO 90
#define THRUSTER_OFF 1500

// actuator conversion values
#define SERVO_OUT_HIGH 2500
#define SERVO_OUT_LOW 500
#define THRUSTER_OUT_HIGH 2000
#define THRUSTER_OUT_LOW 1000
#define THRUSTER_IN_HIGH 100
#define THRUSTER_IN_LOW -100

// sensor baud rates
#define BT_DEBUG_RATE 9600
#define I2C_RATE 400000

// sensor update rates
#define BATTERY_MS 1000 // arbitrary
#define LEAK_MS 1000    // arbitrary
#define PRESSURE_MS 20  // fastest update speed is 50 Hz

// time of last received command (used as a fail safe)
unsigned long last_received = 0;

// micro-ROS objects
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rclc_executor_t executor;

// message objects
frost_interfaces__msg__UCommand command_msg;

// subscriber objects
rcl_subscription_t command_sub;

// publisher objects
BatteryPub battery_pub;
LeakPub leak_pub;
PressurePub pressure_pub;

// sensor objects
SoftwareSerial BTSerial(BT_MC_RX, BT_MC_TX);
MS5837 myPressure;

// actuator objects
Servo myServo1; // top fin
Servo myServo2; // right fin, from front
Servo myServo3; // left fin, from front
Servo myThruster;

// states for state machine in loop function
enum states {
  WAITING_AGENT,
  AGENT_AVAILABLE,
  AGENT_CONNECTED,
  AGENT_DISCONNECTED
} static state;

void error_loop() {
  while (1) {
    delay(100);

#ifdef ENABLE_BT_DEBUG
    BTSerial.println("[ERROR] In error loop");
#endif
  }
}

/**
 * @brief Callback function for the control_command subscriber
 *
 * This function is called whenever a new control command is received from the
 * micro-ROS agent. The function updates the actuator positions based on the
 * received command.
 *
 * @param command_msgin The received control command
 */
void command_sub_callback(const void *command_msgin) {

  last_received = millis();

  const frost_interfaces__msg__UCommand *command_msg =
      (const frost_interfaces__msg__UCommand *)command_msgin;

#ifdef ENABLE_SERVOS
  myServo1.write(command_msg->fin[0] + DEFAULT_SERVO); // top fin
  myServo2.write(command_msg->fin[1] + DEFAULT_SERVO); // right fin, from front
  myServo3.write(command_msg->fin[2] + DEFAULT_SERVO); // left fin, from front
#endif

#ifdef ENABLE_THRUSTER
  int converted = map(command_msg->thruster, THRUSTER_IN_LOW, THRUSTER_IN_HIGH,
                      THRUSTER_OUT_LOW, THRUSTER_OUT_HIGH);
  myThruster.writeMicroseconds(converted);
#endif

#ifdef ENABLE_BT_DEBUG
  BTSerial.println("[ALERT] Command Received: " + String(command_msg->fin[0]) +
                   " " + String(command_msg->fin[1]) + " " +
                   String(command_msg->fin[2]) + " " +
                   String(command_msg->thruster));
#endif
}

/**
 * @brief Create micro-ROS entities
 *
 * This function initializes the micro-ROS entities (node, publishers,
 * subscribers, and executor) and synchronizes the timestamps with the Raspberry
 * Pi.
 *
 * @return true if the entities were created successfully, false otherwise
 */
bool create_entities() {

  // the allocator object wraps the dynamic memory allocation and deallocation
  // methods used in micro-ROS
  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  RCCHECK(
      rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

  // synchronize timestamps with the Raspberry Pi
  // after sync, timing should be able to be accessed with "rmw_uros_epoch"
  // functions
  RCCHECK(rmw_uros_sync_session(SYNC_TIMEOUT));

#ifdef ENABLE_BT_DEBUG
  if (!rmw_uros_epoch_synchronized()) {
    BTSerial.println("[ERROR] Could not synchronize timestamps with agent");
  } else {
    BTSerial.println("[ALERT] Timestamps synchronized with agent");
  }
#endif

  // create publishers
  battery_pub.setup(node);
  leak_pub.setup(node);
  pressure_pub.setup(node);

  // create subscribers
  RCCHECK(rclc_subscription_init_default(
      &command_sub, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, UCommand),
      "control_command"));

  // create executor
  RCSOFTCHECK(rclc_executor_init(&executor, &support.context, CALLBACK_TOTAL,
                                 &allocator));

  // add callbacks to executor
  RCSOFTCHECK(
      rclc_executor_add_subscription(&executor, &command_sub, &command_msg,
                                     &command_sub_callback, ON_NEW_DATA));

#ifdef ENABLE_BT_DEBUG
  BTSerial.println("[ALERT] Micro-ROS entities created successfully");
#endif

  return true;
}

/**
 * @brief Destroy micro-ROS entities
 *
 * This function destroys the micro-ROS entities (node, publishers, subscribers,
 * and executor) and sets the context entity destroy session timeout to 0.
 */
void destroy_entities() {
  rmw_context_t *rmw_context = rcl_context_get_rmw_context(&support.context);
  (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

  // destroy publishers
  battery_pub.destroy(node);
  leak_pub.destroy(node);
  pressure_pub.destroy(node);

  // destroy everything else
  if (rcl_subscription_fini(&command_sub, &node) != RCL_RET_OK) {
#ifdef ENABLE_BT_DEBUG
    BTSerial.println("[ERROR] Failed to destroy command_sub");
#endif
  }
  rclc_executor_fini(&executor);
  if (rcl_node_fini(&node) != RCL_RET_OK) {
#ifdef ENABLE_BT_DEBUG
    BTSerial.println("[ERROR] Failed to destroy node");
#endif
  }
  rclc_support_fini(&support);

#ifdef ENABLE_BT_DEBUG
  BTSerial.println("[ALERT] Micro-ROS entities destroyed successfully");
#endif
}

/**
 * @brief Set up the micro-ROS serial transports
 *
 * This function sets up the micro-ROS serial transports for communication with
 * the Raspberry Pi.
 */
void setup() {

  Serial.begin(BAUD_RATE);
  set_microros_serial_transports(Serial);

  // set up the indicator light
  pinMode(LED_PIN, OUTPUT);

  // set up the I2C
  Wire.begin();
  Wire.setClock(I2C_RATE);

#ifdef ENABLE_BT_DEBUG
  BTSerial.begin(BT_DEBUG_RATE);
#endif

#ifdef ENABLE_SERVOS
  pinMode(SERVO_PIN1, OUTPUT);
  pinMode(SERVO_PIN2, OUTPUT);
  pinMode(SERVO_PIN3, OUTPUT);

  myServo1.attach(SERVO_PIN1, SERVO_OUT_LOW, SERVO_OUT_HIGH);
  myServo2.attach(SERVO_PIN2, SERVO_OUT_LOW, SERVO_OUT_HIGH);
  myServo3.attach(SERVO_PIN3, SERVO_OUT_LOW, SERVO_OUT_HIGH);

  myServo1.write(DEFAULT_SERVO);
  myServo2.write(DEFAULT_SERVO);
  myServo3.write(DEFAULT_SERVO);

#ifdef ENABLE_BT_DEBUG
  BTSerial.println("[ALERT] Servos enabled");
#endif
#endif

#ifdef ENABLE_THRUSTER
  pinMode(THRUSTER_PIN, OUTPUT);
  myThruster.attach(THRUSTER_PIN);
  myThruster.writeMicroseconds(THRUSTER_OFF);
  delay(7000);

#ifdef ENABLE_BT_DEBUG
  BTSerial.println("[ALERT] Thruster enabled");
#endif
#endif

#ifdef ENABLE_BATTERY
  pinMode(CURRENT_PIN, INPUT);
  pinMode(VOLT_PIN, INPUT);

#ifdef ENABLE_BT_DEBUG
  BTSerial.println("[ALERT] Battery Sensor enabled");
#endif
#endif

#ifdef ENABLE_LEAK
  pinMode(LEAK_PIN, INPUT);

#ifdef ENABLE_BT_DEBUG
  BTSerial.println("[ALERT] Leak Sensor enabled");
#endif
#endif

#ifdef ENABLE_PRESSURE
  while (!myPressure.init()) {

#ifdef ENABLE_BT_DEBUG
    BTSerial.println("[ERROR] Could not connect to Pressure Sensor over I2C");
#endif

    delay(1000);
  }

#ifdef ENABLE_BT_DEBUG
  BTSerial.println("[ALERT] Pressure Sensor enabled");
#endif
#endif

  state = WAITING_AGENT;
}

/**
 * @brief Read the battery sensor data
 * 
 * This function reads the battery sensor data (voltage and current) and
 * publishes it to the micro-ROS agent.
 */
void read_battery() {

  // we did some testing to determine the below params, but
  // it's possible they are not completely accurate
  float voltage = (analogRead(VOLT_PIN) * 0.03437) + 0.68;
  float current = (analogRead(CURRENT_PIN) * 0.122) - 11.95;

  // publish the battery data
  battery_pub.publish(voltage, current);
}

/**
 * @brief Read the leak sensor data
 * 
 * This function reads the leak sensor data and publishes it to the micro-ROS
 * agent.
 */
void read_leak() {

  bool leak = digitalRead(LEAK_PIN);

  // publish the leak data
  leak_pub.publish(leak);
}

/**
 * @brief Read the pressure sensor data
 * 
 * This function reads the pressure sensor data and publishes it to the micro-ROS
 * agent.
 */
void read_pressure() {

  myPressure.read();
  float pressure = myPressure.pressure();

  // publish the pressure data
  pressure_pub.publish(pressure);
}

/**
 * @brief Main loop function
 *
 * This function is the main loop for the micro-ROS node. It manages the
 * connection and disconnection of the micro-ROS agent, actuator positions,
 * and sensor data collection.
 */
void loop() {

  // blink the indicator light
  if (millis() % 1000 < 250) {
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, HIGH);
  }

  // fail safe for agent disconnect
  if (millis() - last_received > 5000) {

#ifdef ENABLE_SERVOS
    myServo1.write(DEFAULT_SERVO);
    myServo2.write(DEFAULT_SERVO);
    myServo3.write(DEFAULT_SERVO);
#endif

#ifdef ENABLE_THRUSTER
    myThruster.writeMicroseconds(THRUSTER_OFF);
#endif

#ifdef ENABLE_BT_DEBUG
    BTSerial.println(
        "[ALERT] No command received in timeout, stopping actuators");
#endif
  }

  // state machine to manage connecting and disconnecting the micro-ROS agent
  switch (state) {
  case WAITING_AGENT:
    EXECUTE_EVERY_N_MS(500, state = (RMW_RET_OK == rmw_uros_ping_agent(100, 1))
                                        ? AGENT_AVAILABLE
                                        : WAITING_AGENT;);
    break;

  case AGENT_AVAILABLE:
    state = (true == create_entities()) ? AGENT_CONNECTED : WAITING_AGENT;
    if (state == WAITING_AGENT) {
      destroy_entities();
    };
    break;

  case AGENT_CONNECTED:
    EXECUTE_EVERY_N_MS(200, state = (RMW_RET_OK == rmw_uros_ping_agent(100, 1))
                                        ? AGENT_CONNECTED
                                        : AGENT_DISCONNECTED;);
    if (state == AGENT_CONNECTED) {

      //////////////////////////////////////////////////////////
      // EXECUTES WHEN THE AGENT IS CONNECTED
      //////////////////////////////////////////////////////////

#ifdef ENABLE_BATTERY
      EXECUTE_EVERY_N_MS(BATTERY_MS, read_battery());
#endif

#ifdef ENABLE_LEAK
      EXECUTE_EVERY_N_MS(LEAK_MS, read_leak());
#endif

#ifdef ENABLE_PRESSURE
      EXECUTE_EVERY_N_MS(PRESSURE_MS, read_pressure());
#endif

      rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));

      //////////////////////////////////////////////////////////
    }
    break;

  case AGENT_DISCONNECTED:
    destroy_entities();
    state = WAITING_AGENT;
    break;

  default:
    break;
  }
}
