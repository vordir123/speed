#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "ml_model.h"

const uint8_t CONTROL_PINS[] = {2, 3, 4, 5};
const size_t NUM_PINS = sizeof(CONTROL_PINS)/sizeof(CONTROL_PINS[0]);

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

void setupPins() {
    for (size_t i = 0; i < NUM_PINS; ++i) {
        pinMode(CONTROL_PINS[i], OUTPUT);
        digitalWrite(CONTROL_PINS[i], LOW);
    }
}

void handleSerialCommand(Stream &port, const String &line) {
    // Commands: PIN,<index>,<state>
    if (line.startsWith("PIN")) {
        int first = line.indexOf(',');
        int second = line.indexOf(',', first+1);
        if (first > 0 && second > first) {
            int id = line.substring(first+1, second).toInt();
            int state = line.substring(second+1).toInt();
            if (id >=0 && id < (int)NUM_PINS) {
                digitalWrite(CONTROL_PINS[id], state);
                port.printf("ACK,%d,%d\n", CONTROL_PINS[id], state);
            }
        }
        return;
    }

    // Manual ML analysis: ANALYZE,<id>,<firstByte>
    if (line.startsWith("ANALYZE")) {
        int first = line.indexOf(',');
        int second = line.indexOf(',', first+1);
        if (first > 0 && second > first) {
            CAN_message_t tmp; memset(&tmp, 0, sizeof(tmp));
            tmp.id = strtol(line.substring(first+1, second).c_str(), nullptr, 0);
            tmp.len = 1;
            tmp.buf[0] = line.substring(second+1).toInt();
            int cls = identifyMessage(tmp);
            port.printf("ANALYZE,%d\n", cls);
        }
        return;
    }
}

void setup() {
    Serial.begin(115200);     // USB serial for web interface
    Serial1.begin(115200);    // Serial1 to ESP32
    setupPins();

    can1.begin();
    can1.setBaudRate(500000);
}

void loop() {
    CAN_message_t msg;
    if (can1.available()) {
        can1.read(msg);
        int cls = identifyMessage(msg);
        Serial.printf("CAN,%03X,%d,%d\n", msg.id, msg.len, cls);
        Serial1.printf("CAN,%03X,%d,%d\n", msg.id, msg.len, cls);
    }

    if (Serial.available()) {
        String line = Serial.readStringUntil('\n');
        handleSerialCommand(Serial, line);
    }
    if (Serial1.available()) {
        String line = Serial1.readStringUntil('\n');
        handleSerialCommand(Serial1, line);
    }
}
