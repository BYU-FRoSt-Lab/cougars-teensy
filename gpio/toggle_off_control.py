import time
import gpiod

CONTROL_PIN = 27

# Read value from file
with open('control_state.txt', 'r') as f:
    toggle_state = f.read()

if toggle_state == 'ON':

    toggle_state = 'OFF'

    # open the gpio chip and set the control on/off pin as output
    chip = gpiod.Chip('gpiochip4')
    control_line = chip.get_line(CONTROL_PIN)
    control_line.request(consumer="CONTROL", type=gpiod.LINE_REQ_DIR_OUT)

    # Set the GPIO pin low
    control_line.set_value(0)
    time.sleep(2)

    # Set the GPIO pin high
    control_line.set_value(1)

    # Write updated value back to file
    with open('control_state.txt', 'w') as f:
        f.write(str(toggle_state))

    print("Control Teensy is turning %s..." %(toggle_state))
    time.sleep(5)

    control_line.release()
    
print("COMPLETE: Control Teensy is %s" %(toggle_state))
