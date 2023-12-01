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

void setup() {
  Serial.begin(9600);
  Serial.println("Bonnie robot test!");
  
  bonnie.initBonnie();
  bonnie.initialPosition();
  delay(2000);
  while(true){
    bonnie.forward(1,600);
    //bonnie.chakiraDance(3,600);
    //bonnie.noodleDance(3,800);
  }  
  //bonnie.forward(40,800);
  //bonnie.handsUp(5,600);
  //bonnie.sayHi(2,1400); 
  //conectarse al wifi y iniciar un servidor ApiRest local
  connectWiFi_STA();
  initServoData();
  initButtonData();
  initServer();
}

void loop() {
  updateServoAngles(1000);
  //delay(500);
  //checkButtonStates();
}
void checkButtonStates(){
  if(buttonData[0].state) { // Suponiendo que buttonData[0] corresponde al botón 1
      bonnie.chakiraDance(5,600);
    }
  else if(buttonData[1].state) { // Suponiendo que buttonData[1] corresponde al botón 2
      bonnie.noodleDance(5,800);
  }
}
void initButtonData(){
  for(int i=0; i < 6; i++){
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
