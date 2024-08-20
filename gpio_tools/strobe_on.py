##########################################################
# TURN ON THE STROBE LIGHT
##########################################################

import time
import gpiod

STROBE_PIN = 15
STROBE_RATE = 1 # seconds

# open the gpio chip and set the control program pin as output
chip = gpiod.Chip('gpiochip4')
strobe_line = chip.get_line(STROBE_PIN)
strobe_line.request(consumer="STROBE", type=gpiod.LINE_REQ_DIR_OUT)

def blink_strobe():
    try:
        while True:
            strobe_line.set_value(1)
            time.sleep(STROBE_RATE)
            strobe_line.set_value(0)
            time.sleep(STROBE_RATE)
    finally:
        strobe_line.set_value(0)
        strobe_line.release()
        

if __name__ == "__main__":
    blink_strobe()
