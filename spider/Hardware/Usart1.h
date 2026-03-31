#ifndef __USART1_H
#define __USART1_H

#include <stdio.h>

void BW_Printf(char *formate, ...);
void BW_Init(void);
void BW_SendByte(uint8_t Byte);
void BW_SendArray(uint8_t *Array,uint16_t Length);
void BW_SendString(char *String);
void BW_SendNumber(uint32_t Number, uint8_t Length);
void BW_SendPacket(void);

uint8_t BW_GetRxFlag(void);

extern uint8_t BW_TxPacket[];
extern uint8_t BW_RxPacket[];
#endif
