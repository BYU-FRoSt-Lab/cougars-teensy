##########################################################
# INITS PINS AND SETS CURRENT POWER STATES TO "ON"
# - The docker container runs this script on startup!
#   Be careful editing it
##########################################################

import gpiod
import time

CONTROL_PIN = 27
SENSORS_PIN = 22
CONTROL_PROGRAM_PIN = 24
SENSORS_PROGRAM_PIN = 21

# open the GPIO chip
chip = gpiod.Chip('gpiochip4')
control_line = chip.get_line(CONTROL_PIN)
sensors_line = chip.get_line(SENSORS_PIN)
control_prog_line = chip.get_line(CONTROL_PROGRAM_PIN)
sensors_prog_line = chip.get_line(SENSORS_PROGRAM_PIN)

control_line.request(consumer="CONTROL", type=gpiod.LINE_REQ_DIR_OUT)
sensors_line.request(consumer="SENSORS", type=gpiod.LINE_REQ_DIR_OUT)
control_prog_line.request(consumer="CONTROL_PROG", type=gpiod.LINE_REQ_DIR_OUT)
sensors_prog_line.request(consumer="SENSORS_PROG", type=gpiod.LINE_REQ_DIR_OUT)

# set the GPIO pins high
control_line.set_value(1)
sensors_line.set_value(1)
control_prog_line.set_value(1)
sensors_prog_line.set_value(1)

# turn the Teensys on
time.sleep(5)
control_line.set_value(0)
sensors_line.set_value(0)
time.sleep(5)
control_line.set_value(1)
sensors_line.set_value(1)

control_line.release()
sensors_line.release()
control_prog_line.release()
sensors_prog_line.release()
