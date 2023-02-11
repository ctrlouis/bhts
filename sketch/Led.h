#ifndef Led_h
#define Led_h

class Led {

  private:
    int pin;
    int direction;
    int state;

    void setState();

  public:
    
    Led(int p, int pos, int initialState) {
      pin = p;
      direction = pos;
      state = initialState;
      pinMode(pin, direction);
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
  return direction;
}

int Led::getState() {
  return state;
}

void Led::setState() {
  digitalWrite(pin, state);
}

#endif