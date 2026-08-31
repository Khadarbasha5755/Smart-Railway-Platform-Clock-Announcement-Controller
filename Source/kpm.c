//kpm.c

#include <LPC21xx.h>
#include "types.h"
#include "kpm_defines.h"
#include "defines.h"
#include "lcd_defines.h"
#include "lcd.h"


/* Keypad key mapping */
u8 kpmLUT[4][4] =
{
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};


/* Initialize keypad row pins as output */
void Init_KPM(void)
{
    WRITENIBBLE(IODIR1,ROW0,15);
}


/* Check whether any keypad key is pressed */
u32 colscan(void)
{
    /* Check keypad column pins */
    if(READNIBBLE(IOPIN1,COL0) < 15)
        return 0;       /* Key is pressed */
    else
        return 1;       /* No key is pressed */
}


/* Find the row of the pressed key */
u32 rowcheck(void)
{
    u32 rno;

    /* Activate one row at a time */
    for(rno=0;rno<4;rno++)
    {
        WRITENIBBLE(IOPIN1,ROW0,(~(1<<rno)));

        /* Check whether a key is pressed */
        if(colscan() == 0)
            break;
    }

    /* Clear row pins */
    IOCLR1 = 15 << ROW0;

    return rno;
}


/* Find the column of the pressed key */
u32 colcheck(void)
{
    u32 cno;

    /* Check each column */
    for(cno=0;cno<4;cno++)
    {
        if(READBIT(IOPIN1,(cno+COL0)) == 0)
            break;
    }

    return cno;
}


/* Scan keypad and return the pressed key */
u32 keyscan(void)
{
    u32 row;
    u32 col;
    u32 key;

    /* Wait until a key is pressed */
    while(colscan());

    /* Find row and column */
    row = rowcheck();
    col = colcheck();

    /* Get key value from keypad lookup table */
    key = kpmLUT[row][col];

    /* Wait until key is released */
    while(!colscan());

    return key;
}


/* Read a number from keypad */
u32 readnum(void)
{
    u32 num = 0;
    u8 key;

    while(1)
    {
        /* Read keypad key */
        key = keyscan();

        /* Accept only digits */
        if(key >= '0' && key <= '9')
        {
            /* Add digit to number */
            num = (num * 10) + (key - 48);
        }
        else
        {
            /* Stop when a non-digit key is pressed */
            break;
        }
    }

    return num;
}


/* Read number from keypad and display it on LCD */
u32 ReadNumLCD(u8 p)
{
    u32 num = 0;
    u8 key;
    u8 digits = 0;
    u8 pos = p;

    /* Start entering number on second LCD line */
    CmdLCD(GOTO_LINE2_POS0);

    while(1)
    {
        /* Read keypad key */
        key = keyscan();

        /* Check for numeric key */
        if(key >= '0' && key <= '9')
        {
            /* Add new digit to number */
            num = (num * 10) + (key - '0');

            /* Display digit on LCD */
            LCD_CharXY(1,pos,key);

            pos++;
            digits++;
        }

        /* Handle backspace key */
        else if(key == KEY_BACKSPACE)
        {
            if(digits > 0)
            {
                /* Remove last digit */
                num = num / 10;

                digits--;
                pos--;

                /* Clear last digit from LCD */
                LCD_CharXY(1,pos,' ');

                /* Move cursor back */
                LCD_GotoXY(1,pos);
            }
        }

        /* Handle clear key */
        else if(key == KEY_CLEAR)
        {
            /* Reset entered number */
            num = 0;
            digits = 0;
            pos = 0;

            /* Clear second LCD line */
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("                ");
        }

        /* Handle enter key */
        else if(key == KEY_ENTER)
        {
            /* Finish number entry */
            break;
        }
    }

    return num;
}


u32 ReadPassword(u8 p)
{
    u32 password = 0;
    u8 key;
    u8 digits = 0;
    u8 pos = p;

    CmdLCD(GOTO_LINE2_POS0);

    while(1)
    {
        key = keyscan();

        /* Check whether the key is a number. */
        if(key >= '0' && key <= '9')
        {
            password = (password * 10) + (key - '0');

            /* Display * instead of the actual digit. */
            LCD_CharXY(1,pos,'*');

            pos++;
            digits++;
        }

        /* Backspace key. */
        else if(key == KEY_BACKSPACE)
        {
            if(digits > 0)
            {
                password = password / 10;

                digits--;
                pos--;

                /* Remove the last *. */
                LCD_CharXY(1,pos,' ');

                LCD_GotoXY(1,pos);
            }
        }

        /* Clear password. */
        else if(key == KEY_CLEAR)
        {
            password = 0;
            digits = 0;
            pos = p;

            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("                ");

            CmdLCD(GOTO_LINE2_POS0);
        }

        /* Enter key finishes password entry. */
        else if(key == KEY_ENTER)
        {
            break;
        }
    }

    return password;
}
