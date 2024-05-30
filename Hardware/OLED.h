#ifndef __OLED_H
#define __OLED_H

//#define OLED_WriteReg_Cmd(data) 	(OLED_WriteReg(0x00,data))
//#define OLED_WriteReg_data(data) 	(OLED_WriteReg(0x40,data))

void OLED_Init(void);
void OLED_Clear(void);
void OLED_highlight(uint8_t x);

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t Char);
void OLED_ShowCharbright(uint8_t x,uint8_t y,uint8_t Char);
void OLED_ShowCharsent(uint8_t x,uint8_t y,uint8_t Char,uint8_t data);
void OLED_ShowCharsentbright(uint8_t x,uint8_t y,uint8_t Char,uint8_t data);

#endif
