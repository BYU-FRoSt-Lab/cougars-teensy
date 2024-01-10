#include "gps_pub.h"

#define GPS_RATE 9600
#define GPS_RX_PIN 28
#define GPS_TX_PIN 29

SoftwareSerial gps_serial = SoftwareSerial(GPS_RX_PIN, GPS_TX_PIN);
SFE_UBLOX_GNSS GNSS;

void GPSPub::setup(rcl_node_t node) {
  gps_serial.begin(GPS_RATE);
  GNSS.begin(gps_serial);

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, GPS), "gps_data"));
}

void GPSPub::publish() {
  msg.longitude = GNSS.getLongitude();
  msg.latitude = GNSS.getLatitude();
  msg.altitude = GNSS.getAltitude();
  msg.siv = GNSS.getSIV();
  msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
}
