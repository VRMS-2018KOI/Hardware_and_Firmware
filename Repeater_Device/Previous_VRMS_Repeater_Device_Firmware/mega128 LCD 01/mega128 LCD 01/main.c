/*
 * mega128 LCD 01.c
 *
 * Created: 2017-10-28 오후 5:58:24
 * Author : juwon
 */ 

#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
#include "LCD.h"
#include <string.h>

unsigned char CR_LF[] = "\r\n";

unsigned char ADDR[] = "0x00";

void RF(char *jaddr, char *waddr)
{
	int num = 0;
	int num1 = 0;
	while(UDR0 != '\r'){}
	while(UDR0 != '\n'){}
	while((jaddr[num] = UDR0) != ',')
	{
		num++;
	}
	while((waddr[num1] = UDR0) != '\r')
	{
		num1++;
	}
	jaddr[num] = 0;
	waddr[num1] = 0;
}

void USART_1_Transmit_STR(unsigned char *STR)
{
	while (*STR != 0)
	{
		USART_1_Transmit(*STR);
		++STR;
	}
	USART_1_Transmit(CR_LF[0]);
	USART_1_Transmit(CR_LF[1]);
}

void USART_1_Transmit_SR(unsigned char *STR)
{
	while (*STR != 0)
	{
		USART_1_Transmit(*STR);
		++STR;
	}
}

void USART_1_Transmit( unsigned char data )
{
	while ( !( UCSR1A & (1<<UDRE)) );
	UDR1 = data;
}

unsigned char USART_1_Receive(void)
{
	while ( !(UCSR1A & (1<<RXC)));
	return UDR1;
}

void USART_1_Receive_STR(char *str)
{
	int num = 0;
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	while((str[num] = USART_1_Receive()) != '\r')
	{
		num++;
	}
	str[num] = 0;
}
void USART_1_Receive_SAND(char *str)
{
	int num = 0;
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	while((str[num] = USART_1_Receive()) != '\r')
	{
		num++;
	}
	str[num] = 0;
}

void USART_1_Receive_SR(char *str)
{
	int num = 0;
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	while((str[num] = USART_1_Receive()) != '\r')
	{
		num++;
	}
	str[num] = 0;
}

void USART_1_WIFI(char *str)
{
	int num = 0;
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	
	while((str[num] = USART_1_Receive()) != '\r')
	{
		num++;
	}
	str[num] = 0;
}

void USART_1(char *str)
{
	int num = 0;
	while(USART_1_Receive() != 'O'){}
	while((USART_1_Receive()) != 'K'){}
	/*{
		num++;
	}
	str[num] = 0;*/
}
void USART_1_A(char *str)
{
	int num = 0;
	while(USART_1_Receive() != '\r'){}
	while(USART_1_Receive() != '\n'){}
	while((USART_1_Receive()) != '\r'){}
	{
		num++;
	}
	str[num] = 0;
}

void USART_0_Transmit_STR(unsigned char *STR)
{
	while (*STR != 0)
	{
		USART_0_Transmit(*STR);
		++STR;
	}
}

void USART_0_Transmit( unsigned char data )
{
	while ( !( UCSR0A & (1<<UDRE)) );
	UDR0 = data;
}

unsigned char USART_0_Receive(void)
{
	while ( !(UCSR0A & (1<<RXC)));
	return UDR0;
}

void USART_0_Receive_STR(char *str)
{
	int num = 0;
	while(USART_0_Receive() != '\r'){}
	while(USART_0_Receive() != '\n'){}
	while((str[num] = USART_0_Receive()) != '\r')
	{
		num++;
	}
	str[num] = 0;
}

