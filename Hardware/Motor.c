#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//����GPIOB��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//��PA4��PA5���ų�ʼ��Ϊ�������	
	
	PWM_Init();													//��ʼ��ֱ������ĵײ�PWM
}


/**
  * ��    ����ֱ����������ٶ�
  * ��    ����Speed Ҫ���õ��ٶȣ���Χ��-100~100
  * �� �� ֵ����
  */
void Motor_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)							//���������ת���ٶ�ֵ
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_1);	//PA4�øߵ�ƽ
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);	//PA5�õ͵�ƽ�����÷���Ϊ��ת
		PWM_SetCompare3(Speed);				//PWM����Ϊ�ٶ�ֵ
	}
	else									//���򣬼����÷�ת���ٶ�ֵ
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);	//PA4�õ͵�ƽ
		GPIO_SetBits(GPIOB, GPIO_Pin_1);	//PA5�øߵ�ƽ�����÷���Ϊ��ת
		PWM_SetCompare3(-Speed);			//PWM����Ϊ�����ٶ�ֵ����Ϊ��ʱ�ٶ�ֵΪ��������PWMֻ�ܸ�����
	}
}

