//menu.c

#include "menu.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "rtc.h"
#include "delay.h"
#include "train_info_display.h"
#include "train_db.h"
#include "buzzer.h"


/* Admin Mode password. */
#define ADMIN_PASSWORD 1234


/* Stores the keypad key */
u8 key;


/* Flag to indicate admin/edit mode */
volatile u8 EditMode = 0;


/* Flag to exit train edit menu */
u8 trainMenuExit;


/*
   Check whether arrival time is before departure time.
   Returns 1 for valid time and 0 for invalid time.
*/
u8 IsValidTrainTime(u8 arrHour,u8 arrMin,
                    u8 depHour,u8 depMin)
{
    u16 arr;
    u16 dep;

    /* Convert arrival and departure time into minutes */
    arr = arrHour * 60 + arrMin;
    dep = depHour * 60 + depMin;

    /* Arrival time must be before departure time */
    if(arr >= dep)
        return 0;

    return 1;
}


/* Admin Mode password. */
#define ADMIN_PASSWORD 1234


/* ADMIN PASSWORD CHECK */
u8 CheckAdminPassword(void)
{
    u32 password;

    CmdLCD(CLEAR_LCD);
    StrLCD("Enter Password");

    /* Read password and display * instead of digits. */
    password = ReadPassword(0);

    /* Check entered password. */
    if(password == ADMIN_PASSWORD)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Password Correct");

        #ifdef HARDWARE
            delay_ms(1000);
        #else
            delay_ms(700);
        #endif

        return 1;
    }
    else
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Wrong Password");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        return 0;
    }
}


/*
   Display the main admin menu.
   User can edit RTC time or train information.
*/
void PrintMenu(void)
{
    u8 key;

    /* Display admin mode message */
    CmdLCD(CLEAR_LCD);
    StrLCD("  Entered Into  ");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("   ADMIN MODE   ");
	
    #ifdef HARDWARE
        delay_ms(1500);
    #else
        delay_ms(1000);
    #endif

		
		/* Ask for password after entering Admin Mode. */
		if(CheckAdminPassword())
		{
			
    /* Continuously display the main menu */
    while(1)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("1:Edit RTC Time");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("2:Edit Train");
	
        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif
	
        CmdLCD(CLEAR_LCD);
        StrLCD("3:Exit");
        CmdLCD(GOTO_LINE2_POS0);

        /* Read menu option from keypad */
        key = keyscan();
        CharLCD(key);
		
        #ifdef HARDWARE
            delay_ms(500);
        #else
            delay_ms(300);
        #endif


        /* Perform the selected operation */
        switch(key)
        {
            /* Edit RTC settings */
            case '1':
                EditRTCTime();
                CmdLCD(CLEAR_LCD);
                break;


            /* Open train editing menu */
            case '2':

                trainMenuExit = 0;

                while(!trainMenuExit)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("1:Edit Train");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Arr/Dep Time");
				
                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif
				
                    CmdLCD(CLEAR_LCD);
                    StrLCD("2:Edit Arrival");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("3:Edit Departure");
						
                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif
				
                    CmdLCD(CLEAR_LCD);
                    StrLCD("4:Edit Delay");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("5:Edit Platform");
						
                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif
				
                    CmdLCD(CLEAR_LCD);
                    StrLCD("6:Exit");
                    CmdLCD(GOTO_LINE2_POS0);
				
                    /* Read train menu option */
                    key = keyscan();
                    CharLCD(key);
						
                    #ifdef HARDWARE
                        delay_ms(500);
                    #else
                        delay_ms(300);
                    #endif


                    /* Perform selected train operation */
                    switch(key)
                    {
                        case '1':
                            EditTrainTime();
                            CmdLCD(CLEAR_LCD);
                            break;
												 							
                        case '2':
                            EditArrivalTime();
                            CmdLCD(CLEAR_LCD);
                            break;
												 
                        case '3':
                            EditDepartureTime();
                            CmdLCD(CLEAR_LCD);
                            break;
												 							
                        case '4':
                            EditDelayTime();
                            CmdLCD(CLEAR_LCD);
                            break;
												 
                        case '5':
                            EditPlatform();
                            CmdLCD(CLEAR_LCD);
                            break;
												 
                        /* Exit train menu */
                        case '6':
                            trainMenuExit = 1;
                            CmdLCD(CLEAR_LCD);
                            StrLCD("Exiting");
														CmdLCD(GOTO_LINE2_POS0);
														StrLCD("Edit Train");
                            delay_ms(500);
                            break;
												 
                        /* Invalid train menu option */
                        default:
                            CmdLCD(CLEAR_LCD);
                            StrLCD("Invalid Option");
                            delay_ms(500);
                            break;
                    }
					
                    /* Refresh train display after editing */
                    if(trainMenuExit == 0)
                    {
                        ResetTrainDisplay();
                        CmdLCD(CLEAR_LCD);
                    }
                }

                break;
						
            
            /* Exit admin mode */
            case '3':
                CmdLCD(CLEAR_LCD);
                StrLCD("Exiting");
								CmdLCD(GOTO_LINE2_POS0);
								StrLCD("Admin Mode");
				
                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif
				
                ResetTrainDisplay();
                CmdLCD(CLEAR_LCD);

                return;


            /* Invalid main menu option */
            default:
                CmdLCD(CLEAR_LCD);
                StrLCD("Invalid Option");
				
                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif
				
                break;
        }

        /* Return to normal display mode */
        EditMode = 0;
    }
	}
}


