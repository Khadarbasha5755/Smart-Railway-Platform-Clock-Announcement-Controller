//train_info_display.h

#ifndef __TRAIN_INFO_DISPLAY_H__
#define __TRAIN_INFO_DISPLAY_H__

#include "types.h"

/* Display train number, name, destination and timings */
void DisplayTrain(u32 trainNo,
                  char *trainName,
                  char *dest,
                  u8 platform,
                  u8 arrHour,
                  u8 arrMin,
                  u8 depHour,
                  u8 depMin);


/* Add delay to the given time */
void AddDelay(u8 hour,
              u8 min,
              u16 delay,
              u8 *newHour,
              u8 *newMin);


/* Reset train scrolling display */
void ResetTrainDisplay(void);


/* Force train display refresh */
void ForceTrainRefresh(void);


/* Find the length of a string */
int my_strlen(char *str);


/* Display project welcome screen */
void WelcomeScreen(void);


/* Check whether train is approaching the platform */
void CheckTrainApproaching(u8 currentHour,
                           u8 currentMin,
                           u8 arrHour,
                           u8 arrMin);


/* Display platform, arrival and departure details */
void DisplayTrainDetails(u8 platform,
                         u8 arrHour,
                         u8 arrMin,
                         u8 depHour,
                         u8 depMin);


/* Display RTC time */
void DisplayCurrentRTCTime(u8 hour,u8 min,u8 sec);

/* Display train delay */
void DisplayTrainDelay(u16 delayMin);

#endif