void PORT_set(void)
{
	DDRG = (1 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (0 << 5) | (0 << 6) | (0 << 7);
	DDRA = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
	DDRE = (1 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (0 << 5) | (0 << 6) | (0 << 7);
}

void UART_Init(void)
{
	unsigned int ubrr = 8;
	UBRR1H = (unsigned char) (ubrr >> 8);
	UBRR1L = (unsigned char) ubrr;
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);//(1<<RXCIE1);
	UCSR1C = (1<<UCSZ10)|(1<<UCSZ11);
	
	ubrr = 51;
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
}

void sand_data(int waddr, int jaddr)
{
	char sand[10] = {0,};
	
	LCD_Init();
	_delay_ms(10000);
	LCD_WRITE_STR("Sending...");
	_delay_ms(1000);
	LCD_POS(1, 0);
	_delay_ms(1000);
	USART_1_Transmit_STR("AT+CIPSEND=46");
	_delay_ms(10000);
	switch(waddr)
	{
		case 1:	LCD_WRITE_STR("waddr:1 ");
				USART_1_Transmit_SR("GET /waddr/waddr1");
				break;
		case 2:	LCD_WRITE_STR("waddr:2 ");
				USART_1_Transmit_SR("GET /waddr/waddr2");
				break;
		case 3:	LCD_WRITE_STR("waddr:3 ");
				USART_1_Transmit_SR("GET /waddr/waddr3");
				break;
		case 4:	LCD_WRITE_STR("waddr:4 ");
				USART_1_Transmit_SR("GET /waddr/waddr4");
				break;
		case 5:	LCD_WRITE_STR("waddr:5 ");
				USART_1_Transmit_SR("GET /waddr/waddr5");
				break;
		case 6:	LCD_WRITE_STR("waddr:6 ");
				USART_1_Transmit_SR("GET /waddr/waddr6");
				break;
	}
	switch(jaddr)
	{
		case 1:	LCD_WRITE_STR("jaddr:1");
				USART_1_Transmit_STR("/jaddr/jaddr1/ HTTP/1.1\r\n\r\n");
				USART_1_Receive_SAND(sand);
				break;
		case 2:	LCD_WRITE_STR("jaddr:2");
				USART_1_Transmit_STR("/jaddr/jaddr2/ HTTP/1.1\r\n\r\n");
				USART_1_Receive_SAND(sand);
				break;
		case 3:	LCD_WRITE_STR("jaddr:3");
				USART_1_Transmit_STR("/jaddr/jaddr3/ HTTP/1.1\r\n\r\n");
				USART_1_Receive_SAND(sand);
				break;
		case 4:	LCD_WRITE_STR("jaddr:4");
				USART_1_Transmit_STR("/jaddr/jaddr4/ HTTP/1.1\r\n\r\n");
				USART_1_Receive_SAND(sand);
				break;
		case 5:	LCD_WRITE_STR("jaddr:5");
				USART_1_Transmit_STR("/jaddr/jaddr5/ HTTP/1.1\r\n\r\n");
				USART_1_Receive_SAND(sand);
				break;
		case 6:	LCD_WRITE_STR("jaddr:6");
				USART_1_Transmit_STR("/jaddr/jaddr6/ HTTP/1.1\r\n\r\n");
				USART_1_Receive_SAND(sand);
				break;
	}
	_delay_ms(50000);
	LCD_Init();
	
	if (strcmp(sand, "SEND OK") == 0)
	{
		LCD_WRITE_STR("SUCCESS SEND");
		_delay_ms(50000);
		
		sand[0] = '\0';
	}
	else
	{
		LCD_WRITE_STR("ERROR");
		while(1){}
	}
}

void Loading(void)
{
	_delay_ms(1000);
//////////////////////////////////
	char AT[50] = {0, };
	char WIFI[50] = {0, };
	char SERVERS[50] = {0, };
	char REPEATER[50] = {0, };
		
	LCD_WRITE_STR("Loading...");
	_delay_ms(10000);
	
	LCD_POS(1, 0);
	_delay_ms(1000);
//////////////////////////////////

// Wi-Fi 모듈 동작 여부
	LCD_WRITE_STR("Wi-Fi AT:");
	USART_1_Transmit_STR("AT");
	USART_1_Receive_STR(AT);
	if (strcmp(AT, "OK") == 0)
	{
		_delay_ms(50000);
		LCD_WRITE_STR("OK");
		_delay_ms(50000);
		
		AT[0] = '\0';
	}
	else
	{
		LCD_WRITE_STR("ERROR");
		while(1){}
	}
//////////////////////////////////

// LCD 2번째 라인 초기화	
	LCD_POS(1, 0);
	_delay_ms(1000);
	LCD_WRITE_STR("                ");
	LCD_POS(1, 0);
	_delay_ms(1000);
//////////////////////////////////

// Wi-Fi 접속
	LCD_WRITE_STR("Wi-Fi:");
	USART_1_Transmit_STR("AT+CWMODE=1");
	_delay_ms(5000);
	USART_1_Transmit_STR("AT+CWJAP=\"olleh_WiFi_A586\",\"0000008057\"");
	USART_1_WIFI(WIFI);
	if (strcmp(WIFI, "WIFI GOT IP") == 0)
	{
		_delay_ms(50000);
		LCD_WRITE_STR("SUCCESS");
		_delay_ms(50000);
		
		WIFI[0] = '\0';
	}
	else if (strcmp(WIFI, "WIFI CONNECTED") == 0)
	{
		_delay_ms(50000);
		LCD_WRITE_STR("SUCCESS");
		_delay_ms(50000);
		
		WIFI[0] = '\0';
	}
	else
	{
		LCD_Init();
		_delay_ms(1000);
		LCD_WRITE_STR(WIFI);
		while(1){}
	}
	USART_1_Transmit_STR("AT+CIPMUX=0");
//////////////////////////////////

// LCD 2번째 라인 초기화
	LCD_POS(1, 0);
	_delay_ms(1000);
	LCD_WRITE_STR("                ");
	LCD_POS(1, 0);
	_delay_ms(5000);
//////////////////////////////////

// Servers 접속
	LCD_WRITE_STR("SERVERS:");
	_delay_ms(50000);
	
	USART_1_Transmit_STR("AT+CIPSTART=\"TCP\",\"119.207.186.129\",8888");
	USART_1_Receive_SR(SERVERS);
	if (strcmp(SERVERS, "OK") == 0)
	{
		_delay_ms(50000);
		LCD_WRITE_STR("SUCCESS");
		_delay_ms(50000);
		
		WIFI[0] = '\0';
	}
	else
	{
		LCD_WRITE_STR(SERVERS);
		while(1){}
	}
	
//////////////////////////////////

// LCD 2번째 라인 초기화
	LCD_POS(1, 0);
	_delay_ms(1000);
	LCD_WRITE_STR("                ");
	LCD_POS(1, 0);
	_delay_ms(50000);
//////////////////////////////////

// 중계기 전원 확인 
/*	LCD_WRITE_STR("REPEATER:");
	
	
	
	USART_0_Transmit_STR("\r\nOK\r\n");
	USART_0_Receive_STR(REPEATER);
	if (strcmp(REPEATER, "OK") == 0)
	{
		//_delay_ms(50000);
		LCD_WRITE_STR("SUCCESS");
		//_delay_ms(50000);
		
		WIFI[0] = '\0';
	}
	else
	{
		LCD_WRITE_STR(REPEATER);
		while(1){}
	}*/
//////////////////////////////////

	_delay_ms(50000);
}

void LCD_main(void)
{
	_delay_ms(1000);
	LCD_POS(0, 0);
	_delay_ms(2);
	LCD_WRITE_STR("Master_Repeater");
	_delay_ms(2);
	LCD_POS(1, 0);
	_delay_ms(2);
	LCD_WRITE_STR("MASTER");
}

int main(void)
{
	
	char a[90] = {0,};
	char b[90] = {0,};
	char c[90] = {0,};
	int num1 = 0, num2 = 0;
	
	UART_Init();
	_delay_ms(1000);
	PORT_set();
	PORTE = 0x03;
	_delay_ms(5);
	LCD_Init();
	Loading();
	LCD_Init();
	_delay_ms(1000);
	//LCD_main();
	
	_delay_ms(5000);
	//LCD_Init();
	_delay_ms(1000);
	//LCD_WRITE_STR("Sending...");
	USART_1_Transmit_STR("AT+CIPSEND=30");
	_delay_ms(10000);
	USART_1_Transmit_SR("GET /waddr/waddr1 HTTP/1.1\r\n\r\n");
	USART_1_A(a);
	_delay_ms(1000);
	//LCD_Init();
	_delay_ms(1000);
	LCD_WRITE_STR(a);
    while (1) 
    {
		/*char waddr[10] = {0, };
		char jaddr[10] = {0, };
		RF(jaddr, waddr);
		num1 = atoi(waddr);
		num2 = atoi(jaddr);
		sand_data(num1, num2);
		
		for(int i = 0; i <= 10; i++)
		{
			waddr[i] = 0;
			jaddr[i] = 0;
		}
		
		LCD_Init();
		_delay_ms(1000);
		LCD_main();*/
		
    }
}

