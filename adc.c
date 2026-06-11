#include "types.h"
#include "delay.h"
#include "adc_defines.h"
#include <lpc21XX.h>

void Init_ADC(void)
{
  PINSEL1 |= AIN1;
  ADCR |= PDN_BIT | CLKDIV;
}


void Read_ADC(u32 chno, f32 *eAR, u32 * AdvDVal)
{
  ADCR &= ~(255<<0);
  ADCR |= chno | START_CONV;
  delay_us(3);

  while (((ADDR >> DONE_BIT) & 1) == 0);
  ADCR &= ~(START_CONV);
 *AdvDVal = ((ADDR >> RESULT) & 1023);

  *eAR = (* AdvDVal * (3.3 / 1023));
}

int  read_temp()
{
  f32 eAR;
  u32 AdvDVal;
  u32 temp;
  Init_ADC();
  Read_ADC(CH1, &eAR, &AdvDVal);
  temp = eAR * 100;

  return temp;
}

~
~