/*
   Edit RTC time and date.
   User can edit hour, minute, second, date,
   month, year and day.
*/
void EditRTCTime(void)
{
    u32 key;
    u32 hr,min,sec;
    u32 date,month,year,day;
    u32 value;


    /* Read current RTC values */
    GetRTCTimeInfo(&hr,&min,&sec);
    GetRTCDateInfo(&date,&month,&year);
    GetRTCDay(&day);


    /* Continuously display RTC edit menu */
    while(1)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("1:Hour 2:Min");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("3:Sec  4:Date");
			
        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif
			

        CmdLCD(CLEAR_LCD);
        StrLCD("5:Month 6:Year");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("7:Day   8:Exit");
				
        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif
			

        /* Read RTC edit option */
        CmdLCD(CLEAR_LCD);
        StrLCD("Select Option");
        key = keyscan();
        CmdLCD(GOTO_LINE2_POS0);
        CharLCD(key);

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif


        switch(key)
        {
            /* Edit hour */
            case '1':
							
							EditH:
                CmdLCD(CLEAR_LCD);
                StrLCD("Edit Hour:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                /* Check valid hour range */
                if(value > 23)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Hour");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be 0 to 23");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto EditH;
                }

                hr = value;

                /* Update RTC hour */
                SetRTCTimeInfo(hr,min,sec);

                CmdLCD(CLEAR_LCD);
                StrLCD("Hour Updated");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                break;


            /* Edit minute */
            case '2':
							
							EditM:
                CmdLCD(CLEAR_LCD);
                StrLCD("Edit Minute:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                /* Check valid minute range */
                if(value > 59)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Minute");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be 0 to 59");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto EditM;
                }

                min = value;

                /* Update RTC minute */
                SetRTCTimeInfo(hr,min,sec);

                CmdLCD(CLEAR_LCD);
                StrLCD("Minute Updated");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                break;


            /* Edit second */
            case '3':
							
							EditS:
                CmdLCD(CLEAR_LCD);
                StrLCD("Edit Second:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                /* Check valid second range */
                if(value > 59)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Second");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be 0 to 59");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto EditS;
                }

                sec = value;

                /* Update RTC second */
                SetRTCTimeInfo(hr,min,sec);

                CmdLCD(CLEAR_LCD);
                StrLCD("Second Updated");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                break;


            /* Edit date */
            case '4':
							
							EditD:
                CmdLCD(CLEAR_LCD);
                StrLCD("Edit Date:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                /* Check valid date range */
                if(value < 1 || value > 31)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Date");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be 1 to 31");
									
                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto EditD;
                }

                date = value;

                /* Update RTC date */
                SetRTCDateInfo(date,month,year);

                CmdLCD(CLEAR_LCD);
                StrLCD("Date Updated");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                break;


            /* Edit month */
            case '5':
							
							EditMo:
                CmdLCD(CLEAR_LCD);
                StrLCD("Edit Month:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                /* Check valid month range */
                if(value < 1 || value > 12)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Month");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be 1 to 12");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto EditMo;
                }

                month = value;

                /* Update RTC month */
                SetRTCDateInfo(date,month,year);

                CmdLCD(CLEAR_LCD);
                StrLCD("Month Updated");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                break;


            /* Edit year */
            case '6':
							
							EditY:
                CmdLCD(CLEAR_LCD);
                StrLCD("Edit Year:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                /* Check valid year range */
                if(value < 2000)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Year");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be > 2000");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto EditY;
                }
                else if(value > 2099)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Year");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be < 2099");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto EditY;
                }

                year = value;

                /* Update RTC year */
                SetRTCDateInfo(date,month,year);

                CmdLCD(CLEAR_LCD);
                StrLCD("Year Updated");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                break;


            /* Edit day */
            case '7':
							
							EditDy:
                CmdLCD(CLEAR_LCD);
                StrLCD("Edit Day:");
                CmdLCD(GOTO_LINE2_POS0);

                value = ReadNumLCD(0);

                /* Check valid day range */
                if(value > 6)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Day");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be 0 to 6");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto EditDy;
                }

                day = value;

                /* Update RTC day */
                SetRTCDay(day);

                CmdLCD(CLEAR_LCD);
                StrLCD("Day Updated");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                break;


            /* Exit RTC edit menu */
            case '8':

                CmdLCD(CLEAR_LCD);
                StrLCD("Exiting");
								CmdLCD(GOTO_LINE2_POS0);
								StrLCD("Edit RTC Time");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                ResetTrainDisplay();
                CmdLCD(CLEAR_LCD);

                return;


            /* Invalid RTC menu option */
            default:

                CmdLCD(CLEAR_LCD);
                StrLCD("Invalid Option");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                break;
        }


        /* Read the latest RTC values */
        GetRTCTimeInfo(&hr,&min,&sec);
        GetRTCDateInfo(&date,&month,&year);
        GetRTCDay(&day);
    }
}


