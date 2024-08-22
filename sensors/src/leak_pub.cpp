#include "leak_pub.h"

void LeakPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, LeakStatus),
      "leak_data"));
}

void LeakPub::publish(bool leak) {

  msg.leak = leak;
  msg.header.stamp.sec = rmw_uros_epoch_seconds();
  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}
