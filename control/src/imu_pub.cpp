#include "imu_pub.h"

void IMUPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, IMU), "imu_data"));
}

void IMUPub::update(float roll, float pitch, float yaw) {

  msg.roll = roll;
  msg.pitch = pitch;
  msg.yaw = yaw;
}

void IMUPub::publish() {

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}