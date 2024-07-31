#include "echo_pub.h"
#include "gps_pub.h"
#include "leak_pub.h"
#include "battery_pub.h"

#include <SoftwareSerial.h>

#define ENABLE_BATTERY
#define ENABLE_LEAK
#define ENABLE_GPS
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

// micro-ROS config values
#define BAUD_RATE 6000000
#define SYNC_TIMEOUT 1000

// hardware pin values
#define BT_MC_RX 34
#define BT_MC_TX 35
#define VOLT_PIN 18
#define CURRENT_PIN 17
#define LEAK_PIN 16
#define LED_PIN 13

// sensor serial baud rates
#define BT_DEBUG_RATE 9600
#define GPS_RATE 38400
#define GPS_FALLBACK_RATE 9600
#define ECHO_RATE 115200

// sensor update rates
#define BATTERY_MS 100
#define LEAK_MS 100
#define GPS_MS 1000 // fastest update speed is 1 Hz
#define ECHO_MS 33  // fastest update speed is 30 Hz (?)

// micro-ROS objects
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

// publisher objects
BatteryPub battery_pub;
LeakPub leak_pub;
GPSPub gps_pub;
EchoPub echo_pub;

// sensor objects
SoftwareSerial BTSerial(BT_MC_RX, BT_MC_TX);
Ping1D myPing{Serial5};
SFE_UBLOX_GNSS myGNSS;

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
  battery_pub.setup(node);
  leak_pub.setup(node);
  gps_pub.setup(node);
  echo_pub.setup(node);

  return true;
}

void destroy_entities() {

  rmw_context_t *rmw_context = rcl_context_get_rmw_context(&support.context);
  (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

  // destroy publishers
  battery_pub.destroy(node);
  leak_pub.destroy(node);
  gps_pub.destroy(node);
  echo_pub.destroy(node);

  // destroy everything else
  rcl_node_fini(&node);
  rclc_support_fini(&support);
}

void setup() {

  Serial.begin(BAUD_RATE);
  set_microros_serial_transports(Serial);

  // set up the indicator light
  pinMode(LED_PIN, OUTPUT);

  //////////////////////////////////////////////////////////
  // SENSOR SETUP CODE STARTS HERE
  // - Use the #define statements at the top of this file to
  //   enable and disable each sensor
  //////////////////////////////////////////////////////////

#ifdef ENABLE_BT_DEBUG
  BTSerial.begin(BT_DEBUG_RATE);
#endif

#ifdef ENABLE_BATTERY
  pinMode(CURRENT_PIN, INPUT);
  pinMode(VOLT_PIN, INPUT);
#endif

#ifdef ENABLE_LEAK
  pinMode(LEAK_PIN, INPUT);
#endif

#ifdef ENABLE_GPS
  do {

#ifdef ENABLE_BT_DEBUG
    BTSerial.println("GNSS: trying higher baud rate");
#endif

    Serial7.begin(GPS_RATE);
    if (myGNSS.begin(Serial7) == true) {

#ifdef ENABLE_BT_DEBUG
      BTSerial.println("Success");
#endif

      break;
    }

    delay(100);

#ifdef ENABLE_BT_DEBUG
    BTSerial.println("GNSS: trying fallback baud rate");
#endif

    Serial7.begin(GPS_FALLBACK_RATE);
    if (myGNSS.begin(Serial7) == true) {

#ifdef ENABLE_BT_DEBUG
      BTSerial.println(
          "GNSS: connected at fallback baud rate, switching to higher rate");
#endif

      myGNSS.setSerialRate(GPS_RATE);
      delay(100);
    } else {
      // myGNSS.factoryReset();

#ifdef ENABLE_BT_DEBUG
      BTSerial.println("ERROR: GPS serial connection not found");
#endif

      delay(2000);
    }
  } while (1);
  myGNSS.setUART1Output(COM_TYPE_UBX);
  myGNSS.setI2COutput(COM_TYPE_UBX);
  myGNSS.saveConfiguration();
#endif

#ifdef ENABLE_ECHO
  Serial5.begin(ECHO_RATE);
  while (!myPing.initialize()) {

#ifdef ENABLE_BT_DEBUG
    BTSerial.println("ERROR: Could not initialize ping echosounder");
#endif

    delay(1000);
  }
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

void read_battery() {

  // we did some testing to determine the below params, but
  // it's possible they are not completely accurate
  float voltage = (analogRead(VOLT_PIN) * 0.03437) + 0.68;
  float current = (analogRead(CURRENT_PIN) * 0.122) - 11.95;

  // publish the battery data
  battery_pub.publish(voltage, current);
}

void read_leak() {

  bool leak = digitalRead(LEAK_PIN);
  
  // publish the leak data
  leak_pub.publish(leak);
}

void read_gps() {

  float latitude = myGNSS.getLatitude();
  float longitude = myGNSS.getLongitude();

  // publish the GPS data
  gps_pub.publish(latitude, longitude);
}

void read_echo() {

  if (myPing.update()) {
    // publish the echosounder data
    echo_pub.publish();
  }
}

//////////////////////////////////////////////////////////
// SENSOR VARIABLE UPDATE CODE ENDS HERE
//////////////////////////////////////////////////////////

void loop() {

  // blink the indicator light
  if (millis() % 1000 < 500) {
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

#ifdef ENABLE_BATTERY
      EXECUTE_EVERY_N_MS(BATTERY_MS, read_battery());
#endif

#ifdef ENABLE_LEAK
      EXECUTE_EVERY_N_MS(LEAK_MS, read_leak());
#endif

#ifdef ENABLE_GPS
      EXECUTE_EVERY_N_MS(GPS_MS, read_gps());
#endif

#ifdef ENABLE_ECHO
      EXECUTE_EVERY_N_MS(ECHO_MS, read_echo());
#endif

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