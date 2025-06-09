#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include "obd.hpp"

const char* ssid = "OBD_Reader";
const char* password = "12345678";

WebServer server(80);

// Load file from SPIFFS
String loadFile(const char* path) {
  File file = SPIFFS.open(path, "r");
  if (!file || file.isDirectory()) {
    return "File not found.";
  }
  String content = file.readString();
  file.close();
  return content;
}

void handleRoot() {
  server.send(200, "text/html", loadFile("/index.html"));
}

void handleDTC() {
  String dtcs;
  if (obd.readTroubleCodes(dtcs)) {
    String page = loadFile("/dtc.html");
    page.replace("{{DTC}}", dtcs);
    server.send(200, "text/html", page);
  } else {
    server.send(200, "text/html", loadFile("/nodtc.html"));
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed!");
    return;
  }
  Serial.println("SPIFFS mounted.");

  if (!obd.initialise()) {
    Serial.println("OBD init failed.");
  }

  WiFi.softAP(ssid, password);
  Serial.println("AP IP: " + WiFi.softAPIP().toString());

  server.on("/", handleRoot);
  server.on("/dtc", handleDTC);
  server.begin();
  Serial.println("Server started.");
}

void loop() {
  server.handleClient();
}
