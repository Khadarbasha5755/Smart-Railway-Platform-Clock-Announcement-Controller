# Smart Railway Platform Clock & Announcement Controller

An embedded railway platform management system developed using the **LPC2129 ARM7 microcontroller**. The system combines an RTC-based railway clock, train scheduling, LCD train information display, platform status LEDs, buzzer alerts, and  keypad-based administration.

## 📌 Project Overview

The **Smart Railway Platform Clock & Announcement Controller** is designed to display real-time railway information on a 16×2 LCD.

The system continuously compares the current RTC time with the train schedule stored in the train database. When the current time matches a scheduled train, the system automatically displays the **train number and train name in first line it is fixed and second line displays the platform number, arrival time, and departure time** for the first 5 seconds. For the next 5 seconds, the system displays the **current RTC time and the train delay in minutes**. This 10-second display cycle repeats continuously while the train is scheduled.


If no train is scheduled at the current time, the LCD displays the current **time, day, and date**.

The project also provides an administrator mode where authorized users can modify train timing and delay information.

## ✨ Features

* Real-time clock using RTC
* Automatic train schedule matching
* Train database stored in embedded C
* 16×2 LCD display
* Scrolling train name
* Train number displayed after `<`
* Platform number display
* Arrival and departure time display
* Train delay editing
* Train timing editing
* Keypad-based user input
* Password-protected administrator mode
* External interrupt based admin access
* LED-based train status indication
* Buzzer alert for approaching trains
* Delay notification
* Automatic RTC display when no train is scheduled
* LPC2129 ARM7 microcontroller based
* Designed and tested using Keil and Proteus

## 🛠️ Hardware Requirements

| Component          | Purpose                           |
| ------------------ | --------------------------------- |
| LPC2129            | Main microcontroller              |
| 16×2 LCD           | Display train and RTC information |
| RTC                | Maintains current time and date   |
| 4×4 Keypad         | User/admin input                  |
| Green LED          | Train on platform                 |
| Yellow LED         | Train approaching                 |
| Red LED            | Train delayed                     |
| Buzzer             | Alert/announcement indication     |
| Crystal Oscillator | Microcontroller clock             |
| Power Supply       | System power                      |

## 💻 Software Requirements

* Keil µVision
* ARM7/LPC21xx compiler
* Proteus Design Suite
* Embedded C
* LPC2129 device support package

## 🔌 LPC2129 Peripheral Usage

### LCD

The LCD is used to display:

**When a train is scheduled:**

```text
20701<Tirupati Van
P1 A09:30 D09:40
```

The train name can scroll across the first line.

**When no train is scheduled:**

```text
09:00:00   FRI 
21/08/2026
```

### RTC

The RTC provides:

* Hour
* Minute
* Second
* Date
* Month
* Year
* Day

The current RTC time is continuously compared with the train schedule.

### Keypad

The keypad is used for administrative operations such as:

* Editing RTC Information like hour, min, sec, date, month, year, and day.
* Entering train number
* Editing arrival time
* Editing departure time
* Editing train delay
* Editing train platform
* Navigating the administrator menu

### LEDs

The LED indication represents the train status:

| LED       | Condition                          |
| --------- | ---------------------------------- |
| 🔴 Red    | Train has a delay                  |
| 🟡 Yellow | Train is approaching               |
| 🟢 Green  | Train is currently on the platform |

### Buzzer

The buzzer provides an alert when:

* A train is approaching the platform
* Train delay information is updated

## 🚆 Train Scheduling Logic

Each train contains information such as:

```c
Train Number
Train Name
Destination
Platform Number
Arrival Hour
Arrival Minute
Departure Hour
Departure Minute
Delay Minute
Train Status
```

Example:

```c
{20701,"Tirupati Vande Bharat","GUNTUR",
 1,9,30,9,40,0,0}
```

The system converts the current time into minutes:

```c
currentTime = hour * 60 + minute;
```

The scheduled arrival time is also converted into minutes:

```c
arrivalTime = arrHour * 60 + arrMin;
```

The controller compares the current time with the train schedule and selects the appropriate train.

## 🚦 Train Status

The train status is controlled according to its arrival, delay, and departure time.

### Delayed Train

If the train has a delay:

```text
RED LED = ON
```

The red LED remains ON until the train reaches the approaching period.

If:

```text
delay = 0
```

the red LED remains OFF.

### Approaching Train

Five minutes before the arrival time:

```text
YELLOW LED = ON
```

The buzzer can also be activated to indicate the approaching train.

### Train on Platform

From the arrival time until departure:

```text
GREEN LED = ON
```

After departure, the system searches for the next scheduled train.

## 📺 LCD Display Operation

### Train Information Mode

The first line displays:

```text
TRAIN_NO:TRAIN_NAME
```

For example:

```text
20701<Tirupati V
```

The train name scrolls from **right to left** while the train number remains fixed.

The second line displays:

```text
P1 A09:30 D09:40
```

where:

* `P1` = Platform 1
* `A09:30` = Arrival time
* `D09:40` = Departure time

### RTC Mode

If there is no matching train:

```text
09:00:00   FRI
21/08/2026
```

The system continuously updates the RTC information.

## 🔐 Administrator Mode

