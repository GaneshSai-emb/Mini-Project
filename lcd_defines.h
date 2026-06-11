//lcd_defines.h
//lcd_pin defines

#define LCD_DATA 16  //p0.16 t0 p0.24

#define RS 28   //p0.28

#define RW 30  //p0.30

#define EN 29  //p0.29


//lcd_commands
#define CLEAR_LCD     0X01

#define RET_CUR_HOME   0x02

#define MODE_4BIT_1LINE 0X20

#define MODE_4BIT_2LINE 0x28

#define MODE_8BIT_1LINE 0X30

#define MODE_8BIT_2LINE 0x38

#define DISP_OFF        0x08

#define DISP_ON_CUR_OFF 0x0C

#define DISP_ON_CUR_ON  0x0E

#define DISP_ON_CUR_BLINK 0x0F

#define GOTO_LINE1_POS0   0x80

#define GOTO_LINE2_POS0   0xC0

#define GOTO_CGRAM        0x40

#define SHIFT_CUR_RIGHT   0x06


