#include "timer.h"
#include "string.h"
#include "led.h" 
#include "lcd.h"
#include "common.h"
#include "wdg.h"
#include "stdlib.h"
#include "rtc.h"
#include "usart.h"

extern u16 USART3_RX_STA;
extern vu8 check_sec;

//定时器7中断服务程序		    
void TIM3_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		
		USART3_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIM3更新中断标志    
		TIM_Cmd(TIM3, DISABLE);  //关闭TIM3 
	}	    
}
 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM3_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//TIM3时钟使能    
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	
}
	 

extern u8 WiFiStatus;
extern int t_se;
u8 t1=0;
//定时器7中断服务程序		    
void TIM4_IRQHandler(void)
{ 	
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
	{	 			 
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIM3更新中断标志 
		if(t_se==10)
		{
			t1++;
			//printf("%d\r\n",t1);
			if(t1==1)
			{
				LED0=0;
				LED1=0;
			}
			else if(t1>1)
			{
				LED0=1;
				LED1=1;
				t1=0;
				TIM_Cmd(TIM4, DISABLE); //关闭定时器7
			}
		}
	}	    
}
 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM3时钟使能    
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	TIM_Cmd(TIM4, DISABLE); //关闭定时器7
	
}

extern uint8_t voice[8];
extern uint8_t first[8];
extern uint8_t final[8];
extern char secondWeather[50];
extern char firstWeather[50];
u8 lock_key=0;
u8 t2=0;
void TIM2_IRQHandler(void)
{ 	
	uint16_t length = sizeof(voice) / sizeof(uint8_t);
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//是更新中断
	{	 	
		t2++;
		//printf("%d\r\n",t2);
	
		if(t2==2)
		{
			first[6]=0x01;
			USART2_SendData(first,length);
		}
		else if(t2==4)
		{
			voice[6]=0x00+calendar.hour;
			USART2_SendData(voice,length);
		}
		else if(t2==6)
		{
			final[6]=0x00+calendar.min;
			USART2_SendData(final,length);
		}
		else if(t2==8)
		{
			if (calendar.min!=0)
			{
				first[6]=0x02;
				USART2_SendData(first,length);	
			}			
		}
		else if(t2==10)
		{			
			if (*secondWeather !=NULL) 
			{
				// 字符串中包含转字，并且已经成功分割成前后两个子字符串
				// firstWeather 存储转前的天气，secondWeather 存储转后的天气
				if(strcmp(firstWeather,"晴")==0)
				{
					if(strcmp(secondWeather,"阴")==0)
					{
						first[6]=0x00+17;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"多云")==0)
					{
						first[6]=0x00+18;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"小雨")==0)
					{
						first[6]=0x00+19;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"大雨")==0)
					{
						first[6]=0x00+20;
						USART2_SendData(first,length);
					}	
				}
				else if(strcmp(firstWeather,"阴")==0)
				{
					if(strcmp(secondWeather,"晴")==0)
					{
						first[6]=0x00+27;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"多云")==0)
					{
						first[6]=0x00+28;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"小雨")==0)
					{
						first[6]=0x00+29;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"大雨")==0)
					{
						first[6]=0x00+30;
						USART2_SendData(first,length);
					}
				}	
				else if(strcmp(firstWeather,"多云")==0)
				{
					if(strcmp(secondWeather,"晴")==0)
					{
						first[6]=0x00+12;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"阴")==0)
					{
						first[6]=0x00+13;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"小雨")==0)
					{
						first[6]=0x00+14;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"大雨")==0)
					{
						first[6]=0x00+15;
						USART2_SendData(first,length);
					}	
				}
				else if(strcmp(firstWeather,"小雨")==0)
				{
					if(strcmp(secondWeather,"晴")==0)
					{
						first[6]=0x00+22;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"阴")==0)
					{
						first[6]=0x00+23;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"多云")==0)
					{
						first[6]=0x00+24;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"大雨")==0)
					{
						first[6]=0x00+25;
						USART2_SendData(first,length);
					}	
				}
				else if(strcmp(firstWeather,"大雨")==0)
				{
					if(strcmp(secondWeather,"晴")==0)
					{
						first[6]=0x00+6;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"阴")==0)
					{
						first[6]=0x00+7;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"多云")==0)
					{
						first[6]=0x00+8;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"小雨")==0)
					{
						first[6]=0x00+9;
						USART2_SendData(first,length);
					}
				}	
			}
			else 
			{
				// 字符串中没有转字，只有一个天气值
				if(strcmp(firstWeather,"晴")==0)
				{
					first[6]=0x00+16;
					USART2_SendData(first,length);
				}
				else if(strcmp(firstWeather,"阴")==0)
				{
					first[6]=0x00+26;
					USART2_SendData(first,length);
				}
				else if(strcmp(firstWeather,"多云")==0)
				{
					first[6]=0x00+11;
					USART2_SendData(first,length);
				}
				else if(strcmp(firstWeather,"小雨")==0)
				{
					first[6]=0x00+21;
					USART2_SendData(first,length);
				}
				else if(strcmp(firstWeather,"大雨")==0)
				{
					first[6]=0x00+6;
					USART2_SendData(first,length);
				}	
			} 
		}		
		else if(t2>10)
		{
			lock_key=0;
			t2=0;
			TIM_Cmd(TIM2, DISABLE);
			//printf("TIM2 DISABLE\r\n");
			
		}							
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIM3更新中断标志 
	}	    
}
 


//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM2_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM3时钟使能    
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	TIM_Cmd(TIM2, DISABLE); //关闭定时器7
	
}



