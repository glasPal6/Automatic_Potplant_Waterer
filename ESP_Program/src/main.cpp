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

#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include <time.h>
#include <internet_details.h>

//------------------------------
// Global variables
//------------------------------

// Structure to store the sensor data
struct sensor_data {
    float temperature;
    float humidity;
    float soil_moisture;
    float light_intensity;
    float water_level;
};
const byte sensor_data_size = 5;

// Firebase variables
FirebaseData firebaseData;
FirebaseAuth firebaseAuth;
FirebaseConfig firebaseConfig;

// NTP variables
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 2 * 3600;
const int   daylightOffset_sec = 0;

//------------------------------
//Function declarations
//------------------------------

//Initialize functions
void Wifi_Init();
void NTP_Init();
void Firebase_Init();

// Firebase functions
void checkFirebaseConnection();
bool writeToFirebase(String path, sensor_data data);

// Sensor functions
void sendSensorData(tm timeinfo);
sensor_data getSensorData();

// Time functions
tm getDateAndTime();

//------------------------------
// Main functions
//------------------------------

void setup() {
    // Initialize the serial
    Serial.begin(115200);
    
    // Initialize the components
    Wifi_Init();
    NTP_Init();
    Firebase_Init();

    // Send data to Firebase
    tm timeinfo = getDateAndTime();
    sendSensorData(timeinfo);

    // Sleep for the hour
        // Note: comnect GPIO16 (D0) to RST to wake up
    // ESP.deepSleep((3600 - timeinfo.tm_sec - (timeinfo.tm_min * 60)) * 1e6);
    ESP.deepSleep(5e6);
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

void NTP_Init() {
    // Initialize NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void Firebase_Init() {
    // Initialize Firebase parameters
    firebaseConfig.api_key = FIREBASE_API_KEY;
    firebaseConfig.database_url = FIREBASE_URL;
    firebaseAuth.user.email = FIREBASE_USER_EMAIL;
    firebaseAuth.user.password = FIREBASE_USER_PASSWORD;

    // Firebase wifi purposes
    Firebase.reconnectWiFi(true);
    firebaseData.setResponseSize(4096);
    
    // Firebase token generation and max amount of retries
    firebaseConfig.token_status_callback = tokenStatusCallback;
    firebaseConfig.max_token_generation_retry = 5;

    // Initialize Firebase
    Firebase.begin(&firebaseConfig, &firebaseAuth);
    while (!Firebase.ready()) {
        #ifdef showDetails
            Serial.print(".");
        #endif
        delay(500);
        Firebase.begin(&firebaseConfig, &firebaseAuth);
    }

    // Print the details
    #ifdef showDetails
        while (firebaseAuth.token.uid == "");
        Serial.println("Firebase details: ");
        Serial.printf("\tUser ID: \t%s\n\n", firebaseAuth.token.uid.c_str());
    #endif
}

// Firebase functions
void checkFirebaseConnection() {
    // Check if the token has expired
    while(Firebase.isTokenExpired()) {
        Firebase.refreshToken(&firebaseConfig);
        #ifdef showDetails
            Serial.println(".");
        #endif
    }
}

bool writeToFirebase(String path, sensor_data data){
    // For each of the sensor readings
    for (byte i = 0; i < sensor_data_size; i++) {
        bool writeSuccess = false;
        
        // Send data to correct entry
        switch (i){
            case 0:
                writeSuccess = Firebase.RTDB.setFloat(&firebaseData, path + "/temperature", data.temperature);
                break;
            case 1:
                writeSuccess = Firebase.RTDB.setFloat(&firebaseData, path + "/humidity", data.humidity);
                break;
            case 2:
                writeSuccess = Firebase.RTDB.setFloat(&firebaseData, path + "/soil_moisure", data.soil_moisture);
                break;
            case 3:
                writeSuccess = Firebase.RTDB.setFloat(&firebaseData, path + "/light_intensity", data.light_intensity);
                break;
            case 4:
                writeSuccess = Firebase.RTDB.setFloat(&firebaseData, path + "/water_level", data.water_level);
                break;

            default:
                return 0;
        }

        // Check if the write was successful
        if (!writeSuccess) {
            #ifdef showDetails
                Serial.printf("Write (%hhx) failed\n", i);
                Serial.println("\tReason: " + firebaseData.errorReason());
            #endif
            return 0;
        }
    }

    return 1;
}

// Sensor functions
sensor_data getSensorData(){
    // TODO: Get the sensor data
    sensor_data data;
    
    // Temp values at the moment
    data.temperature = random(0, 100);
    data.humidity = random(0, 100);
    data.soil_moisture = random(0, 100);
    data.light_intensity = random(0, 100);
    data.water_level = random(0, 100);

    return data;
}

void sendSensorData(tm timeinfo) {
    // Check if the token has expired
    checkFirebaseConnection();

    // Get the sensor data
    sensor_data data = getSensorData();

    // Write the data to Firebase
    bool write_Success = false;
    while (!write_Success) {
        write_Success = writeToFirebase(
            "Sensor_Data/" + String(timeinfo.tm_mon + 1) + 
                "/" + String(timeinfo.tm_mday) + 
                "/" + String(timeinfo.tm_hour),
            data
        );
    }
}

// Time functions
tm getDateAndTime(){
    // Get the time
    tm timeinfo;
    while(!getLocalTime(&timeinfo));
    return timeinfo;
}






