#include "echo_pub.h"

void EchoPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64),
      "echo_data"));
}

void EchoPub::publish() { RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL)); }
