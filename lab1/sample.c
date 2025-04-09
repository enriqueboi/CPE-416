#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h> 

int main(void) {
   init();  //initialize board hardware
   clear_screen();
   lcd_cursor(0,0);
   print_num(2);
   return 0;
}
