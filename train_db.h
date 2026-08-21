//train_db.h

#ifndef __TRAIN_DB_H__
#define __TRAIN_DB_H__

#include "types.h"

/* Maximum number of trains in the database */
#define MAX 3

/* Structure to store train information */
typedef struct
{
    u32 trainNo;          /* Train number */
    char trainName[40];   /* Train name */
    char destination[40]; /* Train destination */
    u8 platform;          /* Platform number */
    u8 arrHour;           /* Arrival hour */
    u8 arrMin;            /* Arrival minute */
    u8 depHour;           /* Departure hour */
    u8 depMin;            /* Departure minute */
    u16 delayMin;         /* Train delay in minutes */
    u8 status;            /* Train status */
}TB;


/* Train database */
extern TB traindb[MAX];


/* Find train using train number */
TB *FindTrain(u32 trainNo);

/* Find current train based on RTC time */
TB *GetCurrentTrain(u8 hour,u8 min);

/* Find the next train */
TB *GetNextTrain(TB *current);


/* Pointer to the currently displayed train */
extern TB *CurrentTrain;

/* Flag to force train display */
extern u8 ForceDisplay;

/* Flag to display train delay message */
extern u8 ShowDelayMessage;

/* Stores delay time to be displayed */
extern u16 DelayDisplayMinute;

/* Flag to indicate platform change */
extern volatile u8 PlatformChanged;

/* Flag to indicate train information change */
extern u8 TrainInfoChanged;

#endif
