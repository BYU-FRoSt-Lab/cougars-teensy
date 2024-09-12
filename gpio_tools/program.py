##########################################################
# PUTS THE TEENSY INTO PROGRAM MODE
# - If the Teensy is on, a red light should light up on
#   the board after running this script
##########################################################

import time
import gpiod

from gpio_def import *

# open the gpio chip and set the program pin as output
chip = gpiod.Chip('/dev/gpiochip4')
prog_line = chip.get_line(PROGRAM_PIN)
prog_line.request(consumer="PROG", type=gpiod.LINE_REQ_DIR_OUT)

print("Entering program mode on the Teensy . . .")

# set the GPIO pin low then high
prog_line.set_value(0)
time.sleep(2)
prog_line.set_value(1)

print(". . .")
time.sleep(5)

print("COMPLETE: Entered program mode on the Teensy")
prog_line.release()
