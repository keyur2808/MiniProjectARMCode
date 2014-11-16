#ifndef _ADC_H
#define _ADC_H

void init_adc(void);
unsigned short adc_read(unsigned char adc_num, unsigned char ch);

#endif
