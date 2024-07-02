#ifndef IMU_PUB
#define IMU_PUB

#include "SparkFun_BNO08x_Arduino_Library.h"
#include "publisher.h"
#include <frost_interfaces/msg/imu.h>

class IMUPub : Publisher {

public:
  void setup(rcl_node_t node);
  void update(float roll, float pitch, float yaw);
  void publish();
  using Publisher::destroy;

private:
  frost_interfaces__msg__IMU msg;
};

#endif // IMU_PUB