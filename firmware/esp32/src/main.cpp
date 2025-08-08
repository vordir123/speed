#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// WiFi credentials (replace with your network)
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASSWORD";

// List of controllable pins
const uint8_t CONTROL_PINS[] = {2, 4, 5, 12, 13, 14};
const size_t NUM_PINS = sizeof(CONTROL_PINS)/sizeof(CONTROL_PINS[0]);
bool pinState[NUM_PINS];

AsyncWebServer server(80);

void handlePinRequest(AsyncWebServerRequest *request) {
    if (!request->hasParam("id") || !request->hasParam("state")) {
        request->send(400, "text/plain", "Missing id or state");
        return;
    }
    int id = request->getParam("id")->value().toInt();
    int state = request->getParam("state")->value().toInt();
    if (id < 0 || id >= (int)NUM_PINS) {
        request->send(404, "text/plain", "Invalid pin index");
        return;
    }
    digitalWrite(CONTROL_PINS[id], state);
    pinState[id] = state;
    Serial.printf("PIN,%d,%d\n", CONTROL_PINS[id], state);
    request->send(200, "application/json", String("{\"pin\":") + CONTROL_PINS[id] + ",\"state\":" + state + "}");
}

void setupPins() {
    for (size_t i = 0; i < NUM_PINS; ++i) {
        pinMode(CONTROL_PINS[i], OUTPUT);
        digitalWrite(CONTROL_PINS[i], LOW);
        pinState[i] = false;
    }
}

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);
    setupPins();
    setupWiFi();

    server.on("/api/pin", HTTP_GET, handlePinRequest);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "ESP32 pin controller");
    });
    server.begin();
}

void handleSerialCommand(const String &line) {
    // Simple protocol: PIN,<index>,<state>
    if (line.startsWith("PIN")) {
        int first = line.indexOf(',');
        int second = line.indexOf(',', first+1);
        if (first > 0 && second > first) {
            int id = line.substring(first+1, second).toInt();
            int state = line.substring(second+1).toInt();
            if (id >=0 && id < (int)NUM_PINS) {
                digitalWrite(CONTROL_PINS[id], state);
                pinState[id] = state;
                Serial.printf("ACK,%d,%d\n", CONTROL_PINS[id], state);
            }
        }
    }
}

void loop() {
    if (Serial.available()) {
        String line = Serial.readStringUntil('\n');
        handleSerialCommand(line);
    }
}
