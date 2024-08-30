##########################################################
# TOGGLES THE SENSORS TEENSY POWER
# - Probably don't run these scripts unless you know what
#   you're doing; they mess with the "init.sh" setup
##########################################################

import time
import gpiod

from gpio_def import *

# open the gpio chip and set the control on/off pin as output
chip = gpiod.Chip('/dev/gpiochip4')
sensors_power_line = chip.get_line(SENSORS_POWER_PIN)
sensors_power_line.request(consumer="SENSORS", type=gpiod.LINE_REQ_DIR_OUT)

print("Turning the Sensors Teensy on/off . . .")

# Set the GPIO pin low and then high
sensors_power_line.set_value(0)
time.sleep(5)
sensors_power_line.set_value(1)

print(". . .")
time.sleep(5)
print("COMPLETE: Sensors Teensy is now on/off")

sensors_power_line.release()
