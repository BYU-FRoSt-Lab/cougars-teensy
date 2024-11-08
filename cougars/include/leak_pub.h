#ifndef LEAK_PUB
#define LEAK_PUB

#include "publisher.h"
#include <frost_interfaces/msg/leak_status.h>
#include <std_msgs/msg/bool.h>

/**
 * @author Nelson Durrant
 * @date September 2024
 * 
 * Publisher for leak sensor data.
 */
class LeakPub : Publisher {

public:

  /**
   * This function sets up the leak publisher.
   * 
   * @param node the micro-ROS node
   */
  void setup(rcl_node_t node);

  /**
   * This function publishes the leak data to the micro-ROS agent.
   * 
   * @param leak the leak data (true if leak, false if no leak)
   */
  void publish(bool leak);
  using Publisher::destroy;

private:
  frost_interfaces__msg__LeakStatus msg;
};

#endif // LEAK_PUB