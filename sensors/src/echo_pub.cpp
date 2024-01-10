#include "echo_pub.h"

#define ECHO_RATE 115200
#define ECHO_RX_PIN 21
#define ECHO_TX_PIN 20

SoftwareSerial ping_serial = SoftwareSerial(ECHO_RX_PIN, ECHO_TX_PIN);
Ping1D ping{ping_serial};

void EchoPub::setup(rcl_node_t node) {

  ping_serial.begin(ECHO_RATE);

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, Echo), "echo_data"));
}

void EchoPub::publish() {
  // ping.request(PingMessageId::PING1D_PROFILE);
  // msg.profile_data.data=ping.profile_data();
  ping.update();
  msg.distance = ping.distance();
  msg.conf_level = ping.confidence();
  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}
