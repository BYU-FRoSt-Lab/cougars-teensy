
.. _program_listing_file__home_frostlab_teensy_ws_cougars_include_battery_pub.h:

Program Listing for File battery_pub.h
======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_frostlab_teensy_ws_cougars_include_battery_pub.h>` (``/home/frostlab/teensy_ws/cougars/include/battery_pub.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef BATTERY_PUB
   #define BATTERY_PUB
   
   #include "publisher.h"
   #include <Wire.h>
   #include <frost_interfaces/msg/battery_status.h>
   
   class BatteryPub : Publisher {
   
   public:
     void setup(rcl_node_t node);
   
     void publish(float voltage, float current);
     using Publisher::destroy;
   
   private:
     frost_interfaces__msg__BatteryStatus msg;
   };
   
   #endif // BATTERY_PUB
