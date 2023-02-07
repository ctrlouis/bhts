#ifndef TemperatureSensor_h
#define TemperatureSensor_h
// #include "Arduino.h"

class TemperatureSensor {
  public:
    int pin;
    String location;
    float temperature;    
    TemperatureSensor(int p, String l) {
      pin = p;
      location = l;      
      temperature = 999;
    }
};
#endif