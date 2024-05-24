#include "imu_pub.h"
#include "dvl_pub.h"
#include "depth_pub.h"
#include "pid_control.h"

#include "dvl_driver.h"

#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <frost_interfaces/msg/pid.h>

// #define ENABLE_IMU
#define ENABLE_DVL
// #define ENABLE_DEPTH
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
#define CALLBACK_TOTAL 3
#define TIMER_PUB_PERIOD 3000
#define TIMER_PID_PERIOD 10 // 100 Hz
#define SYNC_TIMEOUT 1000

// hardware pin values
#define BT_MC_RX 34
#define BT_MC_TX 35
#define SERVO_PIN1 9
#define SERVO_PIN2 10
#define SERVO_PIN3 11
#define THRUSTER_PIN 12

// default actuator positions
#define DEFAULT_SERVO 90
#define DEFAULT_THRUSTER 1500

// pressure sensor calibration values
#define AVG_COUNT 10
#define AVG_DEC 0.1
#define FLUID_DENSITY 997

// sensor serial baud rates
#define BT_DEBUG_RATE 9600

// micro-ROS objects
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rclc_executor_t executor;
rcl_subscription_t subscriber;
rcl_timer_t timer_pub;
rcl_timer_t timer_pid;
frost_interfaces__msg__PID msg;
frost_interfaces__msg__PID *pid_request_msg =
    new frost_interfaces__msg__PID;

// publisher objects
IMUPub imu_pub;
DVLPub dvl_pub;
DepthPub depth_pub;

// sensor objects
SoftwareSerial BTSerial(BT_MC_RX, BT_MC_TX);
BNO08x myIMU;
DVL myDVL;
MS5837 myDepth;

// actuator objects
Servo my_servo1;
Servo my_servo2;
Servo my_servo3;
Servo my_thruster;

// control objects
// PID_Control myHeadingPID = new PID_Control(0.1, 0, 0, 0, 180, TIMER_PID_PERIOD, -90);
// PID_Control myDepthPID = new PID_Control(0.1, 0, 0, 0, 180, TIMER_PID_PERIOD, -90);

// global sensor variables
float roll = 0.0;
float pitch = 0.0;
float yaw = 0.0;
float pressure = 0.0;
float depth = 0.0;
float temperature = 0.0;
int depth_pos;
int heading_pos;

// pressure sensor calibration variables
float sum_pressure_at_zero_depth = 0.0;
float sum_depth_error_at_zero_depth = 0.0;
float pressure_at_zero_depth = 0.0;
float depth_error_at_zero_depth = 0.0;

// states for state machine in loop function
enum states {
  WAITING_AGENT,
  AGENT_AVAILABLE,
  AGENT_CONNECTED,
  AGENT_DISCONNECTED
} static state;

// responds to errors with micro-ROS functions
void error_loop() {
  while (1) {
    delay(100);
  }
}

// micro-ROS timer function that publishes topic data
void timer_pub_callback(rcl_timer_t *timer, int64_t last_call_time) {

  (void)last_call_time;
  if (timer != NULL) {
    imu_pub.update(roll, pitch, yaw);
    imu_pub.publish();
    dvl_pub.update(myDVL.wrz, myDVL.wrp, myDVL.wru);
    dvl_pub.publish();
    depth_pub.update(pressure, depth, myDepth.temperature());
    depth_pub.publish();
  }
}

// micro-ROS timer function that runs the PID
void timer_pid_callback(rcl_timer_t *timer, int64_t last_call_time) {

  (void)last_call_time;
  if (timer != NULL) {

    //////////////////////////////////////////////////////////
    // LOW-LEVEL CONTROLLER CODE STARTS HERE
    // - Reference wanted values using
    //   pid_request_msg->velocity, pid_request_msg->yaw, etc
    //////////////////////////////////////////////////////////

    if (pid_request_msg->stop == false) {

      // TODO: add PID stuff here
      // depth_pos = myDepthPID.compute(pid_request_msg->depth, depth);
      // heading_pos = myHeadingPID.compute(pid_request_msg->yaw, yaw);

      my_servo1.write(heading_pos);
      my_servo2.write(depth_pos);
      my_servo3.write(depth_pos);

    } else {

      my_servo1.write(DEFAULT_SERVO);
      my_servo2.write(DEFAULT_SERVO);
      my_servo3.write(DEFAULT_SERVO);
      my_thruster.writeMicroseconds(DEFAULT_THRUSTER);
    }

    //////////////////////////////////////////////////////////
    // LOW-LEVEL CONTROLLER CODE ENDS HERE
    //////////////////////////////////////////////////////////
  }
}

// micro-ROS function that subscribes to requested PID values
void subscription_callback(const void *pid_msgin) {
  pid_request_msg = (frost_interfaces__msg__PID *)pid_msgin;
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
  imu_pub.setup(node);
  dvl_pub.setup(node);
  depth_pub.setup(node);

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
      &subscriber, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, PID), "pid_request"));

  // create timers (handles periodic publications and PID execution)
  RCCHECK(rclc_timer_init_default(
      &timer_pub, &support, RCL_MS_TO_NS(TIMER_PUB_PERIOD), timer_pub_callback));
  RCCHECK(rclc_timer_init_default(&timer_pid, &support,
                                  RCL_MS_TO_NS(TIMER_PID_PERIOD),
                                  timer_pid_callback));

  // create executor
  RCSOFTCHECK(rclc_executor_init(&executor, &support.context, CALLBACK_TOTAL,
                                 &allocator));

  // add callbacks to executor
  RCSOFTCHECK(rclc_executor_add_timer(&executor, &timer_pub));
  RCSOFTCHECK(rclc_executor_add_timer(&executor, &timer_pid));
  RCSOFTCHECK(rclc_executor_add_subscription(
      &executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));

  // wait for first message to arrive from pid_request topic
  pid_request_msg->stop = true;

  return true;
}

