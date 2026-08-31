//rtc.h

#include "types.h"

/* Initialize RTC */
void RTC_Init(void);


/* Get current RTC time */
void GetRTCTimeInfo(u32 *,u32 *,u32 *);


/* Display current RTC time */
void DisplayRTCTime(u32,u32,u32);


/* Get current RTC date */
void GetRTCDateInfo(u32 *,u32 *,u32 *);


/* Display current RTC date */
void DisplayRTCDate(u32,u32,u32);


/* Set RTC time */
void SetRTCTimeInfo(u32,u32,u32);


/* Set RTC date */
void SetRTCDateInfo(u32,u32,u32);


/* Get current day */
void GetRTCDay(u32 *);


/* Display current day */
void DisplayRTCDay(u32);


/* Set current day */
void SetRTCDay(u32);
