import time
import lgpio

SENSORS_PROGRAM_PIN = 21

# open the gpio chip and set the sensors program pin as output
h = lgpio.gpiochip_open(0)
lgpio.gpio_claim_output(h, SENSORS_PROGRAM_PIN)

# Turn the GPIO pin on
lgpio.gpio_write(h, SENSORS_PROGRAM_PIN, 1)
time.sleep(2)

# Turn the GPIO pin off
lgpio.gpio_write(h, SENSORS_PROGRAM_PIN, 0)

print("Entering program mode on the Sensors Teensy...")
time.sleep(5)

print("COMPLETE: Entered program mode on the Sensors Teensy...")