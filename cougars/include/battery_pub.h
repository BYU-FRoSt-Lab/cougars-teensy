#ifndef BATTERY_PUB
#define BATTERY_PUB

#include "publisher.h"
#include <Wire.h>
#include <frost_interfaces/msg/battery_status.h>

/**
 * @brief Publisher for battery sensor data.
 * @author Nelson Durrant
 * @date September 2024
 * 
 * This class is a publisher for the battery sensor data. It publishes the battery
 * data to the micro-ROS agent.
 */
class BatteryPub : Publisher {

public:
  /**
   * @brief Set up the battery publisher
   * 
   * This function sets up the battery publisher.
   * 
   * @param node the micro-ROS node
   */
  void setup(rcl_node_t node);

  /**
   * @brief Publish the battery data
   * 
   * This function publishes the battery data to the micro-ROS agent.
   * 
   * @param voltage the battery voltage (V)
   * @param current the battery current (A)
   */
  void publish(float voltage, float current);
  using Publisher::destroy;

private:
  frost_interfaces__msg__BatteryStatus msg;
};

#endif // BATTERY_PUB