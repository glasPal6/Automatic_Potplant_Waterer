#include "HardwareSerial.h"
#include "c_types.h"
#include "core_esp8266_features.h"
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
#include <time.h>
#include <Servo.h>

//------------------------------
// Global variables
//------------------------------
#define showDetails true

static const char* ntpServer = "pool.ntp.org";
static const long  gmtOffset_sec = 2 * 3600;
static const int   daylightOffset_sec = 0;

static const int servo_pin = 5;
static const uint16_t servo_min = 500;
static const uint16_t servo_max = 5000;
Servo servo_tap;

//------------------------------
//Function 
//------------------------------

void Wifi_Init() 
{
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

void NTP_Init() 
{
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void getDateAndTime(tm *time_info)
{
    while (!getLocalTime(time_info)) { 
        #ifdef showDetails
            Serial.print(".");
        #endif
        delay(500);
    }
}

//------------------------------
// Main functions
//------------------------------

void setup()
{
    // Initialize the serial
    Serial.begin(115200);
    
    // Initialize the components
    servo_tap.attach(servo_pin, servo_min, servo_max, 0);
    Wifi_Init();
    NTP_Init();

    tm time_info;
    getDateAndTime(&time_info);
    #ifdef  showDetails
        Serial.println( "\n" + 
                        String(time_info.tm_year + 1900) + "/" +
                        String(time_info.tm_mon + 1) + "/" +
                        String(time_info.tm_mday) + " - " +
                        String(time_info.tm_hour) + ":" +
                        String(time_info.tm_min) + ":" +
                        String(time_info.tm_sec)
                       );
    #endif // showDetails

    // Sleep for the hour
        // Note: comnect GPIO16 (D0) to RST to wake up
     ESP.deepSleep(5e6);
}

// Not needed due to deep sleep
void loop()
{
    delay(1000);
    servo_tap.writeMicroseconds(2600);
    delay(1000);
    Serial.println(servo_tap.read());
    servo_tap.writeMicroseconds(0);
}


