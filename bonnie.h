#ifndef Bonnie_h
#define Bonnie_h

class Bonnie
{
  public:

    /* Variable declaration */
    int servoPosition[16];
    float increment[16];
    unsigned long finalTime;
    unsigned long partialTime;
    unsigned int pos0=123; // ancho de pulso en cuentas para pocicion 0°
    unsigned int pos180=614; // ancho de pulso en cuentas para la pocicion 180°

    /* Method declaration */
    void initBonnie();
    void initialPosition();
    void moveServo(int time, int  servoTarget[]);

    void sayHi(int count = 1, int speed = 1000);
    void handsUp(int count = 1, int speed = 1000);

    void forward(int steps = 1, int speed = 1000);
    void turnRight(int steps = 1, int speed = 1000);
    void turnLeft(int steps = 1, int speed = 1000);
    void moveRight(int count = 1, int speed = 1000);
    void moveLeft(int count = 1, int speed = 1000);
};

#endif
