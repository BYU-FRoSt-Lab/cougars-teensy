#include "dvl_pub.h"
#include <string>

void DVLPub::setup(rcl_node_t node) { 

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, DVL), "dvl_data"));
}

void DVLPub::update(String wrz, String wrp, String wru) {

  strcpy(msg.wrz.data, wrz.c_str());
  strcpy(msg.wrp.data, wrp.c_str());
  strcpy(msg.wru.data, wru.c_str());
}

void DVLPub::publish() {

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}