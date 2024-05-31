
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"

KEY_STATE KeyState = KEY_CHECK;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

uint8_t Key_Scan(void)
{
	static uint8_t KeyFlag;    //按键标志
	switch (KeyState)
	{
		case KEY_CHECK:
			if(!KEY)
				KeyState = KEY_CONFIRM;
			break;
		case KEY_CONFIRM:
			if(!KEY)
			{
				KeyState = KEY_RELEASE;
				if(Key0 == 0)
				KeyFlag = 1;
				else if(Key1 == 0)
				KeyFlag = 2;
				else if(Key2 == 0)
				KeyFlag = 3;
				else if(Key3 == 0)
				KeyFlag = 4;
			}
			else 
			{
				KeyState = KEY_CHECK;
			}
			break;

		case KEY_RELEASE:
		{	
			if(KEY)//当前按键值为1，说明按键已经释放，切换到开始状态
            {
			KeyState = KEY_CHECK;
			return KeyFlag;
			}
		}
			break;
	}
	return 0;
}
