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
#define SHOW_DETAILS true

#define SLEEP_TIME 5e6
// #define SLEEP_TIME 1e6 * 3600
#define WATER_TIME_MS 5e3

#define SERVO_OPEN 2400
#define SERVO_CLOSE 0
#define SERVO_WAIT_TIME_MS 2.5e3
#define SERVO_PIN 5
#define SERVO_MIN 500
#define SERVO_MAX 2600
Servo servo_tap;

static const char* ntpServer = "pool.ntp.org";
static const long  gmtOffset_sec = 2 * 3600;
static const int   daylightOffset_sec = 0;

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
        #ifdef SHOW_DETAILS
            Serial.print(".");
        #endif
        delay(500);
    }

    // Show details
    #ifdef SHOW_DETAILS
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
        #ifdef SHOW_DETAILS
            Serial.print(".");
        #endif
        delay(500);
    }
}

void tapOperations()
{
    #ifdef SHOW_DETAILS
        Serial.println("Setting up servo");
    #endif
    servo_tap.attach(SERVO_PIN, SERVO_MIN, SERVO_MAX, SERVO_CLOSE);
    delay(SERVO_WAIT_TIME_MS); 

    #ifdef SHOW_DETAILS
        Serial.println("Opening Tap");
    #endif
    servo_tap.writeMicroseconds(SERVO_OPEN);
    delay(WATER_TIME_MS); 
    
    #ifdef SHOW_DETAILS
        Serial.println("Closing Tap");
    #endif
    servo_tap.writeMicroseconds(SERVO_CLOSE);
    delay(SERVO_WAIT_TIME_MS); 
}

//------------------------------
// Main functions
//------------------------------

void setup()
{
    // Initialize the serial
    Serial.begin(115200);
    
    // Initialize the components
    // Wifi_Init();
    // NTP_Init();

    // tm time_info;
    // getDateAndTime(&time_info);
    // #ifdef SHOW_DETAILS
    //     Serial.println( "\n" + 
    //                     String(time_info.tm_year + 1900) + "/" +
    //                     String(time_info.tm_mon + 1) + "/" +
    //                     String(time_info.tm_mday) + " - " +
    //                     String(time_info.tm_hour) + ":" +
    //                     String(time_info.tm_min) + ":" +
    //                     String(time_info.tm_sec)
    //                    );
    // #endif // SHOW_DETAILS
    
    tapOperations();
    
    // Sleep 
        // Note: comnect GPIO16 (D0) to RST to wake up
    #ifdef SHOW_DETAILS
        Serial.println("Sleeping.........zzz");
    #endif
    ESP.deepSleep(SLEEP_TIME);
}

// Not needed due to deep sleep
void loop()
{
}


