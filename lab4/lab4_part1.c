/*
Name:  Jack Young and Enrique Murillo

Assignment Lab 4 Part 1

Description: Traversal using encoders
*/

#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define LEFT_SENSOR 5
#define LEFT_MOTOR 0
#define RIGHT_SENSOR 4
#define RIGHT_MOTOR 1
#define BASE_SPEED 20

#define TRAVERSE_EDGE_TICKS 107
#define TURN_90_TICKS 19
#define TRAVERSE_DIAGONAL_TICKS 151
#define TURN_45_TICKS 30

u08 shape = 0;
u08 buttonCode = 0;

void init_encoder() {

   // enable encoder interrupts

   EIMSK = 0;

   EIMSK |= _BV(PCIE1) | _BV(PCIE0);

   PCMSK1 |= _BV(PCINT13); // PB5 - digital 5

   PCMSK0 |= _BV(PCINT6); // PE6 - digital 4

   // enable pullups

   PORTE |= _BV(PE6);

   PORTB |= _BV(PB5);
}

volatile uint16_t left_encoder = 0;

volatile uint16_t right_encoder = 0;

ISR(PCINT0_vect) {

   left_encoder++; // increment left encoder
}

ISR(PCINT1_vect) {

   right_encoder++; // increment right encoder
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

void rightTurn90() {
   left_encoder = 0;
   right_encoder = 0;
   while ((left_encoder < TURN_90_TICKS) && (right_encoder < TURN_90_TICKS)) {
      motor(LEFT_MOTOR, BASE_SPEED);
      motor(RIGHT_MOTOR, -BASE_SPEED);
   }
   motor(LEFT_MOTOR, 0);
   motor(RIGHT_MOTOR, 0);
   _delay_ms(50);
}

void rightTurn135() {
   left_encoder = 0;
   right_encoder = 0;
   while ((left_encoder < TURN_45_TICKS - 2) && (right_encoder < TURN_45_TICKS - 2)) {
      motor(LEFT_MOTOR, BASE_SPEED);
      motor(RIGHT_MOTOR, -BASE_SPEED);
   }
   motor(LEFT_MOTOR, 0);
   motor(RIGHT_MOTOR, 0);
   _delay_ms(50);
}

void leftTurn135() {
   left_encoder = 0;
   right_encoder = 0;
   while ((left_encoder < TURN_45_TICKS + 20) && (right_encoder < TURN_45_TICKS + 20)) {
      motor(LEFT_MOTOR, -BASE_SPEED);
      motor(RIGHT_MOTOR, BASE_SPEED);
   }
   motor(LEFT_MOTOR, 0);
   motor(RIGHT_MOTOR, 0);
   _delay_ms(50);
}

void straightEdge() {
   left_encoder = 0;
   right_encoder = 0;
   while ((left_encoder < TRAVERSE_EDGE_TICKS) && (right_encoder < TRAVERSE_EDGE_TICKS)) {
      motor(LEFT_MOTOR, BASE_SPEED + 1);
      motor(RIGHT_MOTOR, BASE_SPEED + 4);
   }
   motor(LEFT_MOTOR, 0);
   motor(RIGHT_MOTOR, 0);
   _delay_ms(50);
}

void traverseDiagonal() {
   left_encoder = 0;
   right_encoder = 0;
   while ((left_encoder < TRAVERSE_DIAGONAL_TICKS) && (right_encoder < TRAVERSE_DIAGONAL_TICKS)) {
      motor(LEFT_MOTOR, BASE_SPEED + 1);
      motor(RIGHT_MOTOR, BASE_SPEED + 4);
   }
   motor(LEFT_MOTOR, 0);
   motor(RIGHT_MOTOR, 0);
   _delay_ms(50);
}

void traverseSquare() {
   for (int i = 0; i < 4; i++) {
      straightEdge();
      rightTurn90();
   }
}

void traverseBowtie() {
   straightEdge();
   rightTurn135();
   traverseDiagonal();
   leftTurn135();
   straightEdge();
   leftTurn135();
   traverseDiagonal();
   rightTurn135();
}

u08 buttonController() {
   if (!get_btn()) {
      return 0;
   }
   _delay_ms(400);
   if (get_btn()) {
      return 1;
   }

   shape = !shape;
   if (shape) {
      clear_screen();
      print_string("Bowtie");
   } else {
      clear_screen();
      print_string("Square");
   }
   return 0;
}

int main(void) {
   init(); // initialize board hardware
   set_servo(0, 127);
   set_servo(1, 127);
   clear_screen();
   init_encoder();

   // while (1) {
   //    set_servo(0, 127);
   //    set_servo(1, 127);
   // }

   // left_encoder = 0;
   // right_encoder = 0;
   // while (1) {
   //    lcd_cursor(0, 0);
   //    print_num(left_encoder);
   //    lcd_cursor(0, 1);
   //    print_num(right_encoder);
   //    _delay_ms(100);
   // }

   //_delay_ms(200);

   while (1) {
      while (!buttonController())
         ;
      if (shape) {
         traverseBowtie();
      } else {
         traverseSquare();
      }
   }
}
