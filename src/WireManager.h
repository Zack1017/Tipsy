#ifndef WIRE_MANAGER_H
#define WIRE_MANAGER_H

#include <Wire.h>

class WireManager {
public:
    static void begin();                     // Call once in setup
    static TwoWire& getWire1();              // Use this in other classes

private:
    static bool initialized;
};

#endif
