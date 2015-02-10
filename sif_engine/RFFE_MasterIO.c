
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

void NewRFFE_MasterIO_RX_TX(RFFE_MasterIO* S, DMA_Stream_TypeDef* RX_Stream, u32 DMA_RX_Channel, DMA_Stream_TypeDef* TX_Stream, u32 DMA_TX_Channel) {

  // configure the GPIOs
  if(S->SDATA==0) while(1); // a pin must be hooked here
  if(S->SCLK==0) while(1); // a pin must be hooked here
  
  // configure SDATA pin
  IO_PinClockEnable(S->SDATA);
  IO_PinSetLow(S->SDATA);  
  IO_PinConfiguredAs(S->SDATA,GPIO_AF17_DIGITAL_OUTPUT);  
  IO_PinSetSpeedMHz(S->SDATA, 1);
  IO_PinEnablePullUp(S->SDATA, DISABLE);
  IO_PinEnablePullDown(S->SDATA, ENABLE);
  IO_PinEnableHighDrive(S->SDATA, ENABLE);
 
  // configure SCLK pin
  IO_PinClockEnable(S->SCLK);
  IO_PinSetLow(S->SCLK);
  IO_PinConfiguredAs(S->SCLK,GPIO_AF17_DIGITAL_OUTPUT);  
  IO_PinSetSpeedMHz(S->SCLK, 1);
  IO_PinEnablePullUp(S->SCLK, DISABLE);
  IO_PinEnablePullDown(S->SCLK, ENABLE);
  IO_PinEnableHighDrive(S->SCLK, ENABLE);
  
  // We need to initialize the hooks for DMA_RX...
//  HookIRQ_PPP((u32)S->DMA_RX->Stream, (u32)JobToDo, (u32)S->SA); // From DMA Stream, place the hooks
  S->fnWaitMethod = TimerCountdownWait; // here we should take care of the timings, and choose the best scheme based on CPU MHz and bps of bus...  
  S->WaitParam = 1;
  // interrupt configuration, only for TX side.... RX is driven by TX DMA which controls the clock pulses generation
  NopsWait((u32)S);
}

u32 SetRFFE_MasterIO_Timings(RFFE_MasterIO* S, u32 MaxBps, u32 CPol, u32 CPha, u32 FirstBit ) { // 1200000, RFFE_CPOL_Low, RFFE_CPHA_1Edge, RFFE_FirstBit_MSB

  // members of the structure related to timings should be initialized
  // Here we should decide which delay strategy for the required speed based on SYSCLK and BT availables
  u32 bps = MaxBps;
  
  S->fnWaitMethod = NopsWait; // here we should take care of the timings, and choose the best scheme based on CPU MHz and bps of bus...  
  S->WaitParam = 1;
  
  S->MaxBps = MaxBps;
  S->ActualBps = bps;
  
  return 0;
}


static u32 TimerCountdownWait(u32 u) {
  
  RFFE_MasterIO* MIO = (RFFE_MasterIO*) u;
  ArmBasicTimerCountdown(MIO->BT,MIO->BTn, MIO->WaitParam);
  while(MIO->BT->CountDownDone[MIO->BTn]==0) ;
  return 0;
}

static u32 NopsWait(u32 u) {
  RFFE_MasterIO* MIO = (RFFE_MasterIO*) u;
  u32 n = MIO->WaitParam;
  while(n--) asm("nop\n");
  return 0;
}

static u32 WaitHere(u32 u, u32 delay) {
  RFFE_MasterIO* MIO = (RFFE_MasterIO*) u;
  MIO->WaitParam = delay;
  if(MIO->fnWaitMethod) MIO->fnWaitMethod(u);
  return 0;
}

//=============-------------->

static u32 RFFE_MIO_Start(u32 u, u32 Command) {

  RFFE_MasterIO* S = (RFFE_MasterIO*) u;

  S->ParityError = 0;
  IO_PinSetLow(S->SCLK);
  IO_PinSetLow(S->SDATA);
  IO_PinConfiguredAs(S->SCLK,GPIO_AF17_DIGITAL_OUTPUT);  
  IO_PinConfiguredAs(S->SDATA,GPIO_AF17_DIGITAL_OUTPUT);    
  
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

//  RFFE_MasterIO* S = (RFFE_MasterIO*) u;
  RFFE_MIO_BusPark(u);
  Wait_us(1);
  return 0; // no call back, next job right away
}

