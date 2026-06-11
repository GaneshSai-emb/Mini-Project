#include <lpc214x.h>
#include "types.h"
// include headerfiles
#include "lcd.h"
#include "lcd_defines.h"
#include "rtc.h"
//system clock and peripheral clock macros
#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)
// RTC Prescaler Calculation Macros
// RTC requires 32.768 kHz clock for 1-second increment.
// PREINT and PREFRAC registers divide PCLK to generate 32.768 kHz.

// PREINT = int (PCLK / 32768) - 1;
// PREFRAC = PCLK -((PREINT + 1) * 32768);
// Note: This information collected from LPC2129 Manual

#define PREINT_VAL (int)((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))
//RTC Control Register (CCR) Bit Definitions
// Bit 0  Clock Enable --> 1 = Enable RTC counters  0 = Disable RTC counters
#define RTC_ENABLE (1<<0)

// Bit 1  Clock Reset --> 1 = Reset RTC counters    0 = Normal operation
#define RTC_RESET  (1<<1)
//only for LPC2148
// Bit 4  Clock Source Select 
// 1 = Use external 32.768 kHz oscillator
// 0 = Use internal PCLK as RTC clock source
#define RTC_CLKSRC (1<<4)

#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

#define _LPC2148
void RTC_Init(void)
{
    //disable and reset the rtc
    CCR = RTC_RESET;
     #ifndef _LPC2148
     //set prescaler integer and fractional parts
     PREINT = PREINT_VAL;
     PREFRAC = PREFRAC_VAL;
     //enable the RTC
      CCR = RTC_ENABLE;
        #else
        //enable the rtc with external clocksource
         CCR = RTC_ENABLE  | RTC_CLKSRC;
        #endif
}

char week[][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
/*
Get the current RTC time
hour Pointer to store the current hour
minute Pointer to store the current minute
second Pointer to store the current second
*/
void GetRTCTimeInfo(s32 *hour,s32 *min,s32 *sec)
{
    *hour = HOUR;
    *min = MIN;
    *sec = SEC;
}
/*
Display the RTC time on LCD
hour value (0-23)
minute value (0-59)
second value (0-59) seperated by ':'
*/
void DisplayRTCTime(u32 hour,u32 min,u32 sec)
{
    CmdLCD(GOTO_LINE1_POS0);

    CharLCD(hour/10+48);
    CharLCD(hour%10+48);
    CharLCD(':');

    CharLCD(min/10+48);
    CharLCD(min%10+48);
    CharLCD(':');

    CharLCD(sec/10+48);
    CharLCD(sec%10+48);
}
/*
Get the current RTC date
day Pointer to store the current date (1-31)
month Pointer to store the current month (1-12)
year Pointer to store the current year (four digits)
*/
void GetRTCDateInfo(s32 *date,s32 *month,s32 *year)
{
    *date = DOM;
    *month = MONTH;
    *year = YEAR;
}

/*
Display the RTC date on LCD
Day of month (1-31)
Month (1-12)
Year (four digits) and seperated by '/'
*/
void DisplayRTCDate(u32 date,u32 month,u32 year)
{
    CmdLCD(GOTO_LINE2_POS0);

    CharLCD(date/10+48);
    CharLCD(date%10+48);
    CharLCD('/');
    CharLCD(month/10+48);
    CharLCD(month%10+48);
    CharLCD('/');

    U32LCD(year);
}
/*
Set the RTC time
Hour to set (0-23)
Minute to set (0-59)
Second to set (0-59)
*/
void SetRTCTimeInfo(u32 hour,u32 min,u32 sec)
{
    HOUR = hour;
    MIN = min;
    SEC = sec;
}
/*
Set the RTC date
day of month to set (1-31)
month to set (1-12)
year to set (four digits)
*/
void SetRTCDateInfo(u32 date,u32 month,u32 year)
{
    DOM = date;
    MONTH = month;
    YEAR = year;
}

void GetRTCDay(s32 *dow)
{
    *dow = DOW;
}
/*
Get the current day of the week
dow Pointer to store Day of Week (0=Sunday, ..., 6=Saturday)
*/
void DisplayRTCDay(u32 dow)
{
    CmdLCD(GOTO_LINE1_POS0+10);
    StrLCD(week[dow]);
}
/*

Display the current day of the week on LCD
dow (Day of Week) (0=Sunday, ..., 6=Saturday)
*/
void SetRTCDay(u32 dow)
{
    DOW = dow;
}
//sets hour
void sethour(u32 hour)
{
	HOUR=hour;
}
//sets minute
void setminute(u32 minute)
{
	MIN=minute;
}
//sets seconds
void setsecond(u32 second)
{
	SEC=second;
}
//sets date
void setdate(u32 date)
{
	DOM=date;
}
//sets month
void setmonth(u32 month)
{
	MONTH=month;
}
//sets week
void setweek(u32 week)
{
   DOW=week;
}	
//sets year
void setyear(u32 year)
{
YEAR=year;
}

