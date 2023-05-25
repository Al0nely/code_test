#include "common.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   	  	 	 	 	 
#include "string.h"    
#include "usart3.h" 
#include "lcd.h"

extern u16 USART3_RX_STA;  



//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* atk_8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
		//printf("%s",USART3_RX_BUF);
	} 
	return (u8*)strx;
}



//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	
	u3_printf("%s\r\n",cmd);	//��������
	
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				
				if(atk_8266_check_cmd(ack))
				{
					//printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
				
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//��ATK-ESP8266����ָ������
//data:���͵�����(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))break;//�õ���Ч���� 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}



u8 WiFiStatus=0;
int t_se=0;
void connectwifi()
{
	switch (t_se)
	{
	case 0:
		//printf("case 0\r\n");
		if(atk_8266_send_cmd("AT","OK",100)==0)
		{
			t_se=1;
			printf("t_se = 0,OK\r\n");
		}
		
		break;
	case 1:
		if(atk_8266_send_cmd("ATE0","OK",100)==0)
		{
			t_se=2;
			printf("t_se = 1,OK\r\n");
		}
		
		break;
	case 2:
		if(atk_8266_send_cmd("AT+CWMODE=1","OK",100)==0)
		{
			t_se=3;
			printf("t_se = 2,OK\r\n");
		}
		
		break;
	case 3:
		if(atk_8266_send_cmd("AT+RST","ready",100)==0)
		{
			t_se=4;
			printf("t_se = 3,OK\r\n");
		}
		
		break;
	case 4:
		if(atk_8266_send_cmd("AT+CWJAP=\"helloworld\",\"4206832001\"","WIFI GOT IP",500)==0)
		{
			t_se=5;
			printf("t_se = 4,OK\r\n");
		}
		
		break;
	case 5:
		if(atk_8266_send_cmd("AT+CIPMUX=0","OK",100)==0)
		{
			t_se=6;
			printf("t_se = 5,WiFi,OK\r\n");
		}
		
		break;
	case 6:
		if(atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"175.178.27.47\",60","OK",500)==0)//175.178.27.47
		{
			t_se=7;
			printf("t_se = 6,TCP,OK\r\n");
		}
		
		break;
	case 7:
		
		atk_8266_send_cmd("AT+CIPMODE=1","OK",100);
		atk_8266_send_cmd("AT+CIPSEND",NULL,100);   //0�������ӣ�1��������
		printf("Send Start!\r\n");
		t_se=8;
		break;
	case 8:
		printf("t_se = 8,Sending!\r\n");
		
		atk_8266_send_data("search_weather",NULL,500);
	t_se=9;
		break;
	default:
		break;
	}
	delay_ms(2);
}

u8 delay=0;
void connectwifi2()
{
	switch (t_se)
	{
	case 0:
		if(delay==0)
		{
			USART3_RX_STA=0;
			u3_printf("AT\r\n");
			delay=1;
		}
		if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
		{
			
			if(atk_8266_check_cmd("OK"))
			{
				t_se=1;
				delay=0;
				//printf("t_se = 0,OK\r\n");
			}
			
				USART3_RX_STA=0;
		} 	
		break;
	case 1:
		if(delay==0)
		{
			USART3_RX_STA=0;
			u3_printf("ATE0\r\n");
			delay=1;
		}
		if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
		{
			
			if(atk_8266_check_cmd("OK"))
			{
				t_se=2;
				delay=0;
			//	printf("t_se = 1,OK\r\n");
			}
			
				USART3_RX_STA=0;
		} 	
		
		break;
	case 2:
		if(delay==0)
		{
			USART3_RX_STA=0;
			u3_printf("AT+CWMODE=1\r\n");
			delay=1;
		}
		if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
		{
			
			if(atk_8266_check_cmd("OK"))
			{
				t_se=3;
				delay=0;
			//	printf("t_se = 2,OK\r\n");
			}
			
				USART3_RX_STA=0;
		} 
		
		break;
	case 3:
		if(atk_8266_send_cmd("AT+RST","ready",100)==0)
		{
			t_se=4;
		//	printf("t_se = 3,OK\r\n");
		}
		
		break;
	case 4:
		if(delay==0)
		{
			USART3_RX_STA=0;
			u3_printf("AT+CWJAP=\"helloworld\",\"4206832001\"\r\n");
			delay=1;
		}
		if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
		{
			
			if(atk_8266_check_cmd("WIFI GOT IP"))
			{
				t_se=5;
				delay=0;
		//	printf("t_se = 4,OK\r\n");
			}
			else
			{
				delay=0;
			}
			
				USART3_RX_STA=0;
		}
		
		break;
	case 5:
		if(delay==0)
		{
			USART3_RX_STA=0;
			u3_printf("AT+CIPMUX=0\r\n");
			delay=1;
		}
		if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
		{
			
			if(atk_8266_check_cmd("OK"))
			{
				t_se=6;
				delay=0;
		//	printf("t_se = 5,WiFi,OK\r\n");
			}
			
				USART3_RX_STA=0;
		}
		
		break;
	case 6:
		if(delay==0)
		{
			USART3_RX_STA=0;
			u3_printf("AT+CIPSTART=\"TCP\",\"175.178.27.47\",60\r\n");
			delay=1;
		}
		if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
		{
			
			if(atk_8266_check_cmd("OK"))
			{
				t_se=7;
				delay=0;
			}
			else
			{
				delay=0;
			}
			
				USART3_RX_STA=0;
		}
		
		break;
	case 7:
		atk_8266_send_cmd("AT+CIPMODE=1","OK",100);
		atk_8266_send_cmd("AT+CIPSEND",NULL,100);
		t_se=8;
		break;
	case 8:
		USART3_RX_STA=0;
		delay_ms(1000);
		u3_printf("search_weather\r\n");
		t_se=9;
		
		break;
	default:
		break;
	}
	delay_ms(2);
}






















































