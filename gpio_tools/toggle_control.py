##########################################################
# TOGGLES THE CONTROL TEENSY POWER
##########################################################

import time
import gpiod

CONTROL_POWER_PIN = 27

# open the gpio chip and set the control on/off pin as output
chip = gpiod.Chip('gpiochip4')
control_power_line = chip.get_line(CONTROL_POWER_PIN)
control_power_line.request(consumer="CONTROL", type=gpiod.LINE_REQ_DIR_OUT)

print("Toggling Control Teensy Power")

# Set the GPIO pin low and then high
control_power_line.set_value(0)
time.sleep(5)
control_power_line.set_value(1)

print(". . .")
time.sleep(5)
print("Toggle Complete")

control_power_line.release()
