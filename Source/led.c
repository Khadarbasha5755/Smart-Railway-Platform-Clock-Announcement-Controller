//led.c

#include <LPC21xx.h>
#include "types.h"
#include "train_db.h"


/* Flag used to show train information change */
extern u8 TrainInfoActive;


/* LED pin definitions */
#define GREEN_LED   23
#define YELLOW_LED  24
#define RED_LED     25


/* Initialize LED pins */
void InitLED(void)
{
    /* Configure LED pins as output */
    IODIR0 |= (1 << GREEN_LED);
    IODIR0 |= (1 << YELLOW_LED);
    IODIR0 |= (1 << RED_LED);

    /* Turn OFF all LEDs initially */
    IOCLR0 = (1 << GREEN_LED) |
             (1 << YELLOW_LED) |
             (1 << RED_LED);
}


/* Update LEDs according to train status */
void UpdateTrainStatusLED(u8 hour, u8 min, TB *train)
{
    u16 now;
    u16 arr;
    u16 dep;
    u16 yellowStart;

    /* Turn OFF all LEDs before checking the status */
    IOCLR0 = (1 << GREEN_LED) |
             (1 << YELLOW_LED) |
             (1 << RED_LED);

    /* No train available */
    if(train == 0)
        return;

    /* Convert current time into total minutes */
    now = hour * 60 + min;

    /* Calculate arrival time including delay */
    arr = train->arrHour * 60
        + train->arrMin
        + train->delayMin;

    /* Calculate departure time including delay */
    dep = train->depHour * 60
        + train->depMin
        + train->delayMin;


    /*
       RED LED:
       ON when train information is modified.
    */
    if(TrainInfoActive)
    {
        IOSET0 = (1 << RED_LED);
        return;
    }


    /* Check whether the train has departed */
    if(now >= dep)
    {
        PlatformChanged = 0;
        return;
    }


    /*
       GREEN LED:
       Train is currently on the platform.
    */
    if(now >= arr && now < dep)
    {
        IOSET0 = (1 << GREEN_LED);
        return;
    }


    /*
       YELLOW LED:
       Train is within 5 minutes of arrival.
    */
    if(arr >= 5)
        yellowStart = arr - 5;
    else
        yellowStart = 0;

    if(now >= yellowStart && now < arr)
    {
        IOSET0 = (1 << YELLOW_LED);
        return;
    }


    /*
       RED LED:
       Train has a delay.
    */
    if(train->delayMin > 0)
    {
        IOSET0 = (1 << RED_LED);
        return;
    }
}
