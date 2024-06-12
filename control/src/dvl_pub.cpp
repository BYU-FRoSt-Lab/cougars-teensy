#include "dvl_pub.h"
#include <std_msgs/msg/string.h>

void DVLPub::setup(rcl_node_t node) { 

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, DVL), "dvl_data"));
}

void DVLPub::update(String wrz, String wrp, String wru) {

  sprintf(msg.wrz.data.data, wrz.c_str());
  msg.wrz.data.size = strlen(msg.wrz.data.data);

  sprintf(msg.wrp.data.data, wrp.c_str());
  msg.wrp.data.size = strlen(msg.wrp.data.data);

  sprintf(msg.wru.data.data, wru.c_str());
  msg.wru.data.size = strlen(msg.wru.data.data);
}

void DVLPub::publish() {

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}