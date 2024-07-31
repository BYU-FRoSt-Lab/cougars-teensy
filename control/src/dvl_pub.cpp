#include "dvl_pub.h"

void DVLPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, DVLStrings), "dvl_data"));
}

void DVLPub::publish(String wrz, String wrp, String wru) {

  // TODO: This compiles, but is still broken for some reason
  msg.wrz = (char *)wrz;
  msg.wrp = (char *)wrp;
  msg.wru = (char *)wru;

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}