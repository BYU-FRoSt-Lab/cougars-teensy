#include "gps_pub.h"

void GPSPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, GPS), "gps_data"));
}

void GPSPub::update(float latitude, float longitude) {

  msg.latitude = latitude;
  msg.longitude = longitude;
  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
}

void GPSPub::publish() { RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL)); }
