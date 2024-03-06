#include "leak_pub.h"

void LeakPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, Leak),
      "leak_detected"));
}

void LeakPub::update(bool leak) {

  msg.leak_detected = leak;
  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
}

void LeakPub::publish() {

    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}
