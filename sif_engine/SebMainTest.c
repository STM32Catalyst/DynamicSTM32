
#include "SebEngine.h"

// This file is the example architect, like we would dynamically create a train track and let it go...
// Or a dynamically reprogrammable FPGA for the ultimate goal.

// On top, we include all the available ingredients available for the different examples.
// First step is to create the needed contexts in SRAM as global playground
// Second is to format and configure these SRAM items
// Third is to place all links and hooks properly
// Fourth is to activate all the IRQs for the system to let go.
// later the "global SRAM" could be a local variable so it could be destroyed once the job done.
// case 1:
void SebExample1(void);
void SebExample2(void);
void Test_SPI_MasterHW(void);;
void Test_ConcurrentSPI_MasterHW(void);
void Test_ConcurrentSPI_MasterHW2(void); // the sequences trigger the jobs directly as they are added to see if it works.
vu8 choice=6; // default choice after reset
void I2C_MasterIO_Test(void);
void RFFE_Test(void);

BasicTimer Timer6_us, Timer7_ms;


void SebMainTest(void) {
  
  while(choice==0) ; // stop the emulator, change choice and let it run

  while(1) {
    switch(choice) {
    case 0: // exit and return
      return;
    case 1: // Create an I2C Spy which output its string into an RS232 Tx pin
      SebExample1();
      break;
    case 2:
      SebExample2();
      break;
    case 3:
      Test_SPI_MasterHW();
      break;
    case 4:
      Test_ConcurrentSPI_MasterHW();
      break;
    case 5:
      RFFE_Test(); // Tested ok on Feb 10, 2015 using ST HVDAC device example
      break;
    case 6:
      I2C_MasterIO_Test();
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      break;
    default:while(1);
    };
  };
    
}

// Example 1: Construct an I2C Spy and output the decoded string into RS232, also create an I2C slave which maps 256 bytes with 1 byte sub address
// Ingredients: sebNVIC, sebEXTI, I2C_SlaveIO, ByteVein, SebUART, I2C_By_GPIO(I2C master done in main loop)
// Connectivity: SDA= SCL= TX1= RX1=
// Buffers:

// to make the slave not responding, we need to change to add a "slave busy" flag which latches when bus is idle.
static I2C_SlaveIO mySlave; // can be multiple of them, as array (we pass by pointer)
static IO_PinTypeDef mySDA; // for fast pin access using bitbanding area (we will use a fake HW registers which will check if all pins are not colliding)
static IO_PinTypeDef mySCL; // for fast pin access using bitbanding area
// we have to cleanup the Architect function for the fPin

static u8 mySlaveAdresses[] = { 0x40, 0xB0, 0xC0, 0xD0 }; // ram is faster
static u8 mySlaveMemory[256];
static u8 myRxBuf[10];

static ByteVein myBV_TX;
static u8 myBV_I2CSpyToTX1[1024];

static ByteVein myBV_RX;
static u8 myBV_RX1ToTBD[1024];

static SerialRs232 myRs232;

static IO_PinTypeDef myRs232RX; // for fast pin access using bitbanding area (we will use a fake HW registers which will check if all pins are not colliding)
static IO_PinTypeDef myRs232TX; // for fast pin access using bitbanding area

