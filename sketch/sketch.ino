#include <WiFiNINA.h> // https://www.arduino.cc/reference/en/libraries/wifinina/

// https://github.com/arduino-libraries/ArduinoHttpClient
#ifndef ArduinoHttpClient_h
#define ArduinoHttpClient_h
  #include "HttpClient.h"
  #include "WebSocketClient.h"
#endif

#include "arduino_env.h"
#include "TemperatureSensor.h"

const float maxAnalogValue = pow(2 ,ANALOG_READ_RESOLUTION);

// wifi setup
int statusWifi = WL_IDLE_STATUS;

// temperature sensor setup
TemperatureSensor const sensors[1] = {
  TemperatureSensor(A0, "outside"),
};
const int temperatureSensorSize = sizeof(sensors) / sizeof(sensors[0]);

char serverAddress[15] = "192.168.1.21"; // server address
int port = 8086;

WiFiClient wifi;
HttpClient influxClient = HttpClient(wifi, SECRET_INFLUXDB_ADDRESS, SECRET_INFLUXDB_PORT);

void setup() {
  analogReadResolution(ANALOG_READ_RESOLUTION);
  bool dbAvailable = false;
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);
  wifiConnect(SECRET_WIFI_SSID, SECRET_WIFI_PASS);

  while (!dbAvailable) {
    dbAvailable = testInfluxAccess();
  }
}

void loop() {
  float temperatures[sizeof(sensors)];
  
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
  voltage = (sensorVal/maxAnalogValue) * SENSOR_VOLTAGE;
  temperature = (voltage - .5) * 100;
  return temperature;
}

void printTemperature(float temperature) {
  String message;

  message = String(temperature);
  message += "°C";
  Serial.println(message);
}

bool testInfluxAccess() {
  bool available = false;
  int statusCode;
  
  String path = "/ping";
  influxClient.get(path);
  statusCode = influxClient.responseStatusCode();
  Serial.println("Attempting to connect to InfluxDB ");
  if (statusCode == 204) {
    available = true;
  }
  return available;
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

  influxClient.beginRequest();
  influxClient.post(path);
  influxClient.sendHeader("Content-Type", "text/plain");
  influxClient.sendHeader("Content-Length", data.length());
  influxClient.sendHeader("Authorization", authHeader);
  influxClient.beginBody();
  influxClient.print(data);
  influxClient.endRequest();

  if (waitResponse == true) {
    int statusCode = influxClient.responseStatusCode();
    String response = influxClient.responseBody();
    printStatusCode(statusCode, response);
  }

}

void wifiConnect(char ssid[], char pass[]) {
  while ( statusWifi != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid); // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    statusWifi = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void printStatusCode(int statusCode, String message) {
  Serial.print("Status code: ");
  Serial.print(statusCode);
  Serial.print(", Response: ");
  Serial.println(message);
}