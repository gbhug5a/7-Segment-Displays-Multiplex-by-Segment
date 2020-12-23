/*
This sketch demonstrates multiplexing two 7-segment displays - by segment instead
of by digit.  Seven I/O pins are used to drive the common segment lines, plus one
to drive the common anode or common cathode line of each digit. Pins need not be
consecutive. Either common anode or common cathode displays can be used (but not
mixed) by changing one set of definitions.

Since each segment line is ON only 1/7 of the time, refresh must be fairly rapid
to prevent flicker.  This example refreshes every two millis, which is a refresh
rate of about 71 Hz for each segment, but three or four millis may also work well
enough. This method will probably not work with code that uses delay() since that
function halts code execution and prevents refresh.

This example code lies within loop(), but an alternative might be to set up a
new interrupt for timer0, which is already used for millis(), and move the
refresh function into that interrupt's service routine.

Multiplexing by segment's big advantage is eliminating the need for the seven
resistors on the segment lines, and the transistors on the CA/CC lines, so that
only one resistor is needed on each CA/CC line.  Any number of digits can be
used without changing the refresh rate, but since each segment driver drives that
segment for all digits at the same time, the number of digits may be limited by
the current an I/O pin can source or sink.  Using very low-current displays, such
as the Vishay TDSR1360-IK), can help avoid that problem.
*/

const byte SEGMENTS  = 7;    //Number of segments. 8 if using decimal point
const byte DIGITS    = 2;    //Number of displays used - two in this example
const byte Refresh   = 3;    //Number of millis changes between segments 

   // Define the pins used for the common segments - need not be consecutive

const byte SEGApin = A5;
const byte SEGBpin = A4;
const byte SEGCpin = 2;
const byte SEGDpin = 4;
const byte SEGEpin = 5;
const byte SEGFpin = A3;
const byte SEGGpin = A2;

   // Array allows pins to be addressed in A-G sequence regardless of pin numbers

byte SEGARRAY[]  = {SEGApin, SEGBpin, SEGCpin, SEGDpin, SEGEpin, SEGFpin, SEGGpin};

   // Define pins used by common anodes or common cathodes - add others as needed

const byte CACC0pin  = 6;
const byte CACC1pin  = 3;

   // Array allows using any number of digits - add others as needed

byte  CACCpin[]   = {CACC0pin,CACC1pin};    //The digit's pin number
byte  DIGIT[DIGITS];                        //And its displayed character

   // Use these defs for common cathode displays

const byte SEGON     = HIGH;
const byte SEGOFF    = LOW;
const byte CACCON    = LOW;
const byte CACCOFF   = HIGH;

   // Use these defs for common anode displays

//const byte SEGON     = LOW;
//const byte SEGOFF    = HIGH;
//const byte CACCON    = HIGH;
//const byte CACCOFF   = LOW;

   // The bit value of each segment

const byte segA  = bit(0);
const byte segB  = bit(1);
const byte segC  = bit(2);
const byte segD  = bit(3);
const byte segE  = bit(4);
const byte segF  = bit(5);
const byte segG  = bit(6);
//const byte segH  = bit(7);          //the decimal point if used

   // Segment patterns of the characters "0" thru "9", plus a "10" character

const byte char0 = segA + segB + segC + segD + segE + segF;
const byte char1 = segB + segC;
const byte char2 = segA + segB + segD + segE + segG;
const byte char3 = segA + segB + segC + segD + segG;
const byte char4 = segB + segC + segF + segG;
const byte char5 = segA + segC + segD + segF + segG;
const byte char6 = segA + segC + segD + segE + segF + segG;
const byte char7 = segA + segB + segC;
const byte char8 = segA + segB + segC + segD + segE + segF + segG;
const byte char9 = segA + segB + segC + segD + segF + segG;
const byte char10  = segA + segD + segG;     //used to display "100"

   // Array links a value to its character

byte charArray[] = {char0, char1, char2, char3, char4, char5,
                    char6, char7, char8, char9, char10};

unsigned long PREVmillis;
unsigned long CURmillis;

byte SEGCOUNT;                           //Segment counter - count up to SEGMENTS value
byte CURSEG;                             //Current segment bit position
byte milliCount = 0;                     //Number of millis changes so far
byte i;

void setup() {

  for(i = 0; i < SEGMENTS; ++i) {        //Initialize segment pins to OUTPUT, off
    pinMode(SEGARRAY[i], OUTPUT);
    digitalWrite(SEGARRAY[i], SEGOFF);
  }

  for(i = 0; i < DIGITS; ++i) {          //Same for CACC pins
    pinMode(CACCpin[i],OUTPUT);
    digitalWrite(CACCpin[i],CACCOFF);
  }

  for(i=0; i < DIGITS; ++i) {            //Set all displays to "0"
    DIGIT[i] = char0;
  }

     // Initialize so first refresh will re-init everything

  SEGCOUNT   = SEGMENTS - 1;             //Segments counter - set to end
  CURSEG     = bit(SEGMENTS -1);         //Bit position of last segment

  PREVmillis = millis();

     // Below for demo purposes - normally to display a VALUE 0-9, it would be
     //     DIGIT[i] = charArray[VALUE]

  DIGIT[0]    = char8;                     //Current segment pattern of Digit0 "2"
  DIGIT[1]    = char8;                     //Current segment pattern of Digit1 "4"

}


void loop() {

  CURmillis = millis();
  if (CURmillis != PREVmillis) {
    milliCount++;
    PREVmillis = CURmillis;
  }
  if (milliCount == Refresh) {
    milliCount = 0;
  
       // Turn the current segment OFF while making changes - prevents ghosting

    digitalWrite(SEGARRAY[SEGCOUNT], SEGOFF);

       //This section selects the next segment

    CURSEG     = CURSEG << 1;            //shift left to next bit position
    SEGCOUNT++;                          //used as index into SEGARRAY
    if (SEGCOUNT == SEGMENTS) {          //if done with last segment, start over
      SEGCOUNT = 0;                      //re-initialize
      CURSEG   = 1;
    }

       //This section turns the CA or CC pins on/off per the patterns of the characters
       //If the CURSEG bit of the DIGIT[n] segment pattern is 1, turn on the CACCpin[n]

    for(i = 0; i < DIGITS; ++i) {
      if (DIGIT[i] & CURSEG) digitalWrite(CACCpin[i], CACCON);
      else digitalWrite(CACCpin[i], CACCOFF);
    }

       // Now turn the new segment driver ON

    digitalWrite(SEGARRAY[SEGCOUNT], SEGON);
  }

}
