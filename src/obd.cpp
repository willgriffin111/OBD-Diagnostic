#include "obd.hpp"

OBD obd;

OBD::OBD() {}

bool OBD::initialise() {
    Serial1.begin(9600);      
    return init(PROTO_AUTO);
}

bool OBD::readTroubleCodes(String& dtcList) {
    uint16_t codes[10];
    byte count = readDTC(codes, 10);
    dtcList = "";

    for (byte i = 0; i < count; i++) {
        uint16_t code = codes[i];
        if (code == 0) continue;

        char type;
        switch ((code & 0xC000) >> 14) {
            case 0: type = 'P'; break;
            case 1: type = 'C'; break;
            case 2: type = 'B'; break;
            case 3: type = 'U'; break;
        }

        uint16_t number = code & 0x3FFF;
        char dtc[8];
        snprintf(dtc, sizeof(dtc), "%c%04X", type, number);
        dtcList += dtc;
        dtcList += "\n";
    }

    return count > 0;
}
