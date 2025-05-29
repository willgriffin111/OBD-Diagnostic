#ifndef OBD_HPP
#define OBD_HPP

#include <SparkfunOBD2UART.h>
#include <Arduino.h>

class OBD : public COBD {
public:
    OBD();
    bool initialise();
    bool readTroubleCodes(String& dtcList);

private:
    bool sendPIDCommand(const char* pid, char* response, int bufsize);
};

extern OBD obd;

#endif
