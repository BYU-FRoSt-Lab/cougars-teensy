#####################################################
# NOTICE: this file runs automatically on Pi bootup #
#####################################################

with open('sensors_state.txt', 'w') as f:
    f.write('OFF')

with open('control_state.txt', 'w') as f:
    f.write('OFF')


