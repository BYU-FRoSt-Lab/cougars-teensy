######################################################################
# NOTICE: this file runs automatically on Pi bootup                  #
# BE VERY CAREFUL EDITING IT OR THE NAMES OF THE FILES IT REFERENCES #
######################################################################

with open('sensors_state.txt', 'w') as f:
    f.write('OFF')

with open('control_state.txt', 'w') as f:
    f.write('OFF')


