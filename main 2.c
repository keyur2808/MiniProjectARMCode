/*****************************************************************************/
/*************         SPI : master mode		 *****************************/
/*****************************************************************************/

#include <LPC214x.H>              /* LPC21xx definitions                      */
#include "lcd.h"
#include "adc.h"
#include "uart.h"
#include "pwm.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

unsigned char adu[7];
int i,j=0;
long tmp,opt;
unsigned char b[6];
unsigned char *str;
signed int setpt;

void wait1()
{
	unsigned int i;
	for(i=0;i<=2000000;i++);
}


/*************************      main program      ****************************/
void settemp()
{ int t=(int)tmp;
  
 if(setpt>t)
   {
    
    IODIR0 |= (1<<13);
	IOCLR0 &= (0<<13);
	IOSET0 |= (1<<13);
   	lcd_num(t-1);
   }
 if (setpt<t)
   {
    IODIR0 |= (1<<15);   
    IOCLR0 &= (0<<15);
	IOSET0 |= (1<<15);
   	lcd_num(t);
   }
 }

void setlight()
{
init_pwm(setpt);
}


void compare()
 {
 LCD_setpos(2,1);
 str=&b[1];
 if (b[0]=='-')
 {
 b[0]=0;
 setpt=atoi(str);
 setpt=-setpt;
 }
 else
 {
 setpt=atoi(str);
 }
 lcd_num(setpt);
 switch (b[4])
 {
  case 'C':
           settemp();
		   break;
  case 'V':
           if ((setpt>0) || (setpt<100))
		   {
		   setlight();
		   break;
		   }
		   lcd_putstring("Wrong value");
		   break;

  }
 }




void UART0_Read(void)__irq
{	while((U0LSR & 0X01)==0x00);
    for (j=0;j<6;j++)
	{
	b[j]=(U0RBR);
	}
	if (b[5]==13)
	{
	b[5]='\0';
	compare();
	}
	if (j >= 5)
	{
	j=0;
	}
	LCD_setpos(1,11);
	VICVectAddr=0x00000000;
}


 		  
 
 float calibrate(unsigned int reading,unsigned int sensor)
{	float temp;
    char d[6];
	float voltage;
	long t;
	voltage=(reading*0.003222);		//4882
	switch(sensor)
	{
	 case 1:
	       temp=(1000/10)*(voltage);
	       t=temp*100;
	       d[5]=(t%10)+48;
	       adu[5]=d[5];
	       t=t/10;
	       d[4]=(t%10)+48;
	       adu[4]=d[4];
		   adu[6]='C';
		   for (i=2;i>=0;i--)
	       {
	        t=t/10;
	        d[i]=(t%10)+48;
	        adu[i]=d[i];	
	       }
		   break;
	
	case 2:
		   temp=voltage;
		   t=voltage*100;
		   d[5]=(t%10)+48;
	       adu[5]=d[5];
	       t=t/10;
	       d[4]=(t%10)+48;
	       t=t/10;
		   d[2]=(long)voltage;
		   adu[4]=d[4];
		   adu[6]='V';
		   adu[0]='0';
		   adu[1]='0';
		   break;
    	
	   }

	adu[3]='.';
	d[3]='.';
	
	
	for (i=0;i<6;i++)
	{
	lcd_data(d[i]);
	}
	
	
	return temp;

}

 
 
 
 void putstringu()
{
for (i=0;i<7;i++)
{
UART0_Transmit(adu[i]);
}
 }


int main (void)                  /*          execution starts here           */
{	
    IODIR0=(1<<22);
	IOSET0=(1<<22);
	init_adc();
  	init_lcd();
	lcd_clear();
	LCD_setpos(1,1);
  	lcd_putstring("Sensor:");
	wait1();	   
    uart0_init();

   while (1)
  	{
   	   LCD_setpos(1,11);
	   wait1();
	   tmp=calibrate(adc_read(0,3),1); //ADC0.3
	   putstringu();
	   wait1();
	   opt=calibrate(adc_read(0,4),2); //ADC0.4
	   putstringu();
	   if ((int)tmp==setpt)
	   {		 
		IOSET0=(1<<12);
		IOSET0=(1<<12);
	   }
	   wait1();
	}

}


