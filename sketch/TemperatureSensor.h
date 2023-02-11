#ifndef TemperatureSensor_h
#define TemperatureSensor_h
#include "arduino_env.h"

class TemperatureSensor {

  private:
    int pin;
    String location;
    float temperature;

  public:
    TemperatureSensor(int p, String l) {
      pin = A0;
      location = l;      
      temperature = 999;
    }

    void readTemperature();
    String toString();

    int getPin();
    String getLocation();
    float getTemperature();
};

void TemperatureSensor::readTemperature() {
  int sensorVal;
  float voltage;
  
  sensorVal = analogRead(pin);
  voltage = (sensorVal/ANALOG_MAX_VALUE) * SENSOR_VOLTAGE;
  temperature = (voltage - .5) * 100;
}

String TemperatureSensor::toString() {
  String message;

  message += "pin: ";
  message += String(pin);
  message += " | ";
  message += "location: ";
  message += String(location);
  message += " | ";
  message += "temperature: ";
  message += String(temperature);
  message += "°C";
  return message;
}

int TemperatureSensor::getPin() {
  return pin;
}

String TemperatureSensor::getLocation() {
  return location;
}

float TemperatureSensor::getTemperature() {
  return temperature;
}

#endif