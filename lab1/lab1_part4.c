#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define ROWS 2
#define COLS 8

int main(void) {
   init();

   char numStr[] = "416";
   int numLength = strlen(numStr);

   // Starting positions
   int pos_x = 3;
   int pos_y = 0;

   while (1) {
      // Get accel values
      u08 ax = get_accel_x();
      u08 ay = get_accel_y();

      // Horizontal movement
      if ((200 < ay) && (ay < 245)) {
         pos_x++; // tilt to the right increases horizontal position
      } else if ((55 > ay) && (ay > 10)) {
         pos_x--; // tilt to the left decreases horizontal position
      }

      // Vertical movement
      if ((200 < ax) && (ax < 245)) {
         pos_y++; // tilt upward increases vertical position
      } else if ((55 > ax) && (ax > 10)) {
         pos_y--; // tilt downward decreases vertical position
      }

      // Clamp the x and y positions

      // Left most position
      if (pos_x < -(numLength - 1)) {
         pos_x = -(numLength - 1);
      }

      // Right most position
      if (pos_x > COLS - 1) {
         pos_x = COLS - 1;
      }

      // Top most position
      if (pos_y < 0) {
         pos_y = 0;
      }

      // Bottom most position
      if (pos_y >= ROWS) {
         pos_y = ROWS - 1;
      }

      clear_screen();

      // Number is off the left of the screen
      if (pos_x < 0) {
         // If pos_x is negative, less chars visible
         int visible_count = numLength + pos_x;
         lcd_cursor(0, pos_y);
         // New string will only be as long as amount visible plus null terminator
         char visibleStr[visible_count + 1];
         if (visible_count > 0) {
            // New string is visible_count long, with source
            // Change src pointer by adding the amount of cutoff charecters
            strncpy(visibleStr, numStr - pos_x, visible_count + 1);
            print_string(visibleStr);
         }
      } // Number fits on screen or falls of to right
      else {
         lcd_cursor(pos_x, pos_y);
         print_string(numStr);
      }

      // Delay to see movement
      _delay_ms(200);
   }

   return 0;
}