import time
import lgpio

CONTROL_PIN = 27

# Read value from file
with open('control_state.txt', 'r') as f:
    toggle_state = f.read()

if toggle_state == 'ON':

    toggle_state = 'OFF'

    # open the gpio chip and set the control on/off pin as output
    h = lgpio.gpiochip_open(0)
    lgpio.gpio_claim_output(h, CONTROL_PIN)

    # Set the GPIO pin low
    lgpio.gpio_write(h, CONTROL_PIN, 0)
    time.sleep(2)

    # Set the GPIO pin high
    lgpio.gpio_write(h, CONTROL_PIN, 1)

    # Write updated value back to file
    with open('control_state.txt', 'w') as f:
        f.write(str(toggle_state))

    print("Control Teensy is turning %s..." %(toggle_state))
    time.sleep(5)

print("COMPLETE: Control Teensy is %s" %(toggle_state))