/*
   Edit both arrival and departure time of a train.
*/
void EditTrainTime(void)
{
    u32 trainNo;
    u32 arrHour;
    u32 arrMin;
    u32 depHour;
    u32 depMin;

    TB *train;

    u8 key;
    u8 exitMenu;


TrainTime:

    /* Ask user to enter train number */
    CmdLCD(CLEAR_LCD);
    StrLCD("Edit Train Time");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Train No:");

    trainNo = ReadNumLCD(9);

    #ifdef HARDWARE
        delay_ms(1500);
    #else
        delay_ms(1000);
    #endif


    /* Find train from database */
    train = FindTrain(trainNo);

    if(train == 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Train Not Found");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        goto TrainTime;
    }


    /* Start train edit menu */
    exitMenu = 0;

    while(!exitMenu)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("1:Edit Arrival");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("2:Edit Departure");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        CmdLCD(CLEAR_LCD);
        StrLCD("3:Exit");
				CmdLCD(GOTO_LINE2_POS0);

        key = keyscan();
				
				CharLCD(key);
				
        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif


        switch(key)
        {
            /* Edit arrival time */
            case '1':

                /* Store current departure time */
                depHour = train->depHour;
                depMin  = train->depMin;


							ArrH:
                CmdLCD(CLEAR_LCD);
                StrLCD("Arr Hour:");
                CmdLCD(GOTO_LINE2_POS0);

                arrHour = ReadNumLCD(2);

                /* Validate arrival hour */
                if(arrHour > 23)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Arr Hour");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Use 0-23");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto ArrH;
                }


							ArrM:
                CmdLCD(CLEAR_LCD);
                StrLCD("Arr Minute:");
                CmdLCD(GOTO_LINE2_POS0);

                arrMin = ReadNumLCD(2);

                /* Validate arrival minute */
                if(arrMin > 59)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Arr Min");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Use 0-59");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto ArrM;
                }


                /* Arrival must be before departure */
                if((arrHour * 60 + arrMin) >=
                   (depHour * 60 + depMin))
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Arr Time");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Before Departure");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto ArrH;
                }


                /* Update arrival time */
                train->arrHour = arrHour;
                train->arrMin  = arrMin;

                TrainInfoChanged = 1;
                CurrentTrain = train;
                ForceDisplay = 1;


                /* Display update confirmation */
                CmdLCD(CLEAR_LCD);
                U32LCD(train->trainNo);
                LCD_StringXY(0,6,"Arrival");

                CmdLCD(GOTO_LINE2_POS0);
                StrLCD("Time Updated");

                #ifdef HARDWARE
                    delay_ms(1000);
                #else
                    delay_ms(700);
                #endif

                /* Give buzzer indication */
                Buzzer_Alert();

                break;


            /* Edit departure time */
            case '2':

                /* Store current arrival time */
                arrHour = train->arrHour;
                arrMin  = train->arrMin;


							DepH:
                CmdLCD(CLEAR_LCD);
                StrLCD("Dep Hour:");
                CmdLCD(GOTO_LINE2_POS0);

                depHour = ReadNumLCD(2);

                /* Validate departure hour */
                if(depHour > 23)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Dep Hour");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Use 0-23");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto DepH;
                }


							DepM:
                CmdLCD(CLEAR_LCD);
                StrLCD("Dep Minute:");
                CmdLCD(GOTO_LINE2_POS0);

                depMin = ReadNumLCD(2);

                /* Validate departure minute */
                if(depMin > 59)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Dep Min");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Use 0-59");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto DepM;
                }


                /* Departure must be after arrival */
                if((depHour * 60 + depMin) <=
                   (arrHour * 60 + arrMin))
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Dep Time");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("After Arrival");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto DepH;
                }


                /* Update departure time */
                train->depHour = depHour;
                train->depMin  = depMin;

                TrainInfoChanged = 1;
                CurrentTrain = train;
                ForceDisplay = 1;


                /* Display update confirmation */
                CmdLCD(CLEAR_LCD);
                U32LCD(train->trainNo);
                LCD_StringXY(0,6,"Departure");

                CmdLCD(GOTO_LINE2_POS0);
                StrLCD("Time Updated");

                #ifdef HARDWARE
                    delay_ms(1000);
                #else
                    delay_ms(700);
                #endif

                /* Give buzzer indication */
                Buzzer_Alert();

                break;


            /* Exit train time menu */
            case '3':

                exitMenu = 1;

                CmdLCD(CLEAR_LCD);
                StrLCD("Exiting");
								CmdLCD(GOTO_LINE2_POS0);
								StrLCD("Edit Train Time");

                #ifdef HARDWARE
                    delay_ms(1000);
                #else
                    delay_ms(700);
                #endif

                break;


            /* Invalid option */
            default:

                CmdLCD(CLEAR_LCD);
                StrLCD("Invalid Option");

                #ifdef HARDWARE
                    delay_ms(500);
                #else
                    delay_ms(300);
                #endif

                break;
        }
    }


    /* Refresh train display after editing */
    ResetTrainDisplay();
    CmdLCD(CLEAR_LCD);
}


