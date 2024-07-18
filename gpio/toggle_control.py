##########################################################
# TOGGLES THE CONTROL TEENSY
##########################################################

import time
import gpiod

CONTROL_PIN = 27

# open the gpio chip and set the control on/off pin as output
chip = gpiod.Chip('gpiochip4')
control_line = chip.get_line(CONTROL_PIN)
control_line.request(consumer="CONTROL", type=gpiod.LINE_REQ_DIR_OUT)

print("Toggling Control Teensy Power")

# Set the GPIO pin low and then high
control_line.set_value(0)
time.sleep(5)
control_line.set_value(1)

print(". . .")
time.sleep(5)
print("Toggle Complete")

control_line.release()
