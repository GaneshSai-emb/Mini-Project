/*
 * File Name : adc.c
 * ADC module used for reading analog values and temperature calculation.
 */

#include "types.h"
#include "delay.h"
#include "adc_defines.h"
#include <lpc21XX.h>


/* Initialize ADC */
void Init_ADC(void)
{
    /* Configure ADC pin */
    PINSEL1 |= AIN1;

    /* Enable ADC */
    ADCR |= PDN_BIT | CLKDIV;
}



/* Read ADC value from selected channel */
void Read_ADC(u32 chno, f32 *eAR, u32 *AdvDVal)
{
    /* Select ADC channel */
    ADCR &= ~(255<<0);

    ADCR |= chno | START_CONV;


    /* Wait for conversion complete */
    delay_us(3);

    while(((ADDR >> DONE_BIT) & 1) == 0);



    /* Stop conversion */
    ADCR &= ~(START_CONV);


    /* Store ADC result */
    *AdvDVal = ((ADDR >> RESULT) & 1023);


    /* Convert ADC value to voltage */
    *eAR = (*AdvDVal * (3.3/1023));
}



/* Read temperature from sensor */
int read_temp()
{
    f32 eAR;
    u32 AdvDVal;
    u32 temp;


    /* Initialize ADC */
    Init_ADC();


    /* Read sensor value */
    Read_ADC(CH1,&eAR,&AdvDVal);


    /* Convert voltage to temperature */
    temp = eAR*100;


    return temp;
}
