
#include "SebEngine.h"

// this is the SPI 3 wire communication by S/W GPIO manipulation on the iNemo/FROG connector

//======================------------------------>
u32 SPI_MIO_Start(u32 u, u32 BitMask);
u32 SPI_MIO_Move(u32 u, u32 Param1, u32 Param2, u32 Param3);
u32 SPI_MIO_SendByte(u32 u, u8 byte);
u32 SPI_MIO_ReadByte(u32 u);
u32 SPI_MIO_Stop(u32 u, u32 BitMask);
u32 TimerCountdownWait(u32 u);
u32 NopsWait(u32 u);
u32 WaitHere(u32 u, u32 delay);

void NewSPI_MasterIO(SPI_MasterIO_t* M, IO_Pin_t* MISO, IO_Pin_t* MOSI, IO_Pin_t* SCK, IO_Pin_t* NSS0) {

  if((MOSI==0)&&(MISO==0)) while(1);// MOSI must be MISO to work in this bidir only implementation
  
// we have to initialize the state machine first
  M->MISO = MISO;
  M->MOSI = MOSI;
  M->SCK = SCK;
  M->NSSs[0] = NSS0;
}

u32 SetSPI_MasterIO_Timings(SPI_MasterIO_t* M, u32 MaxBps, u32 CPol, u32 CPha, u32 FirstBit, MCUClocks_t* T ) { // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB

  if(CPol!=SPI_CPOL_Low) while(1); // not supported yet
  if(CPha!=SPI_CPHA_1Edge) while(1); // not supported yet
  
  u32 HalfClockPeriod_Hz;
  u32 HalfClockPeriod_us;
  
  M->MaxBps = MaxBps; // 400khz

  HalfClockPeriod_Hz = MaxBps*2; // Timers runs at 1MHz max overflow speed. 500kHz = 2us
  
  if(M->Timer) {
    
    M->WaitParam = 1000000 / ( M->Timer->OverflowPeriod_us * HalfClockPeriod_Hz);
    if(M->WaitParam) {
      M->fnWaitMethod = TimerCountdownWait; // here we should take care of the timings, and choose the best scheme based on CPU MHz and bps of bus...    
      return 0; // found a tick period compatible with this Basic Timer
    };
    // otherwise, the strategy will be NOPs.
  }

  // Delay will use S/W NOPs because the incoming sys frequency is too low or the bit rate too high
  //!!! In IAR, 96MHz core STM32F437, No optimisation: WaitParam = 2. With max optimisation for speed: WaitParam = 20 (400kHz)
  // Later, we will use the help of the Timer with precise time to tune it dynamically.... more high tech and requires some time to tune.
  M->fnWaitMethod = NopsWait;
  
  HalfClockPeriod_us = (T->CoreClk_Hz )/(MaxBps*12); // Here the feedclock is the core speed for IO emulated we assume 1 cycle per instruction which is not realistic.
  // it should not be zero or the delays won't scale for the communication
  M->WaitParam = HalfClockPeriod_us;
  
  return 0;
}

static void SetPinInput(IO_Pin_t* P) {

  IO_PinClockEnable(P);
  IO_PinSetSpeedMHz(P, 1);  
  IO_PinSetHigh(P);  
  IO_PinSetInput(P);  
  IO_PinEnablePullUpDown(P, ENABLE, DISABLE);
  IO_PinEnableHighDrive(P, ENABLE);
}

static void SetPinOutput(IO_Pin_t* P) {

  IO_PinClockEnable(P);
  IO_PinSetSpeedMHz(P, 1);
  IO_PinSetHigh(P);
  IO_PinSetOutput(P);  
  IO_PinEnablePullUpDown(P, ENABLE, DISABLE);
  IO_PinEnableHighDrive(P, ENABLE);
}

void ConfigureSPI_MasterIO(SPI_MasterIO_t* M) {

  u8 n;
  // configure the GPIOs
  if((M->MISO)&&(M->MOSI!=M->MISO)) // If MISO=MOSI then we don't configure the input pin
    // configure MISO pin
    SetPinInput(M->MISO);
 
  // configure MOSI pin
  if(M->MOSI)
    SetPinOutput(M->MOSI);
  
  // configure SCK pin
  SetPinOutput(M->SCK);
  
  // configure all the NSSs pins
  for(n=0;n<16;n++)
    if(M->NSSs[n])
      SetPinOutput(M->NSSs[n]);
  
}

