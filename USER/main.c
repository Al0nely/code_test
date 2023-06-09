#include "main.h"

struct m_AHT20 AHT20;
char Data[100];
u8 wifi=0;
u8 wifi_IP[20];
u8 wifi_gateway[20];
u8 wifi_netmask[20];


char time_year[50];
char time_m[50];
char place[100];
char place_a[50];
char place_c[50];
char place_p[50];
char Now_temp[50];
char weather[50];
char firstWeather[50];
char secondWeather[50];

char Today_temp[50];
char Dampness[50];

vu8 clock;

extern u16 USART3_RX_STA;  
extern u16 USART2_RX_STA;  
extern int t_se;
void wifi_cmd(void);
void LCD_display(void);
void Init(void);
void KEY(void);


int main(void)
{	
	
	Init();
	LED1=1;
	AHT20.alive=!AHT20_Init();
	while(1)
	{
		LCD_ShowIntNum(2,1,calendar.w_year,4,BLACK,GRAY,16);									  
		LCD_ShowIntNum(42,0,calendar.w_month,2,BLACK,GRAY,16);									  
		LCD_ShowIntNum(66,0,calendar.w_date,2,BLACK,GRAY,16);	 
		LCD_ShowIntNum(8*11+2,0,calendar.hour,2,BLACK,GRAY,16);									  
		LCD_ShowIntNum(8*14+2,0,calendar.min,2,BLACK,GRAY,16);									  
		LCD_ShowIntNum(8*17+2,0,calendar.sec,2,BLACK,GRAY,16);	
		KEY();
		if(AHT20.alive)// 
		{
			AHT20.flag = AHT20_ReadHT(AHT20.HT);
			StandardUnitCon(&AHT20);
			LCD_ShowIntNum(16*3,18*4,AHT20.Temp,2,BLACK,GRAY,16);
			LCD_ShowIntNum(16*3,18*5,AHT20.RH,2,BLACK,GRAY,16);
			
		}
	
		//IWDG_Feed();
		connectwifi2();
		wifi_cmd();
		
//		if((calendar.hour==15)&&(calendar.min==0)&&(calendar.sec==0))
//		{
//			LED0=0;
//			LED1=0;
//			delay_ms(1000);
//			LED0=1;
//			LED1=1;
//			t_se=0;
//		}

	}
}



void Init()
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	delay_init();	    
	
	
	LED_Init();		
	KEY_Init();
	LCD_Init();
	uart_init(115200);
	uart2_init(9600);
	usart3_init(115200); 
	
	RTC_Init(2023,02,05,15,55,56);
	LCD_Fill(0,0,LCD_W,LCD_H,GRAY);
	IIC_Init();							
	
	LED0=1;
	t_se=0;
	LCD_display();	
	LED0=0;
	delay_ms(1000);
	LED0=1;
	//IWDG_Init(4,625);
	TIM_Cmd(TIM4, DISABLE); 
	TIM4_Int_Init(3600,10000);
	TIM_Cmd(TIM2, DISABLE); 
	TIM2_Int_Init(3600,10000);
}