void SebExample1(void) {

  // I2C Slave description, using IO_Pin and EXTI (could later on use Basic Timer to implement a timeout like SMBus is doing)
  IO_PinInit(&mySDA, PH10 ); // Initialize some quick pointers
  IO_PinInit(&mySCL, PH12 ); // Initialize some quick pointers
  
  // This is the data byte vein flowing from I2C Spy to UART TX, and acts as a FIFO
  NewBV(&myBV_TX, (u32)myBV_I2CSpyToTX1, sizeof(myBV_I2CSpyToTX1));
  mySlave.BV = &myBV_TX;
  
  mySlave.SDA = &mySDA; // link the pin to the slave
  mySlave.SCL = &mySCL; // link the pin to the slave
  NewI2C_SlaveIO(&mySlave, (u8*)mySlaveAdresses, countof(mySlaveAdresses), mySlaveMemory, countof(mySlaveMemory));
  SpyI2C_SlaveIO(&mySlave); // this is to go to spy code as well

  mySlave.fnBusFree = u32_fn_u32; // no hooks on the I2C Slave events, for breakpoint purpose, points to a return function... (default is 0)
  mySlave.ctBusFree = 1;
  mySlave.fnReceived = u32_fn_u32;
  mySlave.ctReceived = 2;
  mySlave.fnTransmitted = u32_fn_u32;
  mySlave.ctTransmitted = 3;
  
  // RS232 cell definition
  // Pin definition, RX1=PB7, TX1=PB6
  IO_PinInit(&myRs232RX, PB7 ); // Initialize some quick pointers
  IO_PinInit(&myRs232TX, PB6 ); // Initialize some quick pointers
  // Hook the pins to the RS232 USART (if pin not used, put null pointer)
  myRs232.fRX = &myRs232RX;
  myRs232.fTX = &myRs232TX;
  myRs232.fRTS = 0;
  myRs232.fCTS = 0;
  NewRs232HW( &myRs232, USART1); // The HW connectivity, handle, register base, RX pin, TX pin, CTS pin, RTS pin, if non null.
  // we have also to initialize the BV_TX and BV_RX at higher level...
  // Initialize the Byte Vein from RX1 to fill up until full for now. (depends if you short RX/TX or not)
  NewBV(&myBV_RX, (u32)myBV_RX1ToTBD, sizeof(myBV_RX1ToTBD));

  // This will hook the BV to RS232 USART
  SetRs232BVs( &myRs232, &myBV_TX, &myBV_RX); // hook them to RS232

  // Setup the timings and enable the cell
  SetRs232Timings(&myRs232, 115200, 2, 1); // Things depending on time and internal clocks [This Enables the cell]
  
  // now we need to activate everything after all the architecture is done (the other end of the BV_TX, BV_RX)
  NVIC_Rs232sEnable(ENABLE);// enable interrupts for RS232
  NVIC_EXTIsEnable(ENABLE); // enable interrupts for slave cell
  //===============

  while(1) {

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
    
    Wait_ms(100); // let it flush, here could be too fast
  }
  
}


//====----> Extra level, here the Master is also using the same concept
static BasicTimer BT;

