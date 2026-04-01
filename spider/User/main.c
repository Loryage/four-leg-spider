#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
//打开时应调整编码格式
#include "Usart1.h"
#include "PWM.h"
#include "Servo.h"
#include "spider_con.h"

int main(void)
{	
    OLED_Init();
	LED_Init();
	Key_Init();
	BW_Init();
	Servo_Init();
	OLED_ShowString(0, 15, "=------usart1-----23=",6);
//	spider_check();
	while(1)
	{
		spider_forward();
//		spider_backward();
		if(BW_GetRxFlag()==1)
		{
			//显示数据（16进制转10进制）			
			
			OLED_ShowString(0, 23, "Rece",6);
			OLED_ShowString(0, 31, "ive:",6);
			OLED_ShowNum(32,23,(int)BW_RxPacket[0],3,6);		
			OLED_ShowNum(51,23,(int)BW_RxPacket[1],3,6);
			OLED_ShowNum(70,23,(int)BW_RxPacket[2],3,6);
			OLED_ShowNum(89,23,(int)BW_RxPacket[3],3,6);
			OLED_ShowNum(108,23,(int)BW_RxPacket[4],3,6);
			OLED_ShowNum(32,31,(int)BW_RxPacket[5],3,6);
			OLED_ShowNum(51,31,(int)BW_RxPacket[6],3,6);
			OLED_ShowNum(70,31,(int)BW_RxPacket[7],3,6);
			OLED_ShowNum(89,31,(int)BW_RxPacket[8],3,6);
			OLED_ShowNum(108,31,(int)BW_RxPacket[9],3,6);
			OLED_ShowImage(0, 0, 16,16, BLE_on);

			//返回数据
			BW_TxPacket[0]= BW_RxPacket[0];
			BW_TxPacket[1]= BW_RxPacket[1];
			BW_TxPacket[2]= BW_RxPacket[2];
			BW_TxPacket[3]= BW_RxPacket[3];
			BW_TxPacket[4]= BW_RxPacket[4];
			BW_TxPacket[5]= BW_RxPacket[5];
			BW_TxPacket[6]= BW_RxPacket[6];
			BW_TxPacket[7]= BW_RxPacket[7];
			BW_TxPacket[8]= BW_RxPacket[8];
			BW_TxPacket[9]= BW_RxPacket[9];
			BW_TxPacket[10]= BW_RxPacket[0]+BW_RxPacket[1]+BW_RxPacket[2]+BW_RxPacket[3]+BW_RxPacket[4]+BW_RxPacket[5]+BW_RxPacket[6]+BW_RxPacket[7]+BW_RxPacket[8]+BW_RxPacket[9];
			//校验位
			BW_SendPacket();				
			/*------操作区------*/
			if(BW_RxPacket[0]==0x00){LEDtest_OFF();}
			if(BW_RxPacket[0]==0x01)LEDtest_ON();	
		}
		OLED_Update();
	}
}
