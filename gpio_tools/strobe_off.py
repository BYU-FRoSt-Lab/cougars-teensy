##########################################################
# TURNS OFF THE STROBE LIGHT
##########################################################

import gpiod

from gpio_def import *

# open the gpio chip and set the control program pin as output
chip = gpiod.Chip('/dev/gpiochip4')
strobe_line = chip.get_line(STROBE_PIN)
strobe_line.request(consumer="STROBE", type=gpiod.LINE_REQ_DIR_OUT)

print("COMPLETE: Strobe light is now off")

strobe_line.set_value(0)
strobe_line.release()
