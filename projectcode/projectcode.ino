#include "DHT.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

#define D0  14 
#define D1  13
#define D2  10 

#define moteur_droite 3
#define moteur_droit_inp_1  9
#define moteur_droit_inp_2  8

#define moteur_gauche 2
#define moteur_gauche_inp_1  7
#define moteur_gauche_inp_2  6

#define led_rouge 5
#define led_orange_droite 0
#define led_orange_gauche 1
#define led_blanc 4

#define distance_avant A1//14
#define distance_arriere A0//13
#define temperature_humidity A2// 10
#define DHTTYPE DHT22

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
DHT dht(temperature_humidity, DHTTYPE);

int a,b,c,decision;

void Decision()
{
   a = digitalRead(D0);
   b = digitalRead(D1);
   c = digitalRead(D2);

   decision = 4*c+2*b+a;
}

void Stop()
{
  analogWrite(moteur_gauche,0);
  analogWrite(moteur_droite,0);
  
}


void Left1()
{
  digitalWrite(moteur_gauche_inp_1, LOW);
  digitalWrite(moteur_gauche_inp_2, HIGH);
  analogWrite(moteur_gauche,160);

  digitalWrite(moteur_droit_inp_2, LOW);
  digitalWrite(moteur_droit_inp_1, HIGH);
  analogWrite(moteur_droite,255);
}

void Left2()
{
  digitalWrite(moteur_gauche_inp_1, LOW);
  digitalWrite(moteur_gauche_inp_2, HIGH);
  analogWrite(moteur_gauche,90);

  digitalWrite(moteur_droit_inp_2, LOW);
  digitalWrite(moteur_droit_inp_1, HIGH);
  analogWrite(moteur_droite,255);
}

void Left3()
{
  digitalWrite(moteur_gauche_inp_2, LOW);
  digitalWrite(moteur_gauche_inp_1, HIGH);
  analogWrite(moteur_gauche,255);

  digitalWrite(moteur_droit_inp_1, LOW);
  digitalWrite(moteur_droit_inp_2, HIGH);
  analogWrite(moteur_droite,50);
}




void marche_tout_droit(int vitesse){
  analogWrite(moteur_droite , vitesse);
  analogWrite(moteur_gauche , vitesse);
  digitalWrite(moteur_droit_inp_1,HIGH);
  digitalWrite(moteur_droit_inp_2,LOW);
  digitalWrite(moteur_gauche_inp_1,HIGH);
  digitalWrite(moteur_gauche_inp_2,LOW);
}





void Right1()
{
  digitalWrite(moteur_gauche_inp_1, LOW);
  digitalWrite(moteur_gauche_inp_2, HIGH);
  analogWrite(moteur_gauche,255);

  digitalWrite(moteur_droit_inp_2, LOW);
  digitalWrite(moteur_droit_inp_1, HIGH);
  analogWrite(moteur_droite,160);
}

void Right2()
{
  digitalWrite(moteur_gauche_inp_1, LOW);
  digitalWrite(moteur_gauche_inp_2, HIGH);
  analogWrite(moteur_gauche,255);

  digitalWrite(moteur_droit_inp_2, LOW);
  digitalWrite(moteur_droit_inp_1, HIGH);
  analogWrite(moteur_droite,90);
}

void Right3()
{
  digitalWrite(moteur_gauche_inp_1, LOW);
  digitalWrite(moteur_gauche_inp_2, HIGH);
  analogWrite(moteur_gauche,255);

  digitalWrite(moteur_droit_inp_2, LOW);
  digitalWrite(moteur_droit_inp_1, HIGH);
  analogWrite(moteur_droite,50);
}



void tourner_a_droite(int vitesse){
  analogWrite(moteur_droite , vitesse/8);
  analogWrite(moteur_gauche , vitesse);
  digitalWrite(moteur_droit_inp_1,LOW);
  digitalWrite(moteur_droit_inp_2,HIGH);
  digitalWrite(moteur_gauche_inp_1,HIGH);
  digitalWrite(moteur_gauche_inp_2,LOW);
}

void tourner_a_gauche(int vitesse){
  analogWrite(moteur_droite , vitesse);
  analogWrite(moteur_gauche , 0);
  digitalWrite(moteur_droit_inp_1,HIGH);
  digitalWrite(moteur_droit_inp_2,LOW);
  digitalWrite(moteur_gauche_inp_1,HIGH);
  digitalWrite(moteur_gauche_inp_2,LOW);
}

void retour_arriere_droit(int vitesse){
  analogWrite(moteur_droite , vitesse);
  analogWrite(moteur_gauche , vitesse);
  digitalWrite(moteur_droit_inp_1,LOW);
  digitalWrite(moteur_droit_inp_2,HIGH);
  digitalWrite(moteur_gauche_inp_1,LOW);
  digitalWrite(moteur_gauche_inp_2,HIGH);
}