/*
   Edit arrival hour or minute of a selected train.
*/
void EditArrivalTime(void)
{
    u32 trainNo;
    u32 hour;
    u32 min;
    TB *train;
    u8 key;
    u8 exitMenu;


ArrivalTime:

    /* Enter train number */
    CmdLCD(CLEAR_LCD);
    StrLCD("Edit Arr Time");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Train No:");

    trainNo = ReadNumLCD(9);

    /* Find train in database */
    train = FindTrain(trainNo);

    if(train == 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Train Not Found");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        goto ArrivalTime;
    }


    /* Store current arrival time */
    hour = train->arrHour;
    min  = train->arrMin;

    exitMenu = 0;


    /* Arrival time edit menu */
    while(!exitMenu)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("1:Edit Arr Hour");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("2:Edit Arr Min");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        CmdLCD(CLEAR_LCD);
        StrLCD("3:Exit");
        CmdLCD(GOTO_LINE2_POS0);

        key = keyscan();
        CharLCD(key);

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif


        switch(key)
        {
            /* Edit arrival hour */
            case '1':

							ArrH:
                CmdLCD(CLEAR_LCD);
                StrLCD("Arrival Hour:");
                CmdLCD(GOTO_LINE2_POS0);

                hour = ReadNumLCD(0);

                /* Check hour range */
                if(hour > 23)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Arr Hour");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be < 23");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto ArrH;
                }


                /* Arrival must be before departure */
                if(!IsValidTrainTime(hour,min,
                                     train->depHour,train->depMin))
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Arr Hour");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Before Departure");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto ArrH;
                }


                /* Update arrival hour */
                train->arrHour = hour;

                TrainInfoChanged = 1;
                CurrentTrain = train;
                ForceDisplay = 1;


                /* Display confirmation */
                CmdLCD(CLEAR_LCD);
                U32LCD(train->trainNo);
                LCD_StringXY(0,6,"Arrival");
                CmdLCD(GOTO_LINE2_POS0);
                StrLCD("Hour Updated");

                #ifdef HARDWARE
                    delay_ms(1000);
                #else
                    delay_ms(700);
                #endif

                Buzzer_Alert();

                break;


            /* Edit arrival minute */
            case '2':

							ArrM:
                CmdLCD(CLEAR_LCD);
                StrLCD("Arrival Min:");
                CmdLCD(GOTO_LINE2_POS0);

                min = ReadNumLCD(0);

                /* Check minute range */
                if(min > 59)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Arr Minute");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be < 59");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto ArrM;
                }


                /* Arrival must be before departure */
                if(!IsValidTrainTime(hour,min,
                                     train->depHour,train->depMin))
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Arr Min");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Before Departure");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto ArrM;
                }


                /* Update arrival minute */
                train->arrMin = min;

                TrainInfoChanged = 1;
                CurrentTrain = train;
                ForceDisplay = 1;


                /* Display confirmation */
                CmdLCD(CLEAR_LCD);
                U32LCD(train->trainNo);
                LCD_StringXY(0,6,"Arrival");
                CmdLCD(GOTO_LINE2_POS0);
                StrLCD("Min Updated");

                #ifdef HARDWARE
                    delay_ms(1000);
                #else
                    delay_ms(700);
                #endif

                Buzzer_Alert();

                break;


            /* Exit arrival time menu */
            case '3':

                exitMenu = 1;

                CmdLCD(CLEAR_LCD);
                StrLCD("Exiting");
								CmdLCD(GOTO_LINE2_POS0);
								StrLCD("Edit Arr Time");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                break;


            /* Invalid option */
            default:

                CmdLCD(CLEAR_LCD);
                StrLCD("Invalid Option");
                delay_ms(500);

                break;
        }
    }


    /* Refresh display after editing */
    ResetTrainDisplay();
    CmdLCD(CLEAR_LCD);
}


