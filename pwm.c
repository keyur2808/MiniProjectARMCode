#include<LPC214x.H>
#include"pwm.h"

void init_pwm(int duty)
{	int cyc  =	duty*(0x000493E0)/100;
    PWMTCR   = 0x00;
    PINSEL1 |= 0x00000400;       //p0.21 = PWM5
    PWMTCR   = 0x02;             //pwm reset
    PWMPR    = 0;                //prescaler disable
    PWMPCR   = 0x2000;           //PWM5 enabled - single edge
    PWMMCR   = 0x00000002;       //reset on PWMMR0
    PWMMR0   = 0x000493E0;       //set pwm frequency
    PWMLER   = 0x21;             //latch new match entries
    PWMMR5   = cyc;              //set pwm5 duty cycle
    PWMTCR   = 0x09;             //enable PWM
}

 
