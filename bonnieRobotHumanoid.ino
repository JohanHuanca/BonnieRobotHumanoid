#include "Bonnie.h"
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>

#include "Config.h"  // Sustituir con datos de vuestra red
#include "PublicService.h"
#include "ServoService.h"
#include "ButtonService.h"
#include "Controller.h"
#include "ConnectWifi.h"

Bonnie bonnie;
bool firstMove=true;

void setup() {
  Serial.begin(9600);
  Serial.println("Bonnie robot test!");
  
  bonnie.initBonnie();
  bonnie.initialPosition(1000);
  delay(2000);

  connectWiFi_AP();
  initServoData();
  initButtonData();
  initServer();
}

void loop() {
  //updateServoAngles(1000);
  checkButtonStates();
}
void checkButtonStates(){
  if(buttonData[0].state) { // Suponiendo que buttonData[0] corresponde al botón 1
    bonnie.forward(1,400);
  }else if (buttonData[1].state){
    bonnie.turnLeft(1,400);
  }else if (buttonData[2].state){
    bonnie.turnRight(1,400);
  }else if (buttonData[6].state){
    bonnie.sayHi(1,800);
  }else if (buttonData[7].state){
    if(firstMove){
      bonnie.handUp(1,800);
      firstMove=false;
    }
    bonnie.handUpWaving(1,800);
  }else{
    bonnie.initialPosition(500);
    firstMove=true;
  }
}
void initButtonData(){
  for(int i=0; i < 10; i++){
    buttonData.push_back(ButtonResource(i+1,false)); 
  }
}
void initServoData(){
  for(int i=0; i < 18; i++){
    servoData.push_back(ServoResource(i+1,bonnie.servoPosition[i])); 
  }
}
void updateServoAngles(int speed) {
  int servoCount = servoData.size();
  int degreeArray[servoCount];
  
  for (int i = 0; i < servoCount; i++) {
    degreeArray[i] = servoData[i].angle;
  }
  Serial.print("degreeArray: ");
  for (int i = 0; i < servoCount; i++) {
    Serial.print(degreeArray[i]);
    Serial.print("[");
    Serial.print(i + 1);
    Serial.print("]");
    if (i < servoCount - 1) {
      Serial.print(", ");
    }
  }
  Serial.println();
  bonnie.moveServo(speed, degreeArray);
}
