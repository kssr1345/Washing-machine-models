/* 
 * File:   main.h
 * Author: K SOMA SEKHAR REDDY
 *
 * Created on 12 October, 2021, 11:01 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "clcd.h"
#include "digital_keypad.h"
#include "timers.h"
#define WASHING_PROGRAM_DISPLAY 0x01
#define WATER_LEVEL             0x02
#define START_STOP              0x03
#define START_PROGRAM           0x04
#define PAUSE                   0x05

#define WASHING_PROGRAM_DISPLAY_RESET   0x10
#define WATER_LEVEL_RESET               0x11
#define START_STOP_RESET                0x12
#define START_PROGRAM_RESET             0x01 

#define RESET_NOTHING                   0x00


#define FAN                             RC2
#define FAN_DDR                         TRISC2
#define BUZZER                          RC1
#define BUZZER_DDR                      TRISC1
#define ON                              1
#define OFF                             0


void power_on(void); //staring program
void clear_scr(void); //clearing the screen
void washing_program_display(unsigned char key); //displaying list of washing programs
void wash_level_display(unsigned char key);      //displaying list of water levels
unsigned char start_stop_display(unsigned char key); //displayin start and stop and display
void set_time_for_program(void);                 /// intializing the time
void run_program(unsigned char key);             //Runnning the main program




#endif	/* MAIN_H */

