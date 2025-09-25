#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ESP32Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
const char* ssid = "Zakelyk";
const char* password = "46483655";
const char* databaseServer = "http://192.168.137.1/UAP/save.php";
const char* statusServer = "http://192.168.137.1/UAP/getStatus.php";

#define temp_pin 22
#define waterLevel_pin 34
#define servo_pin 21
#define waterPump_pin 23

String status;
String pump_status;
String servo_status;
int waterLevel;
float temperature;
int heighstWaterLevel = 0;
float normalTemperature = 28.00;
float hotTemperature = 31.00;
float coldTemperature = 25.00;
/*note :
highest water level 2300-2400
normal tempertaure ? hot temperature? cold temperature?
*/
HTTPClient http;
Servo servo;
OneWire oneWire(temp_pin);
DallasTemperature sensors(&oneWire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensors.begin(); // Inisialisasi sensor suhu
  pinMode(waterPump_pin, OUTPUT);
  digitalWrite(waterPump_pin, HIGH);
  servo.attach(servo_pin);
  servo.write(0);

  wifiConnect();
}

void loop() {
  // put your main code here, to run repeatedly:
  sensor();
  getStatus();
  if(status == "aktif" && pump_status == "aktif"){
    pump_action();
  }
  if(status == "aktif" && servo_status == "aktif"){
    servo_action();
  }

  if(heighstWaterLevel <= waterLevel) {
    heighstWaterLevel = waterLevel;
    Serial.print("New Heighst Water Level :"); Serial.println(heighstWaterLevel) ;
    }
  Serial.println(heighstWaterLevel);
  if(WiFi.status() != WL_CONNECTED) wifiConnect();
  delay(1000);
}

void sensor(){
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  waterLevel = analogRead(waterLevel_pin);
  Serial.print("(3/4) * Heighest Water Level : "); Serial.println((3*heighstWaterLevel)/4);
  Serial.print("(2/4) * Heighest Water Level : "); Serial.println((2*heighstWaterLevel)/4);
  Serial.print("Temperature : "); Serial.print(temperature); Serial.println("°C");
  Serial.print("Water Level : "); Serial.println(waterLevel);
  String url = databaseServer;
  url += "?temperature=";
  url += String(temperature);
  url += "&water_level=";
  url += String(waterLevel);
  Serial.println(url);
  http.begin(url);
  int httpResponseCode = http.GET();
  if(httpResponseCode == 200){
    Serial.print("Data sent successfully!");
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void pump_action(){
  if(waterLevel < (3*heighstWaterLevel)/4){
    digitalWrite(waterPump_pin, HIGH);
    Serial.println("Water Pump Mati");
    
  }
  if(waterLevel > (3*heighstWaterLevel)/4){
    digitalWrite(waterPump_pin, LOW);
    Serial.println("Water Pump Nyala");
  }
}

void servo_action(){
  if(waterLevel < (2*heighstWaterLevel)/4){
    servo.write(90);
    delay(1000);
    Serial.println("Servo/Keran Nyala");
  }
  if(waterLevel > (2*heighstWaterLevel)/4){
    servo.write(0);
    delay(1000);
    Serial.println("Set Servo/Keran Ke status mati");
  }
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
    if(status == "mati"){
      servo.write(0);
      digitalWrite(waterPump_pin, HIGH);
    }
    if(pump_status == "mati"){
      digitalWrite(waterPump_pin, HIGH);
    }
    if(servo_status == "mati"){
      servo.write(0);
    }

    Serial.println("Status: " + status);
    Serial.println("Pump Status: " + pump_status);
    Serial.println("Servo Status: " + servo_status);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void wifiConnect(){
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
}



