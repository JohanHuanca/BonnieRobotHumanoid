#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Bonnie.h"

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);

//Declaracion de los 16 servos del PCA9685
#define servo09 0
#define servo08 1
#define servo07 2
#define servo06 3
#define servo05 4
#define servo04 5
#define servo03 6
#define servo02 7
#define servo11 8
#define servo12 9
#define servo13 10
#define servo14 11
#define servo15 12
#define servo16 13
#define servo17 14
#define servo18 15
int servoPins[16] = {servo02, servo03, servo04, servo05, servo06, servo07, servo08, servo09, servo11, servo12, servo13, servo14, servo15, servo16, servo17, servo18};

void Bonnie::initBonnie() {
  // Tu código para inicializar el robot aquí.
  servos.begin(); 
  servos.setPWMFreq(60); //Frecuecia PWM de 60Hz o T=16,66ms
}

void Bonnie::initialPosition() {
  int degreeArray[16] = {90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90};
  for (int i = 0; i < 16; i++) {
    servoPosition[i] = degreeArray[i];
  }
  moveServo(2000, degreeArray);
}

void Bonnie::moveServo(int time, int servoTarget[]) {
    if (time > 10) {
        for (int i = 0; i < 16; i++) {
            increment[i] = ((servoTarget[i]) - servoPosition[i]) / (time / 10.0);
        }
        finalTime =  millis() + time;

        for (int iteration = 1; millis() < finalTime; iteration++) {
            partialTime = millis() + 10;

            for (int i = 0; i < 16; i++) {
                int duty = map((int)(servoPosition[i] + (iteration * increment[i])), 0, 180, pos0, pos180);
                servos.setPWM(servoPins[i], 0, duty);
            }

            while (millis() < partialTime);
        }
    }
    else {
        for (int i = 0; i < 16; i++) {
            int duty = map(servoTarget[i], 0, 180, pos0, pos180);
            servos.setPWM(servoPins[i], 0, duty);
        }
    }

    for (int i = 0; i < 16; i++) {
        servoPosition[i] = servoTarget[i];
    }
}
