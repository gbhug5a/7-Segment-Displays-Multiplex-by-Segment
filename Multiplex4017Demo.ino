/*
This sketch demonstrates multiplexing two 7-segment displays - by segment instead
of by digit - using an external decoder chip for the segment lines - the 74HC4017.
This is otherwise the same as the basic Demo sketch. The 4017 works only for
common cathode displays. The schematic is also found in this repository.
*/

const byte SEGMENTS  = 7;                  //Number of segments. 8 if using DP
const byte DISPLAYS  = 2;                  //Number of displays used - two here

   // Define the pin used to clock the 74HC4017

const byte CLOCK4017pin = 8;

   // Define pins used by common cathodes - add others as needed

const byte CC0pin  = 3;
const byte CC1pin  = 6;

   // Arrays allow using any number of DISPLAYS - add others as needed

byte  CCpin[] = {CC0pin,CC1pin};           //The digit's CC driver pin number
byte  DIGIT[DISPLAYS];                     //And its displayed character

   // Use these defs for common cathode displays - LOW to sink current

const byte CCON    = LOW;
const byte CCOFF   = HIGH;

   // The bit value of each segment

const byte segA  = bit(0);
const byte segB  = bit(1);
const byte segC  = bit(2);
const byte segD  = bit(3);
const byte segE  = bit(4);
const byte segF  = bit(5);
const byte segG  = bit(6);
//const byte segH  = bit(7);               //if the decimal point if used

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

byte SEGCOUNT;                             //Segment counter - zero to six (or seven)
byte CURSEG;                               //Current segment bit position

byte i;
int TEMP = LOW;

void setup() {

  for(i = 0; i < DISPLAYS; ++i) {          //Initialize all CC pins to OUTPUT, OFF
    pinMode(CCpin[i],OUTPUT);
    digitalWrite(CCpin[i],CCOFF);          //Set displayed characters to "0"
    DIGIT[i] = char0;
  }

     //Clock the 4017 once to establish a stable clock line

  pinMode(CLOCK4017pin, OUTPUT);           //Set up 4017 Clock driver pin - OUTPUT
  digitalWrite(CLOCK4017pin,LOW);          //clock the 4017 once
  digitalWrite(CLOCK4017pin,HIGH);         //clock occurs on rising edge

     //Clock the 4017 until the segment A output goes HIGH

  while (TEMP == LOW) {
    pinMode(CLOCK4017pin,INPUT);           //change port to INPUT (floating)
    TEMP = digitalRead(CLOCK4017pin);      //read state of Q0 (and Clock)
    digitalWrite(CLOCK4017pin,HIGH);       //must do this before changing back to output
    pinMode(CLOCK4017pin,OUTPUT);          //switching back to OUTPUT
                                           //     clocks the 4017 if Q0 was LOW
                                           //once Q0 reads HIGH, we're done - segment A ON
  }

     // Initialize so everything else also points to segment A

  SEGCOUNT   = 0;                          //Segments counter - set to segment A
  CURSEG     = bit(0);                     //Bit position of segment A

  PREVmillis = millis();

     // Below for demo purposes - normally to display a VALUE 0-9, it would be
     //     DIGIT[i] = charArray[VALUE]

  DIGIT[0]    = char2;                     //Current segment pattern of Digit0 "2"
  DIGIT[1]    = char4;                     //Current segment pattern of Digit1 "4"

}


void loop() {

  CURmillis = millis();
  if ((CURmillis - PREVmillis) > 1) {    // 2ms refresh period = 71 Hz per segment

    PREVmillis = CURmillis;

       //This section selects the next segment

    CURSEG     = CURSEG << 1;            //shift to next bit position
    SEGCOUNT  += 1;                      //and increment the count
    if (SEGCOUNT == SEGMENTS) {          //if done with last segment, start over
      SEGCOUNT = 0;                      //re-initialize
      CURSEG   = bit(0);
    }

       //Turn the CC pins OFF while making changes - prevents ghosting

    for(i = 0;i < DISPLAYS; ++i) digitalWrite(CCpin[i],CCOFF);

       //Execute one clock pulse on the 4017 - shift output to next segment

    digitalWrite(CLOCK4017pin,LOW);
    digitalWrite(CLOCK4017pin,HIGH);

       //This section turns the CC pins back ON per the patterns of the characters
       //If the CURSEG bit of the DIGIT[n] segment pattern is 1, turn on the CCpin[n]

    for(i = 0; i < DISPLAYS; ++i) {
      if (DIGIT[i] & CURSEG) digitalWrite(CCpin[i], CCON);
    }
  }
}
