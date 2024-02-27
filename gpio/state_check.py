with open('control_state.txt', 'r') as f:
    print("Control Teensy: %s" %(str(f.read())))

with open('sensors_state.txt', 'r') as f:
    print("Sensors Teensy: %s" %(str(f.read())))