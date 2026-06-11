#include <lpc214x.h>
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "adc_defines.h"
#include "adc.h"
#include "rtc.h"
#include "event.h"
#include "delay.h"
#include "kpm.h"
#include "admin.h"
#define _DEBUG
#define SW 0

s32 hour,min,sec;
s32 date,month,year,day;
u32 temperature;
int i,j;
int found=0;
int holdcount=0;

int now,eventtime,remaining;
int rem_min,rem_sec;
int eventactive=0;

int main()
{
    RTC_Init();
    InitLCD();
    InitKPM();
          Init_ADC();
    IODIR0 &= ~(1<<SW);
#ifndef _DEBUG
   SetRTCTimeInfo(7,44,44);
   SetRTCDateInfo(8,10,2026);
   SetRTCDay(1);
#endif
    while(1)
    {
        found=0;

        GetRTCTimeInfo(&hour,&min,&sec);
        GetRTCDateInfo(&date,&month,&year);
        GetRTCDay(&day);
       /* ADMIN SWITCH CHECK */
        if(((IOPIN0>>SW)&1)==0)
        {
            delay_ms(50);
                         while(((IOPIN0>>SW)&1)==0);
                CmdLCD(CLEAR_LCD);
                StrLCD("ADMIN MODE");
                            delay_ms(50);
                adminmode();

        }

        for(i=0;i<TOTAL_MESSAGES;i++)
        {
            now = hour*3600 + min*60 + sec;
            eventtime = messageList[i].hour*3600 + messageList[i].min*60;
            remaining = (eventtime + 900) - now;
            if(messageList[i].enabled && remaining > 0 && remaining <= 900 )
            {
                found = 1;
                if(eventactive=0)
                                {
                                CmdLCD(CLEAR_LCD);
                                }

                /* LINE1 ? EVENT SCROLL */
                CmdLCD(GOTO_LINE1_POS0);
                ScrollLCD(messageList[i].msg);

                /* TIMER CALCULATION */
                rem_min = remaining/60;
                rem_sec = remaining%60;

                /* LINE2 ? TIMER */
                CmdLCD(GOTO_LINE2_POS0);
                                StrLCD("time left:");
                CharLCD((rem_min/10)+'0');
                CharLCD((rem_min%10)+'0');
                CharLCD(':');
                CharLCD((rem_sec/10)+'0');
                CharLCD((rem_sec%10)+'0');
                StrLCD("         ");
                delay_ms(10);

            }

        }
                if(found==0&&eventactive==1)
                {
                CmdLCD(CLEAR_LCD);
                }
        if(found==0)
        {
                    temperature= read_temp();
            DisplayRTCTime(hour,min,sec);
            DisplayRTCDate(date,month,year);
            DisplayRTCDay(day);
                    CmdLCD(GOTO_LINE2_POS0+ 11);
                        U32LCD(read_temp());
            CharLCD(223);
            CharLCD('C');
                        delay_ms(500);

        }
                eventactive=found;
    }

}
~
~
