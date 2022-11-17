#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

#define firebase_host "detektorvodabrana-default-rtdb.europe-west1.firebasedatabase.app"
#define firebase_auth "PbthYEVQmXyddq3mei7KBXU6pAoBFDIlt5CjhMRR"

Servo servo;

int LED1 = 12;
int LED2 = 13;

int nivo_vode = 0;

const char* ssid = "OnePlus";
const char* password = "tariktarik";

const char* zelenaDioda = "Zelena";
const char* crvenaDioda = "Crvena";
const char* branaOtvorena = "Otvorena";
const char* branaZatvorena = "Zatvorena";

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  servo.attach(5);
  servo.write(0);
  
  WiFi.begin(ssid, password);
  Serial.println(WiFi.localIP());

  Serial.begin(115200);
  Firebase.begin(firebase_host, firebase_auth);
}

void loop() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  
  int level = ocitanje_senzora();
  
  if(level < 1000) {    
    digitalWrite(LED2, HIGH);      
    servo.write(0);
    Serial.print("Nivo vode: ");
    Serial.println(level);     
    Firebase.setInt("SenzorVrijednost/nivoVode", level);
    Firebase.setString("SenzorVrijednost/branaOtvorena", branaOtvorena);
    Firebase.setString("SenzorVrijednost/ledDioda", zelenaDioda);
    delay(2000);
  }  
  else {    
    digitalWrite(LED1, HIGH);
    delay(3000);
    servo.write(180);
    Serial.print("Nivo vode: ");
    Serial.println(level);
    Firebase.setInt("SenzorVrijednost/nivoVode", level);
    Firebase.setString("SenzorVrijednost/branaOtvorena", branaZatvorena);
    Firebase.setString("SenzorVrijednost/ledDioda", crvenaDioda);
    delay(2000);
  }  
}

int ocitanje_senzora() {
  nivo_vode = analogRead(4);
  return nivo_vode;
}
