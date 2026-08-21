//main.c

#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "rtc.h"
#include "eint_interrupt.h"
#include "menu.h"
#include "delay.h"
#include "train_info_display.h"
#include "train_db.h"
#include "led.h"
#include "buzzer.h"
#include <LPC21xx.h>

/* RTC time and date variables */
u32 hour,min,sec,date,month,year,day;

/* Variables used for display control */
u8 displayMode = 0;
u8 trainSecondDisplay = 0;
u8 lastSec = 60;
u8 infoLastSec = 60;
u8 trainDisplayTime = 0;
u8 TrainInfoTimer = 0;
u8 TrainInfoActive = 0;

/* Flag set when external interrupt is pressed */
extern volatile u8 edit_request;

/* RTC day values */
#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

int main()
{
    TB *train;
    u16 now;
    u16 dep;

    /* Initialize all required peripherals */
    RTC_Init();
    InitLCD();
    Init_KPM();
    Init_EINT();
    InitLED();
    Buzzer_Init();

    /* Display project welcome message */
    WelcomeScreen();

    /* Set initial RTC time and date */
    SetRTCTimeInfo(9,0,0);
    SetRTCDateInfo(21,8,2026);
    SetRTCDay(FRI);

    while(1)
    {
        /* Check whether admin mode is requested */
        if(edit_request)
        {
            edit_request = 0;
					
            EditMode = 1;

            /* Display admin menu */
            PrintMenu();
						
            /* Show train information change message */
            if(TrainInfoChanged)
            {
                TrainInfoActive = 1;
                TrainInfoTimer = 0;
            }
						
            CmdLCD(CLEAR_LCD);

            /* Reset display control variables */
            displayMode = 0;
            trainSecondDisplay = 0;
            trainDisplayTime = 0;
            lastSec = 60;

            ResetTrainDisplay();
        }

        /* Read current RTC time */
        GetRTCTimeInfo(&hour,&min,&sec);
				
        /* Display train information changed message for a short time */
        if(TrainInfoActive)
        {
            if(sec != infoLastSec)
            {
                infoLastSec = sec;

                TrainInfoTimer++;

                /* Stop the message after 2 seconds */
                if(TrainInfoTimer >= 2)
                {
                    TrainInfoActive = 0;
                    TrainInfoChanged = 0;
                    TrainInfoTimer = 0;
                }
            }
        }

        /* Convert current time into total minutes */
        now = hour * 60 + min;

        /* Check whether a train is currently being displayed */
        if(ForceDisplay)
        {
            TB *nextTrain;

            train = CurrentTrain;

            if(train != 0)
            {
                /* Calculate train departure time including delay */
                dep = train->depHour * 60
                    + train->depMin
                    + train->delayMin;

                /* Check whether the current train has departed */
                if(now >= dep)
                {
                    /* Find the next train */
                    nextTrain = GetNextTrain(CurrentTrain);

                    if(nextTrain != 0)
                    {
                        /* Display the next train */
                        CurrentTrain = nextTrain;
                        ForceDisplay = 1;
                        train = nextTrain;

                        /* Reset train display */
                        ResetTrainDisplay();
                        CmdLCD(CLEAR_LCD);
                        displayMode = 0;
                        trainSecondDisplay = 0;
                        trainDisplayTime = 0;
                        lastSec = sec;
                    }
                    else
                    {
                        /* No more trains available */
                        ForceDisplay = 0;
                        CurrentTrain = 0;
                        train = 0;

                        ResetTrainDisplay();
                        CmdLCD(CLEAR_LCD);
                        displayMode = 0;
                        trainSecondDisplay = 0;
                        trainDisplayTime = 0;
                        lastSec = sec;
                    }
                }
            }
        }
        else
        {
            /* Find a train matching the current time */
            train = GetCurrentTrain(hour,min);

            if(train != 0)
            {
                /* Store the current train */
                CurrentTrain = train;
                ForceDisplay = 1;
            }
        }

        /* Update LEDs only when admin mode is not active */
        if(EditMode == 0)
        {
            UpdateTrainStatusLED(hour,min,train);
        }

        /* Check whether a train is available */
        if(train != 0)
        {
            /* Initialize train display */
            if(displayMode == 0)
            {
                CmdLCD(CLEAR_LCD);
                ResetTrainDisplay();
                displayMode = 1;
							
                trainSecondDisplay = 0;
                trainDisplayTime = 0;
                lastSec = sec;
            }

            /* Display delay information when delay is updated */
            if(ShowDelayMessage)
            {
                CmdLCD(CLEAR_LCD);

                /* Display train number and name */
                LCD_GotoXY(0,0);
                U32LCD(train->trainNo);

                LCD_CharXY(0,5,':');

                LCD_StringXY(0,6,train->trainName);

                /* Display delay time */
                LCD_StringXY(1,0,"Delay:");

                U32LCD(DelayDisplayMinute);

                StrLCD(" Min");
								
                #ifdef HARDWARE
                    delay_ms(1000);
                #else
                    delay_ms(500);
                #endif

                /* Clear delay message flag */
                ShowDelayMessage = 0;

                ResetTrainDisplay();

                CmdLCD(CLEAR_LCD);
								
                trainSecondDisplay = 0;
                trainDisplayTime = 0;
                lastSec = sec;

                continue;
            }

            /* Display train number, name and destination */
            DisplayTrain(train->trainNo,
                         train->trainName,
                         train->destination,
                         train->platform,
                         train->arrHour,
                         train->arrMin,
                         train->depHour,
                         train->depMin);

            /* Check whether train is approaching */
            CheckTrainApproaching(hour,
                                  min,
                                  train->arrHour,
                                  train->arrMin);
						
            /* Change second line display every 5 seconds */
            if(sec != lastSec)
            {
                lastSec = sec;

                trainDisplayTime++;

                if(trainDisplayTime >= 5)
                {
                    trainDisplayTime = 0;

                    /* Toggle between train details and RTC time */
                    if(trainSecondDisplay == 0)
                        trainSecondDisplay = 1;
                    else
                        trainSecondDisplay = 0;

                    /* Clear second LCD line */
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("                ");
                }
            }

            /* Display platform and train timings */
            if(trainSecondDisplay == 0)
            {
                DisplayTrainDetails(train->platform,
                                     train->arrHour,
                                     train->arrMin,
                                     train->depHour,
                                     train->depMin);
            }
            else
            {
                /* Display RTC time and train delay */
                DisplayCurrentRTCTime(hour,min,sec);
								/* Display train delay */
								DisplayTrainDelay(train->delayMin);
            }
        }
        else
        {
            /* No train is available */
            if(displayMode == 1)
            {
                CmdLCD(CLEAR_LCD);
                ResetTrainDisplay();
                displayMode = 0;
                trainSecondDisplay = 0;
                trainDisplayTime = 0;
                lastSec = sec;
            }

            /* Display current RTC time */
            DisplayRTCTime(hour,min,sec);

            /* Get and display current date */
            GetRTCDateInfo(&date,&month,&year);

            DisplayRTCDate(date,month,year);

            /* Get and display current day */
            GetRTCDay(&day);

            DisplayRTCDay(day);
        }
    }
}
