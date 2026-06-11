// KPM.c

#include "defines.h"
#include "lcd_defines.h"
#include "delay.h"
#include <lpc21xx.h>
#include "types.h"
#include "lcd.h"
#include "kpm.h"

/* Keypad lookup table */
u8 KpmLUT[4][4] =
{
	{1,2,3,'+'},
	{4,5,6,'-'},
	{7,8,9,'*'},
	{10,0,15,16}
};

/*
u8 KpmLUT[4][4]=
{
	{'1','2','3','+'},
	{'4','5','6','-'},
	{'7','8','9','*'},
	{'C','0','=','/'}
};
*/

/* Initialize keypad */
void InitKPM(void)
{
	/* Configure row pins (P1.16-P1.19) as outputs */
	WRITENIBBLE(IODIR1,ROW0,15);
}

/* Check whether any key is pressed */
u8 ColScan(void)
{
	if((READNIBBLE(IOPIN1,COL0)) < 15)
		return 0;		/* Key pressed */
	else
		return 1;		/* No key pressed */
}

/* Find row number of pressed key */
u8 RowCheck(void)
{
	u8 rno;

	for(rno=0; rno<=3; rno++)
	{
		/* Make one row low at a time */
		WRITENIBBLE(IOPIN1,ROW0,~(1<<rno));

		if(ColScan()==0)
		{
			break;
		}
	}

	/* Reset all rows */
	WRITENIBBLE(IOPIN1,ROW0,0x0);

	return rno;
}

/* Find column number of pressed key */
u8 ColCheck(void)
{
	u8 cno;

	for(cno=0; cno<=3; cno++)
	{
		if(STATUSBIT(IOPIN1,(COL0+cno))==0)
		{
			break;
		}
	}

	return cno;
}

/* Scan keypad and return key value */
u8 KeyScan(void)
{
	u8 keyv,rno,cno;

	/* Wait until key press */
	while(ColScan());

	/* Get row number */
	rno = RowCheck();

	/* Get column number */
	cno = ColCheck();

	/* Read key value from lookup table */
	keyv = KpmLUT[rno][cno];

	/* Wait until key release */
	while(!ColScan());

	return keyv;
}

/* Read a 2-digit number from keypad */
u32 ReadNumber(int max)
{
	s8 k;
	u32 num = 0;
	u32 cnt = 0;

	CmdLCD(GOTO_LINE2_POS0);

	while(1)
	{
		k = KeyScan();

		/* Accept digits */
		if(k >= 0 && k <= 9)
		{
			if(cnt < 2)
			{
				num = num*10 + k;
				CharLCD(k+'0');
				cnt++;
			}
		}

		/* Backspace key */
		else if(k == 15)
		{
			if(cnt > 0)
			{
				num = num/10;
				cnt--;

				CmdLCD(0x10);
				CharLCD(' ');
				CmdLCD(0x10);
			}
		}

		/* Enter key */
		else if(k == 16)
		{
			if(cnt == 0)
			{
				CmdLCD(CLEAR_LCD);
				StrLCD("NO INPUT");

				delay_ms(1000);

				CmdLCD(CLEAR_LCD);
				StrLCD("ENTER AGAIN");
				CmdLCD(GOTO_LINE2_POS0);

				continue;
			}

			if(num <= max)
			{
				return num;
			}
			else
			{
				CmdLCD(CLEAR_LCD);
				StrLCD("INVALID");

				delay_ms(1000);

				CmdLCD(CLEAR_LCD);
				StrLCD("TRY AGAIN");

				num = 0;
				cnt = 0;

				CmdLCD(GOTO_LINE2_POS0);
			}
		}
	}
}

/* Read a 4-digit number from keypad */
u32 Read4Number(int max)
{
	s8 k;
	u32 num = 0;
	u32 cnt = 0;

	CmdLCD(GOTO_LINE2_POS0);

	while(1)
	{
		k = KeyScan();

		/* Accept digits */
		if(k >= 0 && k <= 9)
		{
			if(cnt < 4)
			{
				num = num*10 + k;
				CharLCD(k+'0');
				cnt++;
			}
		}

		/* Backspace key */
		else if(k == 15)
		{
			if(cnt > 0)
			{
				num = num/10;
				cnt--;

				CmdLCD(0x10);
				CharLCD(' ');
				CmdLCD(0x10);
			}
		}

		/* Enter key */
		else if(k == 16)
		{
			if(num <= max && cnt > 0)
			{
				return num;
			}
			else
			{
				CmdLCD(CLEAR_LCD);
				StrLCD("INVALID");

				delay_ms(1000);

				CmdLCD(CLEAR_LCD);
				StrLCD("TRY AGAIN");

				num = 0;
				cnt = 0;

				CmdLCD(GOTO_LINE2_POS0);
			}
		}
	}
}
