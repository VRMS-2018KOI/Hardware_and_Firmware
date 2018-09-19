#include <avr/io.h>
#include <util/delay.h>

volatile unsigned char *RSW = (volatile unsigned char *)0x65;
volatile unsigned char *DATA = (volatile unsigned char *)0x3B;

void LCD_PIN_SET(volatile unsigned char *RE_RSW, volatile unsigned char *RE_DATA)
{
	*RSW = (volatile unsigned char *)RE_RSW;
	*DATA = (volatile unsigned char *)RE_DATA;
}

void LCD_DISPLAY_CLEAR()
{
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*DATA = (1 << 0) | (0 << 1) | (0 << 2) | (0 << 3) | (0 << 4) | (0 << 5) | (0 << 6) | (0 << 7);
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*DATA = 0x00;
	_delay_ms(2); 
}

void LCD_RETURN_CURSOR()
{
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*DATA = (0 << 0) | (1 << 1) | (0 << 2) | (0 << 3) | (0 << 4) | (0 << 5) | (0 << 6) | (0 << 7);
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*DATA = 0x00;
}

void LCD_ENTRY_MODE(int ID, int SH)
{
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*DATA = (SH << 0) | (ID << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (0 << 5) | (0 << 6) | (0 << 7);
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*DATA = 0x00;
}

void LCD_DISPLAY_CONTROL(int D, int C, int B)
{
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*DATA = (B << 0) | (C << 1) | (D << 2) | (1 << 3) | (0 << 4) | (0 << 5) | (0 << 6) | (0 << 7);
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*DATA = 0x00;
}

void LCD_CURSOR_CONTROL(int SC, int RL)
{
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*DATA = (0 << 0) | (0 << 1) | (RL << 2) | (SC << 3) | (1 << 4) | (0 << 5) | (0 << 6) | (0 << 7);
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*DATA = 0x00;
}

void LCD_FUNCTION_SET(int DL, int N, int F)
{
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*DATA = (0 << 0) | (0 << 1) | (F << 2) | (N << 3) | (DL << 4) | (1 << 5) | (0 << 6) | (0 << 7);
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*DATA = 0x00;
}

void LCD_CGRAM_ADDR(unsigned char ADDR, int en)
{
	*RSW = (0 << 0) | (0 << 1) | (en << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (en << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*DATA = (((ADDR & (1 << 0)) >> 0) << 0) | (((ADDR & (1 << 1)) >> 1) << 1) | (((ADDR & (1 << 2)) >> 2) << 2) | (((ADDR & (1 << 3)) >> 3) << 3) | (((ADDR & (1 << 4)) >> 4) << 4) | (((ADDR & (1 << 5)) >> 5) << 5) | (1 << 6) | (0 << 7);
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*DATA = 0x00;
}

void LCD_DDRAM_ADDR(unsigned char ADDR)
{
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*DATA = (((ADDR & (1 << 0)) >> 0) << 0) | (((ADDR & (1 << 1)) >> 1) << 1) | (((ADDR & (1 << 2)) >> 2) << 2) | (((ADDR & (1 << 3)) >> 3) << 3) | (((ADDR & (1 << 4)) >> 4) << 4) | (((ADDR & (1 << 5)) >> 5) << 5) | (((ADDR & (1 << 6)) >> 6) << 6) | (1 << 7);
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*DATA = 0x00;
}

void LCD_BF_FLAG(int BF, unsigned char ADDR)
{
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*DATA = (((ADDR & (1 << 0)) >> 0) << 0) | (((ADDR & (1 << 1)) >> 1) << 1) | (((ADDR & (1 << 2)) >> 2) << 2) | (((ADDR & (1 << 3)) >> 3) << 3) | (((ADDR & (1 << 4)) >> 4) << 4) | (((ADDR & (1 << 5)) >> 5) << 5) | (((ADDR & (1 << 6)) >> 6) << 6) | (BF << 7);
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*DATA = 0x00;
}

void LCD_WRITE_DATA(unsigned char W_DATA, int RS)
{
	*RSW = (0 << 0) | (0 << 1) | (RS << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (RS << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*DATA = W_DATA;
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*DATA = 0x00;
}

unsigned char LCD_READ_DATA()
{
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	*RSW = (1 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1);
	asm volatile("NOP");
	asm volatile("NOP");
	*RSW = (0 << 0) | (0 << 1) | (0 << 2);
	asm volatile("NOP");
	return *DATA;
}

void LCD_Init()
{
	LCD_FUNCTION_SET(1, 1, 0);
	_delay_ms(2);
	LCD_DISPLAY_CONTROL(1, 0, 0);
	_delay_ms(2);
	LCD_DISPLAY_CLEAR();
	_delay_ms(2);
	LCD_ENTRY_MODE(1, 0);
	_delay_ms(2);
}

void LCD_SAVE_CHAR(unsigned char addr, unsigned char D[])
{
	LCD_CGRAM_ADDR(addr, 0);
	for (int i = 0; i < 8; i++)
		LCD_WRITE_DATA(D[i], 1);
}

void LCD_WRITE_CHAR(unsigned char Data)
{
	LCD_WRITE_DATA(Data, 1);
	_delay_ms(1);
}

void LCD_WRITE_STR(unsigned char *STR)
{
	while (*STR != 0)
	{
		LCD_WRITE_CHAR(*STR);
		++STR;
	}
}

void LCD_POS(unsigned char col, unsigned char row)
{
	LCD_WRITE_DATA(0x80 | (row + col * 0x40), 0);
}