void destroy_entities() {
  rmw_context_t *rmw_context = rcl_context_get_rmw_context(&support.context);
  (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

  // destroy publishers
  imu_pub.destroy(node);
  dvl_pub.destroy(node);
  depth_pub.destroy(node);

  // destroy everything else
  rcl_subscription_fini(&subscriber, &node);
  rcl_timer_fini(&timer_pub);
  rcl_timer_fini(&timer_pid);
  rclc_executor_fini(&executor);
  rcl_node_fini(&node);
  rclc_support_fini(&support);
}

void setup() {

  Serial.begin(BAUD_RATE);
  set_microros_serial_transports(Serial);

  // set up the servo and thruster pins
  pinMode(SERVO_PIN1, OUTPUT);
  pinMode(SERVO_PIN2, OUTPUT);
  pinMode(SERVO_PIN3, OUTPUT);
  pinMode(THRUSTER_PIN, OUTPUT);

  my_servo1.attach(SERVO_PIN1);
  my_servo2.attach(SERVO_PIN2);
  my_servo3.attach(SERVO_PIN3);
  my_thruster.attach(THRUSTER_PIN);

  my_servo1.write(DEFAULT_SERVO);
  my_servo2.write(DEFAULT_SERVO);
  my_servo3.write(DEFAULT_SERVO);
  my_thruster.writeMicroseconds(DEFAULT_THRUSTER);
  delay(7000);

  // set up the I2C
  Wire.begin();
  Wire.setClock(400000);

  //////////////////////////////////////////////////////////
  // SENSOR SETUP CODE STARTS HERE
  // - Use the #define statements at the top of this file to
  //   enable and disable each sensor
  //////////////////////////////////////////////////////////

  #ifdef ENABLE_BT_DEBUG
  BTSerial.begin(BT_DEBUG_RATE);
  #endif

  #ifdef ENABLE_IMU
  while (!myIMU.begin(0x4A, Wire)) {

    #ifdef ENABLE_BT_DEBUG
    BTSerial.println("ERROR: Could not connect to IMU over I2C");
    #endif

    delay(1000);
  }
  if (myIMU.enableRotationVector(10) == false) { // send data update every 10ms (100 Hz)

    #ifdef ENABLE_BT_DEBUG
    BTSerial.println("ERROR: Could not enable rotation vector reports");
    #endif
  }
  #endif

  #ifdef ENABLE_DVL
  BTSerial.println("Setting up DVL");
  myDVL.setup();
  BTSerial.println("DVL setup complete");
  #endif

  #ifdef ENABLE_DEPTH
  while (!myDepth.init()) {

    #ifdef ENABLE_BT_DEBUG
    BTSerial.println("ERROR: Could not connect to Pressure Sensor over I2C");
    #endif

    delay(1000);
  }
  myDepth.setFluidDensity(FLUID_DENSITY);

  // calibrate the pressure sensor
  for (int i = 0; i < AVG_COUNT; i++) {
    myDepth.read();
    sum_pressure_at_zero_depth += myDepth.pressure();
    sum_depth_error_at_zero_depth += myDepth.depth();
  }
  pressure_at_zero_depth = sum_pressure_at_zero_depth * AVG_DEC;
  depth_error_at_zero_depth = sum_depth_error_at_zero_depth * AVG_DEC;
  #endif

  //////////////////////////////////////////////////////////
  // SENSOR SETUP CODE ENDS HERE
  //////////////////////////////////////////////////////////

  state = WAITING_AGENT;
}

void loop() {

  //////////////////////////////////////////////////////////
  // SENSOR VARIABLE UPDATE CODE STARTS HERE
  // - Use the #define statements at the top of this file to
  //   enable and disable each sensor
  //////////////////////////////////////////////////////////

  #ifdef ENABLE_IMU
  if (myIMU.wasReset()) {

    #ifdef ENABLE_BT_DEBUG
    BTSerial.println("ALERT: IMU sensor was reset");
    #endif
    // set reports again
    if (myIMU.enableRotationVector(50) == false) { // send data update every 50ms

      #ifdef ENABLE_BT_DEBUG
      BTSerial.println("ERROR: Could not enable rotation vector reports");
      #endif
    }
  }

  if (myIMU.getSensorEvent() == true) {
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_ROTATION_VECTOR) {
      roll = myIMU.getRoll();
      pitch = myIMU.getPitch();
      yaw = myIMU.getYaw();
    }
  }
  #endif

  #ifdef ENABLE_DVL
  BTSerial.println("Updating DVL");
  myDVL.update();
  BTSerial.println("DVL updated");
  roll = myDVL.roll;
  pitch = myDVL.pitch;
  yaw = myDVL.yaw;
  BTSerial.println("DVL variables updated");
  BTSerial.println(roll);
  BTSerial.println(pitch);
  BTSerial.println(yaw);
  #endif

  #ifdef ENABLE_DEPTH
  myDepth.read();
  pressure = myDepth.pressure() - pressure_at_zero_depth;
  depth = myDepth.depth() - depth_error_at_zero_depth;
  #endif

  //////////////////////////////////////////////////////////
  // SENSOR VARIABLE UPDATE CODE ENDS HERE
  //////////////////////////////////////////////////////////

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
      rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
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