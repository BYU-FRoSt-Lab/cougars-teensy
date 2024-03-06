import time
import lgpio

CONTROL_PROGRAM_PIN = 24

# open the gpio chip and set the control program pin as output
h = lgpio.gpiochip_open(0)
lgpio.gpio_claim_output(h, CONTROL_PROGRAM_PIN)

# Set the GPIO pin low
lgpio.gpio_write(h, CONTROL_PROGRAM_PIN, 0)
time.sleep(2)

# Set the GPIO pin high
lgpio.gpio_write(h, CONTROL_PROGRAM_PIN, 1)

print("Entering program mode on the Control Teensy...")
time.sleep(5)

print("COMPLETE: Entered program mode on the Control Teensy")
