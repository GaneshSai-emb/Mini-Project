/*
 * File Name : maincontrol.c
 * Project   : EventBoard - RTC Driven Message Display System
 * Controller: LPC2148 ARM7
 * Handles RTC event scheduling, LCD scrolling,
 * temperature monitoring and admin mode.
 */

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

/*
 * RTC variables
 * Stores current time and date information
 */
s32 hour,min,sec;
s32 date,month,year,day;

/* Stores temperature value */
u32 temperature;

/* Loop variables */
int i,j;

/* Event status flags */
int found=0;
int eventactive=0;

/* Event timer variables */
int now,eventtime,remaining;
int rem_min,rem_sec;

/*
 * Main Function
 * Initializes peripherals and continuously checks:
 * - Admin switch
 * - Event timing
 * - LCD display
 * - Temperature monitoring
 */
int main()
{
    /* Initialize RTC */
    RTC_Init();

    /* Initialize LCD */
    InitLCD();

    /* Initialize keypad */
    InitKPM();

    /* Initialize ADC */
    Init_ADC();

    /* Configure switch as input */
    IODIR0 &= ~(1<<SW);

#ifndef _DEBUG
    /* Default RTC values for testing */
    SetRTCTimeInfo(7,44,44);
    SetRTCDateInfo(8,10,2026);
    SetRTCDay(1);
#endif

    while(1)
    {
        found=0;

        /* Read RTC values */
        GetRTCTimeInfo(&hour,&min,&sec);
        GetRTCDateInfo(&date,&month,&year);
        GetRTCDay(&day);

        /* Check admin switch */
        if(((IOPIN0>>SW)&1)==0)
        {
            delay_ms(50);

            while(((IOPIN0>>SW)&1)==0);

            CmdLCD(CLEAR_LCD);
            StrLCD("ADMIN MODE");

            /* Enter admin settings */
            adminmode();
        }

        /*
         * Check all events
         */
        for(i=0;i<TOTAL_MESSAGES;i++)
        {
            /* Convert RTC time to seconds */
            now=hour*3600+min*60+sec;

            /* Convert event time to seconds */
            eventtime=messageList[i].hour*3600+
                      messageList[i].min*60;

            /* Calculate remaining time */
            remaining=(eventtime+900)-now;

            /*
             * Display event if active
             */
            if(messageList[i].enabled &&
               remaining>0 &&
               remaining<=900)
            {
                found=1;

                if(eventactive==0)
                    CmdLCD(CLEAR_LCD);

                /* Scroll event message */
                CmdLCD(GOTO_LINE1_POS0);
                ScrollLCD(messageList[i].msg);

                /* Display timer */
                rem_min=remaining/60;
                rem_sec=remaining%60;

                CmdLCD(GOTO_LINE2_POS0);
                StrLCD("time left:");

                CharLCD((rem_min/10)+'0');
                CharLCD((rem_min%10)+'0');
                CharLCD(':');
                CharLCD((rem_sec/10)+'0');
                CharLCD((rem_sec%10)+'0');

                delay_ms(10);
            }
        }

        /*
         * Normal mode display
         */
        if(found==0)
        {
            temperature=read_temp();

            DisplayRTCTime(hour,min,sec);
            DisplayRTCDate(date,month,year);
            DisplayRTCDay(day);

            CmdLCD(GOTO_LINE2_POS0+11);

            U32LCD(read_temp());

            CharLCD(223);
            CharLCD('C');

            delay_ms(500);
        }

        /* Update event state */
        eventactive=found;
    }
}
