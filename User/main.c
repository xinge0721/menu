#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Timer.h"
#include "lian2.h"



uint8_t Key_list,Key_naxt,KeyDeta;
typedef enum
{
	menu_light = 1,
	menu_electrical = 5,
	menu_temperature = 9
}word;


void menu(line* l1)
{
	line* ps  = l1->next;
	while(ps != l1)
	{
		OLED_ShowCharsent(ps->data,1,ps->line,4);
		ps = ps->next;
	}
}
uint8_t add;

int main(void)
{
	OLED_Init();				//OLED��ʼ��
	Key_Init();					//������ʼ��
	Timer_Init();				//�жϺ�����ʼ��
	line* l1 = line_Init();		//��ʼ���˵�����	
	line_tail(l1,1,menu_light);
	line_tail(l1,2,menu_electrical);
	line_tail(l1,3,menu_temperature);
	
	menu(l1);					//���˵���ӡ����Ļ��
	line* pz = l1->next;

	line* l2 = line_Init();		//��ʼ�����ƵĹ�������

	menu_tail(l2, pz, 1, 1);	//���ӹ�����˵�
	menu_tail_t(l2, pz, 1, 2);

	pz = pz->next;

	line* l3 = line_Init();		//��ʼ������Ĺ�������

	menu_tail(l3, pz, 1, 2);
	menu_tail_t(l3, pz, 1, 2);

	pz = pz->next;

	line* l4 = line_Init();		//��ʼ���¶ȵĹ�������
	menu_tail(l4, pz, 1, 3);
	menu_tail_t(l4, pz, 1, 2);


	
	line* pr = l1->next;		//����һ��ָ��ָ��˵��ڱ�λ����һλ
	
	OLED_ShowCharsentbright(pr->data,1,pr->line,4);		//��ǰ���˵���һ�и���
	line* pt = l1;
	line* ps = pr;									

	while (1)
	{
	switch (KeyDeta)		//��ⰴ���Ƿ񱻰���
	{
		case 1:				//����ǰ���һ�������¹���
			if (pr->next == pt )//���ָ�����һλ���ڱ�λ�����������
				{					
					OLED_ShowCharsent(pr->data,1,pr->line,4);//�����еĸ����ָ�
					pr = pr->next ->next;					 //��������next���ƹ��ڱ�λ
					OLED_ShowCharsentbright(pr->data,1,pr->line,4);//����һ�и���
				}
			else			//���ָ��û��ָ���ڱ�λ����һ�������������ĵ���һλ
			{
			    ps = pr;
				pr = pr->next;
				OLED_ShowCharsentbright(pr->data,1,pr->line,4);	//�����еĸ����ָ�
				OLED_ShowCharsent(ps->data,1,ps->line,4);		//��һ�и���
			}
			Delay_ms(100);							//��ͣһ�ᣬ�Է�����
			KeyDeta = 0;							//��������־λ�����ֹ���ַ���ִ�е����
			break;
		case 2:										//ԭ����ͬ���������ϵ�
			if (pr->prior == pt )
				{					
					OLED_ShowCharsent(pr->data,1,pr->line,4);
					
					pr = pr->prior ->prior;
					OLED_ShowCharsentbright(pr->data,1,pr->line,4);
					Delay_ms(100);
				}
			else
			{
				ps = pr;
				pr = pr->prior;
				OLED_ShowCharsentbright(pr->data,1,pr->line,4);
				OLED_ShowCharsent(ps->data,1,ps->line,4);
				Delay_ms(100);
			}
			KeyDeta = 0;
			break;
		case 3:
			pr = pr->up;
			menu(pr);
			KeyDeta = 0;
			break;

		}
	}
}
void TIM2_IRQHandler(void)
{	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)//1ms
	{
		Key_list++;
		if(Key_list == 10)
		{
			KeyDeta  = Key_Scan();
			Key_list = 0;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

