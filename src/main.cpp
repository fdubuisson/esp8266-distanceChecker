#include <ESP8266WebServer.h>
#include "wifi.h"
#include "Sensor.h"
#include "config.h"


ESP8266WebServer server(httpPort);
Sensor sensor(triggerPin, echoPin);

void handleRoot() {
  int readCount = (server.args() == 1 ? server.arg("c").toInt() : 1);
  int sumDistance = 0;
  for (int i=0; i<readCount; i++) {
    int distance = sensor.getDistance();
    Serial.println(distance);
    sumDistance += distance;
  }

  String result = String(sumDistance / readCount);
  Serial.println("Distance: " + result + "cm in " + readCount + " reads");
  server.send(200, "text/plain", result);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void setup() {
  Serial.begin(9600);

  setupWifi(ssid, password);

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

/** Fonction loop() **/
void loop() {
  server.handleClient();
  delay(100);
}
