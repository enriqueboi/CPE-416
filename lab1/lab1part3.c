#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h> 

int Righthit(int pinnumber){
   for(int i = 0; i < 5; i++){
      pinnumber = 8
      digital_dir(pinnumber, 1);
       // digital_out(pinnumber, 0);
       // delay(100);
       digital_out(pinnumber, 1);
       _delay_ms(100);
       digital_out(pinnumber, 0);
       pinnumber++;
      }
      return pinnumber;
}

int Lefthit(int pinnumber){
   pinnumber = 12
   for(int j = 5; j > 0; j--){
      digital_out(pinnumber, 1);
      _delay_ms(100);
      digital_out(pinnumber, 0);
         pinnumber--;
      }
      return pinnumber;
}

int main(void) {
   init();  //initialize board hardware
   int pinnumber = 8;

   while(1){
      //Start
      int counter = 0;
      for(int i 30; i < 0; i-25){
         if(get_btn(1) && (counter % 2 == 0)){
            if(pinnumber <= 8){
               pinnumber = 8;
            }
            _delay_ms(25);
            digital_out(pinnumber, 1);
            Lefthit(pinnumber);
            counter++;
            break;
         }
         if(get_btn(1) && (counter % 2 == 1)){
            if(pinnumber >= 12)
            {
               pinnumber = 12;
            }
            _delay_ms(25);
            digital_out(pinnumber, 1);
            Righthit(pinnumber);
            counter++;
            break;
         }
         _delay_ms(100);
      }
      digital_out(pinnumber, 0);
   }
   
   
   // clear_screen();
   // lcd_cursor(0,0);
   // print_num(2);
   return 0;
}
