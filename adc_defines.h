/* Oscillator and clock configuration */
#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)

/* ADC clock configuration */
#define ADCLK 3000000
#define CLKDIV ((PCLK/ADCLK)-1)

/* ADC channel selection */
#define CH0 0x01
#define CH1 0x02
#define CH2 0x04
#define CH3 0x08

/* ADC control register bits */

/* Power up ADC */
#define PDN_BIT (1<<21)

/* ADC clock divider */
#define CLK_DIV (CLK_DIV<<8)

/* Start ADC conversion */
#define START_CONV (1<<24)

/* ADC result and status bits */

/* Result data position */
#define RESULT 6

/* Conversion complete flag */
#define DONE_BIT 31

/* ADC input pin configuration */

#define AIN0 0x00400000
#define AIN1 0x01000000
#define AIN2 0x04000000
#define AIN3 0x10000000
