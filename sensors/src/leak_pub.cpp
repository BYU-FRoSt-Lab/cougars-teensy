#include "leak_pub.h"

#define LEAK_PIN 16

void LeakPub::setup(rcl_node_t node) {

  pinMode(LEAK_PIN, INPUT);

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, Leak),
      "leak_detected"));
}

void LeakPub::update(int pin) {

  if (digitalRead(pin)) {
    msg.leak_detected = true;
    msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  }
}

void LeakPub::publish() {

    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}
