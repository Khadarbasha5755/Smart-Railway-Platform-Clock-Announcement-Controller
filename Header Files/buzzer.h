//buzzer.h

#ifndef BUZZER_H
#define BUZZER_H


/* Buzzer connected to P0.01 */
#define BUZZER 1


/* Initialize buzzer GPIO pin */
void Buzzer_Init(void);


/* Turn buzzer ON */
void Buzzer_On(void);


/* Turn buzzer OFF */
void Buzzer_Off(void);


/* Generate one short beep */
void Buzzer_Beep(void);


/* Generate multiple beeps */
void Buzzer_Alert(void);


#endif
