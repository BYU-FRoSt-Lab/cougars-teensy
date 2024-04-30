# Crontab runs this script on boot! Be very careful editing it.
# Run-time errors are logged to ~/logs/crontab
# THE ABOVE MIGHT BE OUTDATED

import gpiod

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

# set the .txt files to start OFF
with open('sensors_state.txt', 'w') as f:
    f.write('OFF')

with open('control_state.txt', 'w') as f:
    f.write('OFF')
