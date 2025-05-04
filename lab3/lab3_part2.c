/*
Name:  Jack Young and Enrique Murillo

Assignment Lab 3 Part 2

Description:
*/

#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#define LEFT_SENSOR 5
#define LEFT_MOTOR 0
#define RIGHT_SENSOR 4
#define RIGHT_MOTOR 1

float learningRate = 0.3;

int error = 0;

// Proportional gain
float gainKp = 0.04;

struct motor_command {
   int leftSpeed;
   int rightSpeed;
};

struct reading_pairs {
   u08 leftReading;
   u08 rightReading;
};

struct reading_pairs pairs[200];
// u08 currentPair = 0;

struct hidden_nueron {
   float in1;
   float in2;
   float w1;
   float w2;
   float bias;
   float out;
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

float oldOutWeights[6];
float outCalcs[2];

struct hidden_nueron hiddenN[3];
struct output_nueron outputN[2];

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

// Return a motor command for a given left and right sensor reading
// Command comes back as a stuct of two ints between 0 and 100
struct motor_command compute_proportional(u08 left, u08 right) {
   struct motor_command command;

   error = left - right;

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

// Adds one reading_pair to the pairs array at each call
// Will iterate to fill the array, once full will loop back and overwrite
void captureControllerValues() {

   static int currentIndex = 0;

   if (currentIndex >= 200) {
      currentIndex = 0;
   }

   u08 leftReading = analog(LEFT_SENSOR);
   u08 rightReading = analog(RIGHT_SENSOR);

   clear_screen();

   lcd_cursor(0, 0);
   print_num(currentIndex);

   lcd_cursor(0, 1);
   print_num(leftReading);

   lcd_cursor(4, 1);
   print_num(rightReading);

   struct reading_pairs reading;

   reading.leftReading = leftReading;
   reading.rightReading = rightReading;

   pairs[currentIndex] = reading;
   currentIndex++;

   _delay_ms(10);
}

// Commands motor with speed -100 to 100
void motor(uint8_t num, int speed) {
   int val = mapInt(speed, -100, 100, 97, 157);
   if (speed == 0) {
      val = 127;
   }
   if (num == 0) {
      set_servo(0, val);
   } else {
      set_servo(1, -val);
   }
}

// Run the proportinal controller
void runController() {
   int leftReading = analog(LEFT_SENSOR) + 15;
   int rightReading = analog(RIGHT_SENSOR);

   struct motor_command command = compute_proportional(leftReading, rightReading);

   motor(LEFT_MOTOR, command.leftSpeed);
   motor(RIGHT_MOTOR, command.rightSpeed);
}

float getRand() {
   return (rand() % 100) / 100.0f;
}

// Get starter inputs, set random weights and compute inital output
void initHiddens() {
   for (int i = 0; i < 3; i++) {
      hiddenN[i].in1 = mapFloat(pairs[0].leftReading, 0, 255, 0, 1.0);
      hiddenN[i].in2 = mapFloat(pairs[0].rightReading, 0, 255, 0, 1.0);
      hiddenN[i].w1 = getRand();
      hiddenN[i].w2 = getRand();
      hiddenN[i].bias = getRand();

      float sum = (hiddenN[i].in1 * hiddenN[i].w1) + (hiddenN[i].in2 * hiddenN[i].w2) + hiddenN[i].bias;
      hiddenN[i].out = 1.0 / (1.0 + exp(-sum));
   }
}

// Get starter inputs, set random weights and compute inital output
void initOuts() {
   for (int i = 0; i < 2; i++) {
      outputN[i].in1 = hiddenN[0].out;
      outputN[i].in2 = hiddenN[1].out;
      outputN[i].in3 = hiddenN[2].out;
      outputN[i].w1 = getRand();
      outputN[i].w2 = getRand();
      outputN[i].w3 = getRand();
      outputN[i].bias = getRand();

      float sum = (outputN[i].in1 * outputN[i].w1) +
                  (outputN[i].in2 * outputN[i].w2) +
                  (outputN[i].in3 * outputN[i].w3) +
                  outputN[i].bias;
      outputN[i].out = 1.0 / (1.0 + exp(-sum));
   }
}

struct motor_command compute_neural_network(u08 in1, u08 in2) {

   float leftReading = mapFloat(in1, 0, 255, 0, 1.0);
   float rightReading = mapFloat(in2, 0, 255, 0, 1.0);

