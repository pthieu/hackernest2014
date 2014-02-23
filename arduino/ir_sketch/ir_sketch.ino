#include <Streaming.h>
#include <IRremote.h>
#define SOP '<'
#define EOP '>'
bool started = false;
bool ended = false;
bool record = false;
bool transmit = false;
unsigned int inData[64];
byte index;

typedef unsigned long dword;
dword prev = 0;

IRsend irsend;
decode_results results;

//instantiate a IRrecv object to receive signals on pin 11
IRrecv irrecv(11);

unsigned int inDataR[67] = {8900,4350,650,500,600,500,650,400,650,450,800,300,700,450,650,450,600,500,650,1600,600,1600,550,1650,550,1650,650,1600,600,1550,650,1600,600,1600,550,550,600,500,650,450,700,1550,600,500,600,500,650,450,650,450,550,1650,600,1550,750,1500,650,500,600,1650,550,1600,600,1650,650,1550,750};
unsigned int inDataG[67] = {8900,4400,650,450,650,450,600,500,600,500,600,500,650,500,600,450,600,500,650,1550,750,1450,700,1550,550,1600,700,1550,650,1550,650,1550,600,1600,700,1550,550,550,650,450,550,1650,600,500,600,550,600,450,700,400,650,500,650,1500,700,1550,650,450,600,1600,650,1550,700,1500,700,1550,650};
unsigned int inDataB[67] = {9000,4300,650,450,550,550,600,450,650,500,600,500,600,500,550,550,650,400,700,1500,750,1500,650,1550,700,1550,550,1650,550,1550,700,1550,750,1500,600,500,650,1550,650,450,650,1550,650,450,600,550,550,500,650,500,550,1650,600,500,650,1550,600,500,650,1550,900,1300,650,1550,600,1600,600};

//unsigned int inDataR[66] = {4250,650,500,650,500,500,550,600,500,650,500,500,600,550,550,550,550,550,1650,550,1650,600,1600,650,1600,600,1600,600,1600,650,1600,600,1600,600,500,650,1550,600,500,650,1600,500,550,650,500,500,550,600,550,550,1650,550,550,550,1650,600,550,550,1650,800,1400,600,1600,650,1600,650};
//unsigned int inDataG[66] = {4250,650,500,650,500,500,550,600,500,650,500,500,600,550,550,550,550,550,1650,550,1650,600,1600,650,1600,600,1600,600,1600,650,1600,600,1600,600,500,650,1550,600,500,650,1600,500,550,650,500,500,550,600,550,550,1650,550,550,550,1650,600,550,550,1650,800,1400,600,1600,650,1600,650};
//unsigned int inDataB[66] = {4250,650,500,650,500,500,550,600,500,650,500,500,600,550,550,550,550,550,1650,550,1650,600,1600,650,1600,600,1600,600,1600,650,1600,600,1600,600,500,650,1550,600,500,650,1600,500,550,650,500,500,550,600,550,550,1650,550,550,550,1650,600,550,550,1650,800,1400,600,1600,650,1600,650};
 

void setup(){
  //enable IR signal receiving
  irrecv.enableIRIn();
  //make the LED on pin 13 blink when receiving an IR signal
  irrecv.blink13(1);
  //enable Serial communications
  Serial.begin(9600);
}
void loop()
{
  // Read all serial data available, as fast as possible
  while(Serial.available() > 0)
  {
    char inChar = Serial.read();
    if(inChar == SOP)
    {
       index = 0;
       inData[index] = 0;
       started = true;
       ended = false;
    }
    else if(inChar == EOP)
    {
       ended = true;
       if(record == false && transmit == false)
       {
         if(inData[index-1] == 49)        //<1> -> record signal from devices
         { 
           index = 0;
           inData[index] = 0;
           record = true;
           transmit = false;
         }
         else if(inData[index-1] == 50)   //<2> -> send command to devices
         {
           index = 0;
           inData[index] = 0;
           record = false;
           transmit = true;
         }
       }
       else
       {
         //send -> make pin10 to send IR signal
         if(transmit == true)
         {
           //transmitRaw(&results);
           if(inData[index-1] == 55)
           {
             irsend.sendRaw(inDataR, 67, 38);   
           }
           else if(inData[index-1] == 56)
           {
             irsend.sendRaw(inDataG, 67, 38);
           }
           else if(inData[index-1] == 57)
           {
             irsend.sendRaw(inDataB, 67, 38);
           }
           transmit = false;
         }
         else if(record == true)
         {
           record = false;  
         }          
      }
      break;
    }
    else
    {
      if(index < 64)
      {
        inData[index] = inChar;
        index++;
        inData[index] = 0;
      }
    }
  }
     
  // We are here either because all pending serial
  // data has been read OR because an end of
  // packet marker arrived. Which is it?
  if(started && ended)
  {
    // The end of packet marker arrived. Process the packet

    // Reset for the next packet
    started = false;
    ended = false;
    index = 0;
    inData[index] = '\0';
  }
  
  //if an IR signal was received...
  if(irrecv.decode(&results)){
  //print the protocol the IR signal used
    switch(results.decode_type){
    case UNKNOWN:
      Serial << "UNKNOWN: ";
      break;
    case SONY:
     Serial << "SONY: ";
      break;
    case NEC:
      Serial << "NEC: ";
      break;
    case RC5:
      Serial << "RC5: ";
      break;
    case RC6:
      Serial << "RC6: ";
      break;
    }
    //print the decoded hexadecimal number
    Serial << "0x" << _HEX(results.value) << endl;
    
    //dump(&results);
    //restart the receiver
    irrecv.resume();
  }
  
  //irsend.sendRaw(inDataG, 67, 38);
  //delay(500);
  //irsend.sendRaw(inDataB, 67, 38);
  //delay(500);
  //irsend.sendRaw(inDataR, 67, 38);
}

void printCode(volatile word* code, byte length){
  for(byte i = 0; i < length; i++){
    //Serial << "len[" << i << "i]" << endl; 
    if (i & 1) Serial << code[i] << ", ";
    else if (!(i & 1) && i < length - 1) Serial << code[i] << "," << endl;
    else if (!(i & 1) && i == length - 1) Serial << code[i] << endl;
  }
}

void dump(decode_results *results) {
  int count = results->rawlen;
  //Serial.print(results->value, HEX);
  //Serial.print(" (");
  //Serial.print(results->bits, DEC);
  //Serial.println(" bits)");
  //Serial.print("Raw (");
  Serial.print(count, DEC);
  //Serial.print(" : ");
  
  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(",");
  }
  Serial.println("");
}  
