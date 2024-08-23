##########################################################
# TOGGLES THE SENSORS PROGRAM PIN
# - If sensors is on, a red light should light up on the
#   board after running this script
##########################################################

import time
import gpiod

SENSORS_PROGRAM_PIN = 23

# open the gpio chip and set the sensors program pin as output
chip = gpiod.Chip('/dev/gpiochip4')
sensors_prog_line = chip.get_line(SENSORS_PROGRAM_PIN)
sensors_prog_line.request(consumer="SENSORS_PROG", type=gpiod.LINE_REQ_DIR_OUT)

# set the GPIO pin low then high
sensors_prog_line.set_value(0)
time.sleep(2)
sensors_prog_line.set_value(1)

print("Entering program mode on the Sensors Teensy...")
time.sleep(5)

sensors_prog_line.release()
print("COMPLETE: Entered program mode on the Sensors Teensy")
