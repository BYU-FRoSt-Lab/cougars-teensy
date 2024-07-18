#include "depth_pub.h"
#include "dvl_pub.h"
#include "imu_pub.h"
#include "pid_control.h"

#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <frost_interfaces/msg/pid.h>
#include <frost_interfaces/msg/calibration.h>

// #define ENABLE_IMU
#define ENABLE_DVL
#define ENABLE_DEPTH
// #define ENABLE_BT_DEBUG

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
#define TIMER_PUB_PERIOD 1000 // TODO: Change this for PID testing
#define TIMER_PID_PERIOD 10   // 100 Hz
#define SYNC_TIMEOUT 1000

// hardware pin values
#define BT_MC_RX 34
#define BT_MC_TX 35
#define SERVO_PIN1 9
#define SERVO_PIN2 10
#define SERVO_PIN3 11
#define THRUSTER_PIN 12
#define LED_PIN 13

// default actuator positions
#define DEFAULT_SERVO 90
#define THRUSTER_OFF 1500

// pressure sensor calibration values
#define AVG_COUNT 10
#define AVG_DEC 0.1
#define FLUID_DENSITY 997

// sensor baud rates
#define BT_DEBUG_RATE 9600
#define DVL_RATE 115200
#define I2C_RATE 400000

// sensor update rates
#define IMU_MS 50
#define DVL_MS 66    // fastest update speed is 15 Hz
#define DEPTH_MS 100 // fastest update speed is 10 Hz (?)

// micro-ROS objects
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rclc_executor_t executor;
rcl_timer_t timer_pub;
rcl_timer_t timer_pid;
frost_interfaces__msg__PID msg;
frost_interfaces__msg__PID *pid_request_msg = new frost_interfaces__msg__PID;

// subscriber objects
rcl_subscription_t pid_sub;
rcl_subscription_t calibration_sub;

// publisher objects
IMUPub imu_pub;
DVLPub dvl_pub;
DepthPub depth_pub;

// sensor objects
SoftwareSerial BTSerial(BT_MC_RX, BT_MC_TX);
BNO08x myIMU;
MS5837 myDepth;

// actuator objects
Servo myServo1;
Servo myServo2;
Servo myServo3;
Servo myThruster;

// control objects
PID_Control myHeadingPID();
PID_Control myDepthPID();
PID_Control myVelocityPID();

// global sensor variables
float roll = 0.0;
float pitch = 0.0;
float yaw = 0.0;
float x_velocity = 0.0;
String valid = "n";
String wrz = "";
String wrp = "";
String wru = "";
float pressure = 0.0;
float depth = 0.0;
float temperature = 0.0;
int depth_pos;
int heading_pos;
int velocity_level;

// dvl processing variables
bool reset_dead_reckoning = true;
String data_string = "";

// pressure sensor calibration variables
float sum_pressure_at_zero_depth = 0.0;
float sum_depth_error_at_zero_depth = 0.0;
float pressure_at_zero_depth = 0.0;
float depth_error_at_zero_depth = 0.0;

// pid calibration variables
float heading_kp = 0.0;
float heading_ki = 0.0;
float heading_kd = 0.0;
int heading_min_output = 0;
int heading_max_output = 0;
int heading_bias = 0;

float depth_kp = 0.0;
float depth_ki = 0.0;
float depth_kd = 0.0;
int depth_min_output = 0;
int depth_max_output = 0;
int depth_bias = 0;

float velocity_kp = 0.0;
float velocity_ki = 0.0;
float velocity_kd = 0.0;
int velocity_min_output = 0;
int velocity_max_output = 0;
int velocity_bias = 0;

// calibration flag
bool calibrated = false;

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
    dvl_pub.update(wrz, wrp, wru);
    dvl_pub.publish();
    depth_pub.update(pressure, depth, temperature);
    depth_pub.publish();
  }
}

