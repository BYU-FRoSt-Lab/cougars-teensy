#include "imu_pub.h"
#include "SparkFun_BNO080_Arduino_Library.h"

static BNO080 myIMU;

void IMUPub::setup(rcl_node_t node) {

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, IMU), "imu_data"));
}

void IMUPub::imu_setup() {

  Wire.begin();
  myIMU.begin(0x4A, Wire);
  Wire.setClock(400000);

  myIMU.enableLinearAccelerometer(50); //Send data update every 50ms
  myIMU.enableRotationVector(50); //Send data update every 50ms
}

void IMUPub::imu_update() {

  if (myIMU.dataAvailable() == true)
  {
    msg.gyro_x = (myIMU.getRoll()) * 180.0 / PI; // Convert roll to degrees
    msg.gyro_y = (myIMU.getPitch()) * 180.0 / PI; // Convert pitch to degrees
    msg.gyro_z = (myIMU.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees

    msg.accel_x = myIMU.getLinAccelX();
    msg.accel_y = myIMU.getLinAccelY();
    msg.accel_z = myIMU.getLinAccelZ();
    // byte linAccuracy = myIMU.getLinAccelAccuracy();
  }

}

void IMUPub::publish() {

  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}