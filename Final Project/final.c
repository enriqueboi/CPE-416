/*
Name:  Jack Young and Enrique Murillo

Assignment Final Project

Description: BATTLE BOT!
*/

#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define LEFT_SENSOR 5
#define LEFT_MOTOR 0
#define RIGHT_SENSOR 4
#define RIGHT_MOTOR 1

#define IR_BACK 0
#define IR_FL 1
#define IR_FR 2
#define RANGING_PIN 3

#define IR_LINE_LIM 180

#define RLOOPS_LIM 200
#define RLOOPSB_LIM 100

u08 FL_EN = 1;
u08 FR_EN = 1;
u08 BACK_EN = 1;

int recoveryLoops = 0;
int recoveryLoopsBack = 0;

#define SEARCH_SPEED 20
#define BASE_SPEED 40

enum robotState {
   IDLE,
   SEARCH,
   APPROCH,
   RECOVERY,
   RECOVERY_BACK
};

enum robotState state = IDLE;

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

u08 findNearTarget() {
   u08 range = 0;
   range = analog(RANGING_PIN);

   if (range > 60) {
      motor(LEFT_MOTOR, 0);
      motor(RIGHT_MOTOR, 0);
      return 1;
   } else {
      motor(LEFT_MOTOR, SEARCH_SPEED);
      motor(RIGHT_MOTOR, -SEARCH_SPEED);
   }
   return 0;
}

void pushCan() {
   motor(LEFT_MOTOR, BASE_SPEED);
   motor(RIGHT_MOTOR, BASE_SPEED);
}

void miniSpinR() {
   motor(LEFT_MOTOR, -BASE_SPEED);
   motor(RIGHT_MOTOR, BASE_SPEED);
   _delay_ms(700);
   motor(LEFT_MOTOR, BASE_SPEED);
   motor(RIGHT_MOTOR, -BASE_SPEED);
   _delay_ms(700);
   motor(LEFT_MOTOR, 0);
   motor(RIGHT_MOTOR, 0);
}

void miniSpinL() {
   motor(LEFT_MOTOR, BASE_SPEED);
   motor(RIGHT_MOTOR, -BASE_SPEED);
   _delay_ms(700);
   motor(LEFT_MOTOR, -BASE_SPEED);
   motor(RIGHT_MOTOR, BASE_SPEED);
   _delay_ms(700);
   motor(LEFT_MOTOR, 0);
   motor(RIGHT_MOTOR, 0);
}

// Detect a line and update state
// We will temporairly disable reading the sensor so we don't get stuck in recovery loop
void recoveryDetector() {
   if (FR_EN) {
      if (analog(IR_FR) > IR_LINE_LIM) {
         state = RECOVERY;
         FR_EN = 0;
         miniSpinL();
         return;
      }
   }

   if (FL_EN) {
      if (analog(IR_FL) > IR_LINE_LIM) {
         state = RECOVERY;
         FL_EN = 0;
         miniSpinR();
         return;
      }
   }

   if (BACK_EN) {
      if (analog(IR_BACK) > IR_LINE_LIM + 20) {
         state = RECOVERY_BACK;
         BACK_EN = 0;
         return;
      }
   }
}

int main(void) {
   init(); // initialize board hardware
   set_servo(0, 127);
   set_servo(1, 127);
   clear_screen();
   _delay_ms(100);

   // while (1) {
   //    clear_screen();
   //    print_num(analog(IR_FL));
   //    lcd_cursor(0, 1);
   //    print_num(analog(IR_BACK));
   //    _delay_ms(100);
   // }

   // while (1) {
   //    clear_screen();
   //    print_num(analog(RANGING_PIN));
   //    _delay_ms(100);
   // }

   while (1) {
      // Check if we need to recover after each loop
      clear_screen();
      print_num(state);
      recoveryDetector();

      // Preform action for the state we are in
      switch (state) {
         case IDLE:
            if (get_btn()) {
               state = SEARCH;
            }
            break;
         case SEARCH:
            if (findNearTarget()) {
               state = APPROCH;
            }
            break;
         case APPROCH:
            pushCan();
            break;
         case RECOVERY:
            if (recoveryLoops <= RLOOPS_LIM) {
               motor(LEFT_MOTOR, -BASE_SPEED);
               motor(RIGHT_MOTOR, -BASE_SPEED);
               recoveryLoops++;
            }

            if (recoveryLoops > RLOOPS_LIM) {
               state = SEARCH;
               FR_EN = 1;
               FL_EN = 1;
               BACK_EN = 1;
               recoveryLoops = 0;
            }
            break;
         case RECOVERY_BACK:
            FR_EN = 1;
            FL_EN = 1;
            if (recoveryLoopsBack <= RLOOPSB_LIM) {
               motor(LEFT_MOTOR, BASE_SPEED);
               motor(RIGHT_MOTOR, BASE_SPEED);
               recoveryLoops++;
            }

            if (recoveryLoopsBack > RLOOPSB_LIM) {
               state = SEARCH;
               FR_EN = 1;
               FL_EN = 1;
               BACK_EN = 1;
               recoveryLoopsBack = 0;
            }
            break;
         default:
            return -1;
      }
   }
}
