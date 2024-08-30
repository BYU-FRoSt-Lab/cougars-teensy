##########################################################
# PUTS THE COUGARS TEENSY INTO PROGRAM MODE
# - If cougars is on, a red light should light up on the
#   board after running this script
##########################################################

import time
import gpiod

from gpio_def import *

# open the gpio chip and set the cougars program pin as output
chip = gpiod.Chip('/dev/gpiochip4')
cougars_prog_line = chip.get_line(COUGARS_PROGRAM_PIN)
cougars_prog_line.request(consumer="COUGARS_PROG", type=gpiod.LINE_REQ_DIR_OUT)

print("Entering program mode on the Control Teensy . . .")

# set the GPIO pin low then high
cougars_prog_line.set_value(0)
time.sleep(2)
cougars_prog_line.set_value(1)

print(". . .")
time.sleep(5)

print("COMPLETE: Entered program mode on the Control Teensy")
cougars_prog_line.release()
