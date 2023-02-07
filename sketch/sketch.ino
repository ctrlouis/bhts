#ifndef ArduinoHttpClient_h
#define ArduinoHttpClient_h
#include "HttpClient.h"
#include "WebSocketClient.h"
#endif

#include <WiFiNINA.h>
#include "arduino_secrets.h"

#define INFLUXDB_URL "http://localhost:8086"
#define INFLUXDB_ORG "bhts"
#define INFLUXDB_BUCKET "arduino"

#ifndef TemperatureSensor_h
#define TemperatureSensor_h
#include "Arduino.h"

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

const float maxVoltage = 3.3;

// wifi setup
char ssid[] = SECRET_WIFI_SSID;
char pass[] = SECRET_WIFI_PASS;
int status = WL_IDLE_STATUS;

// temperature sensor setup
TemperatureSensor const sensors[1] = {
  TemperatureSensor(A0, "outside"),
};
const int temperatureSensorSize = sizeof(sensors) / sizeof(sensors[0]);
float temp;

char serverAddress[15] = "192.168.1.21";  // server address
int port = 8086;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);
  wifiConnectt(ssid, pass);
}

void loop() { 
  float temperatures[sizeof(sensors)];
  Serial.print("sizeof(sensors) = ");
  Serial.println(temperatureSensorSize);
  
  for (int i = 0; i < 1; i++) {
    temperatures[i] = getTemperature(sensors[i].pin, maxVoltage);
    printTemperature(temperatures[i]);
    saveTemp(sensors[i].pin, temperatures[i]);
  }
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

void saveTemp(int sensorPin, float temperature) {
  String authHeader, data, path;
  
  authHeader = "Token ";
  authHeader += SECRET_INFLUXDB_TOKEN;
  
  path = "/api/v2/write?bucket=";
  path += INFLUXDB_BUCKET;
  path+= "&org=";
  path += INFLUXDB_ORG;
  path += "&precision=ns";

  data = "temperaturesSensors,sensorPin=";
  data += String(sensorPin);
  data += ",sensorLocation=in temperature=";
  data += temperature;

  client.beginRequest();
  client.post(path);
  client.sendHeader("Content-Type", "text/plain");
  client.sendHeader("Content-Length", data.length());
  client.sendHeader("Authorization", authHeader);
  client.beginBody();
  client.print(data);
  client.endRequest();

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.print(statusCode);
  Serial.print(", Response: ");
  Serial.println(response);
}

void wifiConnectt(char ssid[], char pass[]) {
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void wifiConnect(char ssid[], char pass[]) {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
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