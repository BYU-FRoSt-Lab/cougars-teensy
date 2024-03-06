import time
import lgpio

SENSORS_PROGRAM_PIN = 21

# open the gpio chip and set the sensors program pin as output
h = lgpio.gpiochip_open(0)
lgpio.gpio_claim_output(h, SENSORS_PROGRAM_PIN)

# Set the GPIO pin low
lgpio.gpio_write(h, SENSORS_PROGRAM_PIN, 0)
time.sleep(2)

# Set the GPIO pin high
lgpio.gpio_write(h, SENSORS_PROGRAM_PIN, 1)

print("Entering program mode on the Sensors Teensy...")
time.sleep(5)

print("COMPLETE: Entered program mode on the Sensors Teensy...")