static u32 RFFE_MIO_Move(u32 u, u32 Param1, u32 Param2, u32 Param3) {
  
  u8* TX = (u8*) Param1;
  u8* RX = (u8*) Param2;
  u32 bCount = Param3;
//  RFFE_MasterIO* S = (RFFE_MasterIO*) u;
  
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

  RFFE_MasterIO* S = (RFFE_MasterIO*) u;
  
  IO_PinSetHigh(S->SCLK);
  WaitHere(u, 1);  
  IO_PinSetLow(S->SDATA);
  IO_PinSetOutput(S->SDATA);
  WaitHere(u, 1);  
  IO_PinSetLow(S->SCLK);  
    // SDATA becomes input
  IO_PinConfiguredAs(S->SDATA,GPIO_AF16_DIGITAL_INPUT); 
  WaitHere(u, 1);  
  
  return 0;
}

u32 RFFE_MIO_ReadByte(u32 u) {
  
  u8 Parity = 1;
  u8 BitMask = 0x80;
  u8 bit;
  RFFE_MasterIO* S = (RFFE_MasterIO*) u;
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
  RFFE_MasterIO* S = (RFFE_MasterIO*) u;
  
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


//===================================================
// Now it is time to check if the communication works...

#define REG_WRITE               (2<<5)
#define REG_READ                (3<<5)
#define EXT_REG_WRITE(bc)       (bc)

#define USID                    (7<<8)

#define HVDAC_WRITE_REG(a)      ( USID | REG_WRITE | (a) )
#define HVDAC_READ_REG(a)       ( USID | REG_READ  | (a) )
#define HVDAC_EXT_WRITE(bc)     ( USID | EXT_REG_WRITE(bc) )

#define DAC_C           0
#define DAC_B           1
#define DAC_A           2
#define PWR_TRIG        28
#define PROD_ID         29
#define MANU_ID         30
#define MANU_ID2        31

u8 HVDAC_Regs[32];

static RFFE_MasterIO myRFFE;
static IO_PinTypeDef RFFE_SDATA, RFFE_SCLK;
static u32 RFFE_List[100];
static StuffsArtery mySequenceRFFE;
static BasicTimer BT;

const OneJobType RFFE_StartRegWriteDACA = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_WRITE_REG(DAC_A)} };
const OneJobType RFFE_StartRegWriteDACB = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_WRITE_REG(DAC_B)} };
const OneJobType RFFE_StartRegWriteDACC = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_WRITE_REG(DAC_C)} };
const OneJobType RFFE_StartRegWritePWR_TRIG = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_WRITE_REG(PWR_TRIG)} };

const OneJobType RFFE_StartRegReadDACA = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(DAC_A)} };
const OneJobType RFFE_StartRegReadDACB = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(DAC_B)} };
const OneJobType RFFE_StartRegReadDACC = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(DAC_C)} };
const OneJobType RFFE_StartRegReadPWR_TRIG = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(PWR_TRIG)} };
const OneJobType RFFE_StartRegReadProdID = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(PROD_ID)} };
const OneJobType RFFE_StartRegReadManuID = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(MANU_ID)} };
const OneJobType RFFE_StartRegReadManuID2 = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(MANU_ID2)} };

const OneJobType RFFE_WriteDACA = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, (u32)&HVDAC_Regs[DAC_A], 0, 1} };
const OneJobType RFFE_WriteDACB = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, (u32)&HVDAC_Regs[DAC_B], 0, 1} };
const OneJobType RFFE_WriteDACC = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, (u32)&HVDAC_Regs[DAC_C], 0, 1} };
const OneJobType RFFE_WritePWR_TRIG = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, (u32)&HVDAC_Regs[PWR_TRIG], 0, 1} };

const OneJobType RFFE_ReadDACA = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[DAC_A], 1} };
const OneJobType RFFE_ReadDACB = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[DAC_B], 1} };
const OneJobType RFFE_ReadDACC = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[DAC_C], 1} };
const OneJobType RFFE_ReadPWR_TRIG = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[PWR_TRIG], 1} };
const OneJobType RFFE_ReadProdID = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[PROD_ID], 1} };
const OneJobType RFFE_ReadManuID = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[MANU_ID], 1} };
const OneJobType RFFE_ReadManuID2 = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[MANU_ID2], 1} };

