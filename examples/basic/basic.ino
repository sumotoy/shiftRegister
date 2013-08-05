#include <shiftRegister.h>

#define DATA_PIN   5
#define LATCH_PIN  6
#define CLOCK_PIN  7
#define SR_CHIPS   2

//instantiate
shiftRegister sr;

void setup()
{
  Serial.begin(38400);
  Serial.println("start");
  if (sr.initSR(DATA_PIN,LATCH_PIN,CLOCK_PIN,SR_CHIPS,_74XX595)){
      //good! chip ready
  } else {
      //ARGH! something wrong!
      //Serial.println("ouch");
  }
  
  for (byte i=0;i<16;i++){
      sr.srDigitalWrite(i,HIGH);
      sr.srSend();
      delay(100);
  }
  for (byte i=0;i<16;i++){
      sr.srDigitalWrite(i,LOW);
      sr.srSend();
      delay(100);
  }
}

void loop()
{
  //this will set the byte of the selected chip
  //sr.srSet(B11111111,1);
  //sr.srSet(B11111111,2);
  sr.srPort(0b1111111111111111);
  //this send to chip
  sr.srSend();
  delay(50);
  //sr.srSet(B10101010,1);
  //sr.srSet(B10101010,2);
  
  sr.srPort(0b1111111100000000);
  sr.srSend();
  delay(50);
  //sr.srSet(B01010101,1);
  //sr.srSet(B01010101,2);
  sr.srPort(0b0000000011111111);
  sr.srSend();
  delay(50);
  //sr.srSet(B00000000,1);
  //sr.srSet(B00000000,2);
  sr.srPort(0b0000000000000000);
  sr.srSend();
  delay(50);
}