void EnableSPI_MasterIO(SPI_MasterIO_t* M) {
  
  // nothing to do
}




static u32 TimerCountdownWait(u32 u) {
  
  SPI_MasterIO_t* M = (SPI_MasterIO_t*) u;
  ArmTimerCountdown(M->Timer,M->Cn, M->WaitParam * M->ctWaitMethod);
  while(M->Timer->CountDownDone[M->Cn]==0) ;
  return 0;
}

static u32 NopsWait(u32 u) {
  SPI_MasterIO_t* M = (SPI_MasterIO_t*) u;
  u32 n = M->ctWaitMethod * M->WaitParam;
  while(n--) asm("nop\n");
  return 0;
}

static u32 WaitHere(u32 u, u32 delay) {
  SPI_MasterIO_t* M = (SPI_MasterIO_t*) u;
  M->ctWaitMethod = delay;
  if(M->fnWaitMethod) M->fnWaitMethod(u);
  return 0;
}
 


//=============-------------->

static u32 SPI_MIO_Start(u32 u, u32 BitMask) {

  u8 n;
  SPI_MasterIO_t* S = (SPI_MasterIO_t*) u;

// we don't make sure NSS are high at first.
  IO_PinSetLow(S->SCK);
  IO_PinSetLow(S->MOSI);
  IO_PinSetOutput(S->MOSI);

  Wait_us(1);
  
  for(n=0;n<16;n++) { // put low all the selected pins
    if(S->NSSs[n]==0) break;
    
    if(BitMask & (1<<n)) // if no, continue
        IO_PinSetLow(S->NSSs[n]);
  };
  
  Wait_us(1); //+
  return 0; // no call back, next job right away
}


static u32 SPI_MIO_Stop(u32 u, u32 BitMask) {

  u8 n;
  SPI_MasterIO_t* S = (SPI_MasterIO_t*) u;
  
  Wait_us(1);
  
  for(n=0;n<16;n++) { // put low all the selected pins
    if(S->NSSs[n]==0) break;
    
    if(BitMask & (1<<n)) { // if no, continue
        IO_PinSetOutput(S->NSSs[n]);
        IO_PinSetHigh(S->NSSs[n]);
    };
  };
  
  Wait_us(1);
  return 0; // no call back, next job right away
}


static u32 SPI_MIO_Move(u32 u, u32 Param1, u32 Param2, u32 Param3) {
  
  u8* TX = (u8*) Param1;
  u8* RX = (u8*) Param2;
  u32 bCount = Param3;
//  SPI_MasterIO_t* S = (SPI_MasterIO_t*) u;
  
  if((Param1)&&(Param2)) { // this means dummy read
    if(Param3==0) while(1); // nothing to do at all... problem
    
    while(bCount--) SPI_MIO_ReadByte(u);
  }
  
  // MASTER RECEIVE MODE
  // if the TX adr is null, no bytes to transmit, disable MOSI, and transmit dummy things instead
  if(Param1==0) {
    
    while(bCount--)
      *RX++ = (u8) SPI_MIO_ReadByte(u);
    
    return 0;
  }

  // MASTER TRANSMIT MODE 
  // if the RX adr is null, no bytes to receive, TRANSMIT MODE
  if(Param2==0) {
    while(bCount--)
      SPI_MIO_SendByte(u, *TX++);
  };
  
  if(Param3==0) while(1); // no bytes to transfer? weird!

  return 0; // there is no interrupt generated in this implementation
}

//==== 8>< ----- Sequencer entry function to cook a communication

u32 sq_SPI_MIO_StartJob(u32 u) {
  u32* p = (u32*) u;
  return SPI_MIO_Start(p[0], p[1]);//Param1);//1 parameter
}

// from 3 param down to 1
u32 sq_SPI_MIO_StopJob(u32 u){
  u32* p = (u32*) u;
  return SPI_MIO_Stop(p[0], p[1]);//Param1);
}

u32 sq_SPI_MIO_MoveJob(u32 u){
  u32* p = (u32*) u;
  return SPI_MIO_Move(p[0], p[1], p[2], p[3]);
}

