
#include "SebEngine.h"

#ifdef ADD_EXAMPLES_TO_PROJECT

// This file is the example architect, like we would dynamically create a train track and let it go...
// Or a dynamically reprogrammable FPGA for the ultimate goal.

// On top, we include all the available ingredients available for the different examples.
// First step is to create the needed contexts in SRAM as global playground
// Second is to format and configure these SRAM items
// Third is to place all links and hooks properly
// Fourth is to activate all the IRQs for the system to let go.
// later the "global SRAM" could be a local variable so it could be destroyed once the job done.
// case 1:

void I2C_SlaveIO_Rs232_Test(void);
void SebExample2(void);
void Test_SPI_MasterHW(void);;
void Test_ConcurrentSPI_MasterHW(void);
void TestCode(void);
void ADC_Demo(void);

void I2C_MasterIO_Test(void);
void RFFE_Test(void);

static Timer_t Timer6_us, Timer7_ms;
//*************
vu8 choice=7; // default choice after reset
//*************
void SebMainTest(void) {
  
  MCUInitClocks();
  
  while(choice==0) ; // stop the emulator, change choice and let it run

  while(1) {
    switch(choice) {
    case 0:      return; // exit and return
    case 1:      I2C_SlaveIO_Rs232_Test();      break; // Create an I2C Spy which output its string into an RS232 Tx pin
    case 2:      SebExample2();      break;
    case 3:      Test_SPI_MasterHW();      break;
    case 4:      Test_ConcurrentSPI_MasterHW();      break;
    case 5:      RFFE_Test();      break; // Tested ok on Feb 10, 2015 using ST HVDAC device example
    case 6:      I2C_MasterIO_Test();      break;
    case 7:      NHD_LCD16x2Test();      break;
    case 8:      TestCode();      break;
    case 9:      ButtonsTest();      break;
    case 10:     Timer_T1_T2_Test();      break;
    case 11:     Timer_T3_T4_Test();      break;
    case 12:     Timer_T5_T6_Test();      break;
    case 13:     Timer_T7_T8_Test();      break;
    case 14:     Timer_T9_T10_Test();      break;
    case 15:     Timer_T11_T12_Test();      break;
    case 16:     Timer_T13_T14_Test();      break;
    case 17:     Timer_IC_OC_PWM_Test();      break;
    case 18:     DAC_Test();      break;
    case 19:     ADC_Demo();      break;
    case 20:     Clocks_Demo();      break;
    default:while(1);
    };
  };

}

// Example 1: Construct an I2C Spy and output the decoded string into RS232, also create an I2C slave which maps 256 bytes with 1 byte sub address
// Ingredients: sebNVIC, sebEXTI, I2C_SlaveIO, ByteVein, SebUART, I2C_By_GPIO(I2C master done in main loop)
// Connectivity: SDA= SCL= TX1= RX1=
// Buffers:

// to make the slave not responding, we need to change to add a "slave busy" flag which latches when bus is idle.
static I2C_SlaveIO_t mySlave; // can be multiple of them, as array (we pass by pointer)
static IO_Pin_t mySDA; // for fast pin access using bitbanding area (we will use a fake HW registers which will check if all pins are not colliding)
static IO_Pin_t mySCL; // for fast pin access using bitbanding area
// we have to cleanup the Architect function for the fPin

static u8 mySlaveAdresses[] = { 0x40, 0xB0, 0xC0, 0xD0 }; // ram is faster
static u8 mySlaveMemory[256];
static u8 myRxBuf[10];

static ByteVein_t myBV_TX;
static u8 myBV_I2CSpyToTX1[1024];

static ByteVein_t myBV_RX;
static u8 myBV_RX1ToTBD[1024];

static Rs232_t myRs232;

static IO_Pin_t myRs232RX; // for fast pin access using bitbanding area (we will use a fake HW registers which will check if all pins are not colliding)
static IO_Pin_t myRs232TX; // for fast pin access using bitbanding area

