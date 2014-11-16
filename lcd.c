#include<LPC214x.H>
#include"lcd.h"

  void wait_lcd( void )
{
  LCD_DATA_DIR &= ~LCD_DATA_MASK;
  LCD_CTRL_CLR  |=  LCDRS;	
  LCD_CTRL_SET  |=  LCDRW |LCDEN;
  
  while(IOPIN1 & LCD_BUSY_FLAG);		/* wait for busy flag to become low */
  
  LCD_CTRL_CLR |= LCDEN | LCDRW;
  LCD_DATA_DIR |= LCD_DATA_MASK;
  
}

static void delay(int count)
{
  int j=0,i=0;

  for(j=0;j<count;j++)
  {
    /* At 60Mhz, the below loop introduces
    delay of 10 us */
    for(i=0;i<35;i++);
  }
}
 
void lcd_command( unsigned char command )
{
  unsigned int temp=0;
     
  temp = (unsigned int)command;
  temp = (temp << 16) & LCD_DATA_MASK;

  LCD_CTRL_CLR |= LCDRS;
  LCD_CTRL_SET |= LCDEN;  
  LCD_DATA_CLR |= LCD_DATA_MASK;
  LCD_DATA_SET |= temp;
  delay(10000);
  LCD_CTRL_CLR |= LCDEN;   
  
  LCD_DATA_SET = 0;
}

void lcd_data( unsigned char data )
{
  unsigned int temp = 0;
 
  temp = (unsigned int)data;
  temp = (temp << 16) & LCD_DATA_MASK;
    
  LCD_CTRL_SET |= LCDEN;  
  LCD_CTRL_SET |= LCDRS;
  delay(1);
  LCD_DATA_CLR |= LCDRS;
  LCD_DATA_CLR |= LCD_DATA_MASK;
  LCD_DATA_SET |= temp;
  delay(10000);
  LCD_CTRL_CLR |= LCDEN;
  
  LCD_DATA_SET = 0;
}

void set_lcd_port_output( void )
{
  LCD_CTRL_DIR |=  LCDEN ;  
  LCD_CTRL_DIR |=  LCDRS;
  LCD_CTRL_SET |= LCDRS;
  LCD_CTRL_CLR |=  LCDEN ;	
  LCD_CTRL_CLR |=  LCDRS ;	
  LCD_DATA_DIR |=  LCD_DATA_MASK;
}

void lcd_clear( void)
{

  lcd_command( 0x01 );

}

void LCD_setpos(unsigned char row,unsigned char column)
{
	
	switch(row)
	{
	  
	   case 1:
	   		lcd_command(0x80+column-1);
			break;

	   case 2:
	   		lcd_command(0xc0+column-1);
			break;

       default :

			break;

     }
}

void lcd_putstring( char *string )
{
   while(*string != '\0' )
  {
    lcd_data( *string );
    string++;
  }
}

void init_lcd( void )
{
  set_lcd_port_output();
  delay(100*100);
  lcd_command(0x38);     /*   8-bit interface, two line, 5X7 dots.        */
  
  lcd_clear() ;          /*   LCD clear                                   */
  lcd_command(0x02);     /*   cursor home                                 */
  lcd_command(0x06);     /*   cursor move direction                       */
  lcd_command(0x0C) ;    /*   display on      */
    
}


void lcd_num(unsigned long number)
{

	unsigned char d[6];//={0};
	char i=0,j=0;
	while(number)
	{
		d[i]=number%10;
		number=number/10;
		i++; 				
	} 	
	
//	lcd_data(d[4]+48);lcd_data(d[3]+48);lcd_data(d[2]+48);lcd_data(d[1]+48);lcd_data(d[0]+48);

	if(!number)
	{
		for(j=i;j>0;j--)	
		lcd_data(d[j-1]+48);
	} 
	
	
}
	
