#include "spider_con.h"
#include <Arduino.h>

extern void setServo(int index, int angle);

#define Servo_SetAngle1(a) setServo(0, a)
#define Servo_SetAngle2(a) setServo(1, a)
#define Servo_SetAngle3(a) setServo(2, a)
#define Servo_SetAngle4(a) setServo(3, a)
#define Servo_SetAngle5(a) setServo(4, a)
#define Servo_SetAngle6(a) setServo(5, a)
#define Servo_SetAngle7(a) setServo(6, a)
#define Servo_SetAngle8(a) setServo(7, a)

#define STAGGER_MS 0

void spider_check(void)
{
    delay(1000);
    Servo_SetAngle5(90); Servo_SetAngle1(90); delay(STAGGER_MS);
    Servo_SetAngle6(90); Servo_SetAngle2(90); delay(STAGGER_MS);
    Servo_SetAngle7(90); Servo_SetAngle3(90); delay(STAGGER_MS);
    Servo_SetAngle8(90); Servo_SetAngle4(90);
    delay(1000);
    Servo_SetAngle5(90); Servo_SetAngle1(45); delay(STAGGER_MS);
    Servo_SetAngle6(90); Servo_SetAngle2(135); delay(STAGGER_MS);
    Servo_SetAngle7(90); Servo_SetAngle3(135); delay(STAGGER_MS);
    Servo_SetAngle8(90); Servo_SetAngle4(45);
    delay(1000);
    Servo_SetAngle5(0);   Servo_SetAngle1(45); delay(STAGGER_MS);
    Servo_SetAngle6(180); Servo_SetAngle2(135); delay(STAGGER_MS);
    Servo_SetAngle7(180); Servo_SetAngle3(135); delay(STAGGER_MS);
    Servo_SetAngle8(0);   Servo_SetAngle4(45);
    delay(1000);
    Servo_SetAngle5(90); Servo_SetAngle1(45); delay(STAGGER_MS);
    Servo_SetAngle6(90); Servo_SetAngle2(135); delay(STAGGER_MS);
    Servo_SetAngle7(90); Servo_SetAngle3(135); delay(STAGGER_MS);
    Servo_SetAngle8(90); Servo_SetAngle4(45);
    delay(1000);
}

void spider_forward(void)
{
    Servo_SetAngle5(45);  Servo_SetAngle1(20);  delay(STAGGER_MS);
    Servo_SetAngle6(90);  Servo_SetAngle2(160); delay(STAGGER_MS);
    Servo_SetAngle7(90);  Servo_SetAngle3(90);  delay(STAGGER_MS);
    Servo_SetAngle8(45);  Servo_SetAngle4(90);
    delay(400);
    Servo_SetAngle5(90);  Servo_SetAngle1(90);  delay(STAGGER_MS);
    Servo_SetAngle6(135); Servo_SetAngle2(90);  delay(STAGGER_MS);
    Servo_SetAngle7(135); Servo_SetAngle3(160); delay(STAGGER_MS);
    Servo_SetAngle8(90);  Servo_SetAngle4(20);
    delay(400);
}

void spider_backward(void)
{
    Servo_SetAngle5(90);  Servo_SetAngle1(45);  delay(STAGGER_MS);
    Servo_SetAngle6(135); Servo_SetAngle2(160); delay(STAGGER_MS);
    Servo_SetAngle7(135); Servo_SetAngle3(90);  delay(STAGGER_MS);
    Servo_SetAngle8(90);  Servo_SetAngle4(90);
    delay(400);
    Servo_SetAngle5(45);  Servo_SetAngle1(90);  delay(STAGGER_MS);
    Servo_SetAngle6(90);  Servo_SetAngle2(90);  delay(STAGGER_MS);
    Servo_SetAngle7(90);  Servo_SetAngle3(160); delay(STAGGER_MS);
    Servo_SetAngle8(45);  Servo_SetAngle4(20);
    delay(400);
}
