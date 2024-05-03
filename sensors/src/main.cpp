#include "echo_pub.h"
#include "leak_pub.h"
#include "voltage_pub.h"
#include "gps_pub.h"

#include <SoftwareSerial.h>

#define ENABLE_VOLT
#define ENABLE_LEAK
// #define ENABLE_GPS
// #define ENABLE_ECHO
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

#define BAUD_RATE 6000000
#define CALLBACK_TOTAL 1
#define TIMER_PERIOD 3000
#define SYNC_TIMEOUT 1000

#define LEAK_PIN 16
#define CURRENT_PIN 17
#define VOLT_PIN 18
#define ECHO_RATE 115200

// micro-ROS objects
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rclc_executor_t executor;
rcl_timer_t timer_pub;

// publisher objects
VoltagePub voltage_pub;
LeakPub leak_pub;
EchoPub echo_pub;
GPSPub gps_pub;

// global sensor variables
float latitude = 0.0;
float longitude = 0.0;
float distance = 0.0;
float conf_level = 0.0;
bool leak_detected = false;
float voltage = 0.0;
float current = 0.0;

// sensor objects
Ping1D ping { Serial5 };
SFE_UBLOX_GNSS myGNSS;
long lastTime = 0;
SoftwareSerial BTSerial(34, 35);

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

// micro-ROS function that publishes all the data to their topics
void timer_pub_callback(rcl_timer_t *timer, int64_t last_call_time) {

  (void)last_call_time;
  if (timer != NULL) {

    gps_pub.update(latitude, longitude);
    gps_pub.publish();
    voltage_pub.update(voltage, current);
    voltage_pub.publish();
    leak_pub.update(leak_detected);
    leak_pub.publish();
    echo_pub.update(distance, conf_level);
    echo_pub.publish();
  }
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
  gps_pub.setup(node);
  voltage_pub.setup(node);
  leak_pub.setup(node);
  echo_pub.setup(node);

  // create timer (handles periodic publications)
  RCCHECK(rclc_timer_init_default(
      &timer_pub, &support, RCL_MS_TO_NS(TIMER_PERIOD), timer_pub_callback));

  // create executor
  RCSOFTCHECK(rclc_executor_init(&executor, &support.context, CALLBACK_TOTAL,
                                 &allocator));

  // add callbacks to executor
  RCSOFTCHECK(rclc_executor_add_timer(&executor, &timer_pub));

  return true;
}

void destroy_entities() {
  
  rmw_context_t *rmw_context = rcl_context_get_rmw_context(&support.context);
  (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

  // destroy publishers
  gps_pub.destroy(node);
  voltage_pub.destroy(node);
  leak_pub.destroy(node);
  echo_pub.destroy(node);

  // destroy everything else
  rcl_timer_fini(&timer_pub);
  rclc_executor_fini(&executor);
  rcl_node_fini(&node);
  rclc_support_fini(&support);
}

void setup() {

  Serial.begin(BAUD_RATE);
  set_microros_serial_transports(Serial);

  #ifdef ENABLE_BT_DEBUG
  BTSerial.begin(9600);
  #endif

  // set up the leak detector
  #ifdef ENABLE_LEAK
  pinMode(LEAK_PIN, INPUT);
  #endif

  // set up the voltage sensor
  #ifdef ENABLE_VOLT
  pinMode(CURRENT_PIN, INPUT);
  pinMode(VOLT_PIN, INPUT);
  #endif

  // set up the GPS
  #ifdef ENABLE_GPS
  do {
    BTSerial.println("GNSS: trying 38400 baud");
    Serial7.begin(38400);
    if (myGNSS.begin(Serial7) == true) {
       BTSerial.println("Success");
       break;
    }
     
    delay(100);
    BTSerial.println("GNSS: trying 9600 baud");
    Serial7.begin(9600);
    if (myGNSS.begin(Serial7) == true) {
        BTSerial.println("GNSS: connected at 9600 baud, switching to 38400");
        myGNSS.setSerialRate(38400);
        delay(100);
    } else {
        // myGNSS.factoryReset();
        BTSerial.println("ERROR: GPS serial connection not found");
        delay(2000); //Wait a bit before trying again to limit the Serial output
    }

  } while(1);

  myGNSS.setUART1Output(COM_TYPE_UBX); // Set the UART port to output UBX only
  myGNSS.setI2COutput(COM_TYPE_UBX); // Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfiguration(); // Save the current settings to flash and BBR
  #endif


  // set up the echosounder
  #ifdef ENABLE_ECHO
  Serial5.begin(ECHO_RATE);
  while(!ping.initialize()) {
    // BTSerial.println("error setting up");
    delay(1000);
  }
  #endif
  
  state = WAITING_AGENT;
}

void loop() {

  // update the global leak variables
  #ifdef ENABLE_LEAK
  if (digitalRead(LEAK_PIN)) {
    leak_detected = true;
  } else {
    leak_detected = false;
  }
  #endif

  // update the global voltage variables
  #ifdef ENABLE_VOLT
  voltage = (analogRead(VOLT_PIN) * 0.03437) + 0.68;
  current = (analogRead(CURRENT_PIN) * 0.122) - 11.95;
  #endif

  // update the global GPS variables
  #ifdef ENABLE_GPS
  if (millis() - lastTime > 1000)
  {
    lastTime = millis();
    latitude = myGNSS.getLatitude();
    longitude = myGNSS.getLongitude();
  }
  #endif

  // update the global echosounder variables
  #ifdef ENABLE_ECHO
  if (ping.update()) {
    distance = ping.distance();
    conf_level = ping.confidence();
  }
  #endif

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