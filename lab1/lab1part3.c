#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h> 

int Righthit(int pinnumber){
   for(int i = 0; i < 5; i++){
      pinnumber = 8;
      // digital_dir(pinnumber, 1);
       // digital_out(pinnumber, 0);
       // delay(100);
       digital_out(pinnumber + i, 1);
       for(int i = 50; i > 0; i-- ){
         _delay_ms(1);
         i--;
      }
      digital_out(pinnumber +i , 0);
       pinnumber++;
      }
      return pinnumber;
}

int Lefthit(int pinnumber){

   pinnumber = 12;
   for(int j = 5; j > 0; j--){
      
      digital_out(pinnumber, 1);
      for(int i = 50; i > 0; i-- ){
         _delay_ms(1);
         i--;
      }
      digital_out(pinnumber, 0);
         pinnumber--;
      }
      return pinnumber;
}

void startgame(int pinnumber){
   // int pinnumber = 8;
   
   while(1){
      //Start
      int counter = 0;
      for(int i = 1000; i > 0; i--){
         
         if(get_btn() && (counter % 2 == 0)){
            digital_out(12, 0);
            if(pinnumber <= 8){
               pinnumber = 8;
            }
            counter++;
            _delay_ms(30);
            // digital_out(pinnumber, 1);
            Lefthit(pinnumber);
            digital_out(8, 1);

         }
         if(get_btn() && (counter % 2 == 1)){
            digital_out(8, 0);
            // if(get_btn()){
            //    _delay_ms(10);
            // }
            if(pinnumber >= 12)
            {
               pinnumber = 12;
            }
            counter++;
            _delay_ms(30);
            
            Righthit(pinnumber);
            digital_out(12, 1);
            i += 10;
         }
         _delay_ms(150);
      // }
      i -= 25;
      }
      break;
}
}



int main(void) {
   init();  //initialize board hardware
   int pinnumber = 8;
   digital_dir(8, 1);
   digital_dir(9, 1);
   digital_dir(10, 1);
   digital_dir(11, 1);
   digital_dir(12, 1);
   
   // if(get_btn()){
      // lcd_cursor(0,0);
      // print_num(1);
      _delay_ms(25);
      // if(get_btn()){
         startgame(pinnumber);
      lcd_cursor(0,0);
      print_num(000000);
   // }
      
   // }
   digital_out(pinnumber, 0);
   return 0;
      }
      
   
   
// #include "globals.h"
// #include <util/delay.h>
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include <stdio.h> 

// int Righthit(int pinnumber){
//    for(int i = 0; i < 5; i++){
//       pinnumber = 8;
//       digital_dir(pinnumber, 1);
//        // digital_out(pinnumber, 0);
//        // delay(100);
//        digital_out(pinnumber, 1);
//        _delay_ms(100);
//        digital_out(pinnumber, 0);
//        pinnumber++;
//       }
//       return pinnumber;
// }

// int Lefthit(int pinnumber){
//    pinnumber = 12;
//    for(int j = 5; j > 0; j--){
//       digital_out(pinnumber, 1);
//       _delay_ms(100);
//       digital_out(pinnumber, 0);
//          pinnumber--;
//       }
//       return pinnumber;
// }

// int main(void) {
//    init();  //initialize board hardware
//    int pinnumber = 8;

//    while(1){
//       //Start
//       int counter = 0;
//       for(int i = 30; i < 0; i--){
//          if(get_btn() && (counter % 2 == 0)){
//             if(pinnumber <= 8){
//                pinnumber = 8;
//             }
//             _delay_ms(25);
//             digital_out(pinnumber, 1);
//             Lefthit(pinnumber);
//             counter++;
//             break;
//          }
//          if(get_btn() && (counter % 2 == 1)){
//             if(pinnumber >= 12)
//             {
//                pinnumber = 12;
//             }
//             _delay_ms(25);
//             digital_out(pinnumber, 1);
//             Righthit(pinnumber);
//             counter++;
//             break;
//          }
//          _delay_ms(100);
//          i-=24;
//       }
//       digital_out(pinnumber, 0);
//    }
   
   
//    // clear_screen();
//    // lcd_cursor(0,0);
//    // print_num(2);
//    return 0;
// }