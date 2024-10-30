#include "battery_pub.h"

void BatteryPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, BatteryStatus),
      NAMESPACE "/battery/data"));
}

void BatteryPub::publish(float voltage, float current) {

  msg.voltage = voltage;
  msg.current = current;
  msg.header.stamp.sec = NS_TO_S(rmw_uros_epoch_nanos());
  msg.header.stamp.nanosec = NS_REMAINDER(rmw_uros_epoch_nanos());
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}