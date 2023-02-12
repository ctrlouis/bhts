#include <WiFiNINA.h> // https://www.arduino.cc/reference/en/libraries/wifinina/

// https://github.com/arduino-libraries/ArduinoHttpClient
#ifndef ArduinoHttpClient_h
#define ArduinoHttpClient_h
  #include "HttpClient.h"
  #include "WebSocketClient.h"
#endif

#include "arduino_env.h"
#include "InfluxBucket.h"
#include "Led.h"
#include "TemperatureSensor.h"
#include "wifiConnection.h"
#include "httpClientTools.h"

// wifi setup
WiFiClient wifi;

// influx setup
HttpClient influxClient = HttpClient(wifi, SECRET_INFLUXDB_ADDRESS, SECRET_INFLUXDB_PORT);
InfluxBucket arduinoBucket = InfluxBucket(SECRET_INFLUXDB_ORG, SECRET_INFLUXDB_BUCKET, SECRET_INFLUXDB_TOKEN, "s");

// led setup
Led pauseLed = Led(0, OUTPUT, LOW);
Led recordLed = Led(1, OUTPUT, LOW);
Led wifiLed = Led(2, OUTPUT, LOW);
Led button = Led(3, INPUT);

// temperature sensor setup
TemperatureSensor sensors[3] = {
  TemperatureSensor(A0, "outside"),
  TemperatureSensor(A1, "inside_isolation"),
  TemperatureSensor(A2, "inside_no_isolation"),
};
const int sensorsSize = sizeof(sensors) / sizeof(sensors[0]);

bool buttonReleased = true, record = false, recordChange = false;
int buttonPressed, stopCounter = 0;

void setup() {
  bool dbAvailable = false;
  startBlink();
  pauseLed.high();
  analogReadResolution(ANALOG_READ_RESOLUTION);
  Serial.begin(9600);
  while (!Serial);

  wifiConnect(SECRET_WIFI_SSID, SECRET_WIFI_PASS);

  while (!dbAvailable) {
    dbAvailable = arduinoBucket.testConnection(influxClient);
  }
  wifiLed.high();
}

void loop() {
  buttonPressed = button.read();

  if (buttonPressed == HIGH) {
    buttonReleased = false;
    if (!recordChange && !record) {
      record = true;
      recordChange = true;
      pauseLed.low();
      recordLed.high();
    } else if (!recordChange && record) {
      stopCounter++;
    }
    if (stopCounter > 4) {
        stopCounter = 0;
        record = false;
        recordChange = true;
        pauseLed.high();
        recordLed.low();
    }
  } else if (buttonPressed == LOW) {
    buttonReleased = true;
    recordChange = false;
  }

  if (record) {
    for (int i = 0; i < sensorsSize; i++) {
      sensors[i].readTemperature();
      Serial.println(sensors[i].toString());
      arduinoBucket.addRow("temperatureSensors", sensors[i].getPin(), sensors[i].getLocation(), sensors[i].getTemperature());
    }
    arduinoBucket.write(influxClient);
    arduinoBucket.clearRow();
  }
  delay(500);
}

void startBlink() {
  int duration = 300;
  pauseLed.high();
  recordLed.low();
  wifiLed.low();
  delay(duration);
  pauseLed.low();
  recordLed.high();
  wifiLed.low();
  delay(duration);
  pauseLed.low();
  recordLed.low();
  wifiLed.high();
  delay(duration);
  for(int i = 0; i < 3; i++) {
    pauseLed.low();
    recordLed.low();
    wifiLed.low();
    delay(duration);
    pauseLed.high();
    recordLed.high();
    wifiLed.high();
    delay(duration);
  }
  pauseLed.low();
  recordLed.low();
  wifiLed.low();
  delay(duration);
}