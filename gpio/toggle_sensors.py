import time
import lgpio

SENSORS_PIN = 22

# open the gpio chip and set the LED pin as output
h = lgpio.gpiochip_open(0)
lgpio.gpio_claim_output(h, SENSORS_PIN)

# Turn the GPIO pin on
lgpio.gpio_write(h, SENSORS_PIN, 1)
time.sleep(2)

# Turn the GPIO pin off
lgpio.gpio_write(h, SENSORS_PIN, 0)
