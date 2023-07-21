#define showDetails true

#include <Arduino.h>
//------------------------------
#include <ESP8266WiFi.h>

#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include <internet_details.h>
//------------------------------

FirebaseData firebaseData;
FirebaseAuth firebaseAuth;
FirebaseConfig firebaseConfig;

unsigned long previousMillis = 0;
const long interval = 1000;
unsigned int count = 0;

//------------------------------
//Function declarations

//Initialize functions
void Wifi_Init();
void Firebase_Init();
void checkFirebaseConnection();

void databaseTest();

//------------------------------
// Main functions
void setup() {
  Serial.begin(115200);
  Wifi_Init();
  Firebase_Init();
  delay(1000);
}

void loop() {
  checkFirebaseConnection();
  databaseTest();
}

//------------------------------
// Function definitions
void Wifi_Init() {
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  // Check connection
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

void Firebase_Init() {
  // Initialize Firebase parameters
  firebaseConfig.api_key = FIREBASE_API_KEY;
  firebaseConfig.database_url = FIREBASE_URL;
  firebaseAuth.user.email = FIREBASE_USER_EMAIL;
  firebaseAuth.user.password = FIREBASE_USER_PASSWORD;

  // Firebase wifi purposes
  Firebase.reconnectWiFi(true);
  // firebaseData.setResponseSize(4096);
  
  // Firebase token generation and max amount of retries
  firebaseConfig.token_status_callback = tokenStatusCallback;
  // firebaseConfig.max_token_generation_retry = 5;

  // Initialize Firebase
  Firebase.begin(&firebaseConfig, &firebaseAuth);

  // Print the details
  #ifdef showDetails
    while (firebaseAuth.token.uid == "");
    Serial.println("Firebase details: ");
    Serial.printf("\tUser ID: \t%s\n\n", firebaseAuth.token.uid.c_str());
  #endif
}

void checkFirebaseConnection() {
  // Check if the token has expired
  while(Firebase.isTokenExpired()) {
    Firebase.refreshToken(&firebaseConfig);
  }
}

void databaseTest() {
  if (millis() - previousMillis > interval && Firebase.ready()) {
    previousMillis = millis();
    
    // Write to database
    bool writeSuccess = Firebase.RTDB.setInt(&firebaseData, "test/int", count);
    while (!writeSuccess) {
      writeSuccess = Firebase.RTDB.setInt(&firebaseData, "test/int", count);
    }
    
    #ifdef showDetails
      if (writeSuccess) {
        Serial.println("Write succesful");
        Serial.println("\tPath: " + firebaseData.dataPath());
        Serial.printf("\tValue: %i\n", count);
      } else {
        Serial.println("Write failed");
        Serial.println("\tReason: " + firebaseData.errorReason());
      }
    #endif

    count++;
  }
}


