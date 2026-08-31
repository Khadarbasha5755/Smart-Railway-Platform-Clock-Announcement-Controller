//menu.h

#ifndef __MENU_H__
#define __MENU_H__


#include "types.h"


/* Check password before entering Admin Mode. */
u8 CheckAdminPassword(void);


/* Display the main admin menu */
void PrintMenu(void);


/* Edit RTC time */
void EditRTCTime(void);


/* Edit complete train time */
void EditTrainTime(void);


/* Edit train arrival time */
void EditArrivalTime(void);


/* Edit train departure time */
void EditDepartureTime(void);


/* Edit train delay time */
void EditDelayTime(void);


/* Edit train platform number */
void EditPlatform(void);


/* Flag to indicate admin edit mode */
extern volatile unsigned char EditMode;


#endif
