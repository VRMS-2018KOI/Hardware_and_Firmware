/*
 * VRMS_User_Device_Firmware.c
 *
 * Created: 2018-07-10 오전 10:38:37
 * Author : juwon
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

#include <string.h>

#include "LCD_comgong2.h"

#define F_CPU 16000000UL		// Use 16MHz Crystal

#define read_DATA(address)				(*(volatile	unsigned char *)(0x8000 + address))						// read user DATA
#define write_DATA(address, value)		((*(volatile unsigned char *)(0x8000 + address)) = value)			// write user DATA

LCD lcd;

char ID[15] = {'\0', }, PW[15] = {'\0', }, Device[5] = {0, }, BT_ADDr[20] = {'\0', };

// PORT Number Value Setting
char PF_BT_KEY = 6, PF_BT_VCC = 5;			// Bluetooth Module Control Pin
char PE_SW1 = 4, PE_SW2 = 5, PE_USB = 6;	// External Interrupts Pin
char PF_Red_LED = 0, PF_Green_LED = 1;		// Status LED Pin

int Timer0 = 0, point = 0, status = 0;

void Init(void)
{
	// PORT Input/Output Setting
	DDRF = (1 << PF_Red_LED)|(1 << PF_Green_LED)|(1 << PF_BT_VCC)|(1 << PF_BT_KEY);		// All Output Setting
	DDRE = (0 << PE_SW1)|(0 << PE_SW2)|(0 << PE_USB);									// All Input Setting

	// Interrupts Setting
	EICRB = 63 << 0;		// EICRB = 0011 1111B, Enable Rising Edge Interrupts INT4, INT5, INT6

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
	
	for(int i = 0; i < 5; i++)
		Device[i] = read_DATA(500+i);
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

int Check_User_Data(char *ID[], char *PW[])
{
	char ID_size, PW_size;
	
	if ((ID_size = read_DATA(0)) == 0)
		return 0;
	
	for (int i = 0; i < ID_size; i++)
		(*ID)[i] = read_DATA(i+1);
	*ID[ID_size] = '\0';
	
	PW_size = read_DATA(ID_size+1);
	
	for (int i = 0; i < PW_size; i++)
		(*PW)[i] = read_DATA(i+ID_size+2);
	*PW[PW_size] = '\0';
		
	return 1;
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

void Bluetooth_Connect_Receive(char *str)
{
	cli();
	int num = 0;
	while(Bluetooth_Receive() != '+'){}
	while(Bluetooth_Receive() != ':'){}
	while((*str[num] = Bluetooth_Receive()) != ':')
	{
		num++;
	}
	*str[num] = ',';
	
	do
	{
		num++;
	}
	while((*str[num] = Bluetooth_Receive()) != ':');
	*str[num] = ',';
	
	do 
	{
		num++;
	}
	while((*str[num] = Bluetooth_Receive()) != ',');
	*str[num] = '\0';
}

int Bluetooth_Point_Receive(void)
{
	cli();
	int num = 0; char str_ID[15] = {'\0', }, str_Point[15] = {'\0', };
		
	while(Bluetooth_Receive() != '{'){}
	while((str_ID[num] = Bluetooth_Receive()) != ','){}
	str_ID[num] = '\0';
	
	while(Bluetooth_Receive() != ' '){}
	while((str_Point[num] = Bluetooth_Receive()) != '}'){}
	str_Point[num] = '\0';
	
	if (!strcmp(ID, str_ID))
	{
		return atoi(str_Point);
	}
	return -1;
}

void Bluetooth_Init(void)
{
	PORTF |= 1 << PF_BT_KEY;
	_delay_ms(50);
	PORTF |= 1 << PF_BT_VCC;
	_delay_ms(50);
	
	Bluetooth_Transmit_STR("AT+INIT\r\n");
	_delay_ms(20);
	Bluetooth_Transmit_STR("AT+ROLE=1\r\n");
	_delay_ms(20);
	Bluetooth_Transmit_STR("AT+CLASS=E7CA9\r\n");
	_delay_ms(20);
	Bluetooth_Transmit_STR("AT+INQM=1,1,48\r\n");
	_delay_ms(20);
}

void Bluetooth_Connect(char *str[])
{
	char New_ADDr[20] = {0, };
	Bluetooth_Transmit_STR("AT+INQ\r\n");
	Bluetooth_Connect_Receive(New_ADDr);
	
	if (!strcmp(*str, New_ADDr))
		return;
	else
	{
		char Connect[30] = "AT+LINK=";
		*str[0] = '\0';
		strcpy(*str, New_ADDr);
		strcat(New_ADDr, Connect);
		Bluetooth_Transmit_STR(Connect);
		_delay_ms(20);
		return;
	}
}

void USB_Transmit( unsigned char data )
{
	while ( !( UCSR0A & (1<<UDRE)) );
	UDR0 = data;
}

void USB_Transmit_STR(unsigned char *STR)
{
	while (*STR != 0)
	{
		USB_Transmit(*STR);
		++STR;
	}
}

unsigned char USB_Receive(void)
{
	while ( !(UCSR0A & (1<<RXC)));
	return UDR0;
}

int USB_Connect_Recive( void )
{
	cli();
	int num = 0; char str[20] = {'\0', };
	while(Bluetooth_Receive() != '{'){}
	while((str[num] = Bluetooth_Receive()) != '}') {}
	{
		num++;
	}
	str[num] = '\0';
	
	if (!strcmp(str, "Connect"))
	{
		USB_Transmit_STR(Device);
		USB_Transmit_STR("\r\n");
		return 1;
	}
	return 0;
}

void USB_User_Recive(char *ID[], char *PW[])
{
	cli();
	int num = 0;
	while(Bluetooth_Receive() != '{'){}
	while((*ID[num] = Bluetooth_Receive()) != '}') {}
	{
		num++;
	}
	*ID[num] = '\0';
	
	num = 0;
	while(Bluetooth_Receive() != '{'){}
	while((*PW[num] = Bluetooth_Receive()) != '}') {}
	{
		num++;
	}
	*PW[num] = '\0';	
}

int main(void)
{
	
	Init();
	
	LCD_Init();
	
	Bluetooth_Init();
	
	if (!Check_User_Data(ID, PW))
		while(!((EIFR & (1 << 6)) >> 6)){}
	
    while (1) 
    {
		sei();
		
		Bluetooth_Connect(BT_ADDr);
		
		_delay_ms(500);
    }
}

// Interrupts
ISR(INT4_vect)
{	/*  SW1 Interrupts  */
	cli();
	
	do
	{
		char AT[40] = "{\"0\", \"";
		strcat(ID, AT);
		strcat("\", \"", AT);
		strcat(PW, AT);
		strcat("\", \"", AT);
		strcat(Device, AT);
		strcat("\"}", AT);
		
		Bluetooth_Transmit_STR(AT);
		
		point = Bluetooth_Point_Receive();
		
	} while(point == -1);
	
	char Point_str[15] = {'\0', };
	sprintf(Point_str, "%d", point);
	
	LCD_Clear();
	_delay_ms(2);
	LCD_Write_Str(&lcd, "Name : ");
	_delay_ms(20);
	LCD_Write_Str(&lcd, ID);
	_delay_ms(20);
	LCD_Pos(1, 0);
	_delay_ms(2);
	LCD_Write_Str(&lcd, "Point : ");
	_delay_ms(20);
	LCD_Write_Str(&lcd, Point_str);
	_delay_ms(20);
	
	
}

