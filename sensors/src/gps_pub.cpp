#include "gps_pub.h"

#define GPS_RATE 115200
#define GPS_RX_PIN 28
#define GPS_TX_PIN 29

SFE_UBLOX_GNSS GNSS;
long lastTime = 0;

void GPSPub::setup(rcl_node_t node) {

  do {
    Serial7.begin(38400);
    if (GNSS.begin(Serial7) == true) break;

    delay(100);
    Serial7.begin(9600);
    if (GNSS.begin(Serial7) == true) {
        GNSS.setSerialRate(38400);
        delay(100);
    } else {
        // GNSS.factoryReset();
        delay(2000);
    }
  } while(1);

  GNSS.setUART1Output(COM_TYPE_UBX); // Set the UART port to output UBX only
  GNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  GNSS.saveConfiguration(); //Save the current settings to flash and BBR

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, GPS), "gps_data"));
}

void GPSPub::publish() {

  if (millis() - lastTime > 1000) {

    lastTime = millis();

    // msg.longitude = GNSS.getLongitude();
    // msg.latitude = GNSS.getLatitude();
    // msg.altitude = GNSS.getAltitude();
    // msg.siv = GNSS.getSIV();
    msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
  }
}
