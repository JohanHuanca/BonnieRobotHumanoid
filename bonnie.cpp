#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>//para modulo pc9685 que controla 16 servos
#include <Servo.h>//para controlar servos directamente, solo uso 2 servos en este caso
#include "Bonnie.h"

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);

//Declaracion de los 16 servos del PCA9685
#define pinServo09 0
#define pinServo08 1
#define pinServo07 2
#define pinServo06 3
#define pinServo05 4
#define pinServo04 5
#define pinServo03 6
#define pinServo02 7
#define pinServo11 8
#define pinServo12 9
#define pinServo13 10
#define pinServo14 11
#define pinServo15 12
#define pinServo16 13
#define pinServo17 14
#define pinServo18 15
//Servos conectados directamente
#define pinServo10 14
#define pinServo01 12

Servo servosDirect[2];
//cambiar pin servo 12 y 11
int servoPins[18] = {pinServo01, pinServo10, pinServo09, pinServo08, pinServo07, pinServo06,/**/ pinServo18, pinServo17, pinServo16, pinServo15, pinServo14, pinServo13,/**/ pinServo05, pinServo04, pinServo03,/**/ pinServo11, pinServo12, pinServo02};

void Bonnie::initBonnie() {
  // Tu código para inicializar el robot aquí.
  for(int i=0;i<2;i++){
    servosDirect[i].attach(servoPins[i], 580, 2420);
  }
  servos.begin(); 
  servos.setPWMFreq(50); //Frecuecia PWM de 50Hz o T=16,66ms
}

void Bonnie::initialPosition() {
  // degreeArray[16] = {tobilloInferior_1,tobilloInferior_2,tobilloSuperior_1,tobilloSuperior_2,rodilla_1,rodilla_2...};
  int degreeArray[18] = {84, 86, 32, 112, 95, 92, 100, 96, 172, 58, 90, 88, 100, 8, 176, 90, 150, 0};
  for (int i = 0; i < 18; i++) {
    servoPosition[i] = degreeArray[i];
  }
  moveServo(1000, degreeArray);
}
void Bonnie::moveServo(int time, int servoTarget[]) {
  if (time > 10) {
    int startTime = millis();
    int endTime = startTime + time;
    while (millis() < endTime) {
      int elapsedTime = millis() - startTime;
      for (int i = 0; i < 18; i++) {
        float progress = (float)elapsedTime / time;
        float newPos = lerp(servoPosition[i], servoTarget[i], progress);
        updateServo(i, newPos);
      }
      delay(10); // Mejor si se reemplaza con una estrategia no bloqueante
    }
  } else {
    for (int i = 0; i < 18; i++) {
      updateServo(i, servoTarget[i]);
    }
  }
  memcpy(servoPosition, servoTarget, sizeof(servoPosition)); // Actualiza todas las posiciones de una vez
}

// Función de interpolación lineal
float Bonnie::lerp(float start, float end, float progress) {
  return (1 - progress) * start + progress * end;
}

