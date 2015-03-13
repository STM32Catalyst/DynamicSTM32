
// MIPI RFFE Serial Bus, up to 26MHz (here more like 1 MHz as it is implemented with GPIOs, blocking mode)
// In the example, we use ST's HVDAC code STHVDAC-253M which provides triple 7 bit 20V DAC from 3-5V supply.
// This DAC is primarily used to drive a voltaged driven bias variable capactitor (PARASCAN)

/* Includes ------------------------------------------------------------------*/
#include "SebEngine.h"

//======================------------------------>
u32 RFFE_MIO_Start(u32 u, u32 Command);
u32 RFFE_MIO_Move(u32 u, u32 Param1, u32 Param2, u32 Param3);
u32 RFFE_MIO_BusPark(u32 u);
u32 RFFE_MIO_ReadByte(u32 u);
u32 RFFE_MIO_SendByte(u32 u, u32 byte);
u32 RFFE_MIO_Stop(u32 u, u32 BitMask);
u32 TimerCountdownWait(u32 u);
u32 NopsWait(u32 u);
u32 WaitHere(u32 u, u32 delay);

void NewRFFE_MasterIO_RX_TX(RFFE_MasterIO_t* M) {

  // configure the GPIOs
  if(M->SDATA==0) while(1); // a pin must be hooked here
  if(M->SCLK==0) while(1); // a pin must be hooked here
  
  // configure SDATA pin
  IO_PinClockEnable(M->SDATA);
  IO_PinSetLow(M->SDATA);  
  IO_PinSetOutput(M->SDATA);  
  IO_PinEnablePullUpDown(M->SDATA, DISABLE, ENABLE);
  IO_PinEnableHighDrive(M->SDATA, ENABLE);
 
  // configure SCLK pin
  IO_PinClockEnable(M->SCLK);
  IO_PinSetLow(M->SCLK);
  IO_PinSetOutput(M->SCLK);  
  IO_PinEnablePullUpDown(M->SCLK, DISABLE, ENABLE);
  IO_PinEnableHighDrive(M->SCLK, ENABLE);
  
  // We need to initialize the hooks for DMA_RX...
//  HookIRQ_PPP((u32)S->DMA_RX->Stream, (u32)JobToDo, (u32)S->SA); // From DMA Stream, place the hooks
}

