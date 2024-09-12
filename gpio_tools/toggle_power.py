##########################################################
# TOGGLES THE TEENSY POWER
##########################################################

import time
import gpiod

from gpio_def import *

# open the gpio chip and set the on/off pin as output
chip = gpiod.Chip('/dev/gpiochip4')
power_line = chip.get_line(POWER_PIN)
power_line.request(consumer="POWER", type=gpiod.LINE_REQ_DIR_OUT)

print("Turning the Teensy on/off . . .")

# Set the GPIO pin low and then high
power_line.set_value(0)
time.sleep(5)
power_line.set_value(1)

print(". . .")
time.sleep(5)
print("COMPLETE: Teensy is now on/off")

power_line.release()
