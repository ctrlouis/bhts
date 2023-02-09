#ifndef Led_h
#define Led_h
#include "Arduino.h"

class Led {

  private:
    int pin;
    int position;
    int state;

    void setState();

  public:
    
    Led(int p, int pos, int initialState) {
      pin = p;
      position = pos;
      state = initialState;
      pinMode(pin, position);
      digitalWrite(pin, state);
    }

    void high();
    void low();

    int getPin();
    int getPosition();
    int getState();
    
};

void Led::high() {
  state = HIGH;
  digitalWrite(pin, state);
};

void Led::low() {
  state = LOW;
  digitalWrite(pin, state);
}

int Led::getPin() {
  return pin;
}

int Led::getPosition() {
  return position;
}

int Led::getState() {
  return state;
}

void Led::setState() {
  digitalWrite(pin, state);
}
#endif