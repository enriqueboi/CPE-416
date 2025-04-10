#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

char jack[] = "Jack Young";
char enrique[] = "Enrique Murillo";
char *name;

u08 button = 0;
u08 restart = 0;
int length = 0;

int main(void) {
   init();

   while (1) {
      clear_screen();
      restart = 0;

      // Get current name state
      if (button == 0) {
         name = jack;
         length = strlen(jack);
      } else {
         name = enrique;
         length = strlen(enrique);
      }

      // Print the name from the right, until it hit the left edge
      for (int i = 8; i >= 0; i--) {
         if (get_btn()) {
            setName();
            restart = 1;
            break;
         }

         lcd_cursor(i, 0);
         print_string(name);
         _delay_ms(300);
      }

      // If we got a buttone press, jump back to top of main loop
      if (restart) {
         continue;
      }

      // Print name from the left until it's all the way off the screen
      // Cursor at 0,0 and advancing name pointer each loop iteration
      for (int i = 1; i < length; i++) {
         if (get_btn()) {
            setName();
            restart = 1;
            break;
         }
         clear_screen();
         lcd_cursor(0, 0);
         print_string(name + i);
         _delay_ms(300);
      }
   }

   return 0;
}

// Wait until button is released and switch name state
void setName(void) {
   while (get_btn())
      ;
   _delay_ms(100);
   button = !button;
}