   for (int i = 0; i < 3; i++) {
      hiddenN[i].in1 = leftReading;
      hiddenN[i].in2 = rightReading;
      float sum = (hiddenN[i].in1 * hiddenN[i].w1) + (hiddenN[i].in2 * hiddenN[i].w2) + hiddenN[i].bias;
      hiddenN[i].out = 1.0 / (1.0 + exp(-sum));
   }

   for (int i = 0; i < 2; i++) {
      outputN[i].in1 = hiddenN[0].out;
      outputN[i].in2 = hiddenN[1].out;
      outputN[i].in3 = hiddenN[2].out;

      float sum = (outputN[i].in1 * outputN[i].w1) +
                  (outputN[i].in2 * outputN[i].w2) +
                  (outputN[i].in3 * outputN[i].w3) +
                  outputN[i].bias;
      outputN[i].out = 1.0 / (1.0 + exp(-sum));
   }

   struct motor_command command;

   command.leftSpeed = mapFloat(outputN[0].out, 0.0f, 1.0f, 0, 100);
   command.rightSpeed = mapFloat(outputN[1].out, 0.0f, 1.0f, 0, 100);
   return command;
}

void train_neural_network(int epochs) {
   for (int e = 0; e < epochs; e++) {
      for (int p = 0; p < 200; p++) {
         // Forward pass: compute hidden layer
         u08 in1 = pairs[p].leftReading;
         u08 in2 = pairs[p].rightReading;
         compute_neural_network(in1, in2);

         struct motor_command expected = compute_proportional(in1, in2);

         float target_left = mapFloat(expected.leftSpeed, 0, 100, 0.0f, 1.0f);
         float target_right = mapFloat(expected.rightSpeed, 0, 100, 0.0f, 1.0f);

         float target[2] = {target_left, target_right};

         // Backpropagation for output layer
         for (int i = 0; i < 2; i++) {
            // TODO: Store old weights so we can access for hidden

            oldOutWeights[i * 3] = outputN[i].w1;
            oldOutWeights[i * 3 + 1] = outputN[i].w2;
            oldOutWeights[i * 3 + 2] = outputN[i].w3;

            float error = outputN[i].out - target[i];
            float sigmoid = outputN[i].out * (1 - outputN[i].out);

            outCalcs[i] = error * sigmoid;

            // Update weights and bias
            outputN[i].w1 -= learningRate * outCalcs[i] * outputN[i].in1;
            outputN[i].w2 -= learningRate * outCalcs[i] * outputN[i].in2;
            outputN[i].w3 -= learningRate * outCalcs[i] * outputN[i].in3;
            outputN[i].bias -= learningRate * outCalcs[i];
         }

         // Backpropagation for hidden layer
         for (int i = 0; i < 3; i++) {

            float error = 0.0;
            // Sum of gradients from output layer
            error += outCalcs[0] * oldOutWeights[i];
            error += outCalcs[1] * oldOutWeights[i + 3];

            float sigmoid = hiddenN[i].out * (1 - hiddenN[i].out);

            // Update weights and bias for hidden neuron
            hiddenN[i].w1 -= learningRate * error * sigmoid * in1;
            hiddenN[i].w2 -= learningRate * error * sigmoid * in2;
            hiddenN[i].bias -= learningRate * error * sigmoid;
         }
      }

      learningRate -= 0.002;
   }
}

int main(void) {
   init(); // initialize board hardware
   set_servo(0, 127);
   set_servo(1, 127);

   clear_screen();
   print_string("Proportinal");
   while (!get_btn()) {
      runController();
   }
   _delay_ms(1000);

   clear_screen();
   print_string("Data");
   while (!get_btn()) {
      captureControllerValues();
   }

   _delay_ms(1000);

   clear_screen();
   print_string("Training");
   initHiddens();
   initOuts();

   int epochs = 50;
   train_neural_network(epochs);

   clear_screen();
   print_string("Done");
   while (!get_btn())
      ;

   _delay_ms(1000);
   while (1) {

      u08 leftReading = analog(LEFT_SENSOR);
      u08 rigthReading = analog(RIGHT_SENSOR);

      struct motor_command command = compute_neural_network(leftReading, rigthReading);

      clear_screen();
      print_num(command.leftSpeed);
      lcd_cursor(0, 1);
      print_num(command.rightSpeed);

      motor(LEFT_MOTOR, command.leftSpeed);
      motor(RIGHT_MOTOR, command.rightSpeed);
   }
}
