#ifndef DVL_PUB
#define DVL_PUB

#include "publisher.h"
#include <frost_interfaces/msg/dvl.h>
// ADD DRIVER

class DVLPub : Publisher {

public:
  void setup(rcl_node_t node);
  void update(__cpp_raw_strings wrz, __cpp_raw_strings wrp, __cpp_raw_strings wru);
  void publish();
  using Publisher::destroy;

private:
  frost_interfaces__msg__DVL msg;
};

#endif // DVL_PUB