#include "U8glib.h"

U8GLIB_ST7920_128X64_4X u8g(10);    // SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10, HW SPI

#define reed A0//pin connected to read switch

//storage variables
int reedVal;
long timer;// time between one full rotation (in ms)
int mph;
float radius = 13.5;// tire radius (in inches)
float circumference;
int kph;

int rpm;

int maxReedCounter = 10;//min time (in ms) of one rotation (for debouncing)
int reedCounter;

int gear_n = 7;

const char* gear;

void draw_main(void) {

//  u8g.setFont(u8g_font_6x10);
//  u8g.drawStr(4, 12,"Speed Km/h");

  u8g.drawLine(54,52,54,16);
  u8g.drawLine(55,52,55,16);
  u8g.drawLine(55,15,69,1);
  u8g.drawLine(56,15,70,1);
  u8g.drawLine(70,1,128,1);
  u8g.drawLine(70,2,128,2);

  
  u8g.setFont(u8g_font_6x10);
  u8g.drawStr(105,49,"Km/h");
  
     u8g.setFont(u8g_font_freedoomr25n);
  u8g.setPrintPos(64,50);
  u8g.print("25");

  
   u8g.drawLine(117,6,118,6);
   u8g.drawLine(117,7,118,7);
   u8g.setFont(u8g_font_6x12);
   u8g.drawStr(121, 15,"C");
//   u8g.drawStr(95, 15,"30");
//   u8g.drawLine(106,14,107,14);
//   u8g.drawLine(106,13,107,13);
//   u8g.drawStr(110, 15,"1"); 
u8g.setFont(u8g_font_freedoomr10r);
 u8g.drawStr(95, 18,"30");
    u8g.drawLine(109,15,110,15);
   u8g.drawLine(109,14,110,14);
   u8g.drawStr(108, 18,"1");
 for(int i = 20 ; i >=9 ; i --){ 
   u8g.drawLine(7,i,15,i);
}
u8g.drawLine(9,8,13,8);
       u8g.setFont(u8g_font_freedoomr10r);
    u8g.drawStr(16,22,"100");
     u8g.setFont(u8g_font_6x10);
      u8g.drawStr(38,19,"%");
        u8g.setFont(u8g_font_freedoomr10r);
    u8g.drawStr(5,45,"55 km");
     u8g.setFont(u8g_font_6x10);
    u8g.drawStr(22,43,"Km");
}

void setup(void) {
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
  // flip screen, if required
  u8g.setRot180(); // comment out this line if you do not want to flip the screen
  reedCounter = maxReedCounter;
  circumference = 2*3.14*radius;
  pinMode(reed, INPUT);
  pinMode(gear_n, INPUT);
  
  // TIMER SETUP- the timer interrupt allows precise timed measurements of the reed switch
  //for more info about configuration of arduino timers see http://arduino.cc/playground/Code/Timer1
  cli();//stop interrupts

  //set timer1 interrupt at 1kHz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;
  // set timer count for 1khz increments
  OCR1A = 1999;// = (1/1000) / ((1/(16*10^6))*8) - 1
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);   
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei();//allow interrupts
  //END TIMER SETUP
  
}

//
ISR(TIMER1_COMPA_vect) {//Interrupt at freq of 1kHz to measure reed switch
  reedVal = digitalRead(reed);//get val of A0
  if (reedVal){//if reed switch is closed
    if (reedCounter == 0){//min time between pulses has passed
      mph = (56.8*float(circumference))/float(timer);//calculate miles per hour
      rpm = (float(60000))/(float(timer));
      timer = 0;//reset timer
      reedCounter = maxReedCounter;//reset reedCounter
      kph = mph * 1.609344;
    }
    else{
      if (reedCounter > 0){//don't let reedCounter go negative
        //reedCounter -= 1;//decrement reedCounter
      }
    }
  }
  else{//if reed switch is open
    if (reedCounter > 0){//don't let reedCounter go negative
      reedCounter -= 1;//decrement reedCounter
    }
  }
  if (timer > 1000){
    mph = 0;
  kph = 0;//if no new pulses from reed switch- tire is still, set mph, rpm and kph to 0
  rpm=0;
  }
  else{
    timer += 1;//increment timer
  } 
}

void loop(void) {  
  u8g.firstPage();  
    do {
      draw_main();
    } while( u8g.nextPage() );
}
