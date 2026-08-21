//buzzer.c

#include <LPC21xx.h>
#include "buzzer.h"
#include "delay.h"
#include "defines.h"


/* Initialize buzzer pin */
void Buzzer_Init(void)
{
    /* Configure buzzer pin as output */
    IODIR0 |= (1 << BUZZER);

    /* Turn OFF buzzer initially */
    IOCLR0 = (1 << BUZZER);
}


/* Turn ON buzzer */
void Buzzer_On(void)
{
    IOSET0 = (1 << BUZZER);
}


/* Turn OFF buzzer */
void Buzzer_Off(void)
{
    IOCLR0 = (1 << BUZZER);
}


/* Generate one beep */
void Buzzer_Beep(void)
{
    /* Turn ON buzzer */
    Buzzer_On();
	
    /* Keep buzzer ON for a short time */
    #ifdef HARDWARE
        delay_ms(500);
    #else
        delay_ms(100);
    #endif
	
    /* Turn OFF buzzer */
    Buzzer_Off();
	
    /* Wait before the next beep */
    #ifdef HARDWARE
        delay_ms(500);
    #else
        delay_ms(100);
    #endif
}


/* Generate three beeps */
void Buzzer_Alert(void)
{
    Buzzer_Beep();
    Buzzer_Beep();
    Buzzer_Beep();
}