void I2C_SlaveIO_Rs232_Test(void) {

  // I2C Slave description, using IO_Pin and EXTI (could later on use Basic Timer to implement a timeout like SMBus is doing)
  // This is the data byte vein flowing from I2C Spy to UART TX, and acts as a FIFO
  NewBV(&myBV_TX, (u32)myBV_I2CSpyToTX1, sizeof(myBV_I2CSpyToTX1));
  mySlave.BV = &myBV_TX;
  
  NewI2C_SlaveIO_SDA_SCL(&mySlave, NewIO_Pin(&mySDA,PH10), NewIO_Pin(&mySCL,PH12));
  EmulateMemoryI2C_SlaveIO(&mySlave, (u8*)mySlaveAdresses, countof(mySlaveAdresses), mySlaveMemory, countof(mySlaveMemory));
  ConfigureI2C_SlaveIO(&mySlave);
  SpyI2C_SlaveIO(&mySlave); // this is to go to spy code as well
  EnableI2C_SlaveIO(&mySlave);
  
  // RS232 cell definition
  // Pin definition, RX1=PB7, TX1=PB6
  // Hook the pins to the RS232 USART (if pin not used, put null pointer)
  myRs232.RTS = 0;
  myRs232.CTS = 0;
  NewRs232HW( &myRs232, USART1, NewIO_Pin(&myRs232RX,PB7), NewIO_Pin(&myRs232TX, PB6)); // The HW connectivity, handle, register base, RX pin, TX pin, CTS pin, RTS pin, if non null.
  // we have also to initialize the BV_TX and BV_RX at higher level...
  // Initialize the Byte Vein from RX1 to fill up until full for now. (depends if you short RX/TX or not)
  NewBV(&myBV_RX, (u32)myBV_RX1ToTBD, sizeof(myBV_RX1ToTBD));

  // This will hook the BV to RS232 USART
  SetRs232BVs( &myRs232, &myBV_TX, &myBV_RX); // hook them to RS232

  // Setup the timings and enable the cell
  SetRs232Timings(&myRs232, 115200, 2, 1); // Things depending on time and internal clocks [This Enables the cell]
  
  ConfigureRs232HW(&myRs232);
  EnableRs232HW(&myRs232);
  
  // now we need to activate everything after all the architecture is done (the other end of the BV_TX, BV_RX)
  NVIC_Rs232sEnable(ENABLE);// enable interrupts for RS232
  NVIC_EXTIsEnable(ENABLE); // enable interrupts for slave cell
  //===============

  while(1) {
#if 1
    // here we will use the I2C single master to generate events...

    // can put a breakpoint here
    I2CIO_Start(0x40);  // if return FALSE, chip is not present or bus stuck
    I2CIO_Transmit(0x01);
    I2CIO_Transmit(0x12);
    I2CIO_Transmit(0x23);
    I2CIO_Transmit(0x34);
    I2CIO_Stop();

    // can put a breakpoint here
    mySlaveMemory[0] = 0x55;
    mySlaveMemory[1] = 0x66;
    mySlaveMemory[2] = 0x77;
    mySlaveMemory[3] = 0x88;
    mySlaveMemory[4] = 0x99;

    I2CIO_Start(0x40);  // if return FALSE, chip is not present or bus stuck
    I2CIO_Transmit(0x01);
    I2CIO_Start(0x41);
    myRxBuf[0] = I2CIO_Receive(1);
    myRxBuf[1] = I2CIO_Receive(1);
    myRxBuf[2] = I2CIO_Receive(0);
#endif    
    Wait_ms(100); // let it flush, here could be too fast
  }
  
}


//====----> Extra level, here the Master is also using the same concept
static Timer_t Timer;

