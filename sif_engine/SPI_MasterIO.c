
#include "SebEngine.h"
#include "SPI_MasterIO.h"

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

void NewSPI_MasterIO_RX_TX(SPI_MasterIO* S, DMA_Stream_TypeDef* RX_Stream, u32 DMA_RX_Channel, DMA_Stream_TypeDef* TX_Stream, u32 DMA_TX_Channel) {

  u8 n; // NSSs sweep
// we have to initialize the state machine first
  
  // configure the GPIOs
  if(S->MOSI!=S->MISO) while(1); // MOSI must be MISO to work in this bidir only implementation
  
  // configure MISO pin
  IO_PinClockEnable(S->MISO);
  IO_PinSetHigh(S->MISO);  
  IO_PinConfiguredAs(S->MISO,GPIO_AF16_DIGITAL_INPUT);  
  IO_PinSetSpeedMHz(S->MISO, 1);
  IO_PinEnablePullUp(S->MISO, ENABLE);
  IO_PinEnablePullDown(S->MISO, DISABLE);
  IO_PinEnableHighDrive(S->MISO, ENABLE);
 
  // configure MOSI pin
  IO_PinClockEnable(S->MOSI);
  IO_PinSetHigh(S->MOSI);
  IO_PinConfiguredAs(S->MOSI,GPIO_AF17_DIGITAL_OUTPUT);  
  IO_PinSetSpeedMHz(S->MOSI, 1);
  IO_PinEnablePullUp(S->MOSI, ENABLE);
  IO_PinEnablePullDown(S->MOSI, DISABLE);
  IO_PinEnableHighDrive(S->MOSI, ENABLE);

  // configure SCK pin
  IO_PinClockEnable(S->SCK);
  IO_PinSetHigh(S->SCK);
  IO_PinConfiguredAs(S->SCK,GPIO_AF17_DIGITAL_OUTPUT);  
  IO_PinSetSpeedMHz(S->SCK, 1);
  IO_PinEnablePullUp(S->SCK, ENABLE);
  IO_PinEnablePullDown(S->SCK, DISABLE);
  IO_PinEnableHighDrive(S->SCK, ENABLE);
  
  // configure all the NSSs pins
  for(n=0;n<16;n++) {
    if(S->NSSs[n]==0) break;
    IO_PinClockEnable(S->NSSs[n]);
    IO_PinSetHigh(S->NSSs[n]);    
    IO_PinConfiguredAs(S->NSSs[n],GPIO_AF17_DIGITAL_OUTPUT);  
    IO_PinSetSpeedMHz(S->NSSs[n], 1);
    IO_PinEnablePullUp(S->NSSs[n], ENABLE);
    IO_PinEnablePullDown(S->NSSs[n], DISABLE);
    IO_PinEnableHighDrive(S->NSSs[n], ENABLE); // push pull enabled
  };
  
  // We need to initialize the hooks for DMA_RX...
//  HookIRQ_PPP((u32)S->DMA_RX->Stream, (u32)JobToDo, (u32)S->SA); // From DMA Stream, place the hooks
  S->fnWaitMethod = TimerCountdownWait; // here we should take care of the timings, and choose the best scheme based on CPU MHz and bps of bus...  
  S->WaitParam = 1;
  // interrupt configuration, only for TX side.... RX is driven by TX DMA which controls the clock pulses generation
  NopsWait((u32)S);
}

u32 SetSPI_MasterIO_Timings(SPI_MasterIO* S, u32 MaxBps, u32 CPol, u32 CPha, u32 FirstBit ) { // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB

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
  
  SPI_MasterIO* MIO = (SPI_MasterIO*) u;
  ArmBasicTimerCountdown(MIO->BT,MIO->BTn, MIO->WaitParam);
  while(MIO->BT->CountDownDone[MIO->BTn]==0) ;
  return 0;
}

static u32 NopsWait(u32 u) {
  SPI_MasterIO* MIO = (SPI_MasterIO*) u;
  u32 n = MIO->WaitParam;
  while(n--) asm("nop\n");
  return 0;
}

static u32 WaitHere(u32 u, u32 delay) {
  SPI_MasterIO* MIO = (SPI_MasterIO*) u;
  MIO->WaitParam = delay;
  if(MIO->fnWaitMethod) MIO->fnWaitMethod(u);
  return 0;
}




//=============-------------->

