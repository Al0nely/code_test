#include "key.h"


void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);
    
    //�˿�ģʽ���� KEY0��KEY1����ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
}


u8 key_scanf(u8 mode)   //modeΪ0��֧����������modeΪ1֧��������
{
    static u8 key_up = 1;   
    if(mode)   
        key_up = 1; //֧��������
    if(key_up && (KEY0 == 0))
    {
        delay_ms(10);
        key_up = 0; //����־λ��0
        if(KEY0 == 0)
            return 1;
    }
    else if(KEY0 == 1)
        key_up = 1;
    
    return 0;
}


