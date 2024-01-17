#include "publisher.h"
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <frost_interfaces/msg/gps.h>

#define GPS_RATE 38400
#define GPS_RX_PIN 28
#define GPS_TX_PIN 29

class GPSPub : Publisher {

public:

  void setup(rcl_node_t node) {

    // do {

    //   Serial7.begin(GPS_RATE);
    //   if (GNSS.begin(Serial7) == true) break;

    //   delay(100);
    //   Serial7.begin(9600);
    //   if (GNSS.begin(Serial7) == true) {
    //       GNSS.setSerialRate(GPS_RATE);
    //       delay(100);
    //   } else {
    //       // GNSS.factoryReset();
    //       delay(2000);
    //   }
    // } while(1);

    // GNSS.setUART1Output(COM_TYPE_UBX); // Set the UART port to output UBX only
    // GNSS.setI2COutput(COM_TYPE_UBX); // Set the I2C port to output UBX only (turn off NMEA noise)
    // GNSS.saveConfiguration(); // Save the current settings to flash and BBR

    RCCHECK(rclc_publisher_init_best_effort(
        &publisher, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(frost_interfaces, msg, GPS), "gps_data"));
  }

  void publish() {

    // msg.longitude = GNSS.getLongitude();
    // msg.latitude = GNSS.getLatitude();
    // msg.altitude = GNSS.getAltitude();
    // msg.siv = GNSS.getSIV();
    msg.header.stamp.nanosec = rmw_uros_epoch_nanos();
    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
  }

  using Publisher::destroy;

private:

  frost_interfaces__msg__GPS msg;
  SFE_UBLOX_GNSS GNSS;

};
