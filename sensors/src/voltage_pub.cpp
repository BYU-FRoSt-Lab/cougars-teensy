#include "voltage_pub.h"

#define VOLTAGE_WARNING 15000
#define VOLTAGE_CRITICAL 13000

void VoltagePub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, Volt), "voltage"));
}

void VoltagePub::update(Adafruit_INA260 ina260) {
  
  msg.voltage = ina260.readBusVoltage();
  msg.current = ina260.readCurrent();
  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
}

void VoltagePub::publish() {

  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}