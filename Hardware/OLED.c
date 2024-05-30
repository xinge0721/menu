#include "stm32f10x.h"
#include "Delay.h"
#include "OLED_Font.h"

#define OLED_WriteReg_Cmd(data) 	(OLED_WriteCommand(0x00,data))
#define OLED_WriteReg_data(data) 	(OLED_WriteCommand(0x40,data))
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x))

uint8_t OLED_data ;


void OLED_I2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}

void OLED_WriteCommand(uint8_t ad,uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);
	OLED_I2C_SendByte(ad);
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}
	
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteReg_Cmd(0xB0 | Y);					//设置Y位置
	OLED_WriteReg_Cmd(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteReg_Cmd(0x00 | (X & 0x0F));			//设置X位置低4位
}

void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteReg_data(0x00);
		}
	}
}


void OLED_Init(void)
{
	Delay_ms(100);             //上电后给点延时，等待单片机系统稳定在配置，否则有的型号的屏幕芯片会初始化失败
	OLED_I2C_Init();
	OLED_WriteReg_Cmd(0xAE);
		
	OLED_WriteReg_Cmd(0x20);//寻址模式配置
	OLED_WriteReg_Cmd(0x02);//页地址模式
	
	OLED_WriteReg_Cmd(0xB0);//设置页开始地址作为页地址模式 
	OLED_WriteReg_Cmd(0x18);//设置低一点的列的开始地址作为页地址模式
	OLED_WriteReg_Cmd(0x01);//设置列的高地址作为页的开始地址 
	
	OLED_WriteReg_Cmd(0xD5);//设置屏幕刷新率
	OLED_WriteReg_Cmd(0X80);//最高频率
	
	
	OLED_WriteReg_Cmd(0xA8);//设置多路复用比例
	OLED_WriteReg_Cmd(0x3F);//范围从 16 到 63
	
	OLED_WriteReg_Cmd(0xDA);//com引脚硬件配置
	OLED_WriteReg_Cmd(0x12);//不同设备需要不同的参数，才能让画面显示正常
		
	OLED_WriteReg_Cmd(0xA1);//水平正常(A0相反)
	OLED_WriteReg_Cmd(0xC8);//正常显示(C0相反)
		
	OLED_WriteReg_Cmd(0x40);//设置显示起始行(0x40~0x7f,而0x40就是第一行的意思)
				
	OLED_WriteReg_Cmd(0xD3);//屏幕垂直偏移量
	OLED_WriteReg_Cmd(0x00);//偏移量0(范围0-63D)

	OLED_WriteReg_Cmd(0x81);//设置对比度
	OLED_WriteReg_Cmd(0xff);//设置为FF最高亮度
	
	OLED_WriteReg_Cmd(0xD9); // 设置预充电期间的持续时间 -- set pre-charge period
    OLED_WriteReg_Cmd(0xF1); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	
	
	OLED_WriteReg_Cmd(0xDB);//设置VcomMH反压值
	OLED_WriteReg_Cmd(0x30);
	


	OLED_WriteReg_Cmd(0x8D);//使能电荷泵稳压器
	OLED_WriteReg_Cmd(0x14);//该位为1开启电荷泵
	

	OLED_WriteReg_Cmd(0xA4);//恢复显示RAM内容
	
	OLED_WriteReg_Cmd(0xA6);//设置为普通显示模式(6H：正常显示 A7:反色显示)

	OLED_WriteReg_Cmd(0xAF);//开启显示命令
	OLED_Clear();
}	


/**
  * @概要   打印字符
  * @参数	起始行
  * @参数	起始列
  * @参数	需要打印字符在数组中相对应的位置
  * @返回值 无
  */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t Char)
{
	int i = 0;
	OLED_SetCursor((2*x)-2,(16*y)-16);
	uint16_t adder = 0;
	do
	{
		for(int j = 0; j < 16 ; j++)
		{
			OLED_WriteReg_data(OLED_F8x16[Char][adder]);
			adder++;
		}
		OLED_SetCursor((2*x)-1,(16*y)-16);
	}while(!i++);
}

/**
  * @概要   高亮一个字符
  * @参数	起始行
  * @参数	起始列
  * @参数	需要打印字符在数组中相对应的位置
  * @返回值 无
  */
void OLED_ShowCharbright(uint8_t x,uint8_t y,uint8_t Char)
{
		int i = 0;
	OLED_SetCursor((2*x)-2,(16*y)-16);
	uint16_t adder = 0;
	do
	{
		for(int j = 0; j < 16 ; j++)
		{
			OLED_WriteReg_data(~(OLED_F8x16[Char][adder]));
			adder++;
		}
		OLED_SetCursor((2*x)-1,(16*y)-16);
	}while(!i++);
}

/**
  * @概要   打印一个字符串
  * @参数	起始行
  * @参数	起始列
  * @参数	需要打印字符在数组中相对应的位置
  * @返回值 无
  */
void OLED_ShowCharsent(uint8_t x,uint8_t y,uint8_t Char,uint8_t data)
{
	while(data--)
	{
		OLED_ShowChar(x,y++,Char++);
	}
}
/**
  * @概要   高亮一个字符串
  * @参数	起始行
  * @参数	起始列
  * @参数	需要打印字符在数组中相对应的位置
  * @返回值 无
  */
void OLED_ShowCharsentbright(uint8_t x,uint8_t y,uint8_t Char,uint8_t data)
{
	while(data--)
	{
		OLED_ShowCharbright(x,y++,Char++);
	}
}