void SebExample2(void) {
#if 0 // superseaded
  // I2C Slave description, using IO_Pin and EXTI (could later on use Basic Timer to implement a timeout like SMBus is doing)
  // This is the data byte vein flowing from I2C Spy to UART TX, and acts as a FIFO
  NewBV(&myBV_TX, (u32)myBV_I2CSpyToTX1, sizeof(myBV_I2CSpyToTX1));
  mySlave.BV = &myBV_TX;
  
  NewI2C_SlaveIO_SDA_SCL(&mySlave, NewIO_Pin(&mySDA, PH10), NewIO_Pin(&mySCL,PH12));
  EmulateMemoryI2C_SlaveIO(&mySlave, (u8*)mySlaveAdresses, countof(mySlaveAdresses), mySlaveMemory, countof(mySlaveMemory));
  ConfigureI2C_SlaveIO(&mySlave);
  SpyI2C_SlaveIO(&mySlave); // this is to go to spy code as well
  EnableI2C_SlaveIO(&mySlave);

  //====----> RS232 cell definition  
  // RS232 cell definition
  // Pin definition, RX1=PB7, TX1=PB6
  // Hook the pins to the RS232 USART (if pin not used, put null pointer)
  myRs232.RTS = 0;
  myRs232.CTS = 0;
  NewRs232HW( &myRs232, USART1, NewIO_Pin(&Rs232RX,PB7), NewIO_Pin(&Rs232TX, PB6)); // The HW connectivity, handle, register base, RX pin, TX pin, CTS pin, RTS pin, if non null.
  // we have also to initialize the BV_TX and BV_RX at higher level...
  // Initialize the Byte Vein from RX1 to fill up until full for now. (depends if you short RX/TX or not)
  NewBV(&myBV_RX, (u32)myBV_RX1ToTBD, sizeof(myBV_RX1ToTBD));

  // This will hook the BV to RS232 USART
  SetRs232BVs( &myRs232, &myBV_TX, &myBV_RX); // hook them to RS232

  // Setup the timings and enable the cell
  SetRs232Timings(&myRs232, 115200, 2, 1); // Things depending on time and internal clocks [This Enables the cell]
  
  ConfigureRs232HW(&myRs232);
  EnableRs232HW(&myRs232);
  
  //====----> Now we setup the I2C master
  NewTimer(&Timer, TIM6);
  SetTimerTimings(&Timer, 1, GetMCUClockTree());
  ConfigureTimer(&Timer);
  gMIO.Job = &Job;
  gMIO.Timer = &Timer;
  gMIO.Cn = 0; // use Countdown[0]
  
  NewI2C_MasterIO_SDA_SCK( &gMIO, NewIO_Pin(&MIO_SDA,PH7), NewIO_Pin(&MIO_SCL,PH8));
  SetI2C_MasterIO_Timings( &gMIO, 400*1000, GetMCUClockTree());
  ConfigureI2C_MasterIO(&gMIO);
  EnableI2C_MasterIO(&gMIO);
  
  // now we need to activate everything after all the architecture is done (the other end of the BV_TX, BV_RX)
  NVIC_TimersEnable(ENABLE);// used by I2C Master (#0/4)
  NVIC_Rs232sEnable(ENABLE);// enable interrupts for RS232
  NVIC_EXTIsEnable(ENABLE); // enable interrupts for slave cell
  //===============

  Job.MIO = (u32) &gMIO;
  
  while(1) {

    // here we will use the I2C single master to generate events...
    Job.Address = 0x40;    
    Job.pPreTX = PreTX_Buf;
    Job.pTX = TX_Buf;
    Job.pRX = RX_Buf;
    PreTX_Buf[0] = 0x01;
    TX_Buf[0] = 0x12;
    TX_Buf[1] = 0x23;
    TX_Buf[2] = 0x34;
    Job.PreTX_Sizeof = 1; // 1 byte sub address
    Job.TX_Sizeof = 3;
    Job.RX_Sizeof = 0;
    I2C_MasterIO_PerformJob(u); // check slave address

    // can put a breakpoint here
    mySlaveMemory[0] = 0x55;
    mySlaveMemory[1] = 0x66;
    mySlaveMemory[2] = 0x77;
    mySlaveMemory[3] = 0x88;
    mySlaveMemory[4] = 0x99;
    Job.PreTX_Sizeof = 1; // 1 byte sub address
    Job.TX_Sizeof = 0;
    Job.RX_Sizeof = 3;
    I2C_MasterIO_PerformJob(u); // check slave address

    Wait_ms(100); // let it flush, here could be too fast

  }
#endif  
}

