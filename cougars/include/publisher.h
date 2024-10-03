#ifndef PUBLISHER
#define PUBLISHER

#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>

#define RCCHECK(fn)                                                            \
  {                                                                            \
    rcl_ret_t temp_rc = fn;                                                    \
    if ((temp_rc != RCL_RET_OK)) {                                             \
      error_loop();                                                            \
    }                                                                          \
  }
#define RCSOFTCHECK(fn)                                                        \
  {                                                                            \
    rcl_ret_t temp_rc = fn;                                                    \
    if ((temp_rc != RCL_RET_OK)) {                                             \
    }                                                                          \
  }

/**
 * @author Nelson Durrant
 * @date September 2024
 * @brief Base class for publishers
 * 
 * This class is a base class for publishers.
 */
class Publisher {

public:
  /**
   * @brief Set up the publisher
   * 
   * This function sets up the publisher. It must be implemented by the derived
   * class.
   * 
   * @param node the micro-ROS node
   */
  virtual void setup(rcl_node_t node) = 0;

  /**
   * @brief Destroy the publisher
   * 
   * This function destroys the publisher.
   * 
   * @param node the micro-ROS node
   */
  void destroy(rcl_node_t node) {
    RCCHECK(rcl_publisher_fini(&publisher, &node));
  }

protected:
  rcl_publisher_t publisher;

  void error_loop() {
    while (1) {
      delay(100);
    }
  }
};

#endif // PUBLISHER