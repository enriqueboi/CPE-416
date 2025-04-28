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

u08 leftReading = 0;
u08 rightReading = 0;

int error = 0;

float gainKp = 0.08;

struct motor_command {
   int leftSpeed;
   int rightSpeed;
};

// Arduino Map Implementation
// https://docs.arduino.cc/language-reference/en/functions/math/map/
long map(long x, long in_min, long in_max, long out_min, long out_max) {
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void motor(uint8_t num, int speed) {
   if (speed >= 100) {
      speed = 100;
   }
   if (speed <= 0) {
      speed = 0;
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

      leftReading = analog(LEFT_SENSOR) + 15;
      rightReading = analog(RIGHT_SENSOR);

      struct motor_command command = compute_proportional(leftReading, rightReading);

      motor(LEFT_MOTOR, command.leftSpeed);
      motor(RIGHT_MOTOR, command.rightSpeed);
   }
}

struct motor_command compute_proportional(uint8_t left, uint8_t right) {
   struct motor_command command;

   error = leftReading - rightReading;

   command.leftSpeed = ((gainKp * -error) + 20);
   command.rightSpeed = ((gainKp * error) + 20);

   if (command.leftSpeed > 100) {
      command.leftSpeed = 100;
   }

   if (command.leftSpeed < 0) {
      command.leftSpeed = 0;
   }

   if (command.rightSpeed > 100) {
      command.rightSpeed = 100;
   }

   if (command.rightSpeed < 0) {
      command.rightSpeed = 0;
   }

   return command;
}