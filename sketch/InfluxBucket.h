#ifndef InfluxBucket_h
#define InfluxBucket_h

#ifndef ArduinoHttpClient_h
#define ArduinoHttpClient_h
  #include "HttpClient.h"
  #include "WebSocketClient.h"
#endif

#include "httpClientTools.h"

class InfluxBucket {

  private:
    String org;
    String bucket;
    String token;
    String precision;
    String rows;

  public:
    InfluxBucket(String o, String b, String t, String p ="ns") {
      org = o;
      bucket = b;
      token = t;
      precision = p;
      rows = "";
    }

    bool addRow(String name, int pin, String location, float temperature);
    void clearRow();
    bool write(HttpClient &client, bool waitResponse);
    bool testConnection(HttpClient &client);

    String getOrg();
    String getBucket();
};

bool InfluxBucket::addRow(String name, int pin, String location, float temperature) {
  rows += name;
  rows += ",sensorPin=";
  rows += String(pin);
  rows += ",sensorLocation=";
  rows += location;
  rows += " ";
  rows += "temperature=";
  rows += temperature;
  Serial.print("Row : ");
}

void InfluxBucket::clearRow() {
  rows = "";
}

bool InfluxBucket::write(HttpClient &client, bool waitResponse = false) {
  bool writeDone = false;
  String authHeader, path;

  authHeader = "Token ";
  authHeader += SECRET_INFLUXDB_TOKEN;

  path = "/api/v2/write?bucket=";
  path += bucket;
  path += "&org=";
  path += org;
  path += "&precision=";
  path += precision;

  Serial.println(rows);

  client.beginRequest();
  client.post(path);
  client.sendHeader("Content-Type", "text/plain");
  client.sendHeader("Content-Length", rows.length());
  client.sendHeader("Authorization", authHeader);
  client.beginBody();
  client.print(rows);
  client.endRequest();

  if (waitResponse == true) {
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();
    printStatusCode(statusCode, response);
  }

  writeDone = true;  
  return writeDone;
}

bool InfluxBucket::testConnection(HttpClient &client) {
  bool testSuccess = false;
  int statusCode;
  String path = "/ping";

  Serial.println("Attempting to connect to InfluxDB...");
  client.get(path);
  statusCode = client.responseStatusCode();
  String response = client.responseBody();
  printStatusCode(statusCode, response);
  if (statusCode == 204) {
    testSuccess = true;
  }
  return testSuccess;
}

String InfluxBucket::getOrg() {
  return org;
}

String InfluxBucket::getBucket() {
  return bucket;
}

#endif