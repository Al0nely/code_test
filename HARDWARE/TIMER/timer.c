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

//��ʱ��7�жϷ������		    
void TIM3_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		
		USART3_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIM3�����жϱ�־    
		TIM_Cmd(TIM3, DISABLE);  //�ر�TIM3 
	}	    
}
 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM3_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//TIM3ʱ��ʹ��    
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	
}
	 

extern u8 WiFiStatus;
extern int t_se;
u8 t1=0;
//��ʱ��7�жϷ������		    
void TIM4_IRQHandler(void)
{ 	
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			 
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIM3�����жϱ�־ 
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
				TIM_Cmd(TIM4, DISABLE); //�رն�ʱ��7
			}
		}
	}	    
}
 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM4_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM3ʱ��ʹ��    
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	TIM_Cmd(TIM4, DISABLE); //�رն�ʱ��7
	
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
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//�Ǹ����ж�
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
				// �ַ����а���ת�֣������Ѿ��ɹ��ָ��ǰ���������ַ���
				// firstWeather �洢תǰ��������secondWeather �洢ת�������
				if(strcmp(firstWeather,"��")==0)
				{
					if(strcmp(secondWeather,"��")==0)
					{
						first[6]=0x00+17;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"����")==0)
					{
						first[6]=0x00+18;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"С��")==0)
					{
						first[6]=0x00+19;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"����")==0)
					{
						first[6]=0x00+20;
						USART2_SendData(first,length);
					}	
				}
				else if(strcmp(firstWeather,"��")==0)
				{
					if(strcmp(secondWeather,"��")==0)
					{
						first[6]=0x00+27;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"����")==0)
					{
						first[6]=0x00+28;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"С��")==0)
					{
						first[6]=0x00+29;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"����")==0)
					{
						first[6]=0x00+30;
						USART2_SendData(first,length);
					}
				}	
				else if(strcmp(firstWeather,"����")==0)
				{
					if(strcmp(secondWeather,"��")==0)
					{
						first[6]=0x00+12;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"��")==0)
					{
						first[6]=0x00+13;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"С��")==0)
					{
						first[6]=0x00+14;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"����")==0)
					{
						first[6]=0x00+15;
						USART2_SendData(first,length);
					}	
				}
				else if(strcmp(firstWeather,"С��")==0)
				{
					if(strcmp(secondWeather,"��")==0)
					{
						first[6]=0x00+22;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"��")==0)
					{
						first[6]=0x00+23;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"����")==0)
					{
						first[6]=0x00+24;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"����")==0)
					{
						first[6]=0x00+25;
						USART2_SendData(first,length);
					}	
				}
				else if(strcmp(firstWeather,"����")==0)
				{
					if(strcmp(secondWeather,"��")==0)
					{
						first[6]=0x00+6;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"��")==0)
					{
						first[6]=0x00+7;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"����")==0)
					{
						first[6]=0x00+8;
						USART2_SendData(first,length);
					}
					else if(strcmp(secondWeather,"С��")==0)
					{
						first[6]=0x00+9;
						USART2_SendData(first,length);
					}
				}	
			}
			else 
			{
				// �ַ�����û��ת�֣�ֻ��һ������ֵ
				if(strcmp(firstWeather,"��")==0)
				{
					first[6]=0x00+16;
					USART2_SendData(first,length);
				}
				else if(strcmp(firstWeather,"��")==0)
				{
					first[6]=0x00+26;
					USART2_SendData(first,length);
				}
				else if(strcmp(firstWeather,"����")==0)
				{
					first[6]=0x00+11;
					USART2_SendData(first,length);
				}
				else if(strcmp(firstWeather,"С��")==0)
				{
					first[6]=0x00+21;
					USART2_SendData(first,length);
				}
				else if(strcmp(firstWeather,"����")==0)
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
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIM3�����жϱ�־ 
	}	    
}
 


//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM2_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM3ʱ��ʹ��    
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	TIM_Cmd(TIM2, DISABLE); //�رն�ʱ��7
	
}



