#include <WiFi.h>
#include <WebServer.h>
#include "obd.hpp"

const char* ssid = "OBD_Reader";
const char* password = "12345678";

WebServer server(80);

void handleRoot() {
    server.send(200, "text/html",
        "<h2>ESP32 OBD-II Diagnostic</h2>"
        "<a href=\"/dtc\">Read Engine Trouble Codes</a>");
}

void handleDTC() {
    String codes;
    if (obd.readTroubleCodes(codes)) {
        server.send(200, "text/plain", "DTCs:\n" + codes);
    } else {
        server.send(200, "text/plain", "No DTCs found or failed to read.");
    }
}

void setup() {
    Serial.begin(115200);
    if (!obd.initialise()) {
        Serial.println("OBD initialisation failed.");
    }

    WiFi.softAP(ssid, password);
    Serial.println("WiFi AP started. Connect to:");
    Serial.println(WiFi.softAPIP());

    server.on("/", handleRoot);
    server.on("/dtc", handleDTC);
    server.begin();
    Serial.println("Web server started.");
}

void loop() {
    server.handleClient();
}
