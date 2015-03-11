
#include "SebEngine.h"

static NHD_LCD16x2_t LCD;
static SPI_MasterIO_t LCD_SPI;
static IO_Pin_t NRST;
static IO_Pin_t RS;
static IO_Pin_t CS;
static IO_Pin_t SI;
static IO_Pin_t SCL;
static Timer_t Timer;

static u32 JobList[100];
static StuffsArtery_t myLCDSequence;

u32 NHD_LCD16x2Test(void) {
  
  u32 i;
  
  MCUInitClocks();
  
  // Declare all pins first
  LCD.NRST = IO_PinInit(&NRST, PE2);
  LCD.RS = IO_PinInit(&RS, PE3);
  LCD_SPI.NSSs[0] = IO_PinInit(&CS, PE4);
  LCD_SPI.SCK = IO_PinInit(&SCL, PE5);
  LCD_SPI.MOSI = LCD_SPI.MISO = IO_PinInit(&SI, PE6);
  
  // Use the SPI for the LCD
  LCD.MIO = &LCD_SPI;
  
  // Declare the Timer
  NewTimer_us(&Timer, TIM6, 100, GetMCUClockTree());// 100us tick period
  NVIC_TimersEnable(ENABLE);
  
  // Use Timer for LCD and SPI (2 channels used)
//  LCD.Timer = &Timer;
  LCD_SPI.Timer = &Timer;
  
  
  // Declare the Sequence (Job FIFO List)
  StuffsArtery_t* SA = &myLCDSequence; // program
  NewSA(SA, (u32)&JobList[0], countof(JobList));    
  
  // Use Sequence for LCD
  LCD.SA = SA;
  
  // Initialize the SPI (done by LCD)
  
  // Initialize the Timer (Done by Declaring it)
  
  // Initialize the LCD
  NewNHD_LCD16x2(&LCD);
  SetNHD_LCD16x2_Timings(&LCD);
  
  // that's it! Now we need to control the LCD, by initializing/reset and send a message with delays on it.
  NHD_LCD16x2_Init(&LCD);
  
  LCD.FullFrameRefreshRate = 2;
  LCD.FrameCountdown = 0;

  i = 10000;
  while(1) {
    NHD_ClearRAM(&LCD);
    NHD_LCD_GO_Home(&LCD);
    SebPrintf(&LCD.Print, "Hello\tworld!\n%d %4X", (u32)i, (u32)i);
    NHD_LCD16x2_UpdateDisplay(&LCD);
    i++;
    
    
  }

  
  return 0;
}



