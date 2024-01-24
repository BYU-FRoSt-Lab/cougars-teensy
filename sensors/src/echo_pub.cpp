#include "echo_pub.h"

void EchoPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, Echo), "echo_data"));
}

void EchoPub::update(Ping1D ping) {

  if (ping.update()) {
    msg.distance = ping.distance();
    msg.conf_level = ping.confidence();
    msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  }
}

void EchoPub::publish() {

  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}
