//train_info_display.c

#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "train_info_display.h"
#include "buzzer.h"

/* Variables used for train name and destination scrolling */
static int pos = 10;
static int first = 1;
static u32 lastTrainNo = 0xFFFFFFFF;
static u8 scrollMode = 0;


/* Find the length of a string */
int my_strlen(char *str)
{
    int len = 0;

    /* Count characters until null character */
    while(*str)
    {
        len++;
        str++;
    }

    return len;
}


/* Force the train display to refresh */
void ForceTrainRefresh(void)
{
    ResetTrainDisplay();
    lastTrainNo = 0;
}


/* Reset scrolling position */
void ResetTrainDisplay(void)
{
    pos = 10;
    first = 1;
}


/* Display train number, name and destination */
void DisplayTrain(u32 trainNo,char *trainName,char *dest,u8 platform,
                  u8 arrHour,u8 arrMin,u8 depHour,u8 depMin)
{
    char disp[11] = "          ";
    char *scrollText;
    int len;
    int i;
    int index;

    /* Reset display when a new train is selected */
    if(lastTrainNo != trainNo)
    {
        lastTrainNo = trainNo;
        ResetTrainDisplay();
        CmdLCD(CLEAR_LCD);
    }

    /* Select text to scroll */
    if(scrollMode == 0)
    {
        scrollText = trainName;
    }
    else
    {
        scrollText = dest;
    }

    /* Find length of scrolling text */
    len = my_strlen(scrollText);

    /* Display fixed train information */
    if(first)
    {
        /* Display train number */
        LCD_GotoXY(0,0);
        U32LCD(trainNo);
        LCD_CharXY(0,5,'<');

        /* Display platform number */
        LCD_CharXY(1,0,'P');
        LCD_GotoXY(1,1);
        U32LCD(platform);

        /* Display arrival time */
        LCD_CharXY(1,3,'A');
        LCD_CharXY(1,4,(arrHour/10)+'0');
        LCD_CharXY(1,5,(arrHour%10)+'0');
        LCD_CharXY(1,6,':');
        LCD_CharXY(1,7,(arrMin/10)+'0');
        LCD_CharXY(1,8,(arrMin%10)+'0');

        /* Display departure time */
        LCD_CharXY(1,10,'D');
        LCD_CharXY(1,11,(depHour/10)+'0');
        LCD_CharXY(1,12,(depHour%10)+'0');
        LCD_CharXY(1,13,':');
        LCD_CharXY(1,14,(depMin/10)+'0');
        LCD_CharXY(1,15,(depMin%10)+'0');

        first = 0;
    }

    /* Create the scrolling text */
    for(i=0; i<10; i++)
    {
        index = i - pos;

        if(index >= 0 && index < len)
        {
            disp[i] = scrollText[index];
        }
        else
        {
            disp[i] = ' ';
        }
    }

    disp[10] = '\0';

    /* Display scrolling text from LCD position 6 */
    LCD_StringXY(0,6,disp);

    /* Control scrolling speed */
    #ifdef HARDWARE
        delay_ms(150);
    #else
        delay_ms(50);
    #endif

    /* Move text one position to the left */
    pos--;

    /* Restart scrolling after the complete text passes */
    if(pos < -len)
    {
        pos = 10;

        /* Change between train name and destination */
        if(scrollMode == 0)
        {
            scrollMode = 1;
        }
        else
        {
            scrollMode = 0;
        }

        /* Clear scrolling area */
        LCD_StringXY(0,6,"          ");
    }
}