// There are 2 schemes:
// Polling mode: No hook will cause a built-in flag to be set. It can be cleared by the main loop or RTOS.
// Time critical scheme: Put a hook which will call you back with the high priority, then cascade or raise a flag or lower the priority level.
// 
// Naming convention: Use UpperCase for first word letter, and use Underscore only when needed.




//==================------------------->
// This is the test to check if we can read fingertip chip ID
//============= SHADOW READ/WRITE ALL
const u8 Shadow_WriteAll[2] = { 0x00 };
const u8 Shadow_ReadAll[2] = { 0x80 };

//========== The shared resources for SPI6
static IO_Pin_t MISO6, MOSI6, SCK6, NSS6, INT6;
static SPI_MasterHW_t mySPI6;
static u8 A_ShadowRegs[256];
const OneJob_t SPI6_Start = { sq_SPI_MHW_StartJob, {(u32)&mySPI6, 1} };
const OneJob_t SPI6_ShadowReadAt00 = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI6, (u32)&Shadow_ReadAll[0], 0, 1} };
const OneJob_t SPI6_Dummy = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI6, 0, 0, 1} };
const OneJob_t SPI6_ReadAllShadow = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI6, 0, (u32)&A_ShadowRegs[0], sizeof(A_ShadowRegs)} };
const OneJob_t SPI6_Stop = { sq_SPI_MHW_StopJob, {(u32)&mySPI6, 1 }};

const OneJob_t SPI6_Disarm = {sq_SPI_MHW_DMA_Interrupt, {(u32)&mySPI6, DISABLE }};
const OneJob_t SPI6_Arm = {sq_SPI_MHW_DMA_Interrupt, {(u32)&mySPI6, ENABLE }};

//const OneJob_t SPI6_Disarm = { sq_SPI_MHW_StopJob, {(u32)&mySPI6, 0 }}; // disturb SPI pins AF
//const OneJob_t SPI6_Arm = { sq_SPI_MHW_StartJob, {(u32)&mySPI6, 1} }; // disturb SPI pins AF

const OneJob_t SPI6_Wait_50us = { sq_ArmTimerCountdown, {(u32)&Timer6_us, 3, 5 } };
const OneJob_t SPI6_Wait_5ms =  { sq_ArmTimerCountdown, {(u32)&Timer7_ms, 3, 5 } };

const OneJob_t INT6_Arm = { sq_EXTI_Interrupt, {(u32) &INT6, ENABLE }};
const OneJob_t INT6_Disarm = { sq_EXTI_Interrupt, {(u32) &INT6, DISABLE }};

//========== The shared resources for SPI5
static IO_Pin_t MISO5, MOSI5, SCK5, NSS5, INT5;
static SPI_MasterHW_t mySPI5;
static u8 B_ShadowRegs[256];
const OneJob_t SPI5_Start = { sq_SPI_MHW_StartJob, {(u32)&mySPI5, 1} };
const OneJob_t SPI5_ShadowReadAt00 = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI5, (u32)&Shadow_ReadAll[0], 0, 1} };
const OneJob_t SPI5_Dummy = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI5, 0, 0, 1} };
const OneJob_t SPI5_ReadAllShadow = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI5, 0, (u32)&B_ShadowRegs[0], sizeof(B_ShadowRegs)} };
const OneJob_t SPI5_Stop = { sq_SPI_MHW_StopJob, {(u32)&mySPI5, 1 }};

