#ifndef IMU_PUB
#define IMU_PUB

#include "publisher.h"
#include <frost_interfaces/msg/imu.h>
#include "SparkFun_BNO08x_Arduino_Library.h"

class IMUPub : Publisher {

public:
  void setup(rcl_node_t node);
  void update(float roll, float pitch, float yaw, float accel_x, float accel_y, float accel_z);
  void publish();
  using Publisher::destroy;

private:
  frost_interfaces__msg__IMU msg;
};

#endif // IMU_PUB