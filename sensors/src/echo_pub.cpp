#include "echo_pub.h"

#define ECHO_RATE 115200
#define ECHO_RX_PIN 21
#define ECHO_TX_PIN 20

Ping1D ping { Serial5 };

void EchoPub::setup(rcl_node_t node) {

  Serial5.begin(ECHO_RATE);
  while(!ping.initialize()) {
    delay(1000);
  }

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, Echo), "echo_data"));
}

void EchoPub::publish() {

  if (ping.update()) {
    msg.distance = ping.distance();
    msg.conf_level = ping.confidence();
    msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
  }
}
