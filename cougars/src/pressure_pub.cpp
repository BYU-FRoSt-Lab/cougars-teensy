#include "pressure_pub.h"

void PressurePub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, FluidPressure),
      NAMESPACE "/pressure/data"));
}

void PressurePub::publish(float pressure) {

  msg.fluid_pressure = pressure * 100.0;
  msg.header.stamp.sec = NS_TO_S(rmw_uros_epoch_nanos());
  msg.header.stamp.nanosec = NS_REMAINDER(rmw_uros_epoch_nanos());
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}
