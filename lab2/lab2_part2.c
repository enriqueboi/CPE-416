#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define LEFT_SENSOR 0
#define RIGHT_SENSOR 1

int mode = 0;

// Arduino Map Implementation
// https://docs.arduino.cc/language-reference/en/functions/math/map/
long map(long x, long in_min, long in_max, long out_min, long out_max) {
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void motor(uint8_t num, int8_t speed) {
   int val = map(speed, -100, 100, 97, 157);
   if (speed == 0) {
      val = 127;
   }
   if (num == 0) {
      set_servo(0, val);
   } else {
      set_servo(1, -val);
   }
}

void checkButton() {
   if (!get_btn()) {
      return;
   }

   mode = !mode;
   while (get_btn())
      ;

   clear_screen();
   if (mode == 0) {
      print_string("Fear");
   } else {
      print_string("Aggresion");
   }
   _delay_ms(100);
}

int main(void) {
   init(); // initialize board hardware
   set_servo(0, 127);
   set_servo(1, 127);
   clear_screen();
   print_string("Fear");

   while (1) {
      checkButton();
      u08 left = analog(LEFT_SENSOR);
      u08 right = analog(RIGHT_SENSOR) + 3;

      u08 leftSpeed = 0;
      u08 rightSpeed = 0;

      if (left >= 230) {
         leftSpeed = map(left, 230, 250, 0, 100);
      }

      if (right >= 230) {
         rightSpeed = map(right, 230, 250, 0, 100);
      }

      if (mode == 0) {
         motor(0, leftSpeed);
         motor(1, rightSpeed);
      } else {
         motor(0, rightSpeed);
         motor(1, leftSpeed);
      }
   }
}
