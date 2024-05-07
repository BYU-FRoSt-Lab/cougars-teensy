##########################################################
# CHECK THE VIRTUAL STATE OF CONTROL AND SENSORS
# - If the reported states don't match the physical
#   boards, rebooting the RPi and starting a new docker
#   container should fix the issue
##########################################################

with open('control_state.txt', 'r') as f:
    print("Control Teensy: %s" %(str(f.read())))

with open('sensors_state.txt', 'r') as f:
    print("Sensors Teensy: %s" %(str(f.read())))