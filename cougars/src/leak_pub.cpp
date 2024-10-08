#include "leak_pub.h"

#define NS_TO_S(ns) (ns / 1000000000)
#define NS_REMAINDER(ns) (ns % 1000000000)

void LeakPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, LeakStatus),
      "leak/data"));
}

void LeakPub::publish(bool leak) {

  msg.leak = leak;
  msg.header.stamp.sec = NS_TO_S(rmw_uros_epoch_nanos());
  msg.header.stamp.nanosec = NS_REMAINDER(rmw_uros_epoch_nanos());
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}
