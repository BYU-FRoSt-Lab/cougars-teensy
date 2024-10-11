
.. _program_listing_file__home_frostlab_teensy_ws_cougars_include_leak_pub.h:

Program Listing for File leak_pub.h
===================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_frostlab_teensy_ws_cougars_include_leak_pub.h>` (``/home/frostlab/teensy_ws/cougars/include/leak_pub.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef LEAK_PUB
   #define LEAK_PUB
   
   #include "publisher.h"
   #include <frost_interfaces/msg/leak_status.h>
   #include <std_msgs/msg/bool.h>
   
   class LeakPub : Publisher {
   
   public:
   
     void setup(rcl_node_t node);
   
     void publish(bool leak);
     using Publisher::destroy;
   
   private:
     frost_interfaces__msg__LeakStatus msg;
   };
   
   #endif // LEAK_PUB
