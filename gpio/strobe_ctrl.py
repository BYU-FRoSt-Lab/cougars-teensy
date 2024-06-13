import gpiod
import time
import os

stop = "stop_flag.txt"

LED_PIN = 15 #RX pin of pi
chip = gpiod.Chip('gpiochip4')
led_line = chip.get_line(LED_PIN)
led_line.request(consumer="LED", type=gpiod.LINE_REQ_DIR_OUT)

def blink():
    try:
        while True:
            if not os.path.exists(stop):
                led_line.set_value(1)
                time.sleep(1)
                led_line.set_value(0)
                time.sleep(1)
            else:
                led_line.set_value(0)
                break
    finally:
        led_line.release()
        

if __name__ == "__main__":
    blink()