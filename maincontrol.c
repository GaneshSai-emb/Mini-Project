/*
 * Main application for EventBoard system.
 * Handles RTC scheduling, LCD display,
 * temperature monitoring and admin mode.
 */
int main()
{
    /* Initialize peripherals */
    RTC_Init();
    InitLCD();
    InitKPM();
    Init_ADC();

    /* Configure admin switch as input */
    IODIR0 &= ~(1<<SW);

    while(1)
    {
        /* Reset event flag */
        found=0;

        /* Read RTC information */
        GetRTCTimeInfo(&hour,&min,&sec);
        GetRTCDateInfo(&date,&month,&year);
        GetRTCDay(&day);

        /* Check admin switch */
        if(((IOPIN0>>SW)&1)==0)
        {
            delay_ms(50);

            /* Wait for switch release */
            while(((IOPIN0>>SW)&1)==0);

            CmdLCD(CLEAR_LCD);
            StrLCD("ADMIN MODE");

            /* Enter administrator menu */
            adminmode();
        }

        /* Search active events */
        for(i=0;i<TOTAL_MESSAGES;i++)
        {
            /* Convert time into seconds */
            now=hour*3600+min*60+sec;
            eventtime=messageList[i].hour*3600+
                      messageList[i].min*60;

            /* Calculate remaining event time */
            remaining=(eventtime+900)-now;

            /* Check whether event is active */
            if(messageList[i].enabled &&
               remaining>0 &&
               remaining<=900)
            {
                found=1;

                /* Clear LCD on new event */
                if(eventactive==0)
                    CmdLCD(CLEAR_LCD);

                /* Display scrolling message */
                CmdLCD(GOTO_LINE1_POS0);
                ScrollLCD(messageList[i].msg);

                /* Calculate countdown */
                rem_min=remaining/60;
                rem_sec=remaining%60;

                /* Display timer */
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

        /* Return to normal display */
        if(found==0)
        {
            /* Read temperature */
            temperature=read_temp();

            /* Display RTC details */
            DisplayRTCTime(hour,min,sec);
            DisplayRTCDate(date,month,year);
            DisplayRTCDay(day);

            /* Display temperature */
            CmdLCD(GOTO_LINE2_POS0+11);
            U32LCD(temperature);
            CharLCD(223);
            CharLCD('C');

            delay_ms(500);
        }

        /* Update event state */
        eventactive=found;
    }
}
