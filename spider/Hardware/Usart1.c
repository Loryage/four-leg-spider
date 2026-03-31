#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
//使用A9A10口进行串口通讯RX接黄线
uint8_t BW_TxPacket[11];
uint8_t BW_RxPacket[11];
uint8_t BW_RxFlag;

void BW_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//在1总线上
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//RX
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//TX
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
		
	USART_Cmd(USART2,ENABLE);
}
void BW_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
}
void BW_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for (i=0;i<Length; i++)
	{
		BW_SendByte(Array[i]);
	}
}
void BW_SendString(char *String)
{
	uint8_t i;
	for (i=0;String[i]!='\0';i++)
	{
		BW_SendByte(String[i]);
	}
	
}

uint32_t BW_Pow(uint32_t X, uint8_t Y)
{
	uint32_t Result = 1;
	while(Y--)
	{
		Result *=X;
	}
	return Result;
}


void BW_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		BW_SendByte(Number / BW_Pow(10,Length-i-1)%10+'0');//0 = '0' = 0x30
	}
}

int fputc(int ch, FILE *f)//为printf底层
{
	BW_SendByte(ch);
	return ch;
}


void BW_Printf(char *formate, ...)
{
	char String[100];
	va_list arg;
	va_start(arg,formate);
	vsprintf(String, formate, arg);
	va_end(arg);
	BW_SendString(String);
	
}

void BW_SendPacket(void)//
{
	BW_SendByte(0xA5);
	BW_SendArray(BW_TxPacket,11);
	BW_SendByte(0x5A);
}

uint8_t BW_GetRxFlag(void)
{
	if (BW_RxFlag==1)
	{
		BW_RxFlag=0;
		return 1;
	}
	return 0;
}

void USART2_IRQHandler(void)//提出后数据乱码
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE)==SET)
	{
		uint8_t RxData = USART_ReceiveData(USART2);
		
		if(RxState == 0)
		{
			if(RxData == 0xA5)
			{	
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if(RxState == 1)
		{
			BW_RxPacket[pRxPacket]=RxData;
			pRxPacket++;
			if(pRxPacket>=11)
			{	
				RxState = 2;				
			}
		}
		else if(RxState == 2)
		{
			if(RxData == 0x5A)
			{	
				RxState = 0;
				BW_RxFlag=1;
			}
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}	
}