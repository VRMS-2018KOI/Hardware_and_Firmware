void LCD_PIN_SET(volatile unsigned char RE_RSW, volatile unsigned char RE_DATA);

void LCD_ENABLE(int E);

void LCD_DISPLAY_CLEAR();

void LCD_RETURN_CURSOR();

void LCD_ENTRY_MODE(int ID, int SH);

void LCD_DISPLAY_CONTROL(int D, int C, int B);

void LCD_CURSOR_CONTROL(int SC, int RL);

void LCD_FUNCTION_SET(int DL, int N, int F);

void LCD_CGRAM_ADDR(unsigned char ADDR, int en);

void LCD_DDRAM_ADDR(unsigned char ADDR);

void LCD_BF_FLAG(int BF, unsigned char ADDR);

void LCD_WRITE_DATA(unsigned char W_DATA, int RS);

unsigned char LCD_READ_DATA();

void LCD_Init();

void LCD_SAVE_CHAR(int addr, unsigned char D[]);

void LCD_WRITE_CHAR(unsigned char Data);

void LCD_WRITE_STR(unsigned char *STR);

void LCD_POS(unsigned char col, unsigned char row);