
/* Initialize ADC module */
void Init_ADC(void);
/* Read ADC value and convert into voltage */
void Read_ADC(u8 chno, f32 *eAR, u32 *AdvDVal);
/* Read temperature value from ADC sensor */
int read_temp(void);
