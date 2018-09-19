/*
 * LCD_comgong.c
 *
 * Created: 2018-06-20 오전 9:23:05
 *  Author: juwon
 */ 

#include "LCD_comgong2.h"

void LCD_Data_Pin_seting(int Data_Num, Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_Pin)
{
	pin_seting -> Data[Data_Num] = (volatile unsigned char *)Pin;
	pin_seting -> Num_Data[Data_Num] = Num_Pin;
}
void LCD_RS_Pin_seting(Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_Pin)
{
	pin_seting -> RS = (volatile unsigned char *)Pin;
	pin_seting -> Num_RS = Num_Pin;
}
void LCD_RW_Pin_seting(Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_Pin)
{
	pin_seting -> RW = (volatile unsigned char *)Pin;
	pin_seting -> Num_RW = Num_Pin;
}
void LCD_EN_Pin_seting(Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_Pin)
{
	pin_seting -> EN = (volatile unsigned char *)Pin;
	pin_seting -> Num_EN = Num_Pin;
}

void LCD_Data_Pins_seting(Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_Pin0, char Num_Pin1, char Num_Pin2, char Num_Pin3, char Num_Pin4, char Num_Pin5, char Num_Pin6, char Num_Pin7)
{
	LCD_Data_Pin_seting(0, pin_seting, Pin, Num_Pin0);
	LCD_Data_Pin_seting(1, pin_seting, Pin, Num_Pin1);
	LCD_Data_Pin_seting(2, pin_seting, Pin, Num_Pin2);
	LCD_Data_Pin_seting(3, pin_seting, Pin, Num_Pin3);
	LCD_Data_Pin_seting(4, pin_seting, Pin, Num_Pin4);
	LCD_Data_Pin_seting(5, pin_seting, Pin, Num_Pin5);
	LCD_Data_Pin_seting(6, pin_seting, Pin, Num_Pin6);
	LCD_Data_Pin_seting(7, pin_seting, Pin, Num_Pin7);
}
void LCD_RSW_Pins_seting(Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_RS_Pin, char Num_RW_Pin, char Num_EN_Pin)
{
	LCD_RS_Pin_seting(pin_seting, Pin, Num_RS_Pin);
	LCD_RW_Pin_seting(pin_seting, Pin, Num_RW_Pin);
	LCD_EN_Pin_seting(pin_seting, Pin, Num_EN_Pin);
}

void LCD_Pin_Bit_Clear(Pin_Seting *pin_seting)
{
	for (int i = 0; i <= 7; i++)
		*pin_seting -> Data[i] &= ~(1 << (pin_seting -> Num_Data[i]));
	*pin_seting -> RS &= ~(1 << (pin_seting -> Num_RS));
	*pin_seting -> RW &= ~(1 << (pin_seting -> Num_RW));
	*pin_seting -> EN &= ~(1 << (pin_seting -> Num_EN));
}

void LCD_Display_Clear(Pin_Seting *pin_seting)
{
	LCD_Pin_Bit_Clear(pin_seting);
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	*pin_seting -> Data[0] |= (1 << (pin_seting -> Num_Data[0]));
	asm volatile("NOP"); asm volatile("NOP");
	LCD_Pin_Bit_Clear(pin_seting);
}

void LCD_Return_Cursor(Pin_Seting *pin_seting)
{
	LCD_Pin_Bit_Clear(pin_seting);
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	*pin_seting -> Data[1] |= (1 << (pin_seting -> Num_Data[1]));
	asm volatile("NOP"); asm volatile("NOP");
	LCD_Pin_Bit_Clear(pin_seting);
}

void LCD_Entry_Mode(Pin_Seting *pin_seting, char ID, char SH)
{
	LCD_Pin_Bit_Clear(pin_seting);
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	*pin_seting -> Data[0] |= (SH << (pin_seting -> Num_Data[0]));
	*pin_seting -> Data[1] |= (ID << (pin_seting -> Num_Data[1]));
	*pin_seting -> Data[2] |= (1 << (pin_seting -> Num_Data[2]));
	asm volatile("NOP"); asm volatile("NOP");
	LCD_Pin_Bit_Clear(pin_seting);
}

void LCD_Display_Control(Pin_Seting *pin_seting, char D, char C, char B)
{
	LCD_Pin_Bit_Clear(pin_seting);
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	*pin_seting -> Data[0] |= (B << (pin_seting -> Num_Data[0]));
	*pin_seting -> Data[1] |= (C << (pin_seting -> Num_Data[1]));
	*pin_seting -> Data[2] |= (D << (pin_seting -> Num_Data[2]));
	*pin_seting -> Data[3] |= (1 << (pin_seting -> Num_Data[3]));
	asm volatile("NOP"); asm volatile("NOP");
	LCD_Pin_Bit_Clear(pin_seting);
}