void SebExample2(void) {
#if 0 // superseaded
  // I2C Slave description, using IO_Pin and EXTI (could later on use Basic Timer to implement a timeout like SMBus is doing)
  IO_PinInit(&mySDA, PH10 ); // Initialize some quick pointers
  IO_PinInit(&mySCL, PH12 ); // Initialize some quick pointers
  
  // This is the data byte vein flowing from I2C Spy to UART TX, and acts as a FIFO
  NewBV(&myBV_TX, (u32)myBV_I2CSpyToTX1, sizeof(myBV_I2CSpyToTX1));
  mySlave.BV = &myBV_TX;
  
  mySlave.fSDA = &mySDA; // link the pin to the slave
  mySlave.fSCL = &mySCL; // link the pin to the slave
  NewI2C_SlaveIO(&mySlave, (u8*)mySlaveAdresses, countof(mySlaveAdresses), mySlaveMemory, countof(mySlaveMemory));
  SpyI2C_SlaveIO(&mySlave); // this is to go to spy code as well

  mySlave.fnBusFree = u32_fn_u32; // no hooks on the I2C Slave events, for breakpoint purpose, points to a return function... (default is 0)
  mySlave.ctBusFree = 1;
  mySlave.fnReceived = u32_fn_u32;
  mySlave.ctReceived = 2;
  mySlave.fnTransmitted = u32_fn_u32;
  mySlave.ctTransmitted = 3;

  //====----> RS232 cell definition  
  // RS232 cell definition
  // Pin definition, RX1=PB7, TX1=PB6
  IO_PinInit(&myRs232RX, PB7 ); // Initialize some quick pointers
  IO_PinInit(&myRs232TX, PB6 ); // Initialize some quick pointers
  // Hook the pins to the RS232 USART (if pin not used, put null pointer)
  myRs232.fRX = &myRs232RX;
  myRs232.fTX = &myRs232TX;
  myRs232.fRTS = 0;
  myRs232.fCTS = 0;
  NewRs232HW( &myRs232, USART1); // The HW connectivity, handle, register base, RX pin, TX pin, CTS pin, RTS pin, if non null.
  // we have also to initialize the BV_TX and BV_RX at higher level...
  // Initialize the Byte Vein from RX1 to fill up until full for now. (depends if you short RX/TX or not)
  NewBV(&myBV_RX, (u32)myBV_RX1ToTBD, sizeof(myBV_RX1ToTBD));

  // This will hook the BV to RS232 USART
  SetRs232BVs( &myRs232, &myBV_TX, &myBV_RX); // hook them to RS232

  // Setup the timings and enable the cell
  SetRs232Timings(&myRs232, 115200, 2, 1); // Things depending on time and internal clocks [This Enables the cell]
  
  //====----> Now we setup the I2C master
  u32 u = (u32)&gMIO;
  
  IO_PinInit(&MIO_SDA, PH7 ); // Initialize some quick pointers
  IO_PinInit(&MIO_SCL, PH8 ); // Initialize some quick pointers
  gMIO.SDA = &MIO_SDA; // global shared resource, to be generic later
  gMIO.SCL = &MIO_SCL; // global shared resource, to be generic later

  BT.FeedClockMHz = 96/2; // 96MHz (this should later come from RAM global structure)  
  NewBasicTimer_us(&BT, TIM6, 1); // usec countdown
  gMIO.Job = &Job;
  gMIO.BT = &BT;
  gMIO.BTn = 0; // use Countdown[0]
  
  gMIO.FeedClockMHz = 96; // MCU clock as it is by S/W
  
  NewI2C_MasterIO(&gMIO);
  SetI2C_MasterIO_Timings(&gMIO, 400*1000 );
  
  // now we need to activate everything after all the architecture is done (the other end of the BV_TX, BV_RX)
  NVIC_BasicTimersEnable(ENABLE);// used by I2C Master (#0/4)
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
IO_PinTypeDef MISO6, MOSI6, SCK6, NSS6, INT6;
SPI_MasterHW mySPI6;
u8 A_ShadowRegs[256];
const OneJobType SPI6_Start = { sq_SPI_MHW_StartJob, {(u32)&mySPI6, 1} };
const OneJobType SPI6_ShadowReadAt00 = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI6, (u32)&Shadow_ReadAll[0], 0, 1} };
const OneJobType SPI6_Dummy = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI6, 0, 0, 1} };
const OneJobType SPI6_ReadAllShadow = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI6, 0, (u32)&A_ShadowRegs[0], sizeof(A_ShadowRegs)} };
const OneJobType SPI6_Stop = { sq_SPI_MHW_StopJob, {(u32)&mySPI6, 1 }};

const OneJobType SPI6_Disarm = {sq_SPI_MHW_DMA_Interrupt, {(u32)&mySPI6, DISABLE }};
const OneJobType SPI6_Arm = {sq_SPI_MHW_DMA_Interrupt, {(u32)&mySPI6, ENABLE }};

//const OneJobType SPI6_Disarm = { sq_SPI_MHW_StopJob, {(u32)&mySPI6, 0 }}; // disturb SPI pins AF
//const OneJobType SPI6_Arm = { sq_SPI_MHW_StartJob, {(u32)&mySPI6, 1} }; // disturb SPI pins AF

