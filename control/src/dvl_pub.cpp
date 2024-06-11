#include "dvl_pub.h"
#include <string>

void DVLPub::setup(rcl_node_t node) { 

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, DVL), "dvl_data"));
}

void DVLPub::update(String wrz, String wrp, String wru) {

  std::string wrz_str = wrz.c_str();
  msg.wrz.data = wrz_str;
  msg.wrp.data  = wrp;
  msg.wru = wru;
}

void DVLPub::publish() {

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}