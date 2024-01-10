#ifndef DEPTH_PUB
#define DEPTH_PUB

#include "publisher.h"
#include <MS5837.h>
#include <Wire.h>
#include <frost_interfaces/msg/depth.h>
#include <std_msgs/msg/float64.h>

class DepthPub : Publisher {

public:
  void setup(rcl_node_t node);
  void depth_setup();
  void depth_update();
  void publish();
  using Publisher::destroy;

private:
  void pressure_calibrate();
  frost_interfaces__msg__Depth msg;

};

#endif // DEPTH_PUB