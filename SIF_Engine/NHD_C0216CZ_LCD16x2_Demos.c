
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
  
  // Declare the Timer
  NewTimer(&Timer, TIM6);
  SetTimerTimings(&Timer, 100, GetMCUClockTree());
  ConfigureTimer(&Timer);
  NVIC_TimersEnable(ENABLE);
  
  // Use Timer for LCD and SPI (2 channels used)
//  LCD.Timer = &Timer;
  LCD_SPI.Timer = &Timer;
  
  NewSPI_MasterIO(&LCD_SPI, NewIO_Pin(&SI,PE6), NewIO_Pin(&SI,PE6),NewIO_Pin(&SCL, PE5),NewIO_Pin(&CS, PE4)); 
  SetSPI_MasterIO_Timings(&LCD_SPI, 100000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB, GetMCUClockTree() ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB
  ConfigureSPI_MasterIO(&LCD_SPI);
  EnableSPI_MasterIO(&LCD_SPI);
  
  // Use the SPI for the LCD
  LCD.MIO = &LCD_SPI;  
  LCD.MIO->Cn = 1;

  // Declare the Sequence (Job FIFO List), and...
  // allocate it to LCD
  LCD.SA = NewSA(&myLCDSequence, (u32)&JobList[0], countof(JobList));
 
  // Initialize the LCD
  NewNHD_LCD16x2(&LCD, NewIO_Pin(&NRST, PE2), NewIO_Pin(&RS, PE3));
  SetNHD_LCD16x2_Timings(&LCD);
  ConfigureNHD_LCD16x2(&LCD);
  EnableNHD_LCD16x2(&LCD); // this will initialize the display
  
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



