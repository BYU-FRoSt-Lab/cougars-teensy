##########################################################
# INITS PINS AND SETS CURRENT POWER STATES TO "ON"
# - The docker container runs this script on startup!
#   Be careful editing or moving it
##########################################################

import time
import gpiod

from gpio_def import *

# open the GPIO chip
chip = gpiod.Chip('/dev/gpiochip4')
cougars_power_line = chip.get_line(COUGARS_POWER_PIN)
cougars_prog_line = chip.get_line(COUGARS_PROGRAM_PIN)
strobe_line = chip.get_line(STROBE_PIN)

cougars_power_line.request(consumer="COUGARS_POWER", type=gpiod.LINE_REQ_DIR_OUT)
cougars_prog_line.request(consumer="COUGARS_PROG", type=gpiod.LINE_REQ_DIR_OUT)
strobe_line.request(consumer="STROBE", type=gpiod.LINE_REQ_DIR_OUT)

print("Initializing GPIO pins . . .")

# set the strobe GPIO pin low
strobe_line.set_value(0)

# set the Teensy GPIO pins high
cougars_power_line.set_value(1)
cougars_prog_line.set_value(1)

# turn the Teensy board on
time.sleep(5)
cougars_power_line.set_value(0)
print(". . .")
time.sleep(5)
cougars_power_line.set_value(1)

print("COMPLETE: GPIO pins initialized")
cougars_power_line.release()
cougars_prog_line.release()
strobe_line.release()
