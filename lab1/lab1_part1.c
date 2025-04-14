/*
Name:  Jack Young and Enrique Murillo

Assignment Lab 1 Part 1

Description: Smoothly fade in and out LED 1 then LED 2 on a loop
*/

#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define CYCLE 1000
#define LEVLES 1000

int main(void) {
   init(); // initialize board hardware

   while (1) {
      fadeOn(0);
      fadeOff(0);
      fadeOn(1);
      fadeOff(1);
   }

   return 0;
}

void fadeOn(u08 num) {
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(100);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(10);
      led_off(num);
      _delay_us(90);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(20);
      led_off(num);
      _delay_us(80);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(30);
      led_off(num);
      _delay_us(70);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(40);
      led_off(num);
      _delay_us(60);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(50);
      led_off(num);
      _delay_us(50);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(60);
      led_off(num);
      _delay_us(40);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(70);
      led_off(num);
      _delay_us(30);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(80);
      led_off(num);
      _delay_us(20);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(90);
      led_off(num);
      _delay_us(10);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(100);
      led_off(num);
   }
}

void fadeOff(u08 num) {
   for (int i = 0; i < CYCLE; i++) {
      led_on(num);
      _delay_us(100);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(10);
      led_on(num);
      _delay_us(90);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(20);
      led_on(num);
      _delay_us(80);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(30);
      led_on(num);
      _delay_us(70);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(40);
      led_on(num);
      _delay_us(60);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(50);
      led_on(num);
      _delay_us(50);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(60);
      led_on(num);
      _delay_us(40);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(70);
      led_on(num);
      _delay_us(30);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(80);
      led_on(num);
      _delay_us(20);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(90);
      led_on(num);
      _delay_us(10);
   }
   for (int i = 0; i < CYCLE; i++) {
      led_off(num);
      _delay_us(100);
   }
}