// micro-ROS timer function that runs the PID
void timer_pid_callback(rcl_timer_t *timer, int64_t last_call_time) {

  (void)last_call_time;
  if (timer != NULL) {

    if (calibrated) {

      //////////////////////////////////////////////////////////
      // LOW-LEVEL CONTROLLER CODE STARTS HERE
      // - Reference wanted values using
      //   pid_request_msg->velocity, pid_request_msg->yaw, etc
      //////////////////////////////////////////////////////////

      if (pid_request_msg->stop == false) {

        // reset the dead reckoning on the dvl as soon as we start moving
        if (reset_dead_reckoning) {
          Serial7.write("wcr\n");
          reset_dead_reckoning = false;
        }

        depth_pos = myDepthPID.compute(pid_request_msg->depth, depth);
        heading_pos = myHeadingPID.compute(pid_request_msg->yaw, yaw);

        // check if our velocity measurement is valid
        if (valid == "y") {
          velocity_level =
              myVelocityPID.compute(pid_request_msg->velocity, x_velocity);
        } else {
          velocity_level = THRUSTER_OFF;

  #ifdef ENABLE_BT_DEBUG
          BTSerial.println("ERROR: DVL velocity measurement is invalid");
  #endif
        }

        myServo1.write(heading_pos);
        myServo2.write(depth_pos);
        myServo3.write(depth_pos);
        // myThruster.writeMicroseconds(velocity_level);
      } else {

        myServo1.write(DEFAULT_SERVO);
        myServo2.write(DEFAULT_SERVO);
        myServo3.write(DEFAULT_SERVO);
        myThruster.writeMicroseconds(THRUSTER_OFF);
      }

      //////////////////////////////////////////////////////////
      // LOW-LEVEL CONTROLLER CODE ENDS HERE
      //////////////////////////////////////////////////////////
    }
  }
}

// micro-ROS function that subscribes to requested PID values
void pid_sub_callback(const void *pid_msgin) {
  pid_request_msg = (frost_interfaces__msg__PID *)pid_msgin;
}

// micro-ROS function that subscribes to requested calibration values
void calibration_sub_callback(const void *calibration_msgin) {

  // calibrate the depth PID controller
  depth_kp = calibration_msgin->depth_kp;
  depth_ki = calibration_msgin->depth_ki;
  depth_kd = calibration_msgin->depth_kd;
  depth_min_output = calibration_msgin->depth_min_output;
  depth_max_output = calibration_msgin->depth_max_output;
  depth_bias = calibration_msgin->depth_bias;
  myDepthPID.calibrate(depth_kp, depth_ki, depth_kd, depth_min_output,
                       depth_max_output, TIMER_PID_PERIOD, depth_bias);

  // calibrate the heading PID controller
  heading_kp = calibration_msgin->heading_kp;
  heading_ki = calibration_msgin->heading_ki;
  heading_kd = calibration_msgin->heading_kd;
  heading_min_output = calibration_msgin->heading_min_output;
  heading_max_output = calibration_msgin->heading_max_output;
  heading_bias = calibration_msgin->heading_bias;
  myHeadingPID.calibrate(heading_kp, heading_ki, heading_kd, heading_min_output,
                         heading_max_output, TIMER_PID_PERIOD, heading_bias);

  // calibrate the velocity PID controller
  velocity_kp = calibration_msgin->velocity_kp;
  velocity_ki = calibration_msgin->velocity_ki;
  velocity_kd = calibration_msgin->velocity_kd;
  velocity_min_output = calibration_msgin->velocity_min_output;
  velocity_max_output = calibration_msgin->velocity_max_output;
  velocity_bias = calibration_msgin->velocity_bias;
  myVelocityPID.calibrate(velocity_kp, velocity_ki, velocity_kd,
                          velocity_min_output, velocity_max_output, TIMER_PID_PERIOD,
                          velocity_bias);

  calibrated = true;
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

  // create subscribers
  RCCHECK(rclc_subscription_init_default(
      &pid_sub, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, PID), "pid_request"));

  RCCHECK(rclc_subscription_init_default(
      &calibration_sub, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, Calibration), "calibration"));

  // create timers (handles periodic publications and PID execution)
  RCCHECK(rclc_timer_init_default(&timer_pub, &support,
                                  RCL_MS_TO_NS(TIMER_PUB_PERIOD),
                                  timer_pub_callback));
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
      &executor, &pid_sub, &msg, &pid_sub_callback, ON_NEW_DATA));
  RCSOFTCHECK(rclc_executor_add_subscription(
      &executor, &calibration_sub, &msg, &calibration_sub_callback, ON_NEW_DATA));

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
  rcl_subscription_fini(&pid_sub, &node);
  rcl_subscription_fini(&calibration_sub, &node);
  rcl_timer_fini(&timer_pub);
  rcl_timer_fini(&timer_pid);
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

  myServo1.attach(SERVO_PIN1);
  myServo2.attach(SERVO_PIN2);
  myServo3.attach(SERVO_PIN3);
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

