//Include guard
#ifndef MOTOR_H   
#define MOTOR_H

void initMotorPWM(void);
void moveBackward(unsigned int);
void moveForward(unsigned int);
void turnRight(unsigned int);
void turnLeft(unsigned int);
void moveBackwardRight(unsigned int);
void moveBackwardLeft(unsigned int);
void moveForwardRight(unsigned int);
void moveForwardLeft(unsigned int);
void stop(void);

#endif
