#include <ESP32Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define temp_pin 21
#define waterLevel_pin 34
#define servo_pin 22
#define waterPump_pin 23

int heighstWaterLevel = 0;
float normalTemperature = 28.00;
float hotTemperature = 31.00;
float coldTemperature = 25.00;
/*note :
highest water level 2300-2400
normal tempertaure ? hot temperature? cold temperature?
*/

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
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  int waterLevel = analogRead(waterLevel_pin);
  Serial.print("(3/4) * Heighest Water Level : "); Serial.println((3*heighstWaterLevel)/4);
  Serial.print("(2/4) * Heighest Water Level : "); Serial.println((2*heighstWaterLevel)/4);


  Serial.print("Temperature : "); Serial.print(temperature); Serial.println("°C");
  Serial.print("Water Level : "); Serial.println(waterLevel);

  if(waterLevel < (3*heighstWaterLevel)/4){
    digitalWrite(waterPump_pin, HIGH);
    Serial.println("Water Pump Mati");
    
  }
  if(waterLevel > (3*heighstWaterLevel)/4){
    digitalWrite(waterPump_pin, LOW);
    Serial.println("Water Pump Nyala");
  }
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
  if(heighstWaterLevel <= waterLevel) {
    heighstWaterLevel = waterLevel;
    Serial.print("New Heighst Water Level :"); Serial.println(heighstWaterLevel) ;
    }
  Serial.println(heighstWaterLevel);
  delay(1000);
}

