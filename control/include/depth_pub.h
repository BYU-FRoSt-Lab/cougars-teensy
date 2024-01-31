#ifndef DEPTH_PUB
#define DEPTH_PUB

#include "publisher.h"
#include <frost_interfaces/msg/depth.h>
#include <std_msgs/msg/float64.h>
#include <MS5837.h>

class DepthPub : Publisher {

public:
  void setup(rcl_node_t node);
  void update(float pressure, float depth, float temperature);
  void publish();
  using Publisher::destroy;

private:
  frost_interfaces__msg__Depth msg;
};

#endif // DEPTH_PUB