ISR(INT5_vect)
{	/*  SW2 Interrupts  */
	cli();
	char AT[40] = "{\"1\", \"";
	strcat(ID, AT);
	strcat("\", \"", AT);
	strcat(PW, AT);
	strcat("\", \"", AT);
	strcat(Device, AT);
	strcat("\"}", AT);
	Bluetooth_Transmit_STR(AT);
	
	PORTF |= 1 << PF_Red_LED;
	PORTF &= ~(1 << PF_Green_LED);
	
}

ISR(INT6_vect)
{	/*  USB Power In Interrupts  */
	cli();
	int i;
	do 
	{
		i = USB_Connect_Recive();
	} while (i == 0);
	
	USB_User_Recive(ID, PW);
	
	while(((EIFR & (1 << 6)) >> 6)){}
}

ISR(TIMER0_OVF_vect)
{	/*  Send Status To Server  */
	if(Timer0==3660)
	{
		cli();
		
		do 
		{
			
			char AT[40] = "{\"0\", \"";
			strcat(ID, AT);
			strcat("\", \"", AT);
			strcat(PW, AT);
			strcat("\", \"", AT);
			strcat(Device, AT);
			strcat("\"}", AT);
			
			Bluetooth_Transmit_STR(AT);
		
			point = Bluetooth_Point_Receive();
		
		} while(point == -1);	
		
		char Point_str[15] = {'\0', };
		sprintf(Point_str, "%d", point);
		
		LCD_Clear();
		_delay_ms(2);
		LCD_Write_Str(&lcd, "Name : ");
		_delay_ms(20);
		LCD_Write_Str(&lcd, ID);
		_delay_ms(20);
		LCD_Pos(1, 0);
		_delay_ms(2);
		LCD_Write_Str(&lcd, "Point : ");
		_delay_ms(20);
		LCD_Write_Str(&lcd, Point_str);
		_delay_ms(20);
		
		PORTF = !(PORTF ^ (1<<PF_Red_LED));
	}
	else
	{
		Timer0++;
	}
}

ISR(TIMER2_OVF_vect)
{	/*  Send Status To Server  */
	if(Timer0==61)
	{
		PORTF = ~(PORTF ^ (1<<PF_Green_LED));
	}
	else
	{
		Timer0++;
	}
}