//kpm.h

#include "types.h"


/* Initialize keypad */
void Init_KPM(void);


/* Check whether any key is pressed */
u32 colscan(void);


/* Find the row of the pressed key */
u32 rowcheck(void);


/* Find the column of the pressed key */
u32 colcheck(void);


/* Scan keypad and return pressed key */
u32 keyscan(void);


/* Read a number from keypad */
u32 readnum(void);


/* Read a number from keypad and display it on LCD */
u32 ReadNumLCD(u8 p);


/* Read password from keypad and display * for each digit. */
u32 ReadPassword(u8 p);
