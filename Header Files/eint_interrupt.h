//eint_interrupt.h

#ifndef __EINT_H__
#define __EINT_H__

#include "types.h"

/* Flag set by EINT0 ISR when the Admin switch is pressed. */
extern volatile u8 edit_request;

/* Initialize External Interrupt 0. */
void Init_EINT(void);

#endif
