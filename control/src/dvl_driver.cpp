#include "dvl_driver.h"

DVL::DVL() {

    this->roll = 0.0;
    this->pitch = 0.0;
    this->yaw = 0.0;
    this->wrz = "";
    this->wrp = "";
    this->wru = "";
}

DVL::dvl_setup() {

    dvlSerial.begin(115200);
}

DVL::dvl_update() {

    String dataString;
    char incomingByte;

    if (dvlSerial.available() > 0) {
        incomingByte = dvlSerial.read();
        if (incomingByte != '\n') {
            dataString += (char)incomingByte;
        } else {
            // Process complete message
            char identifier = dataString[2];
            if (identifier == 'p') {
                int numFields = 0;
                int startIndex = 3;  // Skip identifier "p" and comma
                // float roll, pitch, yaw; ADD TO CLASS
                for (int i = startIndex; i < dataString.length(); i++) {
                    if (dataString[i] == ',') {
                        numFields++;
                        if (numFields == 7) {
                            this->roll = dataString.substring(startIndex, i).toFloat();
                        } else if (numFields == 8) {
                            this->pitch = dataString.substring(startIndex, i).toFloat();
                        } else if (numFields == 9) {
                            this->yaw = dataString.substring(startIndex, i).toFloat();
                        }
                        startIndex = i + 1;  // Update start index for next field
                    }
                }
            }

            // decide string type and save to class (wrz, wrp, wru)

            dataString = "";  // Clear data for next message
        }
    }
}
