#include <LPC214x.H> 
#include "adc.h"

#define ADC0 0
#define ADC1 1

void init_adc(void)
{ 
  PINSEL1 = (PINSEL1 & ~(3 << 28)| (1 << 28)); // AD0.3
  PINSEL1 = (PINSEL1 & ~(3 << 18)| (1 << 18)); //AD0.4
}


unsigned short adc_read(unsigned char adc_num, unsigned char ch)
{
  unsigned int i=0;
  
  switch(adc_num)
  {
    case ADC0:
      AD0CR = 0x00200D00 | (1<<ch);    // select channel
      AD0CR |= 0x01000000; // Start A/D Conversion
    
      do
      {
        i = AD0GDR; // Read A/D Data Register
      } while ((i & 0x80000000) == 0); // Wait for end of A/D Conversion
      break;
    
    case ADC1:
      AD1CR = 0x00200D00 | (1<<ch); // select channel
      AD1CR |= 0x01000000; // Start A/D Conversion
      do
      {
        i = AD1GDR; // Read A/D Data Register
      } while ((i & 0x80000000) == 0); // Wait for end of A/D Conversion
      break;
  }
  
  return (i >> 6) & 0x03FF; // bit 6:15 is 10 bit AD value
}

