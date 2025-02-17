/*
 * File:   main.c
 * Author: srijan aditya
 *
 * Created on 9 February, 2025, 3:32 PM
 */


#include <xc.h>
#include "clcd.h"
#include "main.h"
#include "microoven.h"
#include "timers.h"
#include "matrix_keypad.h"

#pragma config WDTE = OFF

unsigned char operational_flag = POWER_ON_SCREEN ;
unsigned char reset_flag = RESET_NOTHING;
extern unsigned char min, sec;


void init_config(void)
{
    //config CLCD;
    init_clcd();
    init_matrix_keypad();
    FAN_DDR=0;
    FAN=0;
//    BUZZER_DDR = 0;
//    BUZZER = 0; 
    init_timer2();
    PEIE=1;
    GIE=1;
    
}

void main(void) 
{
    unsigned char key; unsigned char start_mode=0;
    init_config();
    while(1)
    {
        key= read_matrix_keypad(STATE);
        if(operational_flag == MENU_SCREEN){
            if(key == 1){
                clear_screen();
                operational_flag = MICRO_MODE;
                start_mode=0;
                clcd_print("POWER = 900W", LINE2(0));
                __delay_ms(3000);
                clear_screen();
                reset_flag = RESET_MODE;
            }
            else if(key == 2){
                operational_flag = GRILL_MODE;
                clear_screen();
                start_mode=0;
                reset_flag=RESET_MODE;
            }
            else if(key == 3){
                operational_flag = CONVECTION_MODE;
                clear_screen();
                start_mode=0;
                reset_flag = RESET_MODE;
            }
            else if(key == 4){
                operational_flag = START_MODE;
                clear_screen();
                start_mode=1;
                reset_flag = RESET_MODE;
            }
        }
        if(operational_flag == DISPLAY_TIME){
            if(key==4){
//                operational_flag=START;
                if(start_mode){
                    sec=sec+30;
                    if(sec>59){
                        min++;
                        sec=sec-60;
                    }
                }
            }
            else if(key==5){
                operational_flag= PAUSE;
            }
            else if(key==6){
               operational_flag= STOP; 
            }
        }
        else if(operational_flag== PAUSE){
            if(key==4){
                FAN=1;
                TMR2ON=1;
                operational_flag= DISPLAY_TIME;
            }
        }
        switch(operational_flag)
        {
            case POWER_ON_SCREEN:
                                    // display power on screen 
                                    power_on_screen();
                                    operational_flag = MENU_SCREEN;
                                    clear_screen();
                                    break;
            case MENU_SCREEN:
                                    display_menu();
                                    break;
            case MICRO_MODE:
                                    set_time(key,reset_flag);
                                    break;
            case DISPLAY_TIME:
                                    time_display();
                                    break;
            case PAUSE:
                                    FAN = 0;
                                    TMR2ON = 0;
                                    break;
            case STOP:
                                    FAN = 0;
                                    TMR2ON = 0;
                                    clear_screen();
                                    operational_flag=MENU_SCREEN;
                                    break;
                                    
            case GRILL_MODE:
                                    set_time(key,reset_flag);
                                    break;
            case CONVECTION_MODE:
                                    set_temp(key,reset_flag);
                                    break;
            case START_MODE:
                                    heat_food();
                                    break;
            
        }
        reset_flag = RESET_NOTHING;
    }
}
