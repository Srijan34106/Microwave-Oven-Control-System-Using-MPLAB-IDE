#ifndef MICROOVEN_H
#define MICROOVEN_H

void power_on_screen(void);
void clear_screen(void);
void display_menu(void);
void set_time(unsigned char key, unsigned char reset_flag);
void set_temp(unsigned char key, unsigned char reset_flag);
void time_display(void);
void heat_food(void);

#endif