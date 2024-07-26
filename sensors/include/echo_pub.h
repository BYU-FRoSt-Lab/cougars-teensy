#ifndef ECHO_PUB
#define ECHO_PUB

#include "publisher.h"
#include <ping1d.h>
#include <std_msgs/msg/float64.h>

class EchoPub : Publisher {

public:
  void setup(rcl_node_t node);
  void publish();
  using Publisher::destroy;

private:
  std_msgs__msg__Float64 msg;
};

#endif // ECHO_PUB