import time
import gpiod

SENSORS_PIN = 22

# Read value from file
with open('sensors_state.txt', 'r') as f:
    toggle_state = f.read()

if toggle_state == 'OFF':

    toggle_state = 'ON'

    # open the gpio chip and set the sensors on/off pin as output
    chip = gpiod.Chip('gpiochip4')
    sensors_line = chip.get_line(SENSORS_PIN)
    sensors_line.request(consumer="SENSORS", type=gpiod.LINE_REQ_DIR_OUT)

    # Set the GPIO pin low
    sensors_line.set_value(0)
    time.sleep(2)

    # Set the GPIO pin high
    sensors_line.set_value(1)

    # Write updated value back to file
    with open('sensors_state.txt', 'w') as f:
        f.write(str(toggle_state))

    print("Sensors Teensy is turning %s..." %(toggle_state))
    time.sleep(5)

print("COMPLETE: Sensors Teensy is %s" %(toggle_state))
