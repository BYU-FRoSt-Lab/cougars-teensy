##########################################################
# TOGGLES THE SENSORS TEENSY
##########################################################

import time
import gpiod

SENSORS_PIN = 22

# open the gpio chip and set the control on/off pin as output
chip = gpiod.Chip('gpiochip4')
control_line = chip.get_line(SENSORS_PIN)
control_line.request(consumer="SENSORS", type=gpiod.LINE_REQ_DIR_OUT)

print("Toggling Sensors Teensy Power")

# Set the GPIO pin low and then high
control_line.set_value(0)
time.sleep(5)
control_line.set_value(1)

print(". . .")
time.sleep(5)
print("Toggle Complete")

control_line.release()
