#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Timer.h"
#include "lian2.h"

#define pr(x) pr->x

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
	OLED_Init();
	Key_Init();
	Timer_Init();
	line* l1 = line_Init();
	line_tail(l1,1,menu_light);
	line_tail(l1,2,menu_electrical);
	line_tail(l1,3,menu_temperature);
	menu(l1);
	mn*l2 =menu_Init();
	line* pz = l1->next;
	menu_tail(l2, pz, 1, 1);
	pz = pz->next;
	menu_tail(l2, pz, 2, 2);
	pz = pz->next;
	menu_tail(l2, pz, 3, 3);	
	line* pr  = l1->next;
	OLED_ShowCharsentbright(pr->data,1,pr->line,4);
	while (1)
	{
	line* pt = l1;
	switch (KeyDeta)
	{
		case 1:
			if (pr(next) == pt )
				{					
					OLED_ShowCharsent(pr->data,1,pr->line,4);
					pr = pr(next) ->next;
					OLED_ShowCharsentbright(pr->data,1,pr->line,4);
					Delay_ms(100);
					
				}
			else
			{
				line* ps = pr;
				pr = pr(next);
				OLED_ShowCharsentbright(pr->data,1,pr->line,4);
				OLED_ShowCharsent(ps->data,1,ps->line,4);
				Delay_ms(100);
			}
			KeyDeta = 0;
			break;
		case 2:
			if (pr(prior) == pt )
				{					
					OLED_ShowCharsent(pr->data,1,pr->line,4);
					
					pr = pr(prior) ->prior;
					OLED_ShowCharsentbright(pr->data,1,pr->line,4);
					Delay_ms(100);
				}
			else
			{
				line* ps = pr;
				pr = pr(prior);
				OLED_ShowCharsentbright(pr->data,1,pr->line,4);
				OLED_ShowCharsent(ps->data,1,ps->line,4);
				Delay_ms(100);
			}
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

