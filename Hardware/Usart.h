#ifndef __USART_H__
#define __USART_H__

#include "stdio.h"

void MY_USART_Inti(void);

extern uint8_t RecvArr[]; //存储接收数据的数组
extern uint8_t RecvNum;   //已接收数据长度

#endif
