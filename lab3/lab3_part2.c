/*
Name:  Jack Young and Enrique Murillo

Assignment Lab 3 Part 2

Description:
*/

#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define LEFT_SENSOR 5
#define LEFT_MOTOR 0
#define RIGHT_SENSOR 4
#define RIGHT_MOTOR 1

float learningRate = 0.3;

struct motor_command {
   int leftSpeed;
   int rightSpeed;
};

struct reading_pairs {
   float leftReading;
   float rightReading;
   float leftSpeed;
   float rightSpeed;
};

sturct reading_pairs pairs[200];

struct hidden_nueron {
   float in1;
   float in2;
   float w1;
   float w2;
   float bias;
   float out1;
   float out2;
};

struct output_nueron {
   float in1;
   float in2;
   float in3;
   float w1;
   float w2;
   float w3;
   float bias;
   float out;
};

// Arduino Map Implementation
// https://docs.arduino.cc/language-reference/en/functions/math/map/
long mapInt(long x, long in_min, long in_max, long out_min, long out_max) {
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Arduino Map Implementation
// https://docs.arduino.cc/language-reference/en/functions/math/map/
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

struct motor_command compute_proportional(uint8_t left, uint8_t right);

int main(void) {
   init(); // initialize board hardware
   set_servo(0, 127);
   set_servo(1, 127);

   clear_screen();
   print_string("Proportinal");
   while (!get_btn()) {
      runController();
   }

   clear_screen();
   print_string("Data");
   while (!get_btn()) {
      captureControllerValues();
   }

   print_string("Training");
   while (!get_btn()) {
      train();
   }
}

void train();
void runController();

void captureControllerValues() {

   static int currentIndex = 0;

   if (currentIndex >= 200) {
      currentIndex = 0;
   }

   float leftReading = analog(LEFT_SENSOR);
   float rightReading = analog(RIGHT_SENSOR);

   lcd_cursor(5, 0);
   print_num(currentIndex);

   lcd_cursor(0, 1);
   print_num(leftReading);

   lcd_cursor(4, 1);
   print_num(rightReading);

   struct motor_command command = compute_proportional(leftReading, rightReading);

   struct reading_pairs reading;

   reading.leftReading = mapFloat(leftReading, 0, 255, 0.0f, 1.0f);
   reading.rightReading = mapFloat(rightReading, 0, 255, 0.0f, 1.0f);
   reading.leftSpeed = mapFloat(command.leftSpeed, 0, 100, 0.0f, 1.0f);
   reading.rightSpeed = mapFloat(command.rightSpeed, 0, 100, 0.0f, 1.0f);

   pairs[currentIndex] = reading;
   currentIndex++;
}