u32 SetRFFE_MasterIO_Timings(RFFE_MasterIO_t* M, u32 MaxBps, MCUClocks_t* T ) { // 1200000, RFFE_CPOL_Low, RFFE_CPHA_1Edge, RFFE_FirstBit_MSB

  u32 HalfClockPeriod_Hz;
  u32 HalfClockPeriod_us;
  
  M->MaxBps = MaxBps; // 400khz
  IO_PinSetSpeedMHz(M->SDATA, 1);
  IO_PinSetSpeedMHz(M->SCLK, 1);
  
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

static u32 TimerCountdownWait(u32 u) {
  
  RFFE_MasterIO_t* M = (RFFE_MasterIO_t*) u;
  ArmTimerCountdown(M->Timer,M->Cn, M->WaitParam * M->ctWaitMethod);
  while(M->Timer->CountDownDone[M->Cn]==0) ;
  return 0;
}

static u32 NopsWait(u32 u) {
  RFFE_MasterIO_t* M = (RFFE_MasterIO_t*) u;
  u32 n = M->ctWaitMethod * M->WaitParam;
  while(n--) asm("nop\n");
  return 0;
}

static u32 WaitHere(u32 u, u32 delay) {
  RFFE_MasterIO_t* M = (RFFE_MasterIO_t*) u;
  M->ctWaitMethod = delay;
  if(M->fnWaitMethod) M->fnWaitMethod(u);
  return 0;
}



//=============-------------->

static u32 RFFE_MIO_Start(u32 u, u32 Command) {

  RFFE_MasterIO_t* S = (RFFE_MasterIO_t*) u;

  S->ParityError = 0;
  IO_PinSetLow(S->SCLK);
  IO_PinSetLow(S->SDATA);
  IO_PinSetOutput(S->SCLK);  
  IO_PinSetOutput(S->SDATA);    
  
// we don't make sure NSS are high at first.
  IO_PinSetOutput(S->SDATA);
  WaitHere(u, 1);  
  IO_PinSetHigh(S->SDATA);
  WaitHere(u, 1);  
  IO_PinSetLow(S->SDATA);  
  WaitHere(u, 1);  
  
  // now we push the bits out?
  // The command is 12 bits, 
  u32 BitMask = 0x800;
  u32 Parity = 0;
  do{ // send all bits from MSB bit 11 down to bit 0, then add parity bit.
    
    if(Command & BitMask) {
      IO_PinSetHigh(S->SDATA);
      Parity++;
    }else{
      IO_PinSetLow(S->SDATA);
    };
    
    IO_PinSetHigh(S->SCLK);
    WaitHere(u, 1);
    IO_PinSetLow(S->SCLK);
    WaitHere(u, 1);
    
    BitMask>>=1;
  }while(BitMask);
  
  // now we push the parity bit out
  // If data is 01100011 P=1, if 01001100 P=0
  if((Parity & 1)==0) {
    IO_PinSetHigh(S->SDATA);
    Parity++;
  }else{
    IO_PinSetLow(S->SDATA);
  };
  
  IO_PinSetHigh(S->SCLK);
  WaitHere(u, 1);
  IO_PinSetLow(S->SCLK);
  WaitHere(u, 1);
  
  return 0; // no call back, next job right away
}


static u32 RFFE_MIO_Stop(u32 u, u32 BitMask) {

//  RFFE_MasterIO_t* S = (RFFE_MasterIO_t*) u;
  RFFE_MIO_BusPark(u);
  Wait_us(1);
  return 0; // no call back, next job right away
}

static u32 RFFE_MIO_Move(u32 u, u32 Param1, u32 Param2, u32 Param3) {
  
  u8* TX = (u8*) Param1;
  u8* RX = (u8*) Param2;
  u32 bCount = Param3;
//  RFFE_MasterIO_t* S = (RFFE_MasterIO_t*) u;
  
  if((Param1)&&(Param2)) { // can't read and write at the same time
    while(1); // problem
  }
  
  if(Param3==0) while(1); // need at least 1 byte to transfer!
  
  // MASTER RECEIVE MODE
  // if the TX adr is null, no bytes to transmit, disable MOSI, and transmit dummy things instead
  if(Param1==0) { // TX=0, RX mode
    
    RFFE_MIO_BusPark(u);
    
    while(bCount--)
      *RX++ = (u8) RFFE_MIO_ReadByte(u);
    
    return 0;
  }

  // MASTER TRANSMIT MODE 
  // if the RX adr is null, no bytes to receive, TRANSMIT MODE
  if(Param2==0) { // RX=0, TX mode
    while(bCount--)
      RFFE_MIO_SendByte(u, *TX++);
  };
  
  return 0; // there is no interrupt generated in this implementation
}

u32 RFFE_MIO_BusPark(u32 u) {

  RFFE_MasterIO_t* S = (RFFE_MasterIO_t*) u;
  
  IO_PinSetHigh(S->SCLK);
  WaitHere(u, 1);  
  IO_PinSetLow(S->SDATA);
  IO_PinSetOutput(S->SDATA);
  WaitHere(u, 1);  
  IO_PinSetLow(S->SCLK);  
    // SDATA becomes input
  IO_PinSetInput(S->SDATA);
  WaitHere(u, 1);  
  
  return 0;
}

u32 RFFE_MIO_ReadByte(u32 u) {
  
  u8 Parity = 1;
  u8 BitMask = 0x80;
  u8 bit;
  RFFE_MasterIO_t* S = (RFFE_MasterIO_t*) u;
  u8 byte = 0;

  do {
    IO_PinSetHigh(S->SCLK);
    WaitHere(u, 1);  
    if(IO_PinGet(S->SDATA)) {
      byte |= BitMask;
      Parity ^= 1;
    };
    IO_PinSetLow(S->SCLK);
    WaitHere(u, 1);  
    BitMask>>=1;
  }while(BitMask);

// read the parity bit here  
  IO_PinSetHigh(S->SCLK);
  WaitHere(u, 1);  
  bit = IO_PinGet(S->SDATA);
  if(bit != Parity) {
      S->ParityError |= 1;
  };
  IO_PinSetLow(S->SCLK);
  WaitHere(u, 1);  
  
  return byte;
}

u32 RFFE_MIO_SendByte(u32 u, u32 byte) {
  
  u32 Parity = 0;
  u32 BitMask = 0x80;
  RFFE_MasterIO_t* S = (RFFE_MasterIO_t*) u;
  
  do{ // send all bits from MSB bit 11 down to bit 0, then add parity bit.
    
    if(byte & BitMask) {
      IO_PinSetHigh(S->SDATA);
      Parity++;
    }else{
      IO_PinSetLow(S->SDATA);
    };
    
    IO_PinSetHigh(S->SCLK);
    WaitHere(u, 1);
    IO_PinSetLow(S->SCLK);
    WaitHere(u, 1);
    
    BitMask>>=1;
  }while(BitMask);
  
  // now we push the parity bit out
  // If data is 01100011 P=1, if 01001100 P=0
  if((Parity & 1)==0) {
    IO_PinSetHigh(S->SDATA);
    Parity++;
  }else{
    IO_PinSetLow(S->SDATA);
  };
  
  IO_PinSetHigh(S->SCLK);
  WaitHere(u, 1);
  IO_PinSetLow(S->SCLK);
  WaitHere(u, 1);
  
  return 0;
}


//==== 8>< ----- Sequencer entry function to cook a communication

u32 sq_RFFE_MIO_StartJob(u32 u) {
  u32* p = (u32*) u;
  return RFFE_MIO_Start(p[0], p[1]);//Param1);//1 parameter
}

// from 3 param down to 1
u32 sq_RFFE_MIO_StopJob(u32 u){
  u32* p = (u32*) u;
  return RFFE_MIO_Stop(p[0], p[1]);//Param1);
}

u32 sq_RFFE_MIO_MoveJob(u32 u){
  u32* p = (u32*) u;
  return RFFE_MIO_Move(p[0], p[1], p[2], p[3]);
}

u32 sq_RFFE_MIO_DMA_Interrupt(u32 u){
  return 0;
}

