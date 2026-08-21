//lcd.h

#include "types.h"


/* Write one byte to LCD */
void WriteLCD(u8);


/* Send command to LCD */
void CmdLCD(u8);


/* Initialize LCD */
void InitLCD(void);


/* Display one character on LCD */
void CharLCD(u8);


/* Display a string on LCD */
void StrLCD(s8 *);


/* Display unsigned 32-bit number on LCD */
void U32LCD(u32);


/* Display signed 32-bit number on LCD */
void S32LCD(s32);


/* Display floating point number on LCD */
void F32LCD(f32,u8);


/* Store custom characters in LCD CGRAM */
void BuildCGRAM(u8*,u8);


/* Move LCD cursor to given row and column */
void LCD_GotoXY(u8,u8);


/* Display string at given row and column */
void LCD_StringXY(u8,u8,s8 *);


/* Display character at given row and column */
void LCD_CharXY(u8,u8,u8);
