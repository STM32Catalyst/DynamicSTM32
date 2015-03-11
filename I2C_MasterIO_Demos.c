
#include "SebEngine.h"

// These are the demos using MasterIO cells. If you don't want this demo, exclude it from the project build

//===================================================================
// translated example with sequencer scheme
static I2C_MasterIO_t gMIO;
static IO_Pin_t MIO_SDA, MIO_SCL;
static Timer_t Timer;
// create the instructions
static u32 SubAdr = 0x80;
static u8 SensorRegs[64];

OneJob_t I2C_StartWrite_LPS25H = { sq_I2C_MIO_StartJob, (u32)&gMIO, 0xBA };
OneJob_t I2C_Write_LPS25H = { sq_I2C_MIO_MoveJob, (u32)&gMIO, (u32)&SubAdr, 1, 1 }; // more coming, don't stop bit
OneJob_t I2C_StartRead_LPS25H = { sq_I2C_MIO_StartJob, (u32)&gMIO, 0xBB };
OneJob_t I2C_Read_LPS25H = { sq_I2C_MIO_MoveJob, (u32)&gMIO,  (u32)&SensorRegs, 64, 0 };

static u32 List[100]; // list of pointers
static StuffsArtery_t mySequence;

void I2C_MasterIO_Test(void) {

//  u32 u = (u32)&gMIO;
  MCUInitClocks();
  
  gMIO.SDA = IO_PinInit(&MIO_SDA, PH7 ); // Initialize some quick pointers
  gMIO.SCL = IO_PinInit(&MIO_SCL, PH8 ); // Initialize some quick pointers

  NewTimer_us(&Timer, TIM6, 1, GetMCUClockTree()); // usec countdown
  gMIO.Timer = &Timer;
  gMIO.Cn = 0; // use Countdown[0]
  
  NewI2C_MasterIO(&gMIO);
  SetI2C_MasterIO_Timings(&gMIO, 400*1000, GetMCUClockTree() );
  
  // all zero: no action
  NVIC_TimersEnable(ENABLE);

  //===============
  StuffsArtery_t* P = &mySequence; // program
  NewSA(P, (u32)&List[0], countof(List));  
  gMIO.SA = P;
  
  while(1) {  
    
    AddToSA(P, (u32) &I2C_StartWrite_LPS25H);
    AddToSA(P, (u32) &I2C_Write_LPS25H);
    AddToSA(P, (u32) &I2C_StartRead_LPS25H);
    AddToSA(P, (u32) &I2C_Read_LPS25H);
    StartJobToDoInForeground((u32)P);
    
    while(P->FlagEmptied==0);
    NOPs(1);
  
  };

};
