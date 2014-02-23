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
    //Serial << "Data coming from RPI: " << inChar << endl;
    //Serial << "started : " << started << endl;
    //Serial << "ended : " << ended << endl;
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
         //Serial << "inData[index - 1] : " << inData[index -1] << endl;
         //Serial << "index : " << index << endl; 
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
           irsend.sendRaw(inData, index, 38);
           //Serial << "Sent code length: " << index << ". Carrier frequency: " << 38 << endl;   
           transmit = false;
         }
         else if(record == true)
         {
             if (irrecv.decode(&results)) 
             {
               prev = millis();
               recordSignal();
               irrecv.resume();
             }
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
        //Serial << "inData[" << index << "] :" << inData[index] << endl; 
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
    //Serial << "results.value: " << results.value << endl;
    //print the decoded hexadecimal number
    Serial << (results.value) << endl;
    //restart the receiver
    irrecv.resume();
  }
}

void recordSignal()
{
    int len = results.rawlen - 1;
    
    for(int i = 0; i < len; i++)
    {
      if(results.rawbuf[i] > 255)
      {
        inData[i] = results.rawbuf[i];
      }
    } 
    
    //Serial << "Recorded command, length: " << len << endl;
    //Serial << "Command: " << endl;
    printCode (results.rawbuf, results.rawlen);
}

void printCode(volatile word* code, byte length){
  for(byte i = 0; i < length; i++){
    //Serial << "len[" << i << "i]" << endl; 
    if (i & 1) Serial << code[i] << ", ";
    else if (!(i & 1) && i < length - 1) Serial << code[i] << "," << endl;
    else if (!(i & 1) && i == length - 1) Serial << code[i] << endl;
  }
}

