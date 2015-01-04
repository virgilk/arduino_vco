#include <InvertedTM1638.h>
#include <TM16XX.h>
#include <TM16XXFonts.h>
#include <TM1638.h>
#include <TM1640.h>


#include <TM1638.h>
#include <TM1640.h>

// all times in microseconds

#define TICK_TIME (5)
#define CYCLE_TIME (2500)
#define VCO_GAIN (25)
#define MAX_FREQ (65535L)
#define MIN_FREQ (1L)

unsigned long int totalTime;
unsigned long int echoTime;
volatile boolean echo;

// pins are all named XXX_p
const unsigned int Trig_p = 11; //D11
const unsigned int Echo_p = 10; //D12
//const unsigned int Debug_p = 10;//D10
//const unsigned int Debug2_p = 9; //D09
 
const unsigned int LED_p = 13; // always pin #13
//const unsigned int Freq_p = 5;//D05
unsigned long int freq;

static long int  filterloop(unsigned int input);

char buffer[8]; // our display buffer
//defines a module with data on pin 8, clock on pin 9 
//and strobe on pin 7
//TM1638 module(8, 9, 7, true, 3);

TM1638 module(8,9,7, true, 1);
//data clk strobe

void setup()
{
  
  pinMode( LED_p, OUTPUT );
  pinMode( Trig_p, OUTPUT );
  pinMode( Echo_p, INPUT );
  //pinMode( Freq_p, OUTPUT );
  //pinMode( Debug_p, OUTPUT );
  //pinMode( Debug2_p, OUTPUT );
  
  digitalWrite( Trig_p, LOW );
  digitalWrite( LED_p, LOW );
  //noTone( Freq_p );

  freq = MAX_FREQ;
}




void loop()
{
 delay(300);
  digitalWrite( Trig_p, HIGH );
  delayMicroseconds( 3 * TICK_TIME );
  digitalWrite( Trig_p, LOW );
  
  digitalWrite( LED_p, HIGH );
  echoTime = 0;
  totalTime = 1;
  //digitalWrite( Debug_p, HIGH );
 
  //do { echo = digitalRead( Echo_p );} while( echo == LOW );
  //digitalWrite( Debug_p, LOW );
 
  do
  {
    delayMicroseconds( TICK_TIME );
    echo = digitalRead( Echo_p );
    
    if ( echo == HIGH ) 
    { 
     echoTime++; 
     //digitalWrite( Debug2_p, HIGH );
    }
    //else
    //{
      //digitalWrite( LED_p, LOW );
      //digitalWrite(Debug2_p, LOW);
    //}
  } while( ++totalTime < CYCLE_TIME );
  
  freq = echoTime * VCO_GAIN;//( MAX_FREQ - (echoTime * VCO_GAIN));
  
  //freq = filterloop(freq);
  
  //if( freq > MAX_FREQ ) freq = MAX_FREQ;
  //else if ( freq < MIN_FREQ ) freq = MIN_FREQ;
 
  //delay(50);
  //tone( Freq_p, freq );

   //digitalWrite( LED_p, HIGH );
  //delay(400);
  sprintf(buffer, "%8u", freq);
  module.setDisplayToString(buffer, 0b0, 0);
  //module.setDisplayToHexNumber(freq,0b01,true);
 digitalWrite( LED_p, LOW );
 //delay(400);
   //module.setDisplayToHexNumber(0x11110000,0b10101010,true);
 //delay(100);
}
 
 
 
 
 
 /* Digital filter designed by mkfilter/mkshape/gencode   A.J. Fisher
   Command line: /www/usr/fisher/helpers/mkfilter -Bu -Lp -o 6 -a 2.5000000000e-01 0.0000000000e+00 -l */

#define NZEROS 6
#define NPOLES 6
#define GAIN   3.379723001e+01

static float xv[NZEROS+1], yv[NPOLES+1];

static long int  filterloop( const unsigned int input)
  { 
      { xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4]; xv[4] = xv[5]; xv[5] = xv[6]; 
        xv[6] = input / GAIN;
        yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4]; yv[4] = yv[5]; yv[5] = yv[6]; 
        yv[6] =   (xv[0] + xv[6]) + 6 * (xv[1] + xv[5]) + 15 * (xv[2] + xv[4])
                     + 20 * xv[3]
                     + ( -0.0017509260 * yv[0]) + ( -0.0000000000 * yv[1])
                     + ( -0.1141994251 * yv[2]) + ( -0.0000000000 * yv[3])
                     + ( -0.7776959619 * yv[4]) + (  0.0000000000 * yv[5]);
        return(yv[6]);
      }
  }

 
