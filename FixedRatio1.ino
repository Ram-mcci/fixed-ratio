/*
  Feeding experimentation device 3 (FED3)
  Fixed Ratio 1
  alexxai@wustl.edu
  December, 2020

  This project is released under the terms of the Creative Commons - Attribution - ShareAlike 3.0 license:
  human readable: https://creativecommons.org/licenses/by-sa/3.0/
  legal wording: https://creativecommons.org/licenses/by-sa/3.0/legalcode
  Copyright (c) 2020 Lex Kravitz
*/

#include <FED3.h>                                       //Include the FED3 library 
//#include <Catena.h>
//#include <Catena_TxBuffer.h>                            //Include Catena TxBuffer for byte conversion
#include <SoftwareSerial.h>                             //Include SoftwareSerial library

//using namespace McciCatena;

//Catena gCatena;

//TxBuffer_t b;                                          //Object for Txbuffer

extern RTC_PCF8523 rtc;
  
uint8_t buf[32];    // this sets the largest buffer size
uint8_t *p;
uint16_t au16data[16];
typedef unsigned short  uint16_t;
typedef unsigned char   uint8_t;

// set A0 pin of 4610 as Serial RX pin
SoftwareSerial mySerial(-1, A0); // RX, TX

String sketch = "FR1";                                  //Unique identifier text for each sketch
FED3 fed3 (sketch);                                     //Start the FED3 object

//RTC_PCF8523 rtc; 

void setup() {
  fed3.begin();                                         //Setup the FED3 hardware
  mySerial.begin(115200);
}


enum class SerialMessageOffset : uint8_t
{
unsigned char ID;
unsigned char ADDR_HI;
unsigned char ADDR_LO;
unsigned char BYTEC;
unsigned char calcCRC;
}

unsigned char ID = 0x01;
unsigned char ADDR_HI = 0x00;
unsigned char ADDR_LO = 0x02;
int z; 
z = 0x04;



void put(uint8_t c)
  {
  if (p < buf + sizeof(buf))
    *p++ = c;
  }

void put2(uint32_t v)
  {
  if (v > 0xFFFF)
    v = 0xFFFF;

  put((uint8_t) (v >> 8));
  put((uint8_t) v);
  }

void put2(int32_t v)
  {
  if (v < -0x8000)
    v = -0x8000;
  else if (v > 0x7FFF)
    v = 0x7FFF;

  put((uint8_t)(v >> 8));
  put((uint8_t)v);
  }

uint8_t *getp(void)
  {
  return p;
  }

std::size_t getn(void)
  {
  return p - buf;
  }

uint8_t *getbase(void)
  {
  return buf;
  }

void put2sf(float v)
  {
  int32_t iv;

  if (v > 32766.5f)
    iv = 0x7fff;
  else if (v < -32767.5f)
    iv = -0x8000;
  else
    iv = (int32_t)(v + 0.5f);

  put2(iv);
  }

void put2uf(float v)
  {
  uint32_t iv;

  if (v > 65535.5f)
    iv = 0xffff;
  else if (v < 0.5f)
    iv = 0;
  else
    iv = (uint32_t)(v + 0.5f);

  put2(iv);
  }

void put2u(int32_t v)
  {
  if (v < 0)
    v = 0;
  else if (v > 0xFFFF)
    v = 0xFFFF;
  put2((uint32_t) v);
  }

void putV(float V)
  {
  put2sf(V * 4096.0f + 0.5f);
  }

void loop() {
  fed3.run();                                           //Call fed.run at least once per loop

  if (fed3.Left) {                                      //If left poke is triggered
    fed3.logLeftPoke();                                 //Log left poke
    fed3.ConditionedStimulus();                         //Deliver conditioned stimulus (tone and lights)
    fed3.Feed();                                        //Deliver pellet
  }

  if (fed3.Right) {                                     //If right poke is triggered
    fed3.logRightPoke();                                //Log right poke
  }

  p = buf;

  DateTime now = rtc.now();
  
  putV(fed3.measuredvbat);
  put2((uint32_t)(fed3.numMotorTurns+1));
  put2u((int32_t)(fed3.FR));

  uint8_t event_active; // = (uint8_t)fed3.Event;
  //event_active = (event_active << 4) | fed3.activePoke;
  //put(event_active);

  if (fed3.Event == "LeftShort")
    event_active = 1;

  else if (fed3.Event == "Left")
    event_active = 2;

  else if (fed3.Event == "RightShort")
    event_active = 3;

  else if (fed3.Event == "Right")
    event_active = 4;

  else if (fed3.Event == "Pellet")
    event_active = 5;

  else
    event_active = 0;
  
  put(event_active);

  put(fed3.LeftCount);
  put(fed3.RightCount);
  put(fed3.PelletCount);

  put2u(fed3.BlockPelletCount);

  if (fed3.retInterval < 60000 )
    put2uf(fed3.retInterval/1000.000);

  if (fed3.Left)
    put2uf(fed3.leftInterval/1000.000);

  if (fed3.Right)
    put2uf(fed3.rightInterval/1000.000);


auto pMessage = getbase();
auto nMessage = getn();
u8Buffersize = nMessage;
au8Buffer[ unsigned(SerialMessageOffset::ID) ] = 0x01;
au8Buffer[ unsigned(SeialMessageOffset::ADDR_HI) ] = 0x00;
au8Buffer[ unsigned(SerialMessageOffset::ADDR_LO ] = 0x02;
this->putMessageCount(u8Buffersize);

au8Buffer[ unsigned(SerialMessageOffset::bytec ]    = nMessage

for (uint16_t i=0; i< nMessage; i++)
{
   au8Buffer[ z++] = pMessage++
}
au8Buffer[ unsigned(SerialMessageOffset::calcCRC] = 0x14

  
  mySerial.write(getbase(), getn());






  
}
