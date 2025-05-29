#include "obd.hpp"

OBD obd;

OBD::OBD() {}

bool OBD::initialise() {
    Serial1.begin(9600);
    return init(PROTO_AUTO);
}

bool OBD::sendPIDCommand(const char* pid, char* response, int bufsize) {
    unsigned long startTime = millis();
    write(pid);
    while (millis() - startTime < OBD_TIMEOUT_LONG) {
        int len = receive(response, bufsize, OBD_TIMEOUT_LONG);
        if (len > 0) return true;
    }
    return false;
}

bool OBD::readTroubleCodes(String& dtcList) {
    char response[128];
    dtcList = "";

    if (!sendPIDCommand("03", response, sizeof(response))) return false;

    char* ptr = strstr(response, "43");
    if (!ptr) return false;

    ptr += 2;  // Skip "43"
    while (*ptr && *(ptr + 1)) {
        while (*ptr == ' ') ptr++;

        char hex[5] = {0};
        for (int i = 0; i < 4 && *ptr; ++i, ++ptr) hex[i] = *ptr;

        if (strcmp(hex, "0000") == 0) continue;

        char type;
        int byte1 = strtol(hex, NULL, 16);
        switch ((byte1 & 0xC000) >> 14) {
            case 0: type = 'P'; break;
            case 1: type = 'C'; break;
            case 2: type = 'B'; break;
            case 3: type = 'U'; break;
        }

        int code = byte1 & 0x3FFF;
        char dtc[8];
        snprintf(dtc, sizeof(dtc), "%c%04X", type, code);
        dtcList += dtc;
        dtcList += "\n";
    }

    return !dtcList.isEmpty();
}
