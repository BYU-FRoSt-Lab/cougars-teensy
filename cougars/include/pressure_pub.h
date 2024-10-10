#ifndef PRESSURE_PUB
#define PRESSURE_PUB

#include "publisher.h"
#include <MS5837.h>
#include <sensor_msgs/msg/fluid_pressure.h>

/**
 * @author Nelson Durrant
 * @date September 2024
 * @brief Publisher for pressure sensor data.
 */
class PressurePub : Publisher {

public:

  /**
   * @brief This function sets up the pressure publisher.
   * 
   * @param node the micro-ROS node
   */
  void setup(rcl_node_t node);

  /**
   * @brief This function publishes the pressure data to the micro-ROS agent.
   * 
   * @param pressure the pressure data (TODO: add units)
   */
  void publish(float pressure);
  using Publisher::destroy;

private:
  sensor_msgs__msg__FluidPressure msg;
};

#endif // PRESSURE_PUB