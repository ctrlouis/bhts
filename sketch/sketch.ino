#include <WiFiNINA.h>
#include "arduino_secrets.h"

// wifi setup
char ssid[] = SECRET_WIFI_SSID;
char pass[] = SECRET_WIFI_PASS;
int status = WL_IDLE_STATUS;

// temperature sensor setup
const int sensorPin = A0;
const float maxVoltage = 3.3;
float temp;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);
  wifiConnect(ssid, pass);
}

void loop() { 
  float temperature;

  temperature = getTemperature(sensorPin, maxVoltage);
  printTemperature(temperature);
  delay(500);
}

float getTemperature(int pin, float maxVoltage) {
  int sensorVal;
  float voltage, temperature;
  
  sensorVal = analogRead(pin);
  voltage = (sensorVal/1024.0) * maxVoltage;
  temperature = (voltage - .5) * 100;
  return temperature;
}

void printTemperature(float temperature) {
  String message;

  message = String(temperature);
  message += "°C";
  Serial.println(message);
}

void wifiConnect(char ssid[], char pass[]) {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  Serial.println("----------------------------------------");
  printData();
  Serial.println("----------------------------------------");
}

void printData() {
  long rssi;

  Serial.println("Board Information:");

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());


  // print the received signal strength:
  rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}