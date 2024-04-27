#define showDetails true

#include <Arduino.h>
//------------------------------
// Libraries
//------------------------------

#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include <WiFi.h>
#endif
#include "local_wifi.h"

//------------------------------
// Global variables
//------------------------------

//------------------------------
//Function declarations
//------------------------------

//Initialize functions
void Wifi_Init();

//------------------------------
// Main functions
//------------------------------

void setup() {
    // Initialize the serial
    Serial.begin(115200);
    
    // Initialize the components
    Wifi_Init();

    // Sleep for the hour
        // Note: comnect GPIO16 (D0) to RST to wake up
    // ESP.deepSleep((3600 - timeinfo.tm_sec - (timeinfo.tm_min * 60)) * 1e6);
    // ESP.deepSleep(5e6);
}

// Not needed due to deep sleep
void loop() {}

//------------------------------
// Function definitions
//------------------------------

// Initialize functions
void Wifi_Init() {
    // Connect to WiFi
    WiFi.disconnect(true);
    WiFi.persistent(false);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        #ifdef showDetails
            Serial.print(".");
        #endif
        delay(500);
    }

    // Show details
    #ifdef showDetails
        Serial.println("\nWiFi details: ");
        Serial.print("\tIP Address: \t");
        Serial.println(WiFi.localIP());
        Serial.print("\tMAC Address: \t");
        Serial.println(WiFi.macAddress());
        Serial.print("\tSubnet Mask: \t");
        Serial.println(WiFi.subnetMask());
        Serial.print("\tGateway IP: \t");
        Serial.println(WiFi.gatewayIP());
        Serial.println();
    #endif
}

