#include <ArduinoJson.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
const char* ssid = "Zakelyk";
const char* password = "46483655";
const char* databaseServer = "http://172.20.10.6/UAP/save.php";
const char* statusServer = "http://192.168.137.1/UAP/getStatus.php";

String status;
String pump_status;
String servo_status;

HTTPClient http;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi....");
  }
  String wifiName = WiFi.SSID();
  String localWifiIP = WiFi.localIP().toString();
  // Serial.print();
  Serial.println("Connected to "+ wifiName);
  Serial.println("Local IP WiFi: "+localWifiIP);
  // Your setup code here
}

void loop() {
  // Your loop code here
  getStatus();
  
  // Do something with the obtained values
  delay(1000);
}

void getStatus() {
  http.begin(statusServer);

  int httpResponseCode = http.GET();
  if(httpResponseCode == 200) {
    String payload = http.getString();

    DynamicJsonDocument doc(256); // Sesuaikan dengan ukuran dokument JSON yang sesuai
    deserializeJson(doc, payload);

    // Mengambil nilai dari JSON
    status = doc["status"].as<String>();
    pump_status = doc["pump_status"].as<String>();
    servo_status = doc["servo_status"].as<String>();

    Serial.println("Status: " + status);
    Serial.println("Pump Status: " + pump_status);
    Serial.println("Servo Status: " + servo_status);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}