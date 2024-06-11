#ifndef DVL_PUB
#define DVL_PUB

#include "publisher.h"
#include <frost_interfaces/msg/dvl.h>

class DVLPub : Publisher {

public:
  void setup(rcl_node_t node);
  void update(string wrz, string wrp, string wru);
  void publish();
  using Publisher::destroy;

private:
  frost_interfaces__msg__DVL msg;
};

#endif // DVL_PUB