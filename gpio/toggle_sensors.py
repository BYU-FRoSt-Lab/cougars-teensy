##########################################################
# TOGGLES THE SENSORS TEENSY POWER
##########################################################

import time
import gpiod

SENSORS_POWER_PIN = 22

# open the gpio chip and set the control on/off pin as output
chip = gpiod.Chip('gpiochip4')
sensors_power_line = chip.get_line(SENSORS_POWER_PIN)
sensors_power_line.request(consumer="SENSORS", type=gpiod.LINE_REQ_DIR_OUT)

print("Toggling Sensors Teensy Power")

# Set the GPIO pin low and then high
sensors_power_line.set_value(0)
time.sleep(5)
sensors_power_line.set_value(1)

print(". . .")
time.sleep(5)
print("Toggle Complete")

sensors_power_line.release()