const OneJob_t SPI5_Wait_50us = { sq_ArmTimerCountdown, {(u32)&Timer6_us, 2, 5 } };
const OneJob_t SPI5_Wait_5ms =  { sq_ArmTimerCountdown, {(u32)&Timer7_ms, 2, 5 } };

const OneJob_t INT5_Arm = { sq_EXTI_Interrupt, {(u32) &INT5, ENABLE }};
const OneJob_t INT5_Disarm = { sq_EXTI_Interrupt, {(u32) &INT5, DISABLE }};


//========== The shared resources for SPI5
static IO_Pin_t MISO4, MOSI4, SCK4, NSS4, INT4;
static SPI_MasterHW_t mySPI4;
static u8 C_ShadowRegs[256];
const OneJob_t SPI4_Start = { sq_SPI_MHW_StartJob, {(u32)&mySPI4, 1} };
const OneJob_t SPI4_ShadowReadAt00 = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI4, (u32)&Shadow_ReadAll[0], 0, 1} };
const OneJob_t SPI4_Dummy = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI4, 0, 0, 1} };
const OneJob_t SPI4_ReadAllShadow = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI4, 0, (u32)&C_ShadowRegs[0], sizeof(C_ShadowRegs)} };
const OneJob_t SPI4_Stop = { sq_SPI_MHW_StopJob, {(u32)&mySPI4, 1 }};

const OneJob_t SPI4_Wait_50us = { sq_ArmTimerCountdown, {(u32)&Timer6_us, 1, 5 } };
const OneJob_t SPI4_Wait_5ms =  { sq_ArmTimerCountdown, {(u32)&Timer7_ms, 1, 5 } };

const OneJob_t INT4_Arm = { sq_EXTI_Interrupt, {(u32) &INT4, ENABLE } };
const OneJob_t INT4_Disarm = { sq_EXTI_Interrupt, {(u32) &INT4, DISABLE }};

//========== The sequence of 100 pointed jobs
static u32 List[100]; // list of pointers
static StuffsArtery_t mySequence;


