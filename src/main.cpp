#include <WiFi.h>
#include <WebServer.h>
#include <SdFat.h>
#include <SPI.h>
#include "obd.hpp"

// Embedded HTML
#include "html_embedded/index_html.h"
#include "html_embedded/dtc_html.h"
#include "html_embedded/nodtc_html.h"

#define SD_CS_PIN A0
SdFat SD;
WebServer server(80);

// WiFi settings
const char* ssid = "OBD_Reader";
const char* password = "12345678";

// Save file to SD if not exists
void saveIfMissing(const char* path, const char* content) {
    if (!SD.exists(path)) {
        FsFile file = SD.open(path, O_WRITE | O_CREAT);
        if (file) {
            file.print(content);
            file.close();
            Serial.printf("Saved %s to SD\n", path);
        } else {
            Serial.printf("Failed to write %s\n", path);
        }
    }
}

// Read HTML from SD card
String loadFromSD(const char* path) {
    FsFile file = SD.open(path);
    if (!file) return "File not found.";
    String html;
    while (file.available()) html += (char)file.read();
    file.close();
    return html;
}

void handleRoot() {
    server.send(200, "text/html", loadFromSD("/index.html"));
}

void handleDTC() {
    String dtcs;
    if (obd.readTroubleCodes(dtcs)) {
        String page = loadFromSD("/dtc.html");
        page.replace("{{DTC}}", dtcs);
        server.send(200, "text/html", page);
    } else {
        server.send(200, "text/html", loadFromSD("/nodtc.html"));
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    if (!SD.begin(SD_CS_PIN, 1000000)) {
        Serial.println("SD init failed!");
        return;
    }
    Serial.println("SD OK");

    // Save embedded HTML if requireddd
    saveIfMissing("/index.html", index_html);
    saveIfMissing("/dtc.html", dtc_html);
    saveIfMissing("/nodtc.html", nodtc_html);

    if (!obd.initialise()) {
        Serial.println("OBD init failed");
    }

    WiFi.softAP(ssid, password);
    Serial.println("WiFi AP: " + WiFi.softAPIP().toString());

    server.on("/", handleRoot);
    server.on("/dtc", handleDTC);
    server.begin();
}

void loop() {
    server.handleClient();
}
