/* KPM function declarations */

#include "types.h"

/* Initialize keypad */
void InitKPM(void);

/* Scan columns for key press */
u8 ColScan(void);

/* Find pressed row */
u8 RowCheck(void);

/* Find pressed column */
u8 ColCheck(void);

/* Scan keypad and return key value */
u8 KeyScan(void);

/* Read number from keypad */
u32 ReadNum(void);