void LCD_Cursor_Control(Pin_Seting *pin_seting, char SC, char RL)
{
	LCD_Pin_Bit_Clear(pin_seting);
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	*pin_seting -> Data[2] |= (RL << (pin_seting -> Num_Data[2]));
	*pin_seting -> Data[3] |= (SC << (pin_seting -> Num_Data[3]));
	*pin_seting -> Data[4] |= (1 << (pin_seting -> Num_Data[4]));
	asm volatile("NOP"); asm volatile("NOP");
	LCD_Pin_Bit_Clear(pin_seting);
}

void LCD_Function_Set(Pin_Seting *pin_seting, char DL, char N, char F)
{
	LCD_Pin_Bit_Clear(pin_seting);
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	*pin_seting -> Data[2] |= (F << (pin_seting -> Num_Data[2]));
	*pin_seting -> Data[3] |= (N << (pin_seting -> Num_Data[3]));
	*pin_seting -> Data[4] |= (DL << (pin_seting -> Num_Data[4]));
	*pin_seting -> Data[5] |= (1 << (pin_seting -> Num_Data[5]));
	asm volatile("NOP"); asm volatile("NOP");
	LCD_Pin_Bit_Clear(pin_seting);
}

void LCD_CGRAM_ADDR(Pin_Seting *pin_seting, unsigned char ADDR, char en)
{
	LCD_Pin_Bit_Clear(pin_seting);
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	for (int i = 0; i <= 5; i++)
		*pin_seting -> Data[i] |= (((ADDR & (1 << i)) >> i) << (pin_seting -> Num_Data[i]));
	*pin_seting -> Data[6] |= (1 << (pin_seting -> Num_Data[6]));
	asm volatile("NOP"); asm volatile("NOP");
	LCD_Pin_Bit_Clear(pin_seting);
}

void LCD_DDRAM_ADDR(Pin_Seting *pin_seting, unsigned char ADDR)
{
	LCD_Pin_Bit_Clear(pin_seting);
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	for (int i = 0; i <= 6; i++)
		*pin_seting -> Data[i] |= (((ADDR & (1 << i)) >> i) << (pin_seting -> Num_Data[i]));
	*pin_seting -> Data[7] |= (1 << (pin_seting -> Num_Data[7]));
	asm volatile("NOP"); asm volatile("NOP");
	LCD_Pin_Bit_Clear(pin_seting);
}

int LCD_BF_Flag(Pin_Seting *pin_seting, unsigned char ADDR)		// Need DDRx Data (should update)
{
	LCD_Pin_Bit_Clear(pin_seting);
	*pin_seting -> RW |= (1 << (pin_seting -> Num_RW));
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	for (int i = 0; i <= 6; i++)
		*pin_seting -> Data[i] |= (((ADDR & (1 << i)) >> i) << (pin_seting -> Num_Data[i]));
	asm volatile("NOP"); asm volatile("NOP");
	int BF = ((*pin_seting -> Data[7] & (1 << pin_seting -> Num_Data[7])) >> pin_seting -> Num_Data[7]);
	LCD_Pin_Bit_Clear(pin_seting);
	return BF;
}

void LCD_Write_Data(Pin_Seting *pin_seting, unsigned char W_DATA)
{
	LCD_Pin_Bit_Clear(pin_seting);
	*pin_seting -> RS |= (1 << (pin_seting -> Num_RS));
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	for (int i = 0; i <= 7; i++)
		*pin_seting -> Data[i] |= (((W_DATA & (1 << i)) >> i) << (pin_seting -> Num_Data[i]));
	asm volatile("NOP"); asm volatile("NOP");
	LCD_Pin_Bit_Clear(pin_seting);
}

unsigned char LCD_Read_Data(Pin_Seting *pin_seting)				// Need DDRx Data (should update)
{
	LCD_Pin_Bit_Clear(pin_seting);
	*pin_seting -> RS |= (1 << (pin_seting -> Num_RS));
	*pin_seting -> RW |= (1 << (pin_seting -> Num_RW));
	asm volatile("NOP");
	*pin_seting -> EN |= (1 << (pin_seting -> Num_EN));
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
	unsigned char DATA = 0;
	for (int i = 0; i <= 7; i++)
		DATA = (((*pin_seting -> Data[i] & (1 << pin_seting -> Num_Data[7]) >> pin_seting -> Num_Data[7])) << i);
	asm volatile("NOP"); asm volatile("NOP");
	LCD_Pin_Bit_Clear(pin_seting);
	return DATA;
}

void LCD_Write_Char(Pin_Seting *pin_seting, unsigned char DATA)
{
	LCD_Write_Data(pin_seting, DATA);
	asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP"); asm volatile("NOP");
}

void LCD_Write_Str(Pin_Seting *pin_seting, unsigned char *STR)
{
	while (*STR != 0)
	{
		LCD_Write_Char(pin_seting, *STR);
		++STR;
	}
}

void LCD_Pos(unsigned char col, unsigned char row)
{
	LCD_Write_Data(0x80 | (row + col * 0x40), 0);
}