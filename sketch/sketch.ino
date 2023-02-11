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
Led ledWifi = Led(0, OUTPUT, LOW);

// temperature sensor setup
TemperatureSensor sensors[1] = {
  TemperatureSensor(A0, "outside"),
};
const int sensorsSize = sizeof(sensors) / sizeof(sensors[0]);

void setup() {
  bool dbAvailable = false;

  analogReadResolution(ANALOG_READ_RESOLUTION);
  Serial.begin(9600);
  while (!Serial);

  wifiConnect(SECRET_WIFI_SSID, SECRET_WIFI_PASS);

  while (!dbAvailable) {
    dbAvailable = arduinoBucket.testConnection(influxClient);
  }
  ledWifi.high();
}

void loop() {
  for (int i = 0; i < sensorsSize; i++) {
    sensors[i].readTemperature();
    Serial.println(sensors[i].toString());
    arduinoBucket.addRow("temperatureSensors", sensors[i].getPin(), sensors[i].getLocation(), sensors[i].getTemperature());
    arduinoBucket.write(influxClient, true);
    arduinoBucket.clearRow();
  }
  delay(500);
}

