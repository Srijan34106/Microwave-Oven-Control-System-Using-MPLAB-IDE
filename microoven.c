
#include <xc.h>
#include "microoven.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "main.h"
#include "timers.h"


unsigned char min,sec, pre_heat_time;
extern unsigned char operational_flag;

void power_on_screen(void)
{
    for(unsigned int i=0;i<16;i++){
        clcd_putch(BAR, LINE1(i));
    }
    clcd_print(" Powering ON", LINE2(1));
    clcd_print("Microwave oven", LINE3(0));
    for(unsigned int j=0;j<16;j++){
        clcd_putch(BAR, LINE4(j));
    }
    __delay_ms(2000); 
}

void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN,INST_MODE);
    __delay_ms(500);  
}

void display_menu(void)
{
    clcd_print("1.Micro ", LINE1(0));
    clcd_print("2.Grill ", LINE2(0));
    clcd_print("3.Convection ", LINE3(0));
    clcd_print("4.Start ", LINE4(0));  
}

void set_time(unsigned char key, unsigned char reset_flag){
    static unsigned char blink_pos=0, key_count=0;
    if(reset_flag == RESET_MODE){
        blink_pos=0;
        key_count=0;
        key=ALL_RELEASED;
        min=0;
        sec=0;
        reset_flag = RESET_NOTHING;
    }
    for(unsigned char wait =50;wait--;);   
    clcd_print("SET TIME (MM:SS)", LINE1(0));
    clcd_print("TIME-", LINE2(0));
    clcd_print("*:CLEAR  #:ENTER", LINE4(0));
    if(key!='*' && key!='#' && key!=ALL_RELEASED)
    {
        key_count++;
        if(key_count <=2){
            sec= sec*10+key;
            blink_pos=0;
        }
        else if(key_count <=4){
            min=min*10+key;
            blink_pos=1;
        }
    }
    else if(key =='*'){
        if(blink_pos)
        {
            min=0;
            key_count=2;
        }
        else
        {
            sec=0;
            key_count=0;
        }
    }
    else if(key =='#'){
        FAN = 1;
        TMR2ON=1;
        operational_flag = DISPLAY_TIME;
        clear_screen();
    }
    if (blink_pos ==0){
        clcd_putch(' ', LINE2(9));
        clcd_putch(' ', LINE2(10));
    }
    else{
        clcd_putch(' ', LINE2(6));
        clcd_putch(' ', LINE2(7));
    }
    clcd_putch(sec/10+'0', LINE2(9));
    clcd_putch(sec%10+'0', LINE2(10));
    clcd_putch( ':', LINE2(8));
    clcd_putch(min/10+'0', LINE2(6));
    clcd_putch(min%10+'0', LINE2(7));
}
void time_display(void)
{
    clcd_print("TIME= ", LINE1(1));
    clcd_putch(sec/10+'0', LINE1(12));
    clcd_putch(sec%10+'0', LINE1(13));
    clcd_putch( ':', LINE1(11));
    clcd_putch(min/10+'0', LINE1(9));
    clcd_putch(min%10+'0', LINE1(10));
    clcd_print("4.Start/Resume ",LINE2(1));
    clcd_print("5.Pause ",LINE3(1));
    clcd_print("6.Stop ",LINE4(1));
    
    if(min==0 && sec==0){
        TMR2ON=0;
        FAN=0;
        clear_screen();
        clcd_print("Time Up!!!", LINE2(4));
        clcd_print("Enjoy your meal", LINE3(0));
//        BUZZER=1;
        __delay_ms(1000);
//        BUZZER=0;
        clear_screen();
        operational_flag = MENU_SCREEN;
    } 
}
void set_temp(unsigned char key, unsigned char reset_flag){
    static unsigned char key_count;
    static unsigned int temp;
    if(reset_flag == RESET_MODE){
        temp = 0;
        key_count=0;
        key= ALL_RELEASED;
    }
    clcd_print("SET TEMP. (*C)", LINE1(0));
    clcd_print("TEMP : ", LINE2(0));
    clcd_print("*:CLEAR  #:ENTER", LINE4(0));
    clcd_putch((' '),LINE2(8));
    clcd_putch((' '),LINE2(9));
    clcd_putch((' '),LINE2(10));
    for(unsigned char wait=50; wait--;);
    clcd_putch((temp/100+'0'),LINE2(8));
    clcd_putch(((temp/10)%10+'0'),LINE2(9));
    clcd_putch((temp%10+'0'),LINE2(10));
    
    if(key!='*' && key!='#' && key!=ALL_RELEASED){
        if(key_count<3){
            temp=temp*10+key;
            key_count++;
        }
    }
    if(key =='*'){
        temp=0;
        key_count=0;
    }
     if(key =='#'){
        if(temp>180){
             temp=180;
        }
        clear_screen();
        clcd_print("Pre-Heating", LINE1(2));
        clcd_print("Time-Rem.= ", LINE3(0));
        clcd_print("sec", LINE3(13));
        pre_heat_time=60;
        TMR2ON=1;
        
        while(pre_heat_time!=0){
            clcd_putch((pre_heat_time/100+'0'),LINE3(10));
            clcd_putch(((pre_heat_time/10)%10+'0'),LINE3(11));
            clcd_putch((pre_heat_time%10+'0'),LINE3(12));
        }
        TMR2ON=0;
        clear_screen();
        operational_flag = MICRO_MODE;
    }
}          
  
void heat_food(void){
    sec=30;
    min=0;
    FAN=1;
    TMR2ON=1;
    operational_flag = DISPLAY_TIME;
    
    
}