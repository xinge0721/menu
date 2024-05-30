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
	OLED_Init();				//OLED初始化
	Key_Init();					//按键初始化
	Timer_Init();				//中断函数初始化
	line* l1 = line_Init();		//初始化菜单链表	
	line_tail(l1,1,menu_light);
	line_tail(l1,2,menu_electrical);
	line_tail(l1,3,menu_temperature);
	
	menu(l1);					//将菜单打印在屏幕上
	line* pz = l1->next;

	line* l2 = line_Init();		//初始化开灯的功能链表

	menu_tail(l2, pz, 1, 1);	//连接功能与菜单
	menu_tail_t(l2, pz, 1, 2);

	pz = pz->next;

	line* l3 = line_Init();		//初始化电机的功能链表

	menu_tail(l3, pz, 1, 2);
	menu_tail_t(l3, pz, 1, 2);

	pz = pz->next;

	line* l4 = line_Init();		//初始化温度的功能链表
	menu_tail(l4, pz, 1, 3);
	menu_tail_t(l4, pz, 1, 2);


	
	line* pr = l1->next;		//创造一个指针指向菜单哨兵位的下一位
	
	OLED_ShowCharsentbright(pr->data,1,pr->line,4);		//提前将菜单第一列高亮
	line* pt = l1;
	line* ps = pr;									

	while (1)
	{
	switch (KeyDeta)		//检测按键是否被按下
	{
		case 1:				//如果是按键一，就向下滚动
			if (pr->next == pt )//如果指针的下一位是哨兵位则进入条件内
				{					
					OLED_ShowCharsent(pr->data,1,pr->line,4);//将该行的高亮恢复
					pr = pr->next ->next;					 //连走两次next，绕过哨兵位
					OLED_ShowCharsentbright(pr->data,1,pr->line,4);//将第一行高亮
				}
			else			//如果指针没有指向哨兵位，则一切正常，正常的到下一位
			{
			    ps = pr;
				pr = pr->next;
				OLED_ShowCharsentbright(pr->data,1,pr->line,4);	//将该行的高亮恢复
				OLED_ShowCharsent(ps->data,1,ps->line,4);		//下一行高亮
			}
			Delay_ms(100);							//暂停一会，以防出错
			KeyDeta = 0;							//将按键标志位清零防止出现反复执行的情况
			break;
		case 2:										//原来相同但这是向上的
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

