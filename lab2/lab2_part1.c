#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
   init(); // initialize board hardware
   set_servo(0, 127);
   set_servo(1, 127);

   while (1) {

      for (int i = 0; i <= 100; i++) {
         motor(0, i);
         motor(1, i);
         _delay_ms(50);
      }

      for (int i = 100; i >= 0; i--) {
         motor(0, i);
         motor(1, i);
         _delay_ms(50);
      }

      for (int i = 0; i >= -100; i--) {
         motor(0, i);
         motor(1, i);
         _delay_ms(50);
      }

      for (int i = -100; i <= 0; i++) {
         motor(0, i);
         motor(1, i);
         _delay_ms(50);
      }
   }
}

// Arduino Map Implementation
// https://docs.arduino.cc/language-reference/en/functions/math/map/
long map(long x, long in_min, long in_max, long out_min, long out_max) {
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void motor(uint8_t num, int8_t speed) {
   int val = map(speed, -100, 100, 97, 157);
   if (num == 0) {
      set_servo(0, val);
      clear_screen();
      print_num(val);
   } else {
      set_servo(1, -val);
   }
}
