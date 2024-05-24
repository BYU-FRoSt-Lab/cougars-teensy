#include "dvl_pub.h"

void DVLPub::setup(rcl_node_t node) { 

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, DVL), "dvl_data"));
}

void DVLPub::update(__cpp_raw_strings wrz, __cpp_raw_strings wrp, __cpp_raw_strings wru) {

    msg.wrz = wrz;
    msg.wrp = wrp;
    msg.wru = wru;
}

void DVLPub::publish() {

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}