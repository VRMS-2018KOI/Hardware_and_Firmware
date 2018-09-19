/*
 * LCD_comgong2.h
 *
 * Created: 2018-06-20 오전 9:22:39
 *  Author: juwon
 */ 

#ifndef LCD_COMGONG2_H_
#define LCD_COMGONG2_H_

typedef struct _LCD
{
	volatile unsigned char *Data[8];
	volatile unsigned char *RS;
	volatile unsigned char *RW;
	volatile unsigned char *EN;
	char Num_Data[8];
	char Num_RS;
	char Num_RW;
	char Num_EN;
}LCD ;

typedef LCD Pin_Seting;

void LCD_Data_Pin_seting(int Data_Num, Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_Pin);
void LCD_RS_Pin_seting(Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_Pin);
void LCD_RW_Pin_seting(Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_Pin);
void LCD_EN_Pin_seting(Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_Pin);

void LCD_Data_Pins_seting(Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_Pin0, char Num_Pin1, char Num_Pin2, char Num_Pin3, char Num_Pin4, char Num_Pin5, char Num_Pin6, char Num_Pin7);
void LCD_RSW_Pins_seting(Pin_Seting *pin_seting, volatile unsigned char *Pin, char Num_RS_Pin, char Num_RW_Pin, char Num_EN_Pin);

void LCD_Pin_Bit_Clear(Pin_Seting *pin_seting);

void LCD_Display_Clear(Pin_Seting *pin_seting);

void LCD_Return_Cursor(Pin_Seting *pin_seting);

void LCD_Entry_Mode(Pin_Seting *pin_seting, char ID, char SH);

void LCD_Display_Control(Pin_Seting *pin_seting, char D, char C, char B);

void LCD_Cursor_Control(Pin_Seting *pin_seting, char SC, char RL);

void LCD_Function_Set(Pin_Seting *pin_seting, char DL, char N, char F);

void LCD_CGRAM_ADDR(Pin_Seting *pin_seting, unsigned char ADDR, char en);

void LCD_DDRAM_ADDR(Pin_Seting *pin_seting, unsigned char ADDR);

int LCD_BF_Flag(Pin_Seting *pin_seting, unsigned char ADDR);

void LCD_Write_Data(Pin_Seting *pin_seting, unsigned char W_DATA);

unsigned char LCD_Read_Data(Pin_Seting *pin_seting);

void LCD_Write_Char(Pin_Seting *pin_seting, unsigned char Data);

void LCD_Write_Str(Pin_Seting *pin_seting, unsigned char *STR);

void LCD_Pos(unsigned char col, unsigned char row);

#endif /* LCD_COMGONG2_H_ */