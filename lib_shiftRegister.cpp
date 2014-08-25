/*
Create lib shift register
*/
#define _DEBUG_X86_
#ifdef _DEBUG_X86_
using namespace std;

#include <iostream>
#include <string>
#include <iomanip>
enum{
  PIN_DATA,
  PIN_CLOCK,
  PIN_OUTPUT,
};

enum{
  LOW,
  HIGH
};

string strPin[] =
{
  "PIN_DATA",
  "PIN_CLOCK",
  "PIN_OUTPUT"
};

void digitalWrite(int pin , int value)
{
    if (pin == PIN_DATA)
      cout <<"write pin: "<<strPin[pin]<<" value: "<<value<<endl; 
}

void delay(int d)
{
  return;
}

#endif
class ShiftRegister
{
private:
  int pinData;
  int pinClock;
  int pinOutput;
  int delayPitch;
  
  void SendPitch(int pin);
  void SendClock();
  
public:  
  ShiftRegister(int data,int clock,int output,int delayR);
  void PushData(int value);
  void ActiveOutput();
  void SetValue(char *value , int size);
};

ShiftRegister::ShiftRegister(int data,int clock,int output,int delayR=1)
{
  pinData = data;
  pinClock = clock;
  pinOutput = output;
  delayPitch = delayR;
}

void ShiftRegister::SetValue(char *value , int size)
{
  if(size > 0)
  {
    int size_offset = sizeof(*value) * 8;
    int i_octet = size/size_offset;
    int i_bin = (size  - (i_octet * (size%size_offset)))%size_offset;
    
    while(i_octet >= 0)
    {
      while(i_bin > 0)
      {
	i_bin --;
	int mask = 1 << i_bin;	
	int v = (value[i_octet]) & (1 << i_bin);
	PushData(v);
      }
      i_bin = size_offset;
      i_octet --;
    }
    ActiveOutput();
  }
}

void ShiftRegister::PushData(int value)
{
  digitalWrite(pinData,value);
  SendClock();
}

void ShiftRegister::ActiveOutput()
{
  SendPitch(pinOutput);
}

void ShiftRegister::SendPitch(int pin)
{
  digitalWrite(pin,LOW);
  digitalWrite(pin,HIGH);
  delay(delayPitch);
  digitalWrite(pin,LOW);
}

void ShiftRegister::SendClock()
{
  SendPitch(pinClock);
}

#ifdef _DEBUG_X86_
int main()
{
  char data;
  ShiftRegister R1(PIN_DATA,PIN_CLOCK,PIN_OUTPUT);
  data = 0x0F;
  R1.SetValue(&data,6);
  return 0;
}
#endif