const OneJobType SPI6_Wait_50us = { sq_ArmBasicTimerCountdown, {(u32)&Timer6_us, 3, 5 } };
const OneJobType SPI6_Wait_5ms =  { sq_ArmBasicTimerCountdown, {(u32)&Timer7_ms, 3, 5 } };

const OneJobType INT6_Arm = { sq_EXTI_Interrupt, {(u32) &INT6, ENABLE }};
const OneJobType INT6_Disarm = { sq_EXTI_Interrupt, {(u32) &INT6, DISABLE }};

//========== The shared resources for SPI5
IO_PinTypeDef MISO5, MOSI5, SCK5, NSS5, INT5;
SPI_MasterHW mySPI5;
u8 B_ShadowRegs[256];
const OneJobType SPI5_Start = { sq_SPI_MHW_StartJob, {(u32)&mySPI5, 1} };
const OneJobType SPI5_ShadowReadAt00 = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI5, (u32)&Shadow_ReadAll[0], 0, 1} };
const OneJobType SPI5_Dummy = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI5, 0, 0, 1} };
const OneJobType SPI5_ReadAllShadow = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI5, 0, (u32)&B_ShadowRegs[0], sizeof(B_ShadowRegs)} };
const OneJobType SPI5_Stop = { sq_SPI_MHW_StopJob, {(u32)&mySPI5, 1 }};

const OneJobType SPI5_Wait_50us = { sq_ArmBasicTimerCountdown, {(u32)&Timer6_us, 2, 5 } };
const OneJobType SPI5_Wait_5ms =  { sq_ArmBasicTimerCountdown, {(u32)&Timer7_ms, 2, 5 } };

const OneJobType INT5_Arm = { sq_EXTI_Interrupt, {(u32) &INT5, ENABLE }};
const OneJobType INT5_Disarm = { sq_EXTI_Interrupt, {(u32) &INT5, DISABLE }};


//========== The shared resources for SPI5
IO_PinTypeDef MISO4, MOSI4, SCK4, NSS4, INT4;
SPI_MasterHW mySPI4;
u8 C_ShadowRegs[256];
const OneJobType SPI4_Start = { sq_SPI_MHW_StartJob, {(u32)&mySPI4, 1} };
const OneJobType SPI4_ShadowReadAt00 = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI4, (u32)&Shadow_ReadAll[0], 0, 1} };
const OneJobType SPI4_Dummy = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI4, 0, 0, 1} };
const OneJobType SPI4_ReadAllShadow = { sq_SPI_MHW_MoveJob,  {(u32)&mySPI4, 0, (u32)&C_ShadowRegs[0], sizeof(C_ShadowRegs)} };
const OneJobType SPI4_Stop = { sq_SPI_MHW_StopJob, {(u32)&mySPI4, 1 }};

const OneJobType SPI4_Wait_50us = { sq_ArmBasicTimerCountdown, {(u32)&Timer6_us, 1, 5 } };
const OneJobType SPI4_Wait_5ms =  { sq_ArmBasicTimerCountdown, {(u32)&Timer7_ms, 1, 5 } };

const OneJobType INT4_Arm = { sq_EXTI_Interrupt, {(u32) &INT4, ENABLE } };
const OneJobType INT4_Disarm = { sq_EXTI_Interrupt, {(u32) &INT4, DISABLE }};

//========== The sequence of 100 pointed jobs
static u32 List[100]; // list of pointers
static StuffsArtery mySequence;


