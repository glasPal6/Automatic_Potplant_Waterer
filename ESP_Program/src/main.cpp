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
bool isAuthenticated = false;

unsigned long previousMillis = 0;
const long interval = 1000;
unsigned int count = 0;

//------------------------------
//Function declarations

//Initialize functions
void Wifi_Init();
void Firebase_Init();

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
    Serial.print(".");
    delay(500);
  }

  // Show details
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
}

void Firebase_Init() {
  firebaseConfig.database_url = FIREBASE_URL;
  firebaseConfig.api_key = FIREBASE_API_KEY;

  if (Firebase.signUp(&firebaseConfig, &firebaseAuth, "", "")) {
    Serial.println("Sign up succeeded");
    isAuthenticated = true;
  } else {
    Serial.println("Sign up failed");
    Serial.printf("\tReason: %s\n", firebaseConfig.signer.signupError.message.c_str());
  }

  firebaseConfig.token_status_callback = tokenStatusCallback;
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);
}

void databaseTest() {
  if (millis() - previousMillis > interval && isAuthenticated && Firebase.ready()) {
    previousMillis = millis();
    
    // Write to database
    if (Firebase.RTDB.setInt(&firebaseData, "test/int", count)) {
      Serial.println("Write succesful");
      Serial.println("Path: " + firebaseData.dataPath());
      Serial.println("Type: " + firebaseData.dataType());
    } else {
      Serial.println("Write failed");
      Serial.println("Reason: " + firebaseData.errorReason());
    }

    count++;
  }
}


