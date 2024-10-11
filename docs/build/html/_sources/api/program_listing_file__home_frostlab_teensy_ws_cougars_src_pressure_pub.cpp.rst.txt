
.. _program_listing_file__home_frostlab_teensy_ws_cougars_src_pressure_pub.cpp:

Program Listing for File pressure_pub.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_frostlab_teensy_ws_cougars_src_pressure_pub.cpp>` (``/home/frostlab/teensy_ws/cougars/src/pressure_pub.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "pressure_pub.h"
   
   void PressurePub::setup(rcl_node_t node) {
   
     RCCHECK(rclc_publisher_init_best_effort(
         &publisher, &node,
         ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, FluidPressure),
         "pressure/data"));
   }
   
   void PressurePub::publish(float pressure) {
   
     msg.fluid_pressure = pressure;
     msg.header.stamp.sec = NS_TO_S(rmw_uros_epoch_nanos());
     msg.header.stamp.nanosec = NS_REMAINDER(rmw_uros_epoch_nanos());
     RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
   }
