#include "Bonnie.h"
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>

#include "config.h"  // Sustituir con datos de vuestra red
#include "servoService.h"
#include "servoController.h"
#include "connectWifi.h"

Bonnie bonnie;

void setup() {
  Serial.begin(9600);
  Serial.println("Bonnie robot test!");
  
  bonnie.initBonnie();
  bonnie.initialPosition();
  delay(2000);
  while(true){
    bonnie.chakiraDance(3,600);
    bonnie.noodleDance(3,800);
  }
  //bonnie.forward(40,800);
  //bonnie.handsUp(5,600);
  //bonnie.sayHi(2,1400); 
  //conectarse al wifi y iniciar un servidor ApiRest local
  connectWiFi_AP();
  initServoData();
  initServer();
}

void loop() {
  updateServoAngles(1000);
  delay(500);
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
