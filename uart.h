#ifndef _UART_H
#define _UART_H

typedef unsigned char u8;


void uart0_init(void);
void UART0_Transmit( u8 data);
void UART0_Read(void)__irq;

void uart1_init(void);
void UART1_Transmit( u8 data);
u8 UART1_Read(void);


#endif