/*
   Edit departure hour or minute of a selected train.
*/
void EditDepartureTime(void)
{
    u32 trainNo;
    u32 hour;
    u32 min;
    TB *train;
    u8 key;
    u8 exitMenu;


DepatureTime:

    /* Enter train number */
    CmdLCD(CLEAR_LCD);
    StrLCD("Edit Dep Time");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Train No:");

    trainNo = ReadNumLCD(9);

    /* Find train in database */
    train = FindTrain(trainNo);

    if(train == 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Train Not Found");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        goto DepatureTime;
    }


    /* Store current departure time */
    hour = train->depHour;
    min  = train->depMin;

    exitMenu = 0;


    /* Departure time edit menu */
    while(!exitMenu)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("1:Edit Dep Hour");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("2:Edit Dep Min");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        CmdLCD(CLEAR_LCD);
        StrLCD("3:Exit");
        CmdLCD(GOTO_LINE2_POS0);

        key = keyscan();
        CharLCD(key);

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif


        switch(key)
        {
            /* Edit departure hour */
            case '1':

							DepH:
                CmdLCD(CLEAR_LCD);
                StrLCD("Departure Hour:");
                CmdLCD(GOTO_LINE2_POS0);

                hour = ReadNumLCD(0);

                /* Check hour range */
                if(hour > 23)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Dep Hour");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be < 23");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto DepH;
                }


                /* Departure must be after arrival */
                if(!IsValidTrainTime(train->arrHour,
                                     train->arrMin,
                                     hour,min))
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Dep Hour");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("After Arrival");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto DepH;
                }


                /* Update departure hour */
                train->depHour = hour;

                TrainInfoChanged = 1;
                CurrentTrain = train;
                ForceDisplay = 1;


                /* Display confirmation */
                CmdLCD(CLEAR_LCD);
                U32LCD(train->trainNo);
                LCD_StringXY(0,6,"Departure");
                CmdLCD(GOTO_LINE2_POS0);
                StrLCD("Hour Updated");

                #ifdef HARDWARE
                    delay_ms(1000);
                #else
                    delay_ms(700);
                #endif

                Buzzer_Alert();

                break;


            /* Edit departure minute */
            case '2':

							DepM:
                CmdLCD(CLEAR_LCD);
                StrLCD("Departure Min:");
                CmdLCD(GOTO_LINE2_POS0);

                min = ReadNumLCD(0);

                /* Check minute range */
                if(min > 59)
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Dep Minute");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("Must be < 59");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto DepM;
                }


                /* Departure must be after arrival */
                if(!IsValidTrainTime(train->arrHour,
                                     train->arrMin,
                                     hour,min))
                {
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Invalid Dep Min");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("After Arrival");

                    #ifdef HARDWARE
                        delay_ms(1500);
                    #else
                        delay_ms(1000);
                    #endif

                    goto DepM;
                }


                /* Update departure minute */
                train->depMin = min;

                TrainInfoChanged = 1;
                CurrentTrain = train;
                ForceDisplay = 1;


                /* Display confirmation */
                CmdLCD(CLEAR_LCD);
                U32LCD(train->trainNo);
                LCD_StringXY(0,6,"Departure");
                CmdLCD(GOTO_LINE2_POS0);
                StrLCD("Min Updated");

                #ifdef HARDWARE
                    delay_ms(1000);
                #else
                    delay_ms(700);
                #endif

                Buzzer_Alert();

                break;


            /* Exit departure time menu */
            case '3':

                exitMenu = 1;

                CmdLCD(CLEAR_LCD);
                StrLCD("Exiting");
								CmdLCD(GOTO_LINE2_POS0);
								StrLCD("Edit Dep Time");

                #ifdef HARDWARE
                    delay_ms(1500);
                #else
                    delay_ms(1000);
                #endif

                break;


            /* Invalid option */
            default:

                CmdLCD(CLEAR_LCD);
                StrLCD("Invalid Option");
                delay_ms(500);

                break;
        }
    }


    /* Refresh display after editing */
    ResetTrainDisplay();
    CmdLCD(CLEAR_LCD);
}


