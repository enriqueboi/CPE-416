/*
Name:  Jack Young and Enrique Murillo

Assignment Lab 2 Part 3

Description: PID Line Follower bot
*/

#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define LEFT_SENSOR 5
#define LEFT_MOTOR 0
#define RIGHT_SENSOR 4
#define RIGHT_MOTOR 1

#define MAX_SPEED_CHANGE 10

u08 leftReading = 0;
u08 rightReading = 0;

int leftSpeed = 0;
int rightSpeed = 0;

int prevLeft = 0;
int prevRight = 0;

int error = 0;
int prevError = 0;
int changeError = 0;
int sumError = 0;

float gainKp = 0.08;
float gainKd = 0.8;
float gainKi = 0.02;

// Arduino Map Implementation
// https://docs.arduino.cc/language-reference/en/functions/math/map/
long map(long x, long in_min, long in_max, long out_min, long out_max) {
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void motor(uint8_t num, int speed) {
   if (speed >= 100) {
      speed = 100;
   }
   if (speed <= -100) {
      speed = -100;
   }

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

int main(void) {
   init(); // initialize board hardware
   set_servo(0, 127);
   set_servo(1, 127);
   clear_screen();

   while (1) {
      // clear_screen();

      leftReading = analog(LEFT_SENSOR) + 15;
      rightReading = analog(RIGHT_SENSOR);

      // print_num(leftReading);
      // lcd_cursor(0, 1);
      // print_num(rightReading);

      // From readings, we need to come up with some kind of error
      // Positive error will be too far to right (need CCW correction)
      // Negative error will be too far to left (need CW correction)

      // WHite is ~184
      // Black is ~ 195

      // Target 189

      error = leftReading - rightReading;
      // if (error > 0) {
      //    error = error * error;
      // } else {
      //    error = error * -error;
      // }
      changeError = error - prevError;
      prevError = error;
      sumError += error;

      if (sumError >= 500) {
         sumError = 500;
      }

      if (sumError <= -500) {
         sumError = -500;
      }

      leftSpeed = ((gainKp * -error) + (gainKd * -changeError) + (gainKi * -sumError) + 30);
      rightSpeed = ((gainKp * error) + (gainKd * changeError) + (gainKi * sumError) + 30);

      if (leftSpeed > (prevLeft + MAX_SPEED_CHANGE)) {
         leftSpeed = prevLeft + MAX_SPEED_CHANGE;
      }

      if (leftSpeed < (prevLeft - MAX_SPEED_CHANGE)) {
         leftSpeed = prevLeft - MAX_SPEED_CHANGE;
      }

      if (rightSpeed > (prevRight + MAX_SPEED_CHANGE)) {
         rightSpeed = prevRight + MAX_SPEED_CHANGE;
      }

      if (rightSpeed < (prevRight - MAX_SPEED_CHANGE)) {
         rightSpeed = prevRight - MAX_SPEED_CHANGE;
      }

      prevLeft = leftSpeed;
      prevRight = rightSpeed;

      motor(LEFT_MOTOR, leftSpeed);
      motor(RIGHT_MOTOR, rightSpeed);

      lcd_cursor(1, 0);
      print_num(error);
      lcd_cursor(0, 1);
      print_num(changeError);
   }
}