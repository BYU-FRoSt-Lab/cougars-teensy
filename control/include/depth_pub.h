#ifndef DEPTH_PUB
#define DEPTH_PUB

#include "publisher.h"
#include <MS5837.h>
#include <sensor_msgs/msg/fluid_pressure.h>

class DepthPub : Publisher {

public:
  void setup(rcl_node_t node);
  void publish(float pressure);
  using Publisher::destroy;

private:
  sensor_msgs__msg__FluidPressure msg;
};

#endif // DEPTH_PUB