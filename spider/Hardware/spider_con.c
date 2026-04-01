/*****************information******************
											STM32
 * name			:	四足蜘蛛步态控制函数                     
 * verson		:   V1.0
 * MCU			:	STM32F103C8T6
 * 接口			:	time1和time3初始化得到的8路PWM输出
 * 对应位置     :   视下方为蜘蛛的正前方8(右胯)  4(右肩) |   2(左肩)  6(左胯)
										7(右胯)  3(右肩) |   1(左肩)  5(左胯)
 * GITHUB		:	https://github.com/Loryage/four-leg-spider

**********************BEGIN***********************/
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
//需要前置文件PWM.h和Servo.h
#include "PWM.h"
#include "Servo.h"
//Servo.h配置1-4号舵机控制移动5-8号控制抬脚

void spider_check(void)
{//上电自检
	Delay_ms(1000);//竖直归位
	Servo_SetAngle5(90);Servo_SetAngle1(90);
	Servo_SetAngle6(90);Servo_SetAngle2(90);
	Servo_SetAngle7(90);Servo_SetAngle3(90);
	Servo_SetAngle8(90);Servo_SetAngle4(90);
	Delay_ms(1000);//展开形态
	Servo_SetAngle5(90);Servo_SetAngle1(45);
	Servo_SetAngle6(90);Servo_SetAngle2(135);
	Servo_SetAngle7(90);Servo_SetAngle3(135);
	Servo_SetAngle8(90);Servo_SetAngle4(45);
	Delay_ms(1000);//坐下
	Servo_SetAngle5(0);Servo_SetAngle1(45);
	Servo_SetAngle6(180);Servo_SetAngle2(135);
	Servo_SetAngle7(180);Servo_SetAngle3(135);
	Servo_SetAngle8(0);Servo_SetAngle4(45);
	Delay_ms(1000);//展开形态
	Servo_SetAngle5(90);Servo_SetAngle1(45);
	Servo_SetAngle6(90);Servo_SetAngle2(135);
	Servo_SetAngle7(90);Servo_SetAngle3(135);
	Servo_SetAngle8(90);Servo_SetAngle4(45);
	Delay_ms(1000);
}

void spider_forward(void)
{//前进
	Servo_SetAngle5(45);Servo_SetAngle1(20);Servo_SetAngle7(90);Servo_SetAngle3(90);
	Servo_SetAngle6(90);Servo_SetAngle2(160);Servo_SetAngle8(45);Servo_SetAngle4(90);
	Delay_ms(400);
	Servo_SetAngle5(90);Servo_SetAngle1(90);Servo_SetAngle7(135);Servo_SetAngle3(160);
	Servo_SetAngle6(135);Servo_SetAngle2(90);Servo_SetAngle8(90);Servo_SetAngle4(20);
	Delay_ms(400);	
}

void spider_backward(void)
{//后退
    Servo_SetAngle5(90);Servo_SetAngle1(45);Servo_SetAngle7(135);Servo_SetAngle3(90);
	Servo_SetAngle6(135);Servo_SetAngle2(160);Servo_SetAngle8(90);Servo_SetAngle4(90);
	Delay_ms(400);
	Servo_SetAngle5(45);Servo_SetAngle1(90);Servo_SetAngle7(90);Servo_SetAngle3(160);
	Servo_SetAngle6(90);Servo_SetAngle2(90);Servo_SetAngle8(45);Servo_SetAngle4(20);
	Delay_ms(400);	
}
