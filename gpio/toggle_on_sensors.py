import time
import lgpio

SENSORS_PIN = 22

# Read value from file
with open('sensors_state.txt', 'r') as f:
    toggle_state = f.read()

if toggle_state == 'OFF':

    toggle_state = 'ON'

    # open the gpio chip and set the sensors on/off pin as output
    h = lgpio.gpiochip_open(0)
    lgpio.gpio_claim_output(h, SENSORS_PIN)

    # Turn the GPIO pin on
    lgpio.gpio_write(h, SENSORS_PIN, 0)
    time.sleep(2)

    # Turn the GPIO pin off
    lgpio.gpio_write(h, SENSORS_PIN, 1)

    # Write updated value back to file
    with open('sensors_state.txt', 'w') as f:
        f.write(str(toggle_state))

    print("Sensors Teensy is turning %s..." %(toggle_state))
    time.sleep(5)

print("COMPLETE: Sensors Teensy is %s" %(toggle_state))