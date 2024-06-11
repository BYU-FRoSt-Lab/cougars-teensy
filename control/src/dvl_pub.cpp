#include "dvl_pub.h"

void DVLPub::setup(rcl_node_t node) { 

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, DVL), "dvl_data"));
}

void DVLPub::update(String wrz, String wrp, String wru) {

  // can't copy the strings directly
  for (int i = 0; i < wrz.length(); i++) {
    msg.wrz[i] = wrz[i];
  }
  for (int i = 0; i < wrp.length(); i++) {
    msg.wrp[i] = wrp[i];
  }
  for (int i = 0; i < wru.length(); i++) {
    msg.wru[i] = wru[i];
  }
}

void DVLPub::publish() {

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}