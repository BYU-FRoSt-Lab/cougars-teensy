#ifndef GPS_PUB
#define GPS_PUB

#include "publisher.h"
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <sensor_msgs/msg/nav_sat_fix.h>

class GPSPub : Publisher {

public:
  void setup(rcl_node_t node);
  void publish(float latitude, float longitude);
  using Publisher::destroy;

private:
  sensor_msgs__msg__NavSatFix msg;
};

#endif // GPS_PUB