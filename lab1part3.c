#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h> 

int main(void) {
   init();  //initialize board hardware
    int pinnumber = 2;
   for(int i = 0; i < 4; i++){
    // digital_out(pinnumber, 0);
    // delay(100);
    led_on(pinnumber);
    delay(100);
    led_off(pinnumber);
    
   }
   clear_screen();
   lcd_cursor(0,0);
   print_num(2);
   return 0;
}