u32 sq_SPI_MIO_DMA_Interrupt(u32 u){
  return 0;
}


//======================------------------------>
// This code has been transformed from a hardcoded blocking IO bit bang SPI driver.

//===== 8>< ~~~~ debug end
#define SPI_DELAY 1
static u32 SPI_MIO_SendByte(u32 u, u8 byte)
{
//  u8 byte = 0;
  SPI_MasterIO_t* S = (SPI_MasterIO_t*) u;
  
  // bit 7
  IO_PinSetLow(S->SCK);
  if(byte & 0x80)
    IO_PinSetHigh(S->MOSI);
  else
    IO_PinSetLow(S->MOSI);
  WaitHere(u, 1);
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);
  
  // bit 6
  IO_PinSetLow(S->SCK);
  if(byte & 0x40)
    IO_PinSetHigh(S->MOSI);
  else
    IO_PinSetLow(S->MOSI);
  WaitHere(u, 1);
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

  // bit 5
  IO_PinSetLow(S->SCK);
  if(byte & 0x20)
    IO_PinSetHigh(S->MOSI);
  else
    IO_PinSetLow(S->MOSI);
  WaitHere(u, 1);
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

  // bit 4
  IO_PinSetLow(S->SCK);
  if(byte & 0x10)
    IO_PinSetHigh(S->MOSI);
  else
    IO_PinSetLow(S->MOSI);
  WaitHere(u, 1);
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

  // bit 3
  IO_PinSetLow(S->SCK);
  if(byte & 0x08)
    IO_PinSetHigh(S->MOSI);
  else
    IO_PinSetLow(S->MOSI);
  WaitHere(u, 1);
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

  // bit 2
  IO_PinSetLow(S->SCK);
  if(byte & 0x04)
    IO_PinSetHigh(S->MOSI);
  else
    IO_PinSetLow(S->MOSI);
  WaitHere(u, 1);
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

  // bit 1
  IO_PinSetLow(S->SCK);
  if(byte & 0x02)
    IO_PinSetHigh(S->MOSI);
  else
    IO_PinSetLow(S->MOSI);
  WaitHere(u, 1);
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

  // bit 0
  IO_PinSetLow(S->SCK);
  if(byte & 0x01)
    IO_PinSetHigh(S->MOSI);
  else
    IO_PinSetLow(S->MOSI);
  WaitHere(u, 1);
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);
  
  IO_PinSetLow(S->SCK);
  
  return 0;
}

static u32 SPI_MIO_ReadByte(u32 u)
{
  SPI_MasterIO_t* S = (SPI_MasterIO_t*) u;
  u8 byte = 0;
  
  IO_PinSetInput(S->MOSI); //added by seb for devices which don't have dummy byte in SPI3W mode
  
  // bit 7
  IO_PinSetLow(S->SCK);
  WaitHere(u, 1);
  if(IO_PinRead(S->MOSI))
    byte |= 0x80;
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);
  
  // bit 6
  IO_PinSetLow(S->SCK);
  WaitHere(u, 1);
  if(IO_PinRead(S->MOSI))
    byte |= 0x40;
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

    // bit 5
  IO_PinSetLow(S->SCK);
  WaitHere(u, 1);
  if(IO_PinRead(S->MOSI))
    byte |= 0x20;
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

    // bit 4
  IO_PinSetLow(S->SCK);
  WaitHere(u, 1);
  if(IO_PinRead(S->MOSI))
    byte |= 0x10;
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

    // bit 3
  IO_PinSetLow(S->SCK);
  WaitHere(u, 1);
  if(IO_PinRead(S->MOSI))
    byte |= 0x08;
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

    // bit 2
  IO_PinSetLow(S->SCK);
  WaitHere(u, 1);
  if(IO_PinRead(S->MOSI))
    byte |= 0x04;
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

    // bit 1
  IO_PinSetLow(S->SCK);
  WaitHere(u, 1);
  if(IO_PinRead(S->MOSI))
    byte |= 0x02;
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

    // bit 0
  IO_PinSetLow(S->SCK);
  WaitHere(u, 1);
  if(IO_PinRead(S->MOSI))
    byte |= 0x01;
  
  IO_PinSetHigh(S->SCK);
  WaitHere(u, 1);

  IO_PinSetLow(S->SCK);
  return byte;
}

