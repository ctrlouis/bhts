#ifndef HttpClientTools_h
#define HttpClientTools_h

void printStatusCode(int statusCode, String message) {
  Serial.print("Status code: ");
  Serial.print(statusCode);
  Serial.print(", Response: ");
  Serial.println(message);
}

#endif