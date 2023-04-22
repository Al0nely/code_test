#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include "delay.h"

#define KEY0 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)

void KEY_Init(void);//≥ı ºªØ
u8 key_scanf(u8 mode);
		 				    
#endif
