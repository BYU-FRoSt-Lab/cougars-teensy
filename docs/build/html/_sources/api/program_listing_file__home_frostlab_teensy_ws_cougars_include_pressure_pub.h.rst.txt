
.. _program_listing_file__home_frostlab_teensy_ws_cougars_include_pressure_pub.h:

Program Listing for File pressure_pub.h
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_frostlab_teensy_ws_cougars_include_pressure_pub.h>` (``/home/frostlab/teensy_ws/cougars/include/pressure_pub.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef PRESSURE_PUB
   #define PRESSURE_PUB
   
   #include "publisher.h"
   #include <MS5837.h>
   #include <sensor_msgs/msg/fluid_pressure.h>
   
   class PressurePub : Publisher {
   
   public:
   
     void setup(rcl_node_t node);
   
     void publish(float pressure);
     using Publisher::destroy;
   
   private:
     sensor_msgs__msg__FluidPressure msg;
   };
   
   #endif // PRESSURE_PUB
