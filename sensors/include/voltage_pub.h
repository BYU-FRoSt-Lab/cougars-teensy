#ifndef VOLTAGE_PUB
#define VOLTAGE_PUB

#include "publisher.h"
#include <Adafruit_INA260.h>
#include <Wire.h>
#include <frost_interfaces/msg/volt.h>

class VoltagePub : Publisher {

public:
  void setup(rcl_node_t node);
  void publish(float voltage, float current);
  using Publisher::destroy;

private:
  frost_interfaces__msg__Volt msg;
};

#endif // VOLTAGE_PUB