
.. _program_listing_file__home_frostlab_teensy_ws_cougars_src_battery_pub.cpp:

Program Listing for File battery_pub.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_frostlab_teensy_ws_cougars_src_battery_pub.cpp>` (``/home/frostlab/teensy_ws/cougars/src/battery_pub.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "battery_pub.h"
   
   void BatteryPub::setup(rcl_node_t node) {
   
     RCCHECK(rclc_publisher_init_best_effort(
         &publisher, &node,
         ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, BatteryStatus),
         NAMESPACE "battery/data"));
   }
   
   void BatteryPub::publish(float voltage, float current) {
   
     msg.voltage = voltage;
     msg.current = current;
     msg.header.stamp.sec = NS_TO_S(rmw_uros_epoch_nanos());
     msg.header.stamp.nanosec = NS_REMAINDER(rmw_uros_epoch_nanos());
     RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
   }
