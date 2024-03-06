# Crontab runs this script on boot! Be very careful editing it.
# Run-time errors are logged to ~/logs/crontab

import lgpio

CONTROL_PROGRAM_PIN = 24
SENSORS_PROGRAM_PIN = 21

# open the GPIO chip
h = lgpio.gpiochip_open(0)
lgpio.gpio_claim_output(h, CONTROL_PROGRAM_PIN)
lgpio.gpio_claim_output(h, SENSORS_PROGRAM_PIN)

# set the GPIO pins high
lgpio.gpio_write(h, CONTROL_PROGRAM_PIN, 1)
lgpio.gpio_write(h, SENSORS_PROGRAM_PIN, 1)

# set the .txt files to start OFF
with open('sensors_state.txt', 'w') as f:
    f.write('OFF')

with open('control_state.txt', 'w') as f:
    f.write('OFF')
