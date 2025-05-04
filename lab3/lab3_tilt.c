#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>


int tilt(){
   int epochs = 0;
   while(1){
   clear_screen();
   
   u08 ay = get_accel_y();

   // Horizontal movement
   if(epochs < 1)
   epochs = 1;
      
      if ((220 < ay) && (ay < 245)) {
         epochs += 1;
      } else if ((60 > ay) && (ay > 10)) {
      epochs -= 1;
      // pos_x--; // tilt to the left decreases horizontal position
      }

         lcd_cursor(0,0);
         print_num(epochs);

      _delay_ms(100);

   if(get_btn() == 1){
      return epochs;
   }
   }

}

int main(void) {
   init(); // initialize board hardware

   
   int epochs = tilt();

   lcd_cursor(0,0);
   print_num(epochs);

   // while (1) {

   // }

   return 0;
}