void Test_SPI_MasterHW(void) {

// Create the sequencer entity
  StuffsArtery_t* P = &mySequence; // program
  NewSA(P, (u32)&List[0], countof(List));
//================== Pinout assignment
// ====---> SPI6
  mySPI6.SA = &mySequence;
  mySPI6.FeedClockHz = 48000000;
  NewSPI_MasterHW(&mySPI6, SPI6, NewIO_Pin(&MISO6,PG12), NewIO_Pin(&MOSI6,PG14), NewIO_Pin(&SCK6,PG13), NewIO_Pin(&NSS6,PG8));
  SetSPI_MasterHW_Timings(&mySPI6, 20*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB
  ConfigureSPI_MasterHW(&mySPI6);
// Link the SPI to the corresponding sequencer

// ====---> SPI5
// SPI5_NSS = PF6
// SPI5_MISO = PF8
// SPI5_MOSI = PF9
// SPI5_CLK = PF7
// INT = PA1
// SPI5 RX is DMA2 Stream3 channel 2
// SPI5 TX is DMA2 Stream4 channel 2
// Attach a HW SPI to the entity
  mySPI5.SA = &mySequence;
  mySPI5.FeedClockHz = 48000000;
  NewSPI_MasterHW(&mySPI5, SPI5, NewIO_Pin(&MISO5,PF8), NewIO_Pin(&MOSI5,PF9), NewIO_Pin(&SCK5,PF7), NewIO_Pin(&NSS5,PF6));
  SetSPI_MasterHW_Timings(&mySPI5, 10*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB  
  ConfigureSPI_MasterHW(&mySPI5);
// Link the SPI to the corresponding sequencer

// ====---> SPI4
// SPI4_NSS = PE11
// SPI4_MISO = PE13
// SPI4_MOSI = PE14
// SPI4_CLK = PE12
// INT = PG2
// SPI4 RX is DMA2 Stream0 channel 4
// SPI1 TX is DMA2 Stream1 channel 4
  mySPI4.SA = &mySequence;
  mySPI4.FeedClockHz = 48000000;
  NewSPI_MasterHW(&mySPI4, SPI4, NewIO_Pin(&MISO4,PE13), NewIO_Pin(&MOSI4,PE14), NewIO_Pin(&SCK4,PE12), NewIO_Pin(&NSS4,PE11));
  SetSPI_MasterHW_Timings(&mySPI4, 5*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB  
  ConfigureSPI_MasterHW(&mySPI4);
// Link the SPI to the corresponding sequencer
  
//===========
 
  while(1) {
  
    AddToSA(P, (u32) &SPI6_Start);
    AddToSA(P, (u32) &SPI6_ShadowReadAt00);
    AddToSA(P, (u32) &SPI6_Dummy);
    AddToSA(P, (u32) &SPI6_ReadAllShadow);
    AddToSA(P, (u32) &SPI6_Stop);

    AddToSA(P, (u32) &SPI5_Start);
    AddToSA(P, (u32) &SPI5_ShadowReadAt00);
    AddToSA(P, (u32) &SPI5_Dummy);
    AddToSA(P, (u32) &SPI5_ReadAllShadow);
    AddToSA(P, (u32) &SPI5_Stop);

    AddToSA(P, (u32) &SPI4_Start);
    AddToSA(P, (u32) &SPI4_ShadowReadAt00);
    AddToSA(P, (u32) &SPI4_Dummy);
    AddToSA(P, (u32) &SPI4_ReadAllShadow);
    AddToSA(P, (u32) &SPI4_Stop);
    
    StartJobToDoInForeground((u32)&mySequence);
    while(mySequence.FlagEmptied==0);
    NOPs(1);
  }
}


//=============================------------------------->
static u32 ListA[100]; // list of pointers
static u32 ListB[100];
static u32 ListC[100];
static StuffsArtery_t mySequenceA,mySequenceB,mySequenceC;


  
void Test_ConcurrentSPI_MasterHW(void) {

// Create the sequencer entity
  StuffsArtery_t* C = &mySequenceC; // program
  NewSA(C, (u32)&ListC[0], countof(ListC));
//================== Pinout assignment
// ====---> SPI6
  mySPI6.SA = &mySequenceC;
  mySPI6.FeedClockHz = 48000000;
  NewSPI_MasterHW(&mySPI6, SPI6, NewIO_Pin(&MISO6,PG12), NewIO_Pin(&MOSI6,PG14), NewIO_Pin(&SCK6,PG13), NewIO_Pin(&NSS6,PG8));
  SetSPI_MasterHW_Timings(&mySPI6, 20*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB
  ConfigureSPI_MasterHW(&mySPI6);
// Link the SPI to the corresponding sequencer
  
  // INT6 setupS
  NewIO_Pin(&INT6, PG6);
  IO_PinClockEnable(&INT6);
  IO_PinSetInput(&INT6);    
//  IO_PinSetLow(RS->fRX);//  IO_PinSetSpeedMHz(RS->fRX, 1);//  IO_PinEnableHighDrive(RS->fRX, ENABLE);
  IO_PinEnablePullUpDown(&INT6, ENABLE, DISABLE);
  EXTI_SetEdgesEnable(INT6.Name, DISABLE, ENABLE);
  

// ====---> SPI5
// SPI5_NSS = PF6
// SPI5_MISO = PF8
// SPI5_MOSI = PF9
// SPI5_CLK = PF7
// INT = PA1
// SPI5 RX is DMA2 Stream3 channel 2
// SPI5 TX is DMA2 Stream4 channel 2
  StuffsArtery_t* B = &mySequenceB; // program
  NewSA(B, (u32)&ListB[0], countof(ListB));  
  mySPI5.SA = &mySequenceB;
  mySPI5.FeedClockHz = 48000000;
  NewSPI_MasterHW(&mySPI5, SPI5, NewIO_Pin(&MISO5,PF8), NewIO_Pin(&MOSI5,PF9), NewIO_Pin(&SCK5,PF7), NewIO_Pin(&NSS5,PF6));
  SetSPI_MasterHW_Timings(&mySPI5, 10*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB  
  ConfigureSPI_MasterHW(&mySPI5);
// Link the SPI to the corresponding sequencer

  // INT5 setupS
  NewIO_Pin(&INT5, PA1);
  IO_PinClockEnable(&INT5);
  IO_PinSetInput(&INT5);    
//  IO_PinSetLow(RS->fRX);//  IO_PinSetSpeedMHz(RS->fRX, 1);//  IO_PinEnableHighDrive(RS->fRX, ENABLE);
  IO_PinEnablePullUpDown(&INT5, ENABLE, DISABLE);
  EXTI_SetEdgesEnable(INT5.Name, DISABLE, ENABLE);
  
// ====---> SPI4
// SPI4_NSS = PE11
// SPI4_MISO = PE13
// SPI4_MOSI = PE14
// SPI4_CLK = PE12
// INT = PG2
// SPI4 RX is DMA2 Stream0 channel 4
// SPI1 TX is DMA2 Stream1 channel 4
  StuffsArtery_t* A = &mySequenceA; // program
  NewSA(A, (u32)&ListA[0], countof(ListA));  
  mySPI4.SA = &mySequenceA;
  mySPI4.FeedClockHz = 48000000;
  NewSPI_MasterHW(&mySPI4, SPI4, NewIO_Pin(&MISO4,PE13), NewIO_Pin(&MOSI4,PE14), NewIO_Pin(&SCK4,PE12), NewIO_Pin(&NSS4,PE11));
  SetSPI_MasterHW_Timings(&mySPI4, 5*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB  
  ConfigureSPI_MasterHW(&mySPI4);
// Link the SPI to the corresponding sequencer

  // INT4 setup
  NewIO_Pin(&INT4, PG2);
  IO_PinClockEnable(&INT4);
  IO_PinSetInput(&INT4);    
//  IO_PinSetLow(RS->fRX);//  IO_PinSetSpeedMHz(RS->fRX, 1);//  IO_PinEnableHighDrive(RS->fRX, ENABLE);
  IO_PinEnablePullUpDown(&INT4, ENABLE, DISABLE);
  EXTI_SetEdgesEnable(INT4.Name, DISABLE, ENABLE);
  
  // put the hooks in place
  HookEXTIn(INT4.Name & 0xF, (u32)JobToDo, (u32) &mySequenceA);
  HookEXTIn(INT5.Name & 0xF, (u32)JobToDo, (u32) &mySequenceB);
  HookEXTIn(INT6.Name & 0xF, (u32)JobToDo, (u32) &mySequenceC);
  
  NVIC_EXTIsEnable(ENABLE);  
  
//===========
  // here we test the Basic Timer delays
  NewTimer(&Timer6_us, TIM6);
  SetTimerTimings(&Timer6_us, 10, GetMCUClockTree());
  ConfigureTimer(&Timer6_us);
  NewTimer(&Timer7_ms, TIM7);
  SetTimerTimings(&Timer7_ms, 1000, GetMCUClockTree());
  ConfigureTimer(&Timer7_ms);
  NVIC_TimersEnable(ENABLE);
  
  // 0 is free
  HookTimerCountdown(&Timer6_us, 1, (u32)JobToDo, (u32)&mySequenceA );
  HookTimerCountdown(&Timer6_us, 2, (u32)JobToDo, (u32)&mySequenceB );
  HookTimerCountdown(&Timer6_us, 3, (u32)JobToDo, (u32)&mySequenceC );

  // 0 is free
  HookTimerCountdown(&Timer7_ms, 1, (u32)JobToDo, (u32)&mySequenceA );
  HookTimerCountdown(&Timer7_ms, 2, (u32)JobToDo, (u32)&mySequenceB );
  HookTimerCountdown(&Timer7_ms, 3, (u32)JobToDo, (u32)&mySequenceC );
//==========
  
//===========
  
  while(1) {
    
    AddToSA(C, (u32) &SPI6_Start);
    AddToSA(C, (u32) &SPI6_ShadowReadAt00);

    AddToSA(C, (u32) &SPI6_Disarm); // add a delay of 50us
    AddToSA(C, (u32) &SPI6_Wait_50us); // add a delay of 50us
    AddToSA(C, (u32) &SPI6_Arm); // add a delay of 50us
    
    AddToSA(C, (u32) &SPI6_Dummy);
    
    AddToSA(C, (u32) &SPI6_Disarm); // add a delay of 50us
    AddToSA(C, (u32) &INT6_Arm);
    AddToSA(C, (u32) &INT6_Disarm);
    AddToSA(C, (u32) &SPI6_Arm); // add a delay of 50us
    
    AddToSA(C, (u32) &SPI6_ReadAllShadow);
    AddToSA(C, (u32) &SPI6_Stop);

    AddToSA(B, (u32) &SPI5_Start);
    AddToSA(B, (u32) &SPI5_ShadowReadAt00);
    AddToSA(B, (u32) &SPI5_Dummy);
    AddToSA(B, (u32) &SPI5_ReadAllShadow);
    AddToSA(B, (u32) &SPI5_Stop);


    AddToSA(A, (u32) &SPI4_Start);
    AddToSA(A, (u32) &SPI4_ShadowReadAt00);
    AddToSA(A, (u32) &SPI4_Dummy);
    AddToSA(A, (u32) &SPI4_ReadAllShadow);
    AddToSA(A, (u32) &SPI4_Stop);

//  tmp = __get_PRIMASK(); // if you don't do this, the interrupts might come within JobToDo... causing some delays from time to time. No hangups.
//  __disable_irq();//__set_PRIMASK(tmp | 1);
  
    StartJobToDoInBackground((u32)&mySequenceA);
    StartJobToDoInBackground((u32)&mySequenceB);
    StartJobToDoInBackground((u32)&mySequenceC);
  
//  __enable_irq();//__set_PRIMASK(tmp);
    
    while((mySequenceA.FlagEmptied==0)||(mySequenceB.FlagEmptied==0)||(mySequenceC.FlagEmptied==0));
    NOPs(1);

  }
}

#else
void SebMainTest(void) {
  
  return; // no example activated
}

#endif
u8 Utl_FrameCompare(int16_t* s16p_rawFrame, int16_t* s16p_calibFrame, int16_t s16_hiThres, int16_t s16_loThres, int16_t s16_nodeThers, int16_t s32_len);



volatile int16_t* s16p_rawFrameV;
volatile int16_t* s16p_calibFrameV;
volatile int16_t s16_hiThresV;
volatile int16_t s16_loThresV;
volatile int16_t s16_nodeThersV;
volatile int32_t s32_lenV;
u8 result;

void TestCode(void) {
  
int16_t* s16p_rawFrame = (int16_t*) s16p_rawFrameV;
int16_t* s16p_calibFrame = (int16_t*) s16p_calibFrameV;
int16_t s16_hiThres = s16_hiThresV;
int16_t s16_loThres = s16_loThresV;
int16_t s16_nodeThers = s16_nodeThersV;
int32_t s32_len = s32_lenV;
  
  result = Utl_FrameCompare(s16p_rawFrame, s16p_calibFrame, s16_hiThres, s16_loThres, s16_nodeThers, (u16)s32_len);
  
}
u8 Utl_FrameCompare(int16_t* s16p_rawFrame, int16_t* s16p_calibFrame, int16_t s16_hiThres, int16_t s16_loThres, int16_t s16_nodeThers, int16_t s32_len)
{
             return FALSE;
}
