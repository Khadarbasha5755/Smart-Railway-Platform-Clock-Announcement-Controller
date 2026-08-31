//lcd.c

#include<LPC21xx.h> 
#include "types.h" 
#include "lcd_defines.h" 
#include "defines.h" 
#include "delay.h"


/* Write one byte to LCD */
void WriteLCD(u8 byte) 
{ 
    /* Send data byte to LCD data pins */
    WRITEBYTE(IOPIN0,LCD_DATA,byte);

    /* Select write operation */
    IOCLR0=1<<LCD_RW;

    /* Generate enable pulse to store data in LCD */
    IOSET0=1<<LCD_EN;
    delay_us(1);
    IOCLR0=1<<LCD_EN;

    /* Wait for LCD to complete the operation */
    delay_ms(2);
}


/* Send command to LCD */
void CmdLCD(u8 cmd) 
{ 
    /* Select LCD command register */
    IOCLR0=1<<LCD_RS;

    /* Send command to LCD */
    WriteLCD(cmd);
}


/* Initialize LCD */
void InitLCD(void) 
{ 
    /* Configure LCD data and control pins as output */
    IODIR0|=((0xFF<<LCD_DATA)|(1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN));

    /* Wait for LCD power supply to stabilize */
    delay_ms(15);

    /* Initialize LCD in 8-bit mode */
    CmdLCD(0x30);

    delay_ms(4);
    delay_us(100);

    CmdLCD(0x30);

    delay_us(100);

    CmdLCD(0x30);

    /* Select 8-bit, 2-line LCD mode */
    CmdLCD(MODE_8BIT_2LINE);

    /* Turn ON display and turn OFF cursor */
    CmdLCD(DSP_ON_CUR_OFF);

    /* Clear LCD display */
    CmdLCD(CLEAR_LCD);

    /* Move cursor to the right after displaying character */
    CmdLCD(SHIFT_CUR_RIGHT);
}


/* Display one character on LCD */
void CharLCD(u8 ch) 
{ 
    /* Select LCD data register */
    IOSET0=1<<LCD_RS;

    /* Send character to LCD */
    WriteLCD(ch); 
}


/* Display a string on LCD */
void StrLCD(s8 *str) 
{ 
    /* Display characters until null character */
    while(*str) 
    { 
        CharLCD(*str); 
        str++; 
    } 
}


/* Display unsigned 32-bit number on LCD */
void U32LCD(u32 n) 
{ 
    u8 a[20]; 
    s32 i=0;

    /* Display zero directly */
    if(n==0) 
    { 
        CharLCD('0'); 
    } 
    else 
    { 
        /* Extract digits in reverse order */
        while(n>0) 
        { 
            a[i]=(n%10)+48; 
            n=n/10; 
            i++; 
        }

        /* Display digits in correct order */
        for(--i;i>=0;i--) 
        { 
            CharLCD(a[i]); 
        } 
    } 
}


/* Display signed 32-bit number on LCD */
void S32LCD(s32 n) 
{ 
    /* Display minus sign for negative number */
    if(n<0) 
    { 
        CharLCD('-'); 
        n=-n; 
    }

    /* Display number */
    U32LCD(n); 
}


/* Display floating point number on LCD */
void F32LCD(f32 fn,u8 nDp) 
{ 
    u32 n; 
    s32 i;

    /* Display minus sign for negative value */
    if(fn<0.0) 
    { 
        CharLCD('-'); 
        fn=-fn; 
    }

    /* Display integer part */
    n=fn; 
    U32LCD(n); 
    CharLCD('.');

    /* Display decimal part */
    for(i=0;i<nDp;i++) 
    { 
        fn=(fn-n)*10; 
        n=fn; 
        CharLCD(n+48); 
    } 
}


/* Store custom characters in LCD CGRAM */
void BuildCGRAM(u8 *p,u8 nBytes) 
{ 
    s32 i;

    /* Select CGRAM starting address */
    CmdLCD(GOTO_CGRAM_START);

    /* Select LCD data register */
    IOSET0=1<<LCD_RS;

    /* Write custom character data into CGRAM */
    for(i=0;i<nBytes;i++) 
    { 
        WriteLCD(p[i]); 
    }

    /* Return cursor to first position of LCD */
    CmdLCD(GOTO_LINE1_POS0);
}


/* Move LCD cursor to given row and column */
void LCD_GotoXY(u8 row,u8 col) 
{ 
    if(row==0) 
        CmdLCD(GOTO_LINE1_POS0+col); 
    else 
        CmdLCD(GOTO_LINE2_POS0+col); 
}


/* Display string at given row and column */
void LCD_StringXY(u8 row,u8 col,s8 *str) 
{ 
    /* Move cursor to required position */
    LCD_GotoXY(row,col);

    /* Display string */
    StrLCD(str); 
}


/* Display character at given row and column */
void LCD_CharXY(u8 row, u8 col, u8 ch) 
{ 
    /* Select required LCD line */
    if(row == 0) 
        CmdLCD(GOTO_LINE1_POS0 + col);   /* Line 1 */ 
    else 
        CmdLCD(GOTO_LINE2_POS0 + col);   /* Line 2 */

    /* Display character */
    CharLCD(ch); 
}
