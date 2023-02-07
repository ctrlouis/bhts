#include <WiFiNINA.h> // https://www.arduino.cc/reference/en/libraries/wifinina/

// https://github.com/arduino-libraries/ArduinoHttpClient
#ifndef ArduinoHttpClient_h
#define ArduinoHttpClient_h
  #include "HttpClient.h"
  #include "WebSocketClient.h"
#endif

#include "arduino_env.h"
#include "TemperatureSensor.h"

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
HttpClient client = HttpClient(wifi, SECRET_INFLUXDB_ADDRESS, SECRET_INFLUXDB_PORT);

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);
  wifiConnect(ssid, pass);
}

void loop() { 
  float temperatures[sizeof(sensors)];
  Serial.print("sizeof(sensors) = ");
  Serial.println(temperatureSensorSize);
  
  for (int i = 0; i < 1; i++) {
    temperatures[i] = getTemperature(sensors[i].pin, SENSOR_VOLTAGE);
    printTemperature(temperatures[i]);
    saveTemp(sensors[i].pin, temperatures[i], false);
  }
  delay(500);
}

float getTemperature(int pin, float SENSOR_VOLTAGE) {
  int sensorVal;
  float voltage, temperature;
  
  sensorVal = analogRead(pin);
  voltage = (sensorVal/1024.0) * SENSOR_VOLTAGE;
  temperature = (voltage - .5) * 100;
  return temperature;
}

void printTemperature(float temperature) {
  String message;

  message = String(temperature);
  message += "°C";
  Serial.println(message);
}

void saveTemp(int sensorPin, float temperature, bool waitResponse) {
  String authHeader, data, path;
  
  authHeader = "Token ";
  authHeader += SECRET_INFLUXDB_TOKEN;
  
  path = "/api/v2/write?bucket=";
  path += SECRET_INFLUXDB_BUCKET;
  path+= "&org=";
  path += SECRET_INFLUXDB_ORG;
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

  if (waitResponse = true) {
    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();
    Serial.print("Status code: ");
    Serial.print(statusCode);
    Serial.print(", Response: ");
    Serial.println(response);
  }
  
}

void wifiConnect(char ssid[], char pass[]) {
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