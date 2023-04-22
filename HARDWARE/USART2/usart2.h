#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "main.h"
#include "sys.h" 

#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
	  	

//如果想串口中断接收，请不要注释以下宏定义
void uart2_init(u32 bound);
void u2_printf(char* fmt, ...);
void USART2_SendData(uint8_t* data, uint16_t length);
#endif


