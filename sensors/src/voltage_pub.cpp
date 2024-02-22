#include "voltage_pub.h"

void VoltagePub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, Volt), "voltage"));
}

void VoltagePub::update() {
  
  msg.voltage = 0.0;
  msg.current = 0.0;
  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
}

void VoltagePub::publish() {

  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}