const OneJobType RFFE_Stop = { sq_RFFE_MIO_StopJob,  {(u32)&myRFFE, 0} };


// we play with HVDAC RFFE device : STHVDAC-253M 3 output 7 bit high voltage DAC with embedded charge pump.
// That device could become a higher abstract cell later on...
void RFFE_Test(void) {
  
  IO_PinInit(&RFFE_SDATA, PI6);
  IO_PinInit(&RFFE_SCLK, PI5);
  myRFFE.SDATA = &RFFE_SDATA;
  myRFFE.SCLK = &RFFE_SCLK;

  StuffsArtery* P = &mySequenceRFFE; // program
  NewSA(P, (u32)&RFFE_List[0], countof(RFFE_List));
  myRFFE.SA = P;
  
  BT.FeedClockMHz = 96/2; // 96MHz (this should later come from RAM global structure)  
  NewBasicTimer_us(&BT, TIM6, 1); // usec countdown
  myRFFE.BT = &BT;
  
  NewRFFE_MasterIO_RX_TX(&myRFFE, 0, 0, 0, 0); // this will configure the IO pins too.
  SetRFFE_MasterIO_Timings(&myRFFE, 26000000, 0, 0, 0 ); // RFFE max clock speed is 26Mhz
  
  // Now we create a sequence to try on RFFE...
  
  while(1) {

    AddToSA(P, (u32) &RFFE_StartRegReadDACA);
    AddToSA(P, (u32) &RFFE_ReadDACA);
    AddToSA(P, (u32) &RFFE_Stop);

    AddToSA(P, (u32) &RFFE_StartRegReadDACB);
    AddToSA(P, (u32) &RFFE_ReadDACB);
    AddToSA(P, (u32) &RFFE_Stop);
    
    AddToSA(P, (u32) &RFFE_StartRegReadDACC);
    AddToSA(P, (u32) &RFFE_ReadDACC);
    AddToSA(P, (u32) &RFFE_Stop);
    
    AddToSA(P, (u32) &RFFE_StartRegReadPWR_TRIG);
    AddToSA(P, (u32) &RFFE_ReadPWR_TRIG);
    AddToSA(P, (u32) &RFFE_Stop);

    AddToSA(P, (u32) &RFFE_StartRegReadProdID);
    AddToSA(P, (u32) &RFFE_ReadProdID);    
    AddToSA(P, (u32) &RFFE_Stop);
    
    AddToSA(P, (u32) &RFFE_StartRegReadManuID);
    AddToSA(P, (u32) &RFFE_ReadManuID);
    AddToSA(P, (u32) &RFFE_Stop);
    
    AddToSA(P, (u32) &RFFE_StartRegReadManuID2);
    AddToSA(P, (u32) &RFFE_ReadManuID2);
    AddToSA(P, (u32) &RFFE_Stop);

    StartJobToDoInForeground((u32)&mySequenceRFFE);
    while(mySequenceRFFE.FlagEmptied==0);
    NOPs(1);
    mySequenceRFFE.FlagEmptied = 0; // clear the flag here

    HVDAC_Regs[DAC_A] = 0x55;
    AddToSA(P, (u32) &RFFE_StartRegWriteDACA);
    AddToSA(P, (u32) &RFFE_WriteDACA);
    AddToSA(P, (u32) &RFFE_Stop);

    HVDAC_Regs[DAC_B] = 0xAA;
    AddToSA(P, (u32) &RFFE_StartRegWriteDACB);
    AddToSA(P, (u32) &RFFE_WriteDACB);
    AddToSA(P, (u32) &RFFE_Stop);

    HVDAC_Regs[DAC_C] = 0x5A;
    AddToSA(P, (u32) &RFFE_StartRegWriteDACC);
    AddToSA(P, (u32) &RFFE_WriteDACC);
    AddToSA(P, (u32) &RFFE_Stop);
    
  // add this to the head of the program 
/*    StartJobToDoInForeground((u32)&mySequenceRFFE);
    while(mySequenceRFFE.FlagEmptied==0);
    NOPs(1);
    mySequenceRFFE.FlagEmptied = 0; // clear the flag here
*/    
  };// loop
  
}