Administrator mode is accessed using an external interrupt.

A password is required before accessing administrative functions.

Possible operations include:

```text
1. Edit RTC Time
2. Edit Train
3. Other Train Settings
```

The administrator can modify train information without changing the main program.

## 📂 Project Structure

A typical project structure is:

```text
Smart-Railway-Platform-Clock/
│
├── main.c
│
├── train_db.c
├── train_db.h
│
├── train_info_display.c
├── train_info_display.h
│
├── rtc.c
├── rtc.h
│
├── lcd.c
├── lcd.h
├── lcd_defines.h
│
├── kpm.c
├── kpm.h
│
├── led.c
├── led.h
│
├── buzzer.c
├── buzzer.h
│
├── eint_interrupt.c
├── eint_interrupt.h
│
├── menu.c
├── menu.h
│
├── delay.c
├── delay.h
│
├── types.h
├── defines.h
│
├── README.md
│
└── Proteus/
    └── Smart_Railway_Controller.pdsprj
```

## 🧩 Main Software Modules

### `main.c`

Controls the overall application flow and initializes the required peripherals.

### `train_db.c`

Contains the train database.

Example:

```c
TB traindb[MAX] =
{
    {20701,"Tirupati Vande Bharat","Tirupati",
     1,9,30,9,40,0,0},

    {17256,"Narasapur Express","Narasapur",
     2,14,30,14,40,0,0},

    {12747,"Palnadu Super Fast Express","Vikarabad",
     3,18,0,18,10,0,0}
};
```

### `train_info_display.c`

Responsible for displaying:

* Train number
* Train name
* Platform
* Arrival time
* Departure time
* Delay minutes
* Scrolling train name
* RTC information

### `rtc.c`

Handles:

* RTC initialization
* Time reading
* Date reading
* Day reading
* Time editing
* Date editing

### `lcd.c`

Contains LCD driver functions such as:

```c
InitLCD();
CmdLCD();
CharLCD();
StrLCD();
U32LCD();
S32LCD();
```

### `kpm.c`

Handles keypad initialization and key detection.

### `led.c`

Controls railway status LEDs.

### `buzzer.c`

Controls buzzer alerts.

### `eint_interrupt.c`

Handles external interrupt functionality for administrator access.

### `menu.c`

Handles the administrator menu and user selections.

## 🔄 System Flow

```text
                 +----------------+
                 |    LPC2129     |
                 +-------+--------+
                         |
          +--------------+--------------+
          |              |              |
         RTC           Keypad          EINT
          |              |              |
          +--------------+--------------+
                         |
                  Train Database
                         |
                  Schedule Matching
                         |
              +----------+----------+
              |                     |
        Train Matched          No Train
              |                     |
      Train Information          RTC Info
              |                     |
       +------+------+              |
       |      |      |              |
      LCD    LED   Buzzer           LCD
```

## ⚙️ Working Principle

1. Initialize LPC2129 peripherals.
2. Initialize LCD, RTC, keypad, LEDs, buzzer, and external interrupt.
3. Read the current RTC time.
4. Compare the current time with the train database.
5. Check whether a train is arriving or is already on the platform.
6. If a train is matched:

   * Display train number.
   * Scroll train name.
   * Display platform number.
   * Display arrival time.
   * Display departure time.
   * Display delay minutes.

7. Control LEDs according to train status.
8. Activate buzzer when required.
9. If no train is matched:

   * Display current RTC time.
   * Display day and date.

10. Monitor the external interrupt for administrator access.
11. Allow authorized users to edit train time or delay.
12. Continuously repeat the process.

## 🧪 Simulation

The project can be simulated using **Proteus**.

The simulation includes:

* LPC2129
* LCD
* RTC
* Keypad
* LEDs
* Buzzer
* Required clock and power connections

The firmware can be compiled using Keil and the generated HEX file can be loaded into the LPC2129 Proteus model.

## 🎯 Applications

This project can be used as a prototype for:

* Railway platform information systems
* Train arrival/departure displays
* Railway announcement systems
* Embedded railway automation
* Platform status indication systems
* Educational ARM7 embedded systems projects

## 🚀 Future Enhancements

The project can be extended with:

* UART-based PC monitoring
* GSM/SMS train delay notification
* ESP8266/ESP32 IoT connectivity
* Web-based railway information display
* Automatic voice announcement
* EEPROM-based train database
* Multiple LCD displays
* Real-time online railway data
* Automatic date/time synchronization
* SD card data logging

## 👨‍💻 Technologies Used

```text
Microcontroller : LPC2129 ARM7
Programming     : Embedded C
IDE             : Keil µVision
Simulation      : Proteus
Display         : 16×2 LCD
Input           : 4×4 Keypad
Timekeeping     : RTC
Alert           : Buzzer
Status          : LEDs
```

## 📌 Project Status

**Status:** Completed

The core railway scheduling, RTC display, LCD train information, scrolling train name, keypad administration, LED indication, and buzzer functionality are implemented as part of the project.

## 👤 Author

**Shaik Khadar Basha**

Embedded Systems / ARM7 Project

---

## ⭐ If you find this project useful

Feel free to ⭐ star the repository and use it for learning and educational purposes.
