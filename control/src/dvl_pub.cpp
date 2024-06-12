#include "dvl_pub.h"
#include <std_msgs/msg/string.hpp>

void DVLPub::setup(rcl_node_t node) { 

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, DVL), "dvl_data"));
}

void DVLPub::update(String wrz, String wrp, String wru) {

  auto wrz_msg = std_msgs::msg::String();
  wrz_msg.data = wrz.c_str();
  msg.wrz = wrz_msg;

  strcopy(msg.wrp.data, wrp.c_str());
  msg.wru.data = wru;
}

void DVLPub::publish() {

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}