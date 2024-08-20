#include "pressure_pub.h"

#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <frost_interfaces/msg/u_command.h>

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
#define SERVO_PIN1 9 // top fin
#define SERVO_PIN2 10 // right fin, from front
#define SERVO_PIN3 11 // left fin, from front
#define THRUSTER_PIN 12
#define LED_PIN 13

// default actuator positions
#define DEFAULT_SERVO 90
#define THRUSTER_OFF 1500
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
#define PRESSURE_MS 20 // fastest update speed is 50 Hz

// sensor constants
#define FLUID_DENSITY 997 // this shouldn't matter, we calculate depth ourselves

// time of last command (used as a fail safe)
unsigned long last_command_time = 0;

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
  }
}

// micro-ROS function that subscribes to requested command values
void command_sub_callback(const void *command_msgin) {

  // save time of last command
  last_command_time = millis();

  const frost_interfaces__msg__UCommand *command_msg =
      (const frost_interfaces__msg__UCommand *)command_msgin;

  myServo1.write(-command_msg->fin[0] + DEFAULT_SERVO); // top fin
  myServo2.write(-command_msg->fin[1] + DEFAULT_SERVO); // right fin, from front
  myServo3.write(command_msg->fin[2] + DEFAULT_SERVO);  // left fin, from front
  int converted = map(command_msg->thruster, THRUSTER_IN_LOW, THRUSTER_IN_HIGH,
                      THRUSTER_OUT_LOW, THRUSTER_OUT_HIGH);
  myThruster.writeMicroseconds(converted);

#ifdef ENABLE_BT_DEBUG
  BTSerial.println(
      String(command_msg->fin[0]) + " " + String(command_msg->fin[1]) + " " +
      String(command_msg->fin[2]) + " " + String(command_msg->thruster));
#endif
}

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

  // create publishers
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

  return true;
}

void destroy_entities() {
  rmw_context_t *rmw_context = rcl_context_get_rmw_context(&support.context);
  (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

  // destroy publishers
  pressure_pub.destroy(node);

  // destroy everything else
  rcl_subscription_fini(&command_sub, &node);
  rclc_executor_fini(&executor);
  rcl_node_fini(&node);
  rclc_support_fini(&support);
}

void setup() {

  Serial.begin(BAUD_RATE);
  set_microros_serial_transports(Serial);

  // set up the indicator light
  pinMode(LED_PIN, OUTPUT);

  // set up the servo and thruster pins
  pinMode(SERVO_PIN1, OUTPUT);
  pinMode(SERVO_PIN2, OUTPUT);
  pinMode(SERVO_PIN3, OUTPUT);
  pinMode(THRUSTER_PIN, OUTPUT);

  myServo1.attach(SERVO_PIN1, SERVO_OUT_LOW, SERVO_OUT_HIGH);
  myServo2.attach(SERVO_PIN2, SERVO_OUT_LOW, SERVO_OUT_HIGH);
  myServo3.attach(SERVO_PIN3, SERVO_OUT_LOW, SERVO_OUT_HIGH);
  myThruster.attach(THRUSTER_PIN);

  myServo1.write(DEFAULT_SERVO);
  myServo2.write(DEFAULT_SERVO);
  myServo3.write(DEFAULT_SERVO);
  myThruster.writeMicroseconds(THRUSTER_OFF);
  delay(7000);

  // set up the I2C
  Wire.begin();
  Wire.setClock(I2C_RATE);

  //////////////////////////////////////////////////////////
  // SENSOR SETUP CODE STARTS HERE
  // - Use the #define statements at the top of this file to
  //   enable and disable each sensor
  //////////////////////////////////////////////////////////

#ifdef ENABLE_BT_DEBUG
  BTSerial.begin(BT_DEBUG_RATE);
#endif

#ifdef ENABLE_PRESSURE
  while (!myPressure.init()) {

#ifdef ENABLE_BT_DEBUG
    BTSerial.println("ERROR: Could not connect to Pressure Sensor over I2C");
#endif

    delay(1000);
  }
  myPressure.setFluidDensity(FLUID_DENSITY);
#endif

  //////////////////////////////////////////////////////////
  // SENSOR SETUP CODE ENDS HERE
  //////////////////////////////////////////////////////////

  state = WAITING_AGENT;
}

//////////////////////////////////////////////////////////
// SENSOR VARIABLE UPDATE CODE STARTS HERE
// - Use the #define statements at the top of this file to
//   enable and disable each sensor
//////////////////////////////////////////////////////////

void read_pressure() {

  myPressure.read();
  float pressure = myPressure.pressure();

  // publish the pressure data
  pressure_pub.publish(pressure);
}

//////////////////////////////////////////////////////////
// SENSOR VARIABLE UPDATE CODE ENDS HERE
//////////////////////////////////////////////////////////

void loop() {

  // blink the indicator light
  if (millis() % 1000 < 250) {
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, HIGH);
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

      // if the last command was more than 2 seconds ago, turn off everything
      if (millis() - last_command_time > 2000) {
        myServo1.write(DEFAULT_SERVO);
        myServo2.write(DEFAULT_SERVO);
        myServo3.write(DEFAULT_SERVO);
        myThruster.writeMicroseconds(THRUSTER_OFF);
      }

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