/*
   Edit the delay time of a selected train.
*/
void EditDelayTime(void)
{
    u32 trainNo;
    TB *train;
    u32 delay;


DelayTime:

    /* Enter train number */
    CmdLCD(CLEAR_LCD);
    StrLCD("Edit Delay");
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Train No:");

    trainNo = ReadNumLCD(9);

    #ifdef HARDWARE
        delay_ms(1500);
    #else
        delay_ms(1000);
    #endif


    /* Find train in database */
    train = FindTrain(trainNo);

    if(train == 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Train Not");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Found");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        goto DelayTime;
    }


Delay:

    /* Enter new delay value */
    CmdLCD(CLEAR_LCD);
    LCD_StringXY(0,0,"Train No:");
    U32LCD(train->trainNo);
    LCD_StringXY(1,0,"Delay(Min):");

    delay = ReadNumLCD(11);

    #ifdef HARDWARE
        delay_ms(1500);
    #else
        delay_ms(1000);
    #endif


    /* Maximum delay allowed is 999 minutes */
    if(delay > 999)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Delay");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Must be < 1000 M");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        goto Delay;
    }


    /* Do not allow delay to decrease */
    if(delay < train->delayMin)
    {
        CmdLCD(CLEAR_LCD);

        LCD_GotoXY(0,0);
        StrLCD("Invalid Delay");

        LCD_GotoXY(1,0);
        StrLCD("Delay < Old");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        CmdLCD(CLEAR_LCD);

        goto Delay;
    }


    /* Update train delay */
    train->delayMin = delay;

    TrainInfoChanged = 1;

    /* Give buzzer indication for delay update */
    Buzzer_Alert();


    /* Update train status */
    if(delay == 0)
        train->status = 0;
    else
        train->status = 1;


    /* Make edited train the current train */
    CurrentTrain = train;
    ForceDisplay = 1;

    /* Enable delay message display */
    ShowDelayMessage = 1;
    DelayDisplayMinute = delay;


    /* Display update confirmation */
    CmdLCD(CLEAR_LCD);
    U32LCD(train->trainNo);
    LCD_StringXY(0,6,"Delay");

    CmdLCD(GOTO_LINE2_POS0);
    LCD_StringXY(1,0,"Updated to:");
    U32LCD(train->delayMin);

    #ifdef HARDWARE
        delay_ms(1500);
    #else
        delay_ms(1000);
    #endif


    /* Refresh train scrolling display */
    ForceTrainRefresh();

    CmdLCD(CLEAR_LCD);
    EditMode = 0;
}