void retour_arriere_a_droite(int vitesse){
  analogWrite(moteur_droite , vitesse);
  analogWrite(moteur_gauche , 0);
  digitalWrite(moteur_droit_inp_1,LOW);
  digitalWrite(moteur_droit_inp_2,HIGH);
  digitalWrite(moteur_gauche_inp_1,LOW);
  digitalWrite(moteur_gauche_inp_2,HIGH);
}

void retourner_arriere_a_gauche(int vitesse){
  analogWrite(moteur_droite , 0);
  analogWrite(moteur_gauche , vitesse);
  digitalWrite(moteur_droit_inp_1,LOW);
  digitalWrite(moteur_droit_inp_2,HIGH);
  digitalWrite(moteur_gauche_inp_1,LOW);
  digitalWrite(moteur_gauche_inp_2,HIGH);
}


void frenage(int vitesseDroite , int vitesseGauche){
  int luminosite = max(vitesseDroite,vitesseGauche);
  int i,j;
  for( i=vitesseDroite, j=vitesseGauche ; i > 0 || j>0 ; i-- , j--){
    if(i>=0){
      analogWrite(moteur_droite , i);
    }
    if(j>=0){
      analogWrite(moteur_gauche , j);
    }
    analogWrite(led_rouge,HIGH);
    delay(10);
  }
}

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return 0.01723*pulseIn(echoPin, HIGH);
}

void capteur_luminosite(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("IR: ")); Serial.print(ir);  Serial.print(F("  "));
  Serial.print(F("Full: ")); Serial.print(full); Serial.print(F("  "));
  Serial.print(F("Visible: ")); Serial.print(full - ir); Serial.print(F("  "));
  Serial.print(F("Lux: ")); Serial.println(tsl.calculateLux(full, ir), 6);
}


void capteur_temperature_humidite(){
  float humidite = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidite) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  Serial.print("Hunidite :");
  Serial.print(humidite);
  Serial.println(" %");
  Serial.print("Temperature :");
  Serial.print(temperature);
  Serial.println(" °C");
}






void Lane_Change()
{

  analogWrite(moteur_gauche, 0);
  analogWrite(moteur_droite, 0);            //stop
  delay(500);

  Right3();   //right
  delay(1000);

  analogWrite(moteur_gauche, 0);
  analogWrite(moteur_droite, 0);            //stop
  delay(200);

  marche_tout_droit(150);
  delay(1500);

  analogWrite(moteur_gauche, 0);           //stop
  analogWrite(moteur_droite, 0);
  delay(200);

  Left3();
  delay(1000);

  analogWrite(moteur_gauche, 0);               //stop
  analogWrite(moteur_droite, 0);
  delay(200);

  
  marche_tout_droit(150);
  delay(2000);

  Left3();
  delay(1000);

  analogWrite(moteur_gauche, 0);
  analogWrite(moteur_droite, 0);            //stop
  delay(200);

  marche_tout_droit(150);
  delay(1500);

  analogWrite(moteur_gauche, 0);
  analogWrite(moteur_droite, 0);            //stop
  delay(200);

  Right3();   //right
  delay(1000);

  analogWrite(moteur_gauche, 0);
  analogWrite(moteur_droite, 0);            //stop
  delay(200);
}







void capteur_distance(){
  int cm_avant = readUltrasonicDistance(distance_avant,distance_avant);
  int cm_arriere = readUltrasonicDistance(distance_arriere,distance_arriere);
  Serial.print("Distance avant :");
  Serial.print(cm_avant);
  Serial.println(" cm");
  Serial.print("Distance arriere :");
  Serial.print(cm_arriere);
  Serial.println(" cm");
  if(cm_avant < 20 ){
    Lane_Change();        
  }
}


void capteurs(){
  capteur_distance();
  capteur_temperature_humidite();
  capteur_luminosite();
  if(Serial.available()){
    int cammande = Serial.read();
    switch(cammande){
      case 1:
        Serial.println("on va marche tout droit");
      break;
      
      case 10:
        Serial.println("on va tourner a gauche");
      break;
      
      case 11:
        Serial.println("on va marche tout droit");
      break;
      
      case 2:
        Serial.println("on va marche arriere");
      break;

      case 20:
        Serial.println("on va marche arriere gauche");
      break;

      case 21:
        Serial.println("on va marche arriere droite");
      break;
    }
  }
}





void setup() {
  pinMode(moteur_droite,OUTPUT);
  pinMode(moteur_droit_inp_1,OUTPUT);
  pinMode(moteur_droit_inp_2,OUTPUT);

  pinMode(D0,INPUT);
  pinMode(D1,INPUT);
  pinMode(D2,INPUT);


  pinMode(moteur_gauche,OUTPUT);
  pinMode(moteur_gauche_inp_1,OUTPUT);
  pinMode(moteur_gauche_inp_2,OUTPUT);
  pinMode(led_rouge,OUTPUT);
  pinMode(led_orange_droite,OUTPUT);
  pinMode(led_orange_gauche,OUTPUT);
  pinMode(led_blanc , OUTPUT);
  dht.begin();
  Serial.begin(115200);
 

}


void loop() {
  marche_tout_droit(150);
  capteurs();
}
