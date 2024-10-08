#ifndef PRESSURE_PUB
#define PRESSURE_PUB

#include "publisher.h"
#include <MS5837.h>
#include <sensor_msgs/msg/fluid_pressure.h>

/**
 * @brief Publisher for pressure sensor data.
 * @author Nelson Durrant
 * @date September 2024
 * 
 * This class is a publisher for the pressure sensor data. It publishes the
 * pressure data to the micro-ROS agent.
 */
class PressurePub : Publisher {

public:

  /**
   * @brief Set up the pressure publisher
   * 
   * This function sets up the pressure publisher.
   * 
   * @param node the micro-ROS node
   */
  void setup(rcl_node_t node);

  /**
   * @brief Publish the pressure data
   * 
   * This function publishes the pressure data to the micro-ROS agent.
   * 
   * @param pressure the pressure data (TODO: add units)
   */
  void publish(float pressure);
  using Publisher::destroy;

private:
  sensor_msgs__msg__FluidPressure msg;
};

#endif // PRESSURE_PUB