/*
   Edit the platform number of a selected train.
*/
void EditPlatform(void)
{
    u32 trainNo;
    u32 platform;
    TB *train;


Platform:

    /* Enter train number */
    CmdLCD(CLEAR_LCD);
    LCD_StringXY(0,0,"Edit Platform");
    LCD_StringXY(1,0,"Train No:");

    trainNo = ReadNumLCD(9);


    /* Find train in database */
    train = FindTrain(trainNo);

    if(train == 0)
    {
        CmdLCD(CLEAR_LCD);
        LCD_StringXY(0,0,"Train Not Found");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        goto Platform;
    }


PF:

    /* Enter new platform number */
    CmdLCD(CLEAR_LCD);
    LCD_StringXY(0,0,"Train No:");
    U32LCD(train->trainNo);

    LCD_StringXY(1,0,"Platform:");

    platform = ReadNumLCD(9);


    /* Platform number must be between 1 and 9 */
    if(platform < 1 || platform > 9)
    {
        CmdLCD(CLEAR_LCD);
        LCD_StringXY(0,0,"Invalid Platform");

        #ifdef HARDWARE
            delay_ms(1500);
        #else
            delay_ms(1000);
        #endif

        goto PF;
    }


    /* Update platform number */
    train->platform = platform;

    PlatformChanged = 1;
    TrainInfoChanged = 1;


    /* Give buzzer indication */
    Buzzer_Alert();


    /* Display update confirmation */
    CmdLCD(CLEAR_LCD);

    U32LCD(train->trainNo);
    LCD_StringXY(0,6,"Platform");

    LCD_GotoXY(1,0);
    LCD_StringXY(1,0,"Updated to:");
    U32LCD(train->platform);

    #ifdef HARDWARE
        delay_ms(1500);
    #else
        delay_ms(1000);
    #endif


    /* Refresh train display */
    ResetTrainDisplay();

    /* Display edited train */
    ForceDisplay = 1;
    CurrentTrain = train;
}
