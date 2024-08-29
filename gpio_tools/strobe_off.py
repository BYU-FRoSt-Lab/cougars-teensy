##########################################################
# TURNS OFF THE STROBE LIGHT
##########################################################

import gpiod

STROBE_PIN = 15

# open the gpio chip and set the control program pin as output
chip = gpiod.Chip('/dev/gpiochip4')
strobe_line = chip.get_line(STROBE_PIN)
strobe_line.request(consumer="STROBE", type=gpiod.LINE_REQ_DIR_OUT)
        

if __name__ == "__main__":
    strobe_line.set_value(0)
    strobe_line.release()
