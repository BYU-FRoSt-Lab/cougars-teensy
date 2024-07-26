#ifndef DVL_PUB
#define DVL_PUB

#include "publisher.h"
#include <frost_interfaces/msg/dvlstrings.h>
#include <std_msgs/msg/string.h>

class DVLPub : Publisher {

public:
  void setup(rcl_node_t node);
  void publish(String wrz, String wrp, String wru);
  using Publisher::destroy;

private:
  frost_interfaces__msg__DVLStrings msg;
};

#endif // DVL_PUB