void Test_SPI_MasterHW(void) {

// Create the sequencer entity
  StuffsArtery* P = &mySequence; // program
  NewSA(P, (u32)&List[0], countof(List));
//================== Pinout assignment
// ====---> SPI6
  IO_PinInit(&MISO6, PG12);  
  IO_PinInit(&MOSI6, PG14);  
  IO_PinInit(&SCK6,  PG13);
  IO_PinInit(&NSS6,  PG8);  
// Attach these pins to SPI_MasterHW
  mySPI6.MISO = &MISO6;
  mySPI6.MOSI = &MOSI6;
  mySPI6.SCK = &SCK6;
  mySPI6.NSSs[0] = &NSS6;
// Attach a HW SPI to the entity
  mySPI6.SPI = SPI6;
  mySPI6.SA = &mySequence;
  mySPI6.FeedClockMHz = 48;
  NewSPI_MasterHW_RX_TX(&mySPI6, DMA2_Stream6, DMA_Channel_1, DMA2_Stream5, DMA_Channel_1); // RX Stream, TX Stream
  SetSPI_MasterHW_Timings(&mySPI6, 20*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB
// Link the SPI to the corresponding sequencer

// ====---> SPI5
/*
// SPI5_NSS = PF6
// SPI5_MISO = PF8
// SPI5_MOSI = PF9
// SPI5_CLK = PF7
// INT = PA1
// SPI5 RX is DMA2 Stream3 channel 2
// SPI5 TX is DMA2 Stream4 channel 2
*/  
  IO_PinInit(&MISO5,PF8);  
  IO_PinInit(&MOSI5,PF9);  
  IO_PinInit(&SCK5,PF7);
  IO_PinInit(&NSS5,PF6);  
// Attach these pins to SPI_MasterHW
  mySPI5.MISO = &MISO5;
  mySPI5.MOSI = &MOSI5;
  mySPI5.SCK = &SCK5;
  mySPI5.NSSs[0] = &NSS5;
// Attach a HW SPI to the entity
  mySPI5.SPI = SPI5;
  mySPI5.SA = &mySequence;
  mySPI5.FeedClockMHz = 48;
  NewSPI_MasterHW_RX_TX(&mySPI5, DMA2_Stream3, DMA_Channel_2, DMA2_Stream4, DMA_Channel_2); // RX Stream, TX Stream
  SetSPI_MasterHW_Timings(&mySPI5, 10*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB  
// Link the SPI to the corresponding sequencer

// ====---> SPI4
/*
// SPI4_NSS = PE11
// SPI4_MISO = PE13
// SPI4_MOSI = PE14
// SPI4_CLK = PE12
// INT = PG2
// SPI4 RX is DMA2 Stream0 channel 4
// SPI1 TX is DMA2 Stream1 channel 4
*/  
  
  IO_PinInit(&MISO4, PE13);  
  IO_PinInit(&MOSI4, PE14);  
  IO_PinInit(&SCK4, PE12);
  IO_PinInit(&NSS4, PE11);  
// Attach these pins to SPI_MasterHW
  mySPI4.MISO = &MISO4;
  mySPI4.MOSI = &MOSI4;
  mySPI4.SCK = &SCK4;
  mySPI4.NSSs[0] = &NSS4;
// Attach a HW SPI to the entity
  mySPI4.SPI = SPI4;
  mySPI4.SA = &mySequence;
  mySPI4.FeedClockMHz = 48;
  NewSPI_MasterHW_RX_TX(&mySPI4, DMA2_Stream0, DMA_Channel_4, DMA2_Stream1, DMA_Channel_4); // RX Stream, TX Stream
  SetSPI_MasterHW_Timings(&mySPI4, 5*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB  
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
static StuffsArtery mySequenceA,mySequenceB,mySequenceC;


  
void Test_ConcurrentSPI_MasterHW(void) {

// Create the sequencer entity
  StuffsArtery* C = &mySequenceC; // program
  NewSA(C, (u32)&ListC[0], countof(ListC));
//================== Pinout assignment
// ====---> SPI6
  IO_PinInit(&MISO6, PG12);  
  IO_PinInit(&MOSI6, PG14);  
  IO_PinInit(&SCK6,  PG13);
  IO_PinInit(&NSS6,  PG8);  
// Attach these pins to SPI_MasterHW
  mySPI6.MISO = &MISO6;
  mySPI6.MOSI = &MOSI6;
  mySPI6.SCK = &SCK6;
  mySPI6.NSSs[0] = &NSS6;
// Attach a HW SPI to the entity
  mySPI6.SPI = SPI6;
  mySPI6.SA = &mySequenceC;
  mySPI6.FeedClockMHz = 48;
  NewSPI_MasterHW_RX_TX(&mySPI6, DMA2_Stream6, DMA_Channel_1, DMA2_Stream5, DMA_Channel_1); // RX Stream, TX Stream
  SetSPI_MasterHW_Timings(&mySPI6, 20*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB
// Link the SPI to the corresponding sequencer
  
  // INT6 setupS
  IO_PinInit(&INT6, PG6);
  IO_PinClockEnable(&INT6);
  IO_PinConfiguredAs(&INT6,GPIO_AF16_DIGITAL_INPUT);    
//  IO_PinSetLow(RS->fRX);//  IO_PinSetSpeedMHz(RS->fRX, 1);//  IO_PinEnableHighDrive(RS->fRX, ENABLE);
  IO_PinEnablePullUp(&INT6, ENABLE);
  IO_PinEnablePullDown(&INT6, DISABLE);
  EXTI_SetEdgesEnable(INT6.Name, DISABLE, ENABLE);
  

// ====---> SPI5
/*
// SPI5_NSS = PF6
// SPI5_MISO = PF8
// SPI5_MOSI = PF9
// SPI5_CLK = PF7
// INT = PA1
// SPI5 RX is DMA2 Stream3 channel 2
// SPI5 TX is DMA2 Stream4 channel 2
*/  
  StuffsArtery* B = &mySequenceB; // program
  NewSA(B, (u32)&ListB[0], countof(ListB));  
  IO_PinInit(&MISO5,PF8);  
  IO_PinInit(&MOSI5,PF9);  
  IO_PinInit(&SCK5,PF7);
  IO_PinInit(&NSS5,PF6);  
// Attach these pins to SPI_MasterHW
  mySPI5.MISO = &MISO5;
  mySPI5.MOSI = &MOSI5;
  mySPI5.SCK = &SCK5;
  mySPI5.NSSs[0] = &NSS5;
// Attach a HW SPI to the entity
  mySPI5.SPI = SPI5;
  mySPI5.SA = &mySequenceB;
  mySPI5.FeedClockMHz = 48;
  NewSPI_MasterHW_RX_TX(&mySPI5, DMA2_Stream3, DMA_Channel_2, DMA2_Stream4, DMA_Channel_2); // RX Stream, TX Stream
  SetSPI_MasterHW_Timings(&mySPI5, 10*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB  
// Link the SPI to the corresponding sequencer

  // INT5 setupS
  IO_PinInit(&INT5, PA1);
  IO_PinClockEnable(&INT5);
  IO_PinConfiguredAs(&INT5,GPIO_AF16_DIGITAL_INPUT);    
//  IO_PinSetLow(RS->fRX);//  IO_PinSetSpeedMHz(RS->fRX, 1);//  IO_PinEnableHighDrive(RS->fRX, ENABLE);
  IO_PinEnablePullUp(&INT5, ENABLE);
  IO_PinEnablePullDown(&INT5, DISABLE);
  EXTI_SetEdgesEnable(INT5.Name, DISABLE, ENABLE);
  
// ====---> SPI4
/*
// SPI4_NSS = PE11
// SPI4_MISO = PE13
// SPI4_MOSI = PE14
// SPI4_CLK = PE12
// INT = PG2
// SPI4 RX is DMA2 Stream0 channel 4
// SPI1 TX is DMA2 Stream1 channel 4
*/  
  StuffsArtery* A = &mySequenceA; // program
  NewSA(A, (u32)&ListA[0], countof(ListA));  
  IO_PinInit(&MISO4, PE13);  
  IO_PinInit(&MOSI4, PE14);  
  IO_PinInit(&SCK4, PE12);
  IO_PinInit(&NSS4, PE11);  
// Attach these pins to SPI_MasterHW
  mySPI4.MISO = &MISO4;
  mySPI4.MOSI = &MOSI4;
  mySPI4.SCK = &SCK4;
  mySPI4.NSSs[0] = &NSS4;
// Attach a HW SPI to the entity
  mySPI4.SPI = SPI4;
  mySPI4.SA = &mySequenceA;
  mySPI4.FeedClockMHz = 48;
  NewSPI_MasterHW_RX_TX(&mySPI4, DMA2_Stream0, DMA_Channel_4, DMA2_Stream1, DMA_Channel_4); // RX Stream, TX Stream
  SetSPI_MasterHW_Timings(&mySPI4, 5*1000000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB  
// Link the SPI to the corresponding sequencer

  // INT4 setup
  IO_PinInit(&INT4, PG2);
  IO_PinClockEnable(&INT4);
  IO_PinConfiguredAs(&INT4,GPIO_AF16_DIGITAL_INPUT);    
//  IO_PinSetLow(RS->fRX);//  IO_PinSetSpeedMHz(RS->fRX, 1);//  IO_PinEnableHighDrive(RS->fRX, ENABLE);
  IO_PinEnablePullUp(&INT4, ENABLE);
  IO_PinEnablePullDown(&INT4, DISABLE);
  EXTI_SetEdgesEnable(INT4.Name, DISABLE, ENABLE);
  
  // put the hooks in place
  HookEXTIn(INT4.Name & 0xF, (u32)JobToDo, (u32) &mySequenceA);
  HookEXTIn(INT5.Name & 0xF, (u32)JobToDo, (u32) &mySequenceB);
  HookEXTIn(INT6.Name & 0xF, (u32)JobToDo, (u32) &mySequenceC);
  
  NVIC_EXTIsEnable(ENABLE);  
  
//===========
  // here we test the Basic Timer delays

  Timer6_us.FeedClockMHz = 96/2; // 96MHz (this should later come from RAM global structure)  
  Timer7_ms.FeedClockMHz = 96/2; // 96MHz (this should later come from RAM global structure)  
  NewBasicTimer_us(&Timer6_us, TIM6, 10);// 1us tick period
  NewBasicTimer_us(&Timer7_ms, TIM7, 1000);// 1ms period
  NVIC_BasicTimersEnable(ENABLE);
  
  // 0 is free
  HookBasicTimerCountdown(&Timer6_us, 1, (u32)JobToDo, (u32)&mySequenceA );
  HookBasicTimerCountdown(&Timer6_us, 2, (u32)JobToDo, (u32)&mySequenceB );
  HookBasicTimerCountdown(&Timer6_us, 3, (u32)JobToDo, (u32)&mySequenceC );

  // 0 is free
  HookBasicTimerCountdown(&Timer7_ms, 1, (u32)JobToDo, (u32)&mySequenceA );
  HookBasicTimerCountdown(&Timer7_ms, 2, (u32)JobToDo, (u32)&mySequenceB );
  HookBasicTimerCountdown(&Timer7_ms, 3, (u32)JobToDo, (u32)&mySequenceC );
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
//  __set_PRIMASK(tmp | 1);
  
    StartJobToDoInBackground((u32)&mySequenceA);
    StartJobToDoInBackground((u32)&mySequenceB);
    StartJobToDoInBackground((u32)&mySequenceC);
  
//  __set_PRIMASK(tmp);
    
    while((mySequenceA.FlagEmptied==0)||(mySequenceB.FlagEmptied==0)||(mySequenceC.FlagEmptied==0));
    NOPs(1);

  }
}



void Test_ConcurrentSPI_MasterHW2(void) { // this one does not work. The stack never ends. We need to take care of sequences which do not use interrupt, not to call StartJobToDo which disable interrupts...

}