void wifi_cmd()
{
	char i[50];
	char *ptr = Data;
	char check[100]; 
	char *comma_ptr;
	int year, month, day, hour, minute, second;
	
	if(t_se==9)
	{
		if(USART3_RX_STA&0X8000)
		{
			
			res_data();
			if(*Data == NULL)
			{
				printf("Data is NULL!\r\n");
				t_se=8;
			}
			else
			{
				printf("Data = %s\r\n",Data);
				comma_ptr = strchr(ptr, ';');
				if (comma_ptr != NULL) 
				{
					strncpy(check, ptr, comma_ptr - ptr);
					check[comma_ptr - ptr] = '\0';
					ptr = comma_ptr + 1;
				}
				printf("check = %s\r\n",check);

				if (strcmp(check,"OK")==0)
				{
					comma_ptr = strchr(ptr, ';');
					if (comma_ptr != NULL) 
					{
						strncpy(i, ptr, comma_ptr - ptr);
						i[comma_ptr - ptr] = '\0';
						ptr = comma_ptr + 1;
					}
					printf("i = %s\r\n",i);
					
					comma_ptr = strchr(ptr, ';');
					if (comma_ptr != NULL) 
					{
						strncpy(place, ptr, comma_ptr - ptr);
						place[comma_ptr - ptr] = '\0';
						ptr = comma_ptr + 1;
					}
					printf("place = %s\r\n",place);
					
					comma_ptr = strchr(ptr, ';');
					if (comma_ptr != NULL) 
					{
						strncpy(Now_temp, ptr, comma_ptr - ptr);
						Now_temp[comma_ptr - ptr] = '\0';
						ptr = comma_ptr + 1;
					}
					printf("Now_temp = %s\r\n",Now_temp);
					
					comma_ptr = strchr(ptr, ';');
					if (comma_ptr != NULL) 
					{
						strncpy(weather, ptr, comma_ptr - ptr);
						weather[comma_ptr - ptr] = '\0';
						ptr = comma_ptr + 1;
					}
					printf("weather = %s\r\n",weather);
					
					comma_ptr = strchr(ptr, ';');
					if (comma_ptr != NULL) 
					{
						strncpy(Today_temp, ptr, comma_ptr - ptr);
						Today_temp[comma_ptr - ptr] = '\0';
						ptr = comma_ptr + 1;
					}
					printf("Today_temp = %s\r\n",Today_temp);
					
					strcpy(Dampness, ptr);
					printf("Dampness = %s\r\n",Dampness);
				
					
					sscanf(i, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
					printf("%d-%d-%d %d:%d:%d\r\n",year, month, day, hour, minute, second);
					if((year!=calendar.w_year)||(month!=calendar.w_month)||(day!=calendar.w_date)\
						||(hour!=calendar.hour)||(abs(minute-calendar.min)>1)||(abs(second-calendar.sec)>2))
					{
						BKP_WriteBackupRegister(BKP_DR1, 0X4050);
					}
				
					RTC_Init(year,month,day,hour,minute,second+1);
					
					ptr = place;
					comma_ptr = strchr(ptr, ',');
					if (comma_ptr != NULL) 
					{
						strncpy(place_c, ptr, comma_ptr - ptr);
						place_c[comma_ptr - ptr] = '\0';
						ptr = comma_ptr + 1;
					}
					printf("place_c = %s\r\n",place_c);
					
					strcpy(place_p, ptr);
					printf("place_p = %s\r\n",place_p);
					
					ptr = weather;
					comma_ptr = strstr(ptr, "转");
					if (comma_ptr != NULL) 
					{
						// 字符串中包含转字
						strncpy(firstWeather, ptr, comma_ptr - ptr);
						firstWeather[comma_ptr - ptr] = '\0';
						ptr = comma_ptr + 2;
						// 使用 strncpy() 将转字后面的天气字符串复制到 secondWeather 中
						strncpy(secondWeather, ptr, sizeof(secondWeather) - 1);
						secondWeather[sizeof(secondWeather) - 1] = '\0';
					} 
					else 
					{
						// 字符串中没有转字，只有一个天气值
						strcpy(firstWeather, weather);
					}
					printf("firstWeather = %s , secondWeather = %s\r\n",firstWeather,secondWeather);

					LCD_Fill(0,18*6,128,18*7,GRAY);

					USART3_RX_STA=0;
					LCD_display();
					t_se = 10;
					TIM_Cmd(TIM4, ENABLE); //关闭定时器7
				}	
				
			}
		}
		
	}
	
	
}


void LCD_display()
{
		LCD_ShowString(1,0,"    -  -  ",BLACK,GRAY,16,1);
		LCD_ShowString(8*11+1,0,"  :  :",BLACK,GRAY,16,1);
		
		LCD_Fill(0,18,LCD_H,18*2,GRAY);
		LCD_ShowChinese(0,18,(u8*)place_p,BLACK,GRAY,16,1);
		LCD_ShowChinese(18*6+4,18,(u8*)place_c,BLACK,GRAY,16,1);
		
		LCD_ShowChinese(0,18*2,"今日天气     ",BLACK,GRAY,16,1);
		LCD_ShowChinese(16*7,18*2,"℃",BLACK,GRAY,16,1);
		LCD_ShowString(16*4,18*2,":",BLACK,GRAY,16,1);
		LCD_Fill(16*4+8,18*2,16*7,18*3,GRAY);
		LCD_ShowString(16*4+8,18*2,(const u8*)Today_temp,BLACK,GRAY,16,1);
		LCD_Fill(0,18*3,LCD_H,18*4,GRAY);
		LCD_ShowChinese(0,18*3,(u8*)weather,BLACK,GRAY,16,1);
		
		LCD_ShowChinese(0,18*4,"温度",BLACK,GRAY,16,1);
		LCD_ShowString(16*2,18*4,":",BLACK,GRAY,16,1);
		LCD_ShowChinese(16*2+8,18*4,"    ℃",BLACK,GRAY,16,1);
		LCD_ShowString(16*5+8,18*4,"/",BLACK,GRAY,16,1);
		LCD_ShowChinese(16*5+16,18*4,"    ℃",BLACK,GRAY,16,1);
		

		LCD_Fill(16*6,18*4,16*8,18*5,GRAY);
		LCD_ShowString(16*6+8,18*4,(const u8*)Now_temp,BLACK,GRAY,16,1);
		
		
		LCD_ShowChinese(0,18*5,"湿度",BLACK,GRAY,16,1);
		LCD_ShowString(16*2,18*5,":",BLACK,GRAY,16,1);
		LCD_ShowString(16*2+8,18*5,"    %",BLACK,GRAY,16,1);
		LCD_ShowString(16*5,18*5,"/",BLACK,GRAY,16,1);
		LCD_ShowString(16*5+16,18*5,"    %",BLACK,GRAY,16,1);
		
		LCD_Fill(16*6,18*5,16*8,18*6,GRAY);
		LCD_ShowString(16*6+8,18*5,(const u8*)Dampness,BLACK,GRAY,16,1);
		
}


/*
1:现在是
2:分

6:大雨
7:大雨转晴
8:大雨转阴
9:大雨转多云
10:大雨转小雨

11:多云
12:多云转晴
13:多云转阴
14:多云转小雨
15:多云转大雨

16:晴
17:晴转阴
18:晴转多云
19:晴转小雨
20:晴转大雨

21:小雨
22:小雨转晴
23:小雨转阴
24:小雨转多云
25:小雨转大雨

26:阴
27:阴转晴
28:阴转多云
29:阴转小雨
30:阴转大雨
*/
extern uint8_t voice[8];
extern uint8_t first[8];
extern uint8_t final[8];
void KEY()
{
	u8 key;
	key = key_scanf(1);
		if(key)
		{
			switch(key)
			{
					case 1 :   	
								TIM_Cmd(TIM2, ENABLE); 
					break;
			}
		}
	
}