/* Display project welcome screen */
void WelcomeScreen(void)
{
    char msg[] =
        "Smart Railway Platform Clock Announcement Controller Project";

    char disp[17] = "          ";

    int pos = 16;
    int len;
    int i;
    int index;
    int count = 0;

    /* Find message length */
    len = my_strlen(msg);

    CmdLCD(CLEAR_LCD);

    /* Display welcome message */
    LCD_GotoXY(0,0);
    StrLCD("   Welcome To   ");

    /* Scroll project title */
    while(count < 77)
    {
        /* Clear display buffer */
        for(i=0; i<16; i++)
        {
            disp[i] = ' ';
        }

        disp[16] = '\0';

        /* Copy required characters into display buffer */
        for(i=0; i<16; i++)
        {
            index = i - pos;

            if(index >= 0 && index < len)
            {
                disp[i] = msg[index];
            }
            else
            {
                disp[i] = ' ';
            }
        }

        /* Display scrolling message */
        LCD_StringXY(1,0,disp);

        /* Control scrolling speed */
        #ifdef HARDWARE
            delay_ms(200);
        #else
            delay_ms(50);
        #endif

        pos--;
        count++;
    }

    CmdLCD(CLEAR_LCD);
}


/* Check whether train is 5 minutes away from arrival */
void CheckTrainApproaching(u8 currentHour,u8 currentMin,u8 arrHour,u8 arrMin)
{
    u32 currentTotal;
    u32 arrivalTotal;
    u32 difference;

    static u8 buzzerDone = 0;

    /* Convert current time into minutes */
    currentTotal = currentHour * 60 + currentMin;

    /* Convert arrival time into minutes */
    arrivalTotal = arrHour * 60 + arrMin;

    /* Handle arrival time after midnight */
    if(arrivalTotal < currentTotal)
    {
        arrivalTotal += 24 * 60;
    }

    /* Find remaining time before arrival */
    difference = arrivalTotal - currentTotal;

    /* Turn on buzzer when train is 5 minutes away */
    if(difference == 5)
    {
        if(buzzerDone == 0)
        {
            Buzzer_Alert();

            buzzerDone = 1;
        }
    }

    /* Allow buzzer again for the next train */
    if(difference > 5)
    {
        buzzerDone = 0;
    }
}


/* Display platform, arrival and departure details */
void DisplayTrainDetails(u8 platform,
                         u8 arrHour,
                         u8 arrMin,
                         u8 depHour,
                         u8 depMin)
{
    /* Display platform number */
    LCD_CharXY(1,0,'P');

    LCD_GotoXY(1,1);
    U32LCD(platform);

    /* Display arrival time */
    LCD_CharXY(1,3,'A');

    LCD_CharXY(1,4,(arrHour/10)+'0');
    LCD_CharXY(1,5,(arrHour%10)+'0');
    LCD_CharXY(1,6,':');
    LCD_CharXY(1,7,(arrMin/10)+'0');
    LCD_CharXY(1,8,(arrMin%10)+'0');

    /* Display departure time */
    LCD_CharXY(1,10,'D');

    LCD_CharXY(1,11,(depHour/10)+'0');
    LCD_CharXY(1,12,(depHour%10)+'0');
    LCD_CharXY(1,13,':');
    LCD_CharXY(1,14,(depMin/10)+'0');
    LCD_CharXY(1,15,(depMin%10)+'0');
}


/* Display RTC time */
void DisplayCurrentRTCTime(u8 hour,u8 min,u8 sec)
{
    /* Display current hour */
    LCD_CharXY(1,0,(hour/10)+'0');
    LCD_CharXY(1,1,(hour%10)+'0');
    LCD_CharXY(1,2,':');

    /* Display current minute */
    LCD_CharXY(1,3,(min/10)+'0');
    LCD_CharXY(1,4,(min%10)+'0');
    LCD_CharXY(1,5,':');

    /* Display current second */
    LCD_CharXY(1,6,(sec/10)+'0');
    LCD_CharXY(1,7,(sec%10)+'0');
	
}

/* Display train delay */
void DisplayTrainDelay(u16 delayMin)
{
	
		/* Display train delay */
    LCD_StringXY(1,9,"DY:");

    LCD_CharXY(1,12,(delayMin/100)+'0');
    LCD_CharXY(1,13,((delayMin/10)%10)+'0');
    LCD_CharXY(1,14,(delayMin%10)+'0');

    LCD_StringXY(1,15,"M");
}
