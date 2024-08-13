#ifndef LEAK_PUB
#define LEAK_PUB

#include "publisher.h"
#include <frost_interfaces/msg/leak_status.h>
#include <std_msgs/msg/bool.h>

class LeakPub : Publisher {

public:
  void setup(rcl_node_t node);
  void publish(bool leak);
  using Publisher::destroy;

private:
  frost_interfaces__msg__LeakStatus msg;
};

#endif // LEAK_PUB