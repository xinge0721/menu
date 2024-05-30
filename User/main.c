#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Timer.h"
#include "lian2.h"



uint8_t Key_list,Key_naxt,KeyDeta,swap;
typedef enum
{									//�����ַ���Ӧ���к�
	menu_light = 1,				 	//�������
	menu_electrical = 5,			//�������
	menu_temperature = 9,			//�¶ȿ���
	menu_light_green = 13,			//�̵ƣ�
	menu_light_red   = 17,			//��ƣ�
	menu_electrical_switch = 21,	//������
	menu_electrical_speed  = 25,	//ת�٣�
}word;

typedef enum 
{
	first_line    = 1,  //��һ��
	second_line   = 2,	//�ڶ���
	thirdly_line  = 3,  //������
	fourthly_line = 4,  //������

}line_number;
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
	line_tail(l1,first_line,menu_light);
	line_tail(l1,second_line,menu_electrical);
	line_tail(l1,thirdly_line,menu_temperature);
	
	menu(l1);					//���˵���ӡ����Ļ��
	line* pz = l1->next;		//���˵��Ŀ��ص�λ����pr

	line* l2 = line_Init();		//��ʼ�����ƵĹ�������

	menu_tail(l2, pz, first_line, menu_light_green);	//���ӹ�����˵����ڱ�λ
	menu_tail_t(l2, pz, second_line, menu_light_red); //��������Ĺ���λ���Ҷ�Ӧ�Ĳ˵�λ�������Ĳ�����ϵ

	pz = pz->next;

	line* l3 = line_Init();		//��ʼ������Ĺ�������

	menu_tail(l3, pz, first_line, menu_electrical_switch);
	menu_tail_t(l3, pz, second_line, menu_electrical_speed);

	pz = pz->next;

	line* l4 = line_Init();		//��ʼ���¶ȵĹ�������
	menu_tail(l4, pz, first_line, 3);
	menu_tail_t(l4, pz,second_line, 2);


	
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
		case 2:										//��case 1��ͬ���������ϵ�
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
		case 3:					//�����л�
			if(!swap)
				pt = (pr->up) -> prior;
			else
				pt = l1;
			
				pr = pr->up;		//��ת��ָ�븶��pr�����ʱpr��ָ���ܲ˵�����֮��Ȼ
				OLED_Clear();		//�����Ļ��ֹ�˵�������߹��ܽ��滥�����
				menu(pt);			//���´�ӡ�½���
				OLED_ShowCharsentbright(pr->data,1,pr->line,4);		//��ǰ���˵���һ�и���
				KeyDeta = 0;		//��������־λ�����ֹ���ַ���ִ�е����
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