#ifdef ENABLE_IMU
  while (!myIMU.begin(0x4A, Wire)) {

#ifdef ENABLE_BT_DEBUG
    BTSerial.println("ERROR: Could not connect to IMU over I2C");
#endif

    delay(1000);
  }
  if (myIMU.enableRotationVector(IMU_MS) == false) {

#ifdef ENABLE_BT_DEBUG
    BTSerial.println("ERROR: Could not enable rotation vector reports");
#endif
  }
#endif

#ifdef ENABLE_DVL
  Serial7.begin(DVL_RATE);
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

// read and update the IMU sensor values
void read_imu() {

  if (myIMU.wasReset()) {

#ifdef ENABLE_BT_DEBUG
    BTSerial.println("ALERT: IMU sensor was reset");
#endif
    if (myIMU.enableRotationVector(IMU_MS) == false) {

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
}

// read and update the DVL sensor values
void read_dvl() {

  if (Serial7.available()) {
    char incoming_byte = Serial7.read();
    if (incoming_byte != '\n') {
      data_string += (char)incoming_byte;
    } else {

#ifdef ENABLE_BT_DEBUG
      BTSerial.println("ALERT: Got DVL message");
#endif

      char identifier = data_string[2];

      // check dead reckoning report
      if (identifier == 'p') {
        wrp = data_string;

        // parse the data for roll, pitch, and yaw
        int num_fields = 0;
        int start_index = 3;
        for (uint i = start_index; i < data_string.length(); i++) {
          if (data_string[i] == ',') {
            num_fields++;
            if (num_fields == 7) {
              roll =
                  data_string.substring(start_index, i).toFloat(); // in degrees
            } else if (num_fields == 8) {
              pitch =
                  data_string.substring(start_index, i).toFloat(); // in degrees
            } else if (num_fields == 9) {
              yaw =
                  data_string.substring(start_index, i).toFloat(); // in degrees
            }
            start_index = i + 1;
          }
        }

        // check velocity report
      } else if (identifier == 'z') {
        wrz = data_string;

        // parse the data for x velocity
        int num_fields = 0;
        int start_index = 3;
        for (uint i = start_index; i < data_string.length(); i++) {
          if (data_string[i] == ',') {
            num_fields++;
            if (num_fields == 2) {
              x_velocity = data_string.substring(start_index, i).toFloat();
            } else if (num_fields == 5) {
              valid = data_string.substring(start_index, i);
            }
            start_index = i + 1;
          }
        }

        // check transducer report
      } else if (identifier == 'u') {
        wru = data_string;
      } else if (identifier == 'a') {

#ifdef ENABLE_BT_DEBUG
        BTSerial.println("ALERT: DVL Dead Reckoning Reset Successful");
#endif
      }

      data_string = "";
    }
  }
}

// read and update the depth sensor values
void read_depth() {

  myDepth.read();
  pressure = myDepth.pressure() - pressure_at_zero_depth;
  depth = myDepth.depth() - depth_error_at_zero_depth;
  temperature = myDepth.temperature();
}

void loop() {

  // blink the indicator light
  if (millis() % 1000 < 500) {
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, HIGH);
  }

  //////////////////////////////////////////////////////////
  // SENSOR VARIABLE UPDATE CODE STARTS HERE
  // - Use the #define statements at the top of this file to
  //   enable and disable each sensor
  //////////////////////////////////////////////////////////

#ifdef ENABLE_IMU
  EXECUTE_EVERY_N_MS(IMU_MS, read_imu());
#endif

#ifdef ENABLE_DVL
  EXECUTE_EVERY_N_MS(DVL_MS, read_dvl());
#endif

#ifdef ENABLE_DEPTH
  EXECUTE_EVERY_N_MS(DEPTH_MS, read_depth());
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
