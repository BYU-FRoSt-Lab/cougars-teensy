#include "depth_pub.h"

void DepthPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, Depth),
      "depth_data"));
}

void DepthPub::update(float pressure, float depth, float temperature) {

  msg.pressure = pressure;
  msg.depth = depth;
  msg.temperature = temperature;
}

void DepthPub::publish() {

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}
