#include <Arduino.h>

void setup() { 
    Serial.begin(115200);
}

void loop() {
    Serial.print("1");
    delay(1000);
    Serial.print("2");
    delay(1000);
}
