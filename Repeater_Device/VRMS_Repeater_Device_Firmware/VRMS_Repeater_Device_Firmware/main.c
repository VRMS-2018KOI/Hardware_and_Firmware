/*
 * VRMS_Repeater_Device_Firmware.c
 *
 * Created: 2018-07-06 오후 3:01:31
 * Author : juwon
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#include <string.h>
#include <stdlib.h>

#include "LCD_comgong2.h"

#define F_CPU 16000000UL		// Use 16MHz Crystal

LCD lcd;

char ID[15] = {'\0', }, PW[15] = {'\0', }, Device[5] = {0, }, GPS_X[10] = {'\0', }, GPS_Y[15] = {'\0', }, ADDr;

// PORT Number Value Setting
char PF_BT_KEY = 6, PF_BT_VCC = 5;			// Bluetooth Module Control Pin
char PF_CH_Change = 4, PF_UART_EN = 3;
char PB_SW1 = 3, PB_SW2 = 4, PB_SW3 = 5, PB_SW4 = 6; // Repeater ADDr Setting
char PF_Red_LED = 0, PF_Green_LED = 1;		// Status LED Pin
char PE_RF_OPTION = 2, PE_RF_VCC = 3;
char PE_GPS_Ready = 4;

int Timer0 = 0;

void Init(void)
{
	// PORT Input/Output Setting
	DDRF = (1 << PF_Red_LED)|(1 << PF_Green_LED)|(1 << PF_BT_VCC)|(1 << PF_BT_KEY)|(1 << PF_CH_Change)|(1 << PF_UART_EN);	// All Output Setting
	DDRB = (0 << PB_SW1)|(0 << PB_SW2)|(0 << PB_SW3)|(0 << PB_SW4);															// All Input Setting
	DDRE = (1 << PE_RF_OPTION)|(1 << PE_RF_VCC)|(0 << PE_GPS_Ready);

/*	USART0 Setting
	Baud Rate	: 9600
	Data Bit	: 8 Bit
	Stop Bit	: 1 Bit
	Parity Bit	: NONE  */
	
	/* Set baud rate */
	UBRR0H = (unsigned char)(103>>8);
	UBRR0L = (unsigned char)103;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = 3<<UCSZ0;
	
/*	USART1 Setting
	Baud Rate	: 38400
	Data Bit	: 8 Bit
	Stop Bit	: 1 Bit
	Parity Bit	: NONE  */
	
	/* Set baud rate */
	UBRR1H = (unsigned char)(25>>8);
	UBRR1L = (unsigned char)25;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSR1C = 3<<UCSZ0;
	
	TCCR0= 0x07;
	TCCR2= 0x05;
	TIMSK=0x65;
	
	ADDr = (((PINB & (1 << PB_SW1)) >> PB_SW1) << 0) | (((PINB & (1 << PB_SW2)) >> PB_SW2) << 1) | (((PINB & (1 << PB_SW3)) >> PB_SW3) << 2) | (((PINB & (1 << PB_SW4)) >> PB_SW4) << 3);
	
	PORTF |= (1 << PF_UART_EN);
}

void LCD_Init(void)
{
	DDRA = 0xFF;
	DDRG = 0x07;
	LCD_Data_Pins_seting(&lcd, 0x3B, 0, 1, 2, 3, 4, 5, 6, 7);
	LCD_RSW_Pins_seting(&lcd, 0x65, 2, 1, 0);
	
	LCD_Write_Str(&lcd, "User Device 1.0v");
	
	_delay_ms(2);
	
	LCD_Pos(1, 0);
}

void LCD_Clear()
{
	LCD_Function_Set(&lcd, 1, 1, 0);
	_delay_ms(2);
	LCD_Display_Control(&lcd, 1, 0, 0);
	_delay_ms(2);
	LCD_Display_Clear(&lcd);
	_delay_ms(2);
	LCD_Entry_Mode(&lcd, 1, 0);
	_delay_ms(2);
}

void Bluetooth_Transmit( unsigned char data )
{
	while ( !( UCSR1A & (1<<UDRE)) );
	UDR1 = data;
}

void Bluetooth_Transmit_STR(unsigned char *STR)
{
	while (*STR != 0)
	{
		Bluetooth_Transmit(*STR);
		++STR;
	}
}

unsigned char Bluetooth_Receive(void)
{
	while ( !(UCSR1A & (1<<RXC)));
	return UDR1;
}

void Bluetooth_Init(void)
{
	PORTF |= 1 << PF_BT_KEY;
	_delay_ms(50);
	PORTF |= 1 << PF_BT_VCC;
	_delay_ms(50);
	
	Bluetooth_Transmit_STR("AT+INIT\r\n");
	_delay_ms(20);
	Bluetooth_Transmit_STR("AT+CLASS=E7CA9\r\n");
	_delay_ms(20);
}

unsigned char U0_Receive(void)
{
	while ( !(UCSR0A & (1<<RXC)));
	return UDR0;
}

void U0_Transmit( unsigned char data )
{
	while ( !( UCSR0A & (1<<UDRE)) );
	UDR0 = data;
}

void GPS_Recive(char *GPS_Data[])
{
	int num = 0;
	while(U0_Receive() != '&'){}
	while((*GPS_Data[num] = U0_Receive()) != '\r')
	{
		num++;
	}
	*GPS_Data[num] = '\0';
}

void GPS_Init(void)
{
	char GPS_DATA[100];
	do 
	{
		GPS_DATA[0] = "\0";
		GPS_Recive(GPS_DATA);
		
	} while (!strncmp(GPS_DATA, "GPGGA", 5));
	
	int n = 0;
	int j = 0;
	
	while (GPS_DATA[n] == ','){n++;}
	do
	{
		n++;
	}while (GPS_DATA[n] == ',');
	n++;
	while ((GPS_X[j] = GPS_DATA[n]) == ','){n++; j++;}
	GPS_X[j] = '\0';
	j = 0; n++;
	
	while ((GPS_Y[j] = GPS_DATA[n]) == ','){n++; j++;}
	GPS_Y[j] = '\0';
	
	GPS_X[4] = GPS_X[3];
	GPS_X[3] = GPS_X[2];
	GPS_X[2] = '.';
	
	GPS_Y[5] = GPS_Y[4];
	GPS_Y[4] = GPS_Y[3];
	GPS_Y[3] = '.';
}

void RF_Transmit_STR(unsigned char *STR)
{
	while (*STR != 0)
	{
		U0_Transmit(*STR);
		++STR;
	}
}

void RF_Init(void)
{
	PORTF |= 1 << PF_CH_Change;
	
	UBRR1H = (unsigned char)(51>>8);
	UBRR1L = (unsigned char)51;
	
	char char_ADDr = '';
	
	itoa( ADDr, char_ADDr, 16 );
	
	RF_Transmit_STR("@IDFFFFFF");
	U0_Transmit(char_ADDr);
	U0_Transmit('@');
	
	_delay_ms(20);
	
	RF_Transmit_STR("@REST@");
	
	_delay_ms(20);
}

void Wifi_Init(void)
{
	UBRR0H = (unsigned char)(8>>8);
	UBRR0L = (unsigned char)8;
	
}




int main(void)
{
    Init();
	
	if(ADDr == 0)
	{
		Wifi_Init();
	}
	else
	{
		GPS_Init();
		Bluetooth_Init();
	}
	
	RF_Init();
	
    while (1) 
    {
		// Send to Server
    }
}

