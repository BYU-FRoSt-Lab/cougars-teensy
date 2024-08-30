##########################################################
# TURNS ON THE STROBE LIGHT
##########################################################

import time
import gpiod

from gpio_def import *

# open the gpio chip and set the control program pin as output
chip = gpiod.Chip('/dev/gpiochip4')
strobe_line = chip.get_line(STROBE_PIN)
strobe_line.request(consumer="STROBE", type=gpiod.LINE_REQ_DIR_OUT)

try:
    while True:
        strobe_line.set_value(1)
        time.sleep(STROBE_RATE)
        strobe_line.set_value(0)
        time.sleep(STROBE_RATE)
finally:
    print("COMPLETE: Strobe light is now off")
    strobe_line.set_value(0)
    strobe_line.release()
