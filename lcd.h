#include "types.h"
void WriteLCD(u8 data);             // Write data byte to LCD
void CmdLCD(u8 cmd);                // Send command to LCD
void CharLCD(u8 ascii);             // Display one ASCII character
void InitLCD(void);                 // Initialize LCD module
void StrLCD(char* str);             // Display string on LCD
void U32LCD(u32 n);                 // Display unsigned 32-bit number
void S32LCD(s32 n);                 // Display signed 32-bit number
void BinLCD(u32 n, u8 nbd);         // Display number in binary
void BuildCGRAM(u8* p, u8 nBytes);  // Create custom LCD characters
void f32LCD(f32 fnum, u8 ndp);      // Display floating-point number
void OctLCD(u32 n);                 // Display number in octal
void HexLCD(u32 n);                 // Display number in hexadecimal
void ScrollLCD(const char *msg);    // Scroll text message on LCD








