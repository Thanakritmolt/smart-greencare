#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ========== Pin Mapping ==========
#define SOIL_PIN A0
#define BUZZER1_PIN 2     
#define RELAY_PIN 3       
#define BLUE_LED 5
#define GREEN_LED 6
#define TRIG_PIN 7
#define ECHO_PIN 8
#define RED_LED 9
#define BUZZER2_PIN 10    

// ========== Modules ==========
DHT dht;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ========== Setup ==========
void setup() {
  pinMode(SOIL_PIN, INPUT);
  pinMode(BUZZER1_PIN, OUTPUT);
  pinMode(BUZZER2_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(BUZZER1_PIN, HIGH);
  digitalWrite(BUZZER2_PIN, HIGH);
  digitalWrite(RELAY_PIN, LOW);

  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  dht.setup(4);

  Serial.println("System Ready...");
}

// ========== Ultrasonic ==========
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

// ========== Loop ==========
void loop() {
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  int soil = analogRead(SOIL_PIN);
  long waterLevel = readDistanceCM();

 
  Serial.print("Soil: "); Serial.print(soil);
  Serial.print(" | Temp: "); Serial.print(temperature);
  Serial.print("C | Humi: "); Serial.print(humidity);
  Serial.print("% | Water Dist: "); Serial.print(waterLevel); Serial.println(" cm");


  lcd.setCursor(0, 0);
  lcd.print("Temp: "); lcd.print(temperature); lcd.print(" C  ");
  lcd.setCursor(0, 1);
  lcd.print("Humi: "); lcd.print(humidity); lcd.print(" %  ");

  if (soil > 600) {
    digitalWrite(RELAY_PIN, HIGH);         
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);


    digitalWrite(BUZZER1_PIN, LOW); delay(300);
    digitalWrite(BUZZER1_PIN, HIGH); delay(300);
  } else {
    digitalWrite(RELAY_PIN, LOW);          
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(BUZZER1_PIN, HIGH);       
  }

  // Check water
  if (waterLevel > 10) {
    digitalWrite(RED_LED, HIGH);


    digitalWrite(BUZZER2_PIN, LOW); delay(200);
    digitalWrite(BUZZER2_PIN, HIGH); delay(200);
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER2_PIN, HIGH);     
  }

  delay(1000); 
}
