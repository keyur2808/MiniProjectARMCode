#include<LPC214x.H>
#include"uart.h"


void uart0_init(void)
{
	PINSEL0 |= 0x00000005;
	U0IER    =0x00;
	VICIntEnable=VICIntEnable|(1<<6);
    VICIntSelect=0x00000000;
    VICVectCntl0=0x00000026;
    VICVectAddr0=(unsigned long)UART0_Read;
	U0LCR = 0x83;                   /* 8 bits, no Parity, 1 Stop bit */
	U0DLL = 97;                     /* 9600 Baud Rate @ 15MHz VPB Clock         */
	U0FCR = 0x87;
	U0LCR = 0x03;
	U0IER = 0x01;
}

void UART0_Transmit( u8 data)
{
	while((U0LSR & 0X20)==0x00);

	U0THR = data;
}

	

void uart1_init(void)
{
	PINSEL0 = 0x00050000;
	U1LCR   = 0x83;                   /* 8 bits, no Parity, 1 Stop bit */
	U1FCR   =0x07;
	U1DLL   = 97;                     /* 9600 Baud Rate @ 15MHz VPB Clock         */
	U1LCR   = 0x03;
}

void UART1_Transmit( u8 data)
{
	while((U1LSR & 0X20)==0x00);

	U1THR = data;
}

u8 UART1_Read( void)
{
	while((U1LSR & 0X01)==0x00);

	return(U1RBR);
}
