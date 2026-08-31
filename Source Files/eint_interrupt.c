//eint_interrupt.c

#include <LPC21xx.h>
#include "types.h"

/* VIC channel number for External Interrupt 0. */
#define EINT0_CH 14

/* Function prototype for EINT0 interrupt service routine. */
void eint0_isr(void) __irq;

/* Flag used to request Admin Mode from main program. */
extern volatile u8 EditMode;

/* Set to 1 when EINT0 button is pressed. */
volatile u8 edit_request = 0;


/*----------------------------------------------------------
              EXTERNAL INTERRUPT INITIALIZATION
  ----------------------------------------------------------*/
void Init_EINT(void)
{
    /* Configure P0.14 as EINT0 function. */
    PINSEL0 &= ~(3 << 2);
    PINSEL0 |= (3 << 2);

    /* Clear any pending EINT0 interrupt. */
    EXTINT = (1 << 0);

    /* Configure EINT0 as edge triggered interrupt. */
    EXTMODE |= (1 << 0);

    /* Configure EINT0 as falling-edge triggered. */
    EXTPOLAR &= ~(1 << 0);

    /* Configure EINT0 as an IRQ interrupt. */
    VICIntSelect &= ~(1 << EINT0_CH);

    /* Assign EINT0 ISR address to VIC vector slot 0. */
    VICVectAddr0 = (unsigned int)eint0_isr;

    /* Enable vector slot 0 for EINT0. */
    VICVectCntl0 = (1 << 5) | EINT0_CH;

    /* Enable EINT0 interrupt in VIC. */
    VICIntEnable = (1 << EINT0_CH);
}


/*----------------------------------------------------------
              EINT0 INTERRUPT SERVICE ROUTINE
  ----------------------------------------------------------*/
void eint0_isr(void) __irq
{
    /* Inform main program that EINT0 was pressed. */
    edit_request = 1;

    /* Clear the EINT0 interrupt flag. */
    EXTINT = 1 << 0;

    /* End the current interrupt vector. */
    VICVectAddr = 0;
}