static u32 SPI_MIO_Start(u32 u, u32 BitMask) {

  u8 n;
  SPI_MasterIO* S = (SPI_MasterIO*) u;

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
  SPI_MasterIO* S = (SPI_MasterIO*) u;
  
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
//  SPI_MasterIO* S = (SPI_MasterIO*) u;
  
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

#define SPIIO_Set_CLK_Low               IO_PinSetLow(S->SCK)
#define SPIIO_Set_CLK_High              IO_PinSetHigh(S->SCK)
#define SPIIO_Set_CLK_Out               IO_PinSetOutput(S->SCK)

#define SPIIO_Set_SDA_Low               IO_PinSetLow(S->MOSI)
#define SPIIO_Set_SDA_High              IO_PinSetHigh(S->MOSI)
#define SPIIO_Set_SDA_In                IO_PinSetInput(S->MOSI)
#define SPIIO_Set_SDA_Out               IO_PinSetOutput(S->MOSI)
#define SPIIO_Get_SDA                   IO_PinRead(S->MOSI)

//===== 8>< ~~~~ debug end
#define SPI_DELAY 1
static u32 SPI_MIO_SendByte(u32 u, u8 byte)
{
//  u8 byte = 0;
  SPI_MasterIO* S = (SPI_MasterIO*) u;
  
  // bit 7
  SPIIO_Set_CLK_Low;
  if(byte & 0x80)
    SPIIO_Set_SDA_High;
  else
    SPIIO_Set_SDA_Low;
  WaitHere(u, 1);
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);
  
  // bit 6
  SPIIO_Set_CLK_Low;
  if(byte & 0x40)
    SPIIO_Set_SDA_High;
  else
    SPIIO_Set_SDA_Low;
  WaitHere(u, 1);
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

  // bit 5
  SPIIO_Set_CLK_Low;
  if(byte & 0x20)
    SPIIO_Set_SDA_High;
  else
    SPIIO_Set_SDA_Low;
  WaitHere(u, 1);
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

  // bit 4
  SPIIO_Set_CLK_Low;
  if(byte & 0x10)
    SPIIO_Set_SDA_High;
  else
    SPIIO_Set_SDA_Low;
  WaitHere(u, 1);
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

  // bit 3
  SPIIO_Set_CLK_Low;
  if(byte & 0x08)
    SPIIO_Set_SDA_High;
  else
    SPIIO_Set_SDA_Low;
  WaitHere(u, 1);
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

  // bit 2
  SPIIO_Set_CLK_Low;
  if(byte & 0x04)
    SPIIO_Set_SDA_High;
  else
    SPIIO_Set_SDA_Low;
  WaitHere(u, 1);
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

  // bit 1
  SPIIO_Set_CLK_Low;
  if(byte & 0x02)
    SPIIO_Set_SDA_High;
  else
    SPIIO_Set_SDA_Low;
  WaitHere(u, 1);
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

  // bit 0
  SPIIO_Set_CLK_Low;
  if(byte & 0x01)
    SPIIO_Set_SDA_High;
  else
    SPIIO_Set_SDA_Low;
  WaitHere(u, 1);
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);
  
  SPIIO_Set_CLK_Low;
  
  return 0;
}

static u32 SPI_MIO_ReadByte(u32 u)
{
  SPI_MasterIO* S = (SPI_MasterIO*) u;
  u8 byte = 0;
  
  SPIIO_Set_SDA_In; //added by seb for devices which don't have dummy byte in SPI3W mode
  
  // bit 7
  SPIIO_Set_CLK_Low;
  WaitHere(u, 1);
  if(SPIIO_Get_SDA)
    byte |= 0x80;
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);
  
  // bit 6
  SPIIO_Set_CLK_Low;
  WaitHere(u, 1);
  if(SPIIO_Get_SDA)
    byte |= 0x40;
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

    // bit 5
  SPIIO_Set_CLK_Low;
  WaitHere(u, 1);
  if(SPIIO_Get_SDA)
    byte |= 0x20;
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

    // bit 4
  SPIIO_Set_CLK_Low;
  WaitHere(u, 1);
  if(SPIIO_Get_SDA)
    byte |= 0x10;
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

    // bit 3
  SPIIO_Set_CLK_Low;
  WaitHere(u, 1);
  if(SPIIO_Get_SDA)
    byte |= 0x08;
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

    // bit 2
  SPIIO_Set_CLK_Low;
  WaitHere(u, 1);
  if(SPIIO_Get_SDA)
    byte |= 0x04;
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

    // bit 1
  SPIIO_Set_CLK_Low;
  WaitHere(u, 1);
  if(SPIIO_Get_SDA)
    byte |= 0x02;
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

    // bit 0
  SPIIO_Set_CLK_Low;
  WaitHere(u, 1);
  if(SPIIO_Get_SDA)
    byte |= 0x01;
  
  SPIIO_Set_CLK_High;
  WaitHere(u, 1);

  SPIIO_Set_CLK_Low;
  return byte;
}



