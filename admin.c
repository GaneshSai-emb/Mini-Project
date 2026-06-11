#include <stdlib.h>
#include "lcd_defines.h"
#include "lcd.h"
#include "kpm.h"
#include "rtc.h"
#include "delay.h"
void rtc_edit()  // this function will make the rtc edit
{
    u8 key1;
    u8 hour;
	  u8 minute;
	  u8 second;
	  u8 date;
	  u8 month;
	  u32 year;
	   u8 week;
 
  while(1)
    {
        CmdLCD(CLEAR_LCD);
        CmdLCD(GOTO_LINE1_POS0);
        // this  displays the rtc edit menu
        StrLCD("1.H 2.MI 3.S 4.D");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("5.M 6.YR 7.W 8.E");
 
        key1 = KeyScan();//key 1 reads the input of the menu options
            switch(key1)   
            {
                case 1:   // set hour
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Enter Hour(0-23)");
                    hour = ReadNumber(23); 
                    sethour(hour);      
                    break;

                case 2:   // minute edit
                    CmdLCD(CLEAR_LCD);
                    StrLCD("MIN EDIT (0-59)");
                     minute = ReadNumber(59);
                     // display hour
                    setminute(minute); 
                    break;

                case 3:   // second edit placeholder
                    CmdLCD(CLEAR_LCD);
                    StrLCD("SEC EDIT(0-59)");
		    second= ReadNumber(59);
		    setsecond(second);
                    delay_ms(1000);
                    break;
		    case 4:   // date edit placeholder
                    CmdLCD(CLEAR_LCD);
                    StrLCD("DATE EDIT(0-31)");
		   date = ReadNumber(31);
		   setdate(date);
                   delay_ms(1000);
                    break;
		   case 5:   // month edit placeholder
                    CmdLCD(CLEAR_LCD);
                    StrLCD("MONTH EDIT(0-12)");
		    month = ReadNumber(12);
		    setmonth(month);
                    delay_ms(1000);
                    break;
		    case 6:   // year edit placeholder
                    CmdLCD(CLEAR_LCD);
                    StrLCD("YEAR EDIT");
		    year = Read4Number(4095);
		    setyear(year);
                    delay_ms(1000);
                    break;
		    case 7:   // week edit placeholder
                    CmdLCD(CLEAR_LCD);
                    StrLCD("WEEK EDIT(0-6)");
		    week = ReadNumber(6);
		    SetRTCDay(week);
                    delay_ms(1000);
                    break;

                case 8:  // exit RTC menu
                    CmdLCD(CLEAR_LCD);
		    return;					
		default : StrLCD("INVALID CHOICE");								
            }
        }
    }

void event_edit()
{
	u8 key,event_no;

    while(1)
    {
	    CmdLCD(CLEAR_LCD);
        StrLCD("ENTER EVENT NO");
        event_no = KeyScan();
        CmdLCD(GOTO_LINE2_POS0);
        CharLCD(event_no + '0');

        if(event_no <= 9)
        {
            CmdLCD(CLEAR_LCD);
            StrLCD("1.Act 2.DeAct");

            key = KeyScan();

            if(key == 1)
            {
                messageList[event_no].enabled = 1;
                return;
            }
            else if(key == 2)
            {
                messageList[event_no].enabled = 0;
                return;
            }
            else
            {
                CmdLCD(CLEAR_LCD);
                StrLCD("INVALID");
                delay_ms(1000);
                return;
            }
        }
        else
        {
            CmdLCD(CLEAR_LCD);
            StrLCD("INVALID");
            delay_ms(1000);
            return;
        }
    }
} 
void adminmode()
{
    u8 key;

    while(1)
    {
 
        CmdLCD(CLEAR_LCD);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("1.RTC 2.EVENT");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("3.EXIT");

        delay_ms(100);  

        key = KeyScan();

              
            switch(key)
            {
                case 1:
                    CmdLCD(CLEAR_LCD);
                    StrLCD("RTC EDIT");
                    delay_ms(200);
                    rtc_edit();
                    break;
                case 2:
                    CmdLCD(CLEAR_LCD);
                    StrLCD("EVENT EDIT");
                    delay_ms(200);
                    event_edit();
                    break;
		  
                case 3:
                    CmdLCD(CLEAR_LCD);
                    delay_ms(200);
                    return;

            }
        }
    }




