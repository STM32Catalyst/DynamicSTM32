
#include "SebEngine.h"
#include "myChappie.h"

// This will be for now the white board, where we cook (at compile time for now) the universe of our body limit
// Here we are going to create all the entities that we support in the STM32F437 as playground. We stick to the spec's names as much as possible
// Everything we create starts with my* prefix and the corresponding pointer without it.

//static I2C_MasterHW_t myI2C1, myI2C2, myI2C3; // Support 3 HW I2C Masters
static I2C_MasterIO_t myI2C4, myI2C5, myI2C6; // Add an IO emulated I2C Masters
static I2C_SlaveIO_t myI2CS; // one emulated I2C slave with GPIOs
static RFFE_MasterIO_t myRFFE; // one RFFE Master using GPIOs
static Rs232_t myUART1, myUART2, myUART3, myUART4, myUART5, myUART6, myUART7, myUART8; // we are going to use all 8 USART/UART as RS232 mode
static ADC_t myADC1, myADC2, myADC3; // 3 ADC SAR type in the chip.
//static MCUClocks_t myClocks; // This will contain the internal clock tree informations
static DAC_t myDAC1, myDAC2; // 2 HW DACs available
static Timer_t myTimer1, myTimer2, myTimer3, myTimer4, myTimer5, myTimer6, myTimer7, myTimer8, myTimer9, myTimer10, myTimer11, myTimer12, myTimer13, myTimer14, myTimer15, myTimer16; // 16 HW timers.
static SPI_MasterHW_t mySPI1, mySPI2, mySPI3, mySPI4, mySPI5, mySPI6; // 6 HW SPI, 1,4,5,6 are higher speed capable SPIs
static SPI_MasterIO_t mySPI7, mySPI8, mySPI9, mySPI10; // these are IO emulated SPIs.

// higher level devices
static Buttons_t myButtons; // The rock 5 way switch and extra button (like Boot pin could be used as well in normal mode after booting)
static NHD_LCD16x2_t myLCD; // a 16x2 char LCD Low power display, which relies on SPI master for communication

// we should create a mega structure which groups all this...
//MCUClocks_t* GetMCUClockTree(void) // unique global resource
void Conception(void) {
  
}

void Birth(void) {
  MCUInitClocks(); // breathing starts
}

void Disintegration(void) {
  
}


// === workbench to create new items below =====>
// Tasks can be added by tasks
// Tasks can be running in background or immediate
// For now we don't add up the reshuffling of the tasks, or the null task... (empty slot)
// conception of the main loop job scheduler... and later rescheduler...

#if 0
static OneJob_t I2C_StartWrite_LPS25H = { sq_I2C_MIO_StartJob, (u32)&gMIO, 0xBA };
static OneJob_t I2C_Write_LPS25H = { sq_I2C_MIO_MoveJob, (u32)&gMIO, (u32)&SubAdr, 1, 1 }; // more coming, don't stop bit
static OneJob_t I2C_StartRead_LPS25H = { sq_I2C_MIO_StartJob, (u32)&gMIO, 0xBB };
static OneJob_t I2C_Read_LPS25H = { sq_I2C_MIO_MoveJob, (u32)&gMIO,  (u32)&SensorRegs, 64, 0 };

static u32 List[100]; // list of pointers
static StuffsArtery_t mySequence;

void Chappie_Test(void) {

//  u32 u = (u32)&gMIO;
  MCUInitClocks();
  
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
#endif