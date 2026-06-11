// lcd.c

#include "DEFINES.H"
#include "types.h"
#include <lpc21xx.h>
#include "lcd_defines.h"
#include "delay.h"

/* Write data/command byte to LCD */
void WriteLCD(u8 data)
{
	SCLRBIT(IOCLR0,RW);		/* Write mode */
	WRITEBYTE(IOPIN0,LCD_DATA,data);

	SSETBIT(IOSET0,EN);		/* Enable pulse */
	delay_us(1);
	SCLRBIT(IOCLR0,EN);

	delay_ms(2);			/* LCD processing delay */
}

/* Send command to LCD */
void CmdLCD(u8 cmd)
{
	SCLRBIT(IOCLR0,RS);		/* Command register */
	WriteLCD(cmd);
}

/* Send character to LCD */
void CharLCD(u8 ascii)
{
	SSETBIT(IOSET0,RS);		/* Data register */
	WriteLCD(ascii);
}

/* Initialize LCD */
void InitLCD(void)
{
	/* Configure LCD data pins as output */
	WRITEBYTE(IODIR0,LCD_DATA,0xFF);

	/* Configure control pins as output */
	SETBIT(IODIR0,RS);
	SETBIT(IODIR0,RW);
	SETBIT(IODIR0,EN);

	delay_ms(15);

	/* LCD initialization sequence */
	CmdLCD(MODE_8BIT_1LINE);
	delay_ms(5);

	CmdLCD(MODE_8BIT_1LINE);
	delay_us(100);

	CmdLCD(MODE_8BIT_1LINE);

	/* LCD settings */
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DISP_ON_CUR_OFF);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
}

/* Display string on LCD */
void StrLCD(u8* str)
{
	while(*str)
	{
		CharLCD(*str++);
	}
}

/* Display unsigned integer */
void U32LCD(u32 n)
{
	u8 a[10];
	s32 i=0;

	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		/* Store digits in reverse order */
		while(n)
		{
			a[i++]=(n%10)+'0';
			n/=10;
		}

		/* Display digits */
		for(--i;i>=0;i--)
		{
			CharLCD(a[i]);
		}
	}
}

/* Display signed integer */
void S32LCD(s32 n)
{
	if(n<0)
	{
		CharLCD('-');
		n=-n;
	}

	U32LCD(n);
}

/* Display binary number */
void BinLCD(u32 n,u8 nbd)
{
	s32 i;

	for(i=nbd-1;i>=0;i--)
	{
		CharLCD(((n>>i)&1)+'0');
	}
}

/* Create custom character in CGRAM */
void BuildCGRAM(u8* p,u8 nBytes)
{
	s32 i;

	CmdLCD(GOTO_CGRAM);

	for(i=0;i<nBytes;i++)
	{
		CharLCD(p[i]);
	}

	CmdLCD(GOTO_LINE2_POS0);
}

/* Display floating point number */
void f32LCD(f32 fnum,u8 ndp)
{
	u32 inum;

	if(fnum<0.0)
	{
		CharLCD('-');
		fnum=-fnum;
	}

	inum=fnum;

	U32LCD(inum);
	CharLCD('.');

	while(ndp)
	{
		fnum=(fnum-inum)*10;
		inum=fnum;

		CharLCD(inum+'0');
		ndp--;
	}
}

/* Display hexadecimal number */
void HexLCD(u32 n)
{
	u8 a[8],rem;
	s32 i=0;

	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n)
		{
			rem=n%16;

			if(rem<10)
				rem+=48;
			else
				rem+=55;

			a[i++]=rem;
			n/=16;
		}

		for(--i;i>=0;i--)
		{
			CharLCD(a[i]);
		}
	}
}

/* Display octal number */
void OctLCD(u32 n)
{
	u8 a[12];
	s32 i=0;

	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n)
		{
			a[i++]=(n%8)+'0';
			n/=8;
		}

		for(--i;i>=0;i--)
		{
			CharLCD(a[i]);
		}
	}
}
