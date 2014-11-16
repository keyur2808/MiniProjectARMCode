#ifndef _LCD_H
#define _LCD_H

#define LCD_CTRL_DIR	IODIR0
#define LCD_CTRL_SET	IOSET0
#define LCD_CTRL_CLR	IOCLR0

#define LCD_DATA_DIR	IODIR1
#define LCD_DATA_SET	IOSET1
#define LCD_DATA_CLR	IOCLR1

#define LCDRS  (1 << 29)
#define LCDRW  (1 << 28)
#define LCDEN  (1 << 7)

#define LCD_D0 (1 << 16)
#define LCD_D1 (1 << 17)
#define LCD_D2 (1 << 18)
#define LCD_D3 (1 << 19)
#define LCD_D4 (1 << 20)
#define LCD_D5 (1 << 21)
#define LCD_D6 (1 << 22)
#define LCD_D7 (1 << 23)

#define LCD_DATA_MASK      (  LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3 |LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7 )

#define LCD_BUSY_FLAG      LCD_D7

#define LCD_CONTROL_MASK   ( LCDRS | LCDRW | LCDEN )

void  init_lcd(void);
void lcd_putstring(char *string);
void lcd_clear(void);
void LCD_setpos(unsigned char row,unsigned char column);
void lcd_command(unsigned char command);
void lcd_data(unsigned char data);
void lcd_num(unsigned long number);
float temp_reading(unsigned int ,unsigned int );



#endif
