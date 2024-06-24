#include "dvl_pub.h"

void DVLPub::setup(rcl_node_t node) { 

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, DVL), "dvl_data"));
}

void DVLPub::update(String wrz, String wrp, String wru) {

  // TO DO: This compiles, but is broken for some reason
  // sprintf(msg.wrz.data, wrz.c_str());
  // msg.wrz.size = strlen(msg.wrz.data);

  // sprintf(msg.wrp.data, wrp.c_str());
  // msg.wrp.size = strlen(msg.wrp.data);

  // sprintf(msg.wru.data, wru.c_str());
  // msg.wru.size = strlen(msg.wru.data);
}

void DVLPub::publish() {

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}