//train_db.c

#include "train_db.h"

/* Train database */
TB traindb[MAX] =
{
	/* Train No, Train Name, Destination, Platform,
       Arrival Hour, Arrival Min, Departure Hour, Departure Min,
       Delay Hour, Delay Min 
	*/
	
    {20701,"Tirupati Vande Bharat","Tirupati",1,9,30,9,40,0,0},
    {17256,"Narasapur Express","Narasapur",2,10,30,10,40,0,0},
    {12747,"Palnadu Super Fast Express","Vikarabad",3,11,00,11,10,0,0}
};

/* Variables used for train display and status */
u8 ShowDelayMessage = 0;
u16 DelayDisplayMinute = 0;
TB *CurrentTrain = 0;
u8 ForceDisplay = 0;
volatile u8 PlatformChanged = 0;
u8 TrainInfoChanged = 0;


/* Find a train using its train number */
TB *FindTrain(u32 trainNo)
{
    s32 i;

    /* Search the complete train database */
    for(i=0;i<MAX;i++)
    {
        if(traindb[i].trainNo == trainNo)
        {
            /* Return the address of the matching train */
            return &traindb[i];
        }
    }

    /* Train not found */
    return 0;
}


/* Find the train that should be displayed at current time */
TB *GetCurrentTrain(u8 hour,u8 min)
{
    u16 now;
    u16 arr;
    u16 dep;
    s32 i;

    /* Convert current time into total minutes */
    now = hour * 60 + min;

    /* Check all trains in the database */
    for(i=0;i<MAX;i++)
    {
        /* Calculate arrival time including delay */
        arr = traindb[i].arrHour * 60
            + traindb[i].arrMin
            + traindb[i].delayMin;

        /* Calculate departure time including delay */
        dep = traindb[i].depHour * 60
            + traindb[i].depMin
            + traindb[i].delayMin;

        /* If train has a delay */
        if(traindb[i].delayMin > 0)
        {
            /* Display the train until its departure */
            if(now < dep)
            {
                return &traindb[i];
            }
        }
        else
        {
            /* Display train from 5 minutes before arrival
               until departure */
            if((now >= (arr - 5)) && (now < dep))
            {
                return &traindb[i];
            }
        }
    }

    /* No train is available */
    return 0;
}


/* Find the train after the current train */
TB *GetNextTrain(TB *current)
{
    s32 i;

    /* Check whether a current train exists */
    if(current == 0)
        return 0;

    /* Find the current train in the database */
    for(i=0;i<MAX;i++)
    {
        if(&traindb[i] == current)
        {
            /* Return the next train if available */
            if(i + 1 < MAX)
            {
                return &traindb[i + 1];
            }

            /* No next train available */
            return 0;
        }
    }

    /* Current train not found */
    return 0;
}
