#ifndef GPS_PUB
#define GPS_PUB

#include "publisher.h"
#include <frost_interfaces/msg/gps.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

class GPSPub : Publisher {

public:
  void setup(rcl_node_t node);
  void update(float latitude, float longitude);
  void publish();
  using Publisher::destroy;

private:
  frost_interfaces__msg__GPS msg;
};

#endif // GPS_PUB