// Función para actualizar un servo individual
void Bonnie::updateServo(int index, float position) {
  if (index < 2) {
    servosDirect[index].write((int)position);
  } else {
    int duty = map((int)position, 0, 180, pos0, pos180);
    servos.setPWM(servoPins[index], 0, duty);
  }
}
void Bonnie::forward(int steps, int speed){
  int degree[18] = {84, 86, 32, 112, 95, 92, 100, 96, 172, 58, 90, 88, 100, 8, 176, 90, 150, 0};
  int hips=10;//angulo cadera de inclinacion de las caderas
  int hips2=0;//recompensacion cadera para levantar el otro pie
  int hips3=20;//paso para atras al caminar
  int hips4=20;//paso de la pierna para adelante al caminar
  for (int i = 0; i < steps; i++) {  
    int degreeArray1[18] = {degree[0]-hips, degree[1]+0, degree[2]+hips4, degree[3]-hips4, degree[4]-hips, degree[5]+0, degree[6]-hips, degree[7]-hips3, degree[8]+0, degree[9]-hips3, degree[10]-hips, degree[11]+0, degree[12]+0, degree[13]+15, degree[14]+0, degree[15]+0, degree[16]-15, degree[17]+0};
    int degreeArray2[18] = {degree[0]-hips, degree[1]+0, degree[2]+0, degree[3]+0, degree[4]+hips2, degree[5]+0, degree[6]-hips, degree[7]-hips4, degree[8]-(hips4*2), degree[9]+hips4, degree[10]-hips, degree[11]+0, degree[12]+0, degree[13]+15, degree[14]+0, degree[15]+0, degree[16]-15, degree[17]+0};
    int degreeArray3[18] = {degree[0]-hips, degree[1]+hips3, degree[2]+0, degree[3]+hips3, degree[4]-hips, degree[5]+0, degree[6]-hips, degree[7]+0, degree[8]-hips4, degree[9]+hips4, degree[10]-hips, degree[11]+0, degree[12]+0, degree[13]+15, degree[14]+0, degree[15]+0, degree[16]-15, degree[17]+0};
    int degreeArray4[18] = {degree[0]+hips, degree[1]+hips3, degree[2]+0, degree[3]+hips3, degree[4]+hips, degree[5]+0, degree[6]+hips, degree[7]+0, degree[8]-hips4, degree[9]+hips4, degree[10]+hips, degree[11]+0, degree[12]+0, degree[13]+15, degree[14]+0, degree[15]+0, degree[16]-15, degree[17]+0};
    int degreeArray5[18] = {degree[0]+hips, degree[1]+hips4, degree[2]+(hips4*2), degree[3]-hips4, degree[4]+hips, degree[5]+0, degree[6]+hips, degree[7]+0, degree[8]+0, degree[9]+0, degree[10]-hips2, degree[11]+0, degree[12]+0, degree[13]+15, degree[14]+0, degree[15]+0, degree[16]-15, degree[17]+0};
    int degreeArray6[18] = {degree[0]+hips, degree[1]+0, degree[2]+hips4, degree[3]-hips4, degree[4]+hips, degree[5]+0, degree[6]+hips, degree[7]-hips3, degree[8]+0, degree[9]-hips3, degree[10]+hips, degree[11]+0, degree[12]+0, degree[13]+15, degree[14]+0, degree[15]+0, degree[16]-15, degree[17]+0};
    moveServo(speed, degreeArray1); 
    moveServo(speed, degreeArray2);
    moveServo(speed, degreeArray3);
    moveServo(speed, degreeArray4);
    moveServo(speed, degreeArray5);
    moveServo(speed, degreeArray6);
  } 
}
void Bonnie::sayHi(int counts, int speed){
  for (int i = 1; i <= counts; i++) {  
    int degreeArray1[18] = {97, 100, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 165, 90, 90, 90};
    int degreeArray2[18] = {97, 100, 36, 82, 95, 92, 98, 92, 160, 95, 90, 88, 100, 15, 75, 90, 90, 90};
    int degreeArray3[18] = {97, 100, 36, 82, 95, 92, 98, 92, 160, 95, 90, 88, 170, 0, 75, 90, 90, 90};
    int degreeArray4[18] = {97, 100, 36, 82, 95, 92, 98, 92, 160, 95, 90, 88, 100, 15, 75, 90, 90, 90};
    moveServo(speed, degreeArray1);
    moveServo(speed, degreeArray2);
    moveServo(speed/2, degreeArray3);
    moveServo(speed, degreeArray4);
  }
  int degreeArray5[18] = {97, 100, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 165, 90, 90, 90};
  moveServo(speed, degreeArray5);
}
void Bonnie::handsUp(int counts, int speed){
  int degreeArray1[18] = {97, 100, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 165, 90, 90, 90};
  int degreeArray2[18] = {97, 100, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 0, 90, 90, 90};
  moveServo(speed, degreeArray1);
  moveServo(speed, degreeArray2);
  for (int i = 1; i <= counts; i++) {  
    int degreeArray3[18] = {97, 100, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 140, 0, 0, 90, 90, 90};
    int degreeArray4[18] = {97, 100, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 60, 0, 90, 90, 90};
    moveServo(speed, degreeArray3);
    moveServo(speed, degreeArray4);
  }
  int degreeArray5[18] = {97, 100, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 165, 90, 90, 90};
  moveServo(speed, degreeArray5);
}
void Bonnie::noodleDance(int counts, int speed){
  int degreeArray1[18] = {90, 94, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 165, 90, 150, 0};
  int degreeArray2[18] = {90, 94, 36, 112, 95, 52, 98, 92, 160, 65, 90, 128, 100, 15, 165, 90, 150, 0};
  moveServo(speed, degreeArray1);
  moveServo(speed, degreeArray2);
  for (int i = 1; i <= counts; i++) {  
    int degreeArray3[18] = {90, 94, 36, 112, 95, 52, 98, 92, 160, 65, 90, 128, 160, 135, 165, 30, 90, 0};
    int degreeArray4[18] = {90, 94, 36, 112, 95, 52, 98, 92, 160, 65, 90, 128, 40, 75, 165, 150, 30, 0};
    moveServo(speed, degreeArray3);
    moveServo(speed, degreeArray4);
  }
  int degreeArray5[18] = {90, 94, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 165, 90, 150, 0};
  moveServo(speed, degreeArray5);
}
void Bonnie::chakiraDance(int counts, int speed){
  int degreeArray1[18] = {90, 94, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 165, 90, 150, 0};
  int degreeArray2[18] = {90, 64, 36, 82, 95, 92, 98, 62, 130, 65, 90, 88, 100, 15, 60, 90, 150, 60};
  int degreeArray3[18] = {90, 109, 36, 127, 95, 92, 98, 92, 130, 100, 90, 88, 160, 15, 100, 90, 150, 110};
  moveServo(speed, degreeArray1);
  moveServo(speed, degreeArray2);
  moveServo(speed, degreeArray3);
  for(int i = 1; i <= counts; i++){
    int degreeArray4[18] = {90, 94, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 160, 15, 40, 30, 150, 60};
    int degreeArray5[18] = {90, 94, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 160, 15, 100, 30, 150, 120};
    moveServo(speed, degreeArray4);
    moveServo(speed, degreeArray5);
  }
  int degreeArray6[18] = {90, 94, 36, 102, 95, 92, 98, 92, 160, 75, 90, 88, 160, 15, 100, 30, 150, 120};
  int degreeArray7[18] = {90, 94, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 100, 90, 150, 120};
  int degreeArray8[18] = {90, 94, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 0, 90, 150, 165};
  int degreeArray9[18] = {90, 120, 96, 82, 95, 92, 98, 62, 100, 95, 90, 88, 160, 30, 120, 30, 135, 45};
  int degreeArray10[18] = {90, 120, 96, 82, 110, 92, 98, 92, 160, 65, 105, 88, 160, 30, 120, 30, 135, 45};
  int degreeArray11[18] = {90, 120, 96, 82, 110, 92, 98, 92, 160, 65, 105, 88, 160, 10, 120, 30, 65, 45};
  int degreeArray12[18] = {90, 94, 36, 112, 80, 92, 98, 62, 100, 95, 75, 88, 160, 100, 120, 30, 150, 45};
  moveServo(speed, degreeArray6);
  moveServo(speed, degreeArray7);
  moveServo(speed, degreeArray8);
  moveServo(speed, degreeArray9);
  moveServo(speed, degreeArray10);
  moveServo(speed, degreeArray11);
  moveServo(speed, degreeArray12);
  int degreeArray13[18] = {90, 94, 36, 112, 95, 92, 98, 92, 160, 65, 90, 88, 100, 15, 165, 90, 150, 0};
  moveServo(speed, degreeArray13);
}

