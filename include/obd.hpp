#ifndef OBD_HPP
#define OBD_HPP

#include <SparkfunOBD2UART.h>
#include <Arduino.h>

class OBD : public COBD {
public:
    OBD();
    bool initialise();
    bool readTroubleCodes(String& dtcList);
};

extern OBD obd;

#endif
