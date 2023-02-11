#ifndef Led_h
#define Led_h

class Led {

  private:
    int pin;
    int direction;
    int state;

  public:
    Led(int p, int d, int initialState = LOW) {
      pin = p;
      direction = d;
      state = initialState;
      pinMode(pin, direction);
      if (direction == OUTPUT) {
        digitalWrite(pin, state);
      }
    }

    void high();
    void low();
    void toggle();
    int read();
    bool isHigh();
    bool isLow();

    int getPin();
    int getDirection();
    int getLastState();
    void setState();
    
};

void Led::high() {
  state = HIGH;
  digitalWrite(pin, state);
};

void Led::low() {
  state = LOW;
  digitalWrite(pin, state);
}

void Led::toggle() {
  switch (state) {
    case LOW:
      state = HIGH;
      break;
    case HIGH:
      state = LOW;
      break;
  }
  digitalWrite(pin, state);
}

int Led::read() {
  state = digitalRead(pin);
  return state;
}

bool Led::isHigh() {
  Led::read();
  return state == HIGH;
}

int Led::getPin() {
  return pin;
}

int Led::getDirection() {
  return direction;
}

int Led::getLastState() {
  return state;
}

void Led::setState() {
  digitalWrite(pin, state);
}

#endif