#include "Bonnie.h"

Bonnie bonnie;

void setup() {
  Serial.begin(115200);
  Serial.println("Bonnie robot test!");

  bonnie.initBonnie();
  bonnie.initialPosition();
  delay(5000);

  // Aquí puedes poner los métodos de movimiento que desarrollaste para Bonnie.
}

void loop() {

}

void serialEvent() {
  //Sample serial input: "90,150,150,30,90,90,90,30,30,150,90,90,30,30,30,150,150,150,90,90,"
  String receivedDegrees;
  receivedDegrees = Serial.readString();
  executeAngle(receivedDegrees);
}

void executeAngle(String receivedDegrees) {
  int angleArray[16], r = 0, t = 0;

  for (int i = 0; i < receivedDegrees.length(); i++)
  {
    if (receivedDegrees.charAt(i) == ',')
    {
      angleArray[t] = receivedDegrees.substring(r, i).toInt();
      r = (i + 1);
      t++;
    }
  }
  for (int i = 0; i < 16; i++) {
    Serial.print(angleArray[i]);
    Serial.print("|");
  }
  Serial.println("");
  bonnie.moveServo(2000, angleArray);
}
