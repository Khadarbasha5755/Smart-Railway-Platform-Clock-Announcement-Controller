//led.h

#include "types.h"
#include "train_db.h"

#ifndef __LED_H__
#define __LED_H__


/* Initialize LED pins */
void InitLED(void);


/* Update train status LEDs based on current time */
void UpdateTrainStatusLED(u8 hour,u8 min,TB *train);

#endif
