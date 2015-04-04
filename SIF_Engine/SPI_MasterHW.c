
#include "sebEngine.h"
//~~~~~~~~~~~~~~~~~~~~~ global variables to use differently~~~~~~~~~~~

// Some notes on performance
// Right now we don't use the double buffer scheme of the DMA, so there is a pause between transferred blocks

// 96MHz M437                NSS low to TX out to TX byte       Between 2 TX blocks        Between TX and RX blocks       Last byte to NSS High
// Std lib no optimisation       8.6 + 5.1 us                           7.1 us                    7.1 us                         2.2 us 
// no std lib, no opt            8.6 + 2.1 us                           4.2 us                    4.0 us                         1.9 us
// no std lib, speed opt         7.5 + 1.5 us                           3.2 us                    3.1 us                         1.3 us


static u8 Dummy[1024];

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


void NewSPI_MasterHW(SPI_MasterHW_t* S, SPI_TypeDef* SPI, IO_Pin_t* MISO, IO_Pin_t* MOSI, IO_Pin_t* SCK, IO_Pin_t* NSS0) {
  
  if(S==0) while(1);
  if(SPI==0) while(1);
  if(SCK==0) while(1);
  if((MISO==0)&&(MOSI==0)) while(1);

  S->SPI = SPI;
  S->MISO = MISO;
  S->MOSI = MOSI;
  S->SCK = SCK;
  S->NSSs[0] = NSS0;
  
  ClockGateEnable_PPP((u32)S->SPI, ENABLE);  // Clock enable SPI HW
  
  // here we have to find out which DMA stream and channel fits the needs automatically.
}

typedef struct {
  SPI_TypeDef* PPP;
  SignalName_t RX;
  SignalName_t TX;
} SPI_Info_t;


const SPI_Info_t SPII[] = {
  
  { SPI1, SPI1_RX, SPI1_TX },
  { SPI2, SPI2_RX, SPI2_TX },
  { SPI3, SPI3_RX, SPI3_TX },
  { SPI4, SPI4_RX, SPI4_TX },
  { SPI5, SPI5_RX, SPI5_TX },
  { SPI6, SPI6_RX, SPI6_TX },
};

SPI_Info_t* SPI_GetInfo(SPI_TypeDef* PPP) {

  u8 n;
  
  for(n=0;n<countof(SPII);n++)    
      if(PPP==SPII[n].PPP) // the stream is a memory location, it is unique
        return (SPI_Info_t*) &SPII[n];
  
  return 0; // failed to find it
}

void ConfigureSPI_MasterHW(SPI_MasterHW_t* S) {
  
  NVIC_InitTypeDef NVIC_InitStructure;  
//  SPI_InitTypeDef  SPI_InitStructure;
//  DMA_InitTypeDef DMA_TX_InitStruct,DMA_RX_InitStruct;
  
  u8 n; // NSSs sweep

  // configure MISO pin
  SetPinInput(S->MISO);
  IO_PinConfiguredAs(S->MISO,GetPinAF(S->MISO->Name,(u32)S->SPI));  

  // configure MOSI pin
  SetPinOutput(S->MOSI);
  IO_PinConfiguredAs(S->MOSI,GetPinAF(S->MOSI->Name,(u32)S->SPI));    

  // configure SCK pin
  SetPinOutput(S->SCK);
  IO_PinConfiguredAs(S->SCK,GetPinAF(S->SCK->Name,(u32)S->SPI));    
  
  // configure all the NSSs pins
  for(n=0;n<16;n++)
    if(S->NSSs[n])
      SetPinOutput(S->NSSs[n]);

  //---- Here we patch the code to automatically find the right DMA and DMA stream for this SPI...
  SPI_Info_t* SPII = SPI_GetInfo(S->SPI);
  DMA_StreamChannelInfo_t* TXSCI = Get_pStreamChannelForPPP_Signal((u32)SPII->PPP, SPII->TX, 0);
  DMA_StreamChannelInfo_t* RXSCI = Get_pStreamChannelForPPP_Signal((u32)SPII->PPP, SPII->RX, 0);
  // override the passing parameters
  DMA_Stream_TypeDef* RX_Stream = RXSCI->Stream;
  u32 DMA_RX_Channel = RXSCI->Channel;
  DMA_Stream_TypeDef* TX_Stream = TXSCI->Stream;
  u32 DMA_TX_Channel = TXSCI->Channel;
  //=============================
  
  // DMA TX
  S->DMA_TX = Get_pDMA_Info(TX_Stream);

  // DMA RX  
  S->DMA_RX = Get_pDMA_Info(RX_Stream);

// DMA matters
  ClockGateEnable_PPP((u32)S->DMA_RX->DMA, ENABLE);  // Clock enable DMA stream RX
  ClockGateEnable_PPP((u32)S->DMA_TX->DMA, ENABLE);  // Clock enable DMA stream TX
  
  // Next, configure the TX and RX stream DMAs (if exist, otherwise, stop)
  /* Initialize the DMA_Channel member */
  S->DMA_TXI.DMA_Channel = DMA_TX_Channel;
//  S->TX_Channel = DMA_TX_Channel; // for debugging
  S->DMA_RXI.DMA_Channel = DMA_RX_Channel;
//  S->RX_Channel = DMA_RX_Channel; // for debugging
  /* Initialize the DMA_PeripheralBaseAddr member */
  S->DMA_TXI.DMA_PeripheralBaseAddr = (u32) (&(S->SPI->DR));
  S->DMA_RXI.DMA_PeripheralBaseAddr = (u32) (&(S->SPI->DR));
  /* Initialize the DMA_Memory0BaseAddr member */
  S->DMA_TXI.DMA_Memory0BaseAddr = 0;
  S->DMA_RXI.DMA_Memory0BaseAddr = 0;
  /* Initialize the DMA_DIR member */
  S->DMA_TXI.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  S->DMA_RXI.DMA_DIR = DMA_DIR_PeripheralToMemory;
  /* Initialize the DMA_BufferSize member */
  S->DMA_TXI.DMA_BufferSize = 0;
  S->DMA_RXI.DMA_BufferSize = 0;
  /* Initialize the DMA_PeripheralInc member */
  S->DMA_TXI.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  S->DMA_RXI.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  /* Initialize the DMA_MemoryInc member */
  S->DMA_TXI.DMA_MemoryInc = DMA_MemoryInc_Enable;
  S->DMA_RXI.DMA_MemoryInc = DMA_MemoryInc_Enable;
  /* Initialize the DMA_PeripheralDataSize member */
  S->DMA_TXI.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  S->DMA_RXI.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /* Initialize the DMA_MemoryDataSize member */
  S->DMA_TXI.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  S->DMA_RXI.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  /* Initialize the DMA_Mode member */
  S->DMA_TXI.DMA_Mode = DMA_Mode_Normal;
  S->DMA_RXI.DMA_Mode = DMA_Mode_Normal;
  /* Initialize the DMA_Priority member */
  S->DMA_TXI.DMA_Priority = DMA_Priority_High;
  S->DMA_RXI.DMA_Priority = DMA_Priority_High;
  /* Initialize the DMA_FIFOMode member */
  S->DMA_TXI.DMA_FIFOMode = DMA_FIFOMode_Disable;
  S->DMA_RXI.DMA_FIFOMode = DMA_FIFOMode_Disable;
  /* Initialize the DMA_FIFOThreshold member */
  S->DMA_TXI.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  S->DMA_RXI.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  /* Initialize the DMA_MemoryBurst member */
  S->DMA_TXI.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  S->DMA_RXI.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  /* Initialize the DMA_PeripheralBurst member */
  S->DMA_TXI.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  S->DMA_RXI.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  //==<<=<<=<<===============================================
  DMA_Init(S->DMA_RX->Stream, &S->DMA_RXI);
  //==>>=>>=>>===============================================
  DMA_Init(S->DMA_TX->Stream, &S->DMA_TXI);
  
  BookDMA_Stream(S->DMA_RX->Stream);
  BookDMA_Stream(S->DMA_TX->Stream);
  // Wire NVIC Interrupts for DMA TX only (it is the only way to get interrupt when all the bits have been shifted in or out. RX is too early. BUSY might even be better!

  // We need to initialize the hooks for DMA_RX...
  HookIRQ_PPP((u32)S->DMA_RX->Stream, (u32)JobToDo, (u32)S->SA); // From DMA Stream, place the hooks
  
  // interrupt configuration, only for TX side.... RX is driven by TX DMA which controls the clock pulses generation
  NVIC_InitStructure.NVIC_IRQChannel = S->DMA_TX->Stream_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; //sebENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  // interrupt configuration, only for RX side.... RX is driven by TX DMA which controls the clock pulses generation
  NVIC_InitStructure.NVIC_IRQChannel = S->DMA_RX->Stream_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

static const u32 SPI_MasterHW_Psc[] = {
 SPI_BaudRatePrescaler_2,
 SPI_BaudRatePrescaler_4,
 SPI_BaudRatePrescaler_8,
 SPI_BaudRatePrescaler_16,
 SPI_BaudRatePrescaler_32,
 SPI_BaudRatePrescaler_64,
 SPI_BaudRatePrescaler_128,
 SPI_BaudRatePrescaler_256 
};

  

void SetSPI_MasterHW_Timings(SPI_MasterHW_t* S, u32 MinBps, u32 MaxBps ) { // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB

  // members of the structure related to timings should be initialized
  u32 IsAPB1 = (GetSignal2InfoBy_PPP((u32)S->SPI)->fnClk == RCC_APB1PeriphClockCmd); // clock domain (is it a fast or slow SPI? APB1 slow or APB2 fast?) 
  u32 bps, psc;
  u32 Hz;
  
  // find the feeding clock directly

  if(IsAPB1) {
    Hz = S->Clocks->OutAPB1Clk_Hz.Value;
  }else{
    Hz = S->Clocks->OutAPB2Clk_Hz.Value;
  };
    
  S->Bps.Min = MinBps;

  for(psc=0;psc<8;psc++) { // try all possible prescaler values
    //bps = (1000000 * S->FeedClockMHz) >> (psc + 1);
    bps = (Hz) >> (psc + 1);    
    if(bps<=MinBps) break;
  };
  
  if(psc>7) while(1); // failed to find proper prescaler. Was the FeedClockMHz initialized?
  
  // we have found the highest compatible clock
  S->Bps.Value = bps;
//  SPI_I2S_DeInit(S->SPI);
  S->SPII.SPI_BaudRatePrescaler = SPI_MasterHW_Psc[psc]; //SPI_BaudRatePrescaler_8;// Depends on clock info in SPI structure

  if((IsAPB1==TRUE) && (S->Clocks->OutAPB1Clk_Hz.Value==0))
    S->Clocks->OutAPB1Clk_Hz.Min = MinBps * 2; // or 4 depending on which clock domain is used

  if((IsAPB1==FALSE) && (S->Clocks->OutAPB1Clk_Hz.Value==0))
    S->Clocks->OutAPB2Clk_Hz.Min = MinBps * 4; // or 4 depending on which clock domain is used
}

void SetSPI_MasterHW_Format(SPI_MasterHW_t* S, u32 CPol, u32 CPha, u32 FirstBit ) {

  S->SPII.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// Imposed. For 3 wires, just short MISO MOSI pins externally. Don't use Bidir to save pins with this code.
  S->SPII.SPI_DataSize = SPI_DataSize_8b; // fixed support
  S->SPII.SPI_CPOL = CPol;//SPI_CPOL_Low; // User choose
  S->SPII.SPI_CPHA = CPha;//SPI_CPHA_1Edge; // User choose
  S->SPII.SPI_NSS = SPI_NSS_Soft;// Fixed
  S->SPII.SPI_FirstBit = FirstBit;//SPI_FirstBit_MSB; // USer choose
  S->SPII.SPI_CRCPolynomial = 7; // Fixed for now
  S->SPII.SPI_Mode = SPI_Mode_Master; // Fixed for this driver
  
  SPI_Init(S->SPI, &S->SPII);  
}

static void DMA_Interrupt(u32 u, FunctionalState Enable) {
  SPI_MasterHW_t* S = (SPI_MasterHW_t*) u;
  DMA_ClearFlag(S->DMA_RX->Stream, S->DMA_RX->Flags); // clear all flags
  DMA_ITConfig(S->DMA_RX->Stream, DMA_IT_TC, Enable); // enable transfer complete interrupt  
}

static void SPI_Disable_MOSI(u32 u) {

  SPI_MasterHW_t* S = (SPI_MasterHW_t*) u;
  u32 Name = S->MOSI->Name;
  GPIO_TypeDef* GPIOx = GPIOs[Name>>4];
  
  ClearAndSetShiftedBits(GPIOx->MODER, 3, 0, (Name & 0xF)*2);
/*  // make MOSI as floating and send a dummy byte
  GPIOG->MODER  &= ~(0x03 << (14 * 2)); // [00] force MOSI to be input (not force output)
  //GPIOG->MODER  |= (0x02 << (14 * 2)); // [10] force MOSI to be AF output (force output)
*/  
}


static void SPI_Enable_MOSI(u32 u) {
  
  SPI_MasterHW_t* S = (SPI_MasterHW_t*) u;
  u32 Name = S->MOSI->Name;
  GPIO_TypeDef* GPIOx = GPIOs[Name>>4];
  
  ClearAndSetShiftedBits(GPIOx->MODER, 3, 2, (Name & 0xF)*2);
/*  // make sure MOSI is output
  GPIOG->MODER  &= ~(0x03 << (14 * 2)); // [00] force MOSI to be input (not force output)
  GPIOG->MODER  |= (0x02 << (14 * 2)); // [10] force MOSI to be AF output (force output)
*/  
}



//==============================================

//=============== These are the optional I2C spy mode to see what is happening on a bus.
// the slave function is then disabled


//=================================================

static u32 SPI_Start(u32 u, u32 BitMask) {

  u8 n;
  SPI_MasterHW_t* S = (SPI_MasterHW_t*) u;
  
  S->SPI->CR1 |= SPI_CR1_SPE;//#SPI_Cmd(S->SPI, ENABLE); // this should trigger the interrupt for sending the first byte... enable the spi adter the interrupt enable
  
  for(n=0;n<16;n++) { // put low all the selected pins
    if(S->NSSs[n]==0) break;
    
    if(BitMask & (1<<n)) // if no, continue
        IO_PinSetLow(S->NSSs[n]);
  };
  
  DMA_Interrupt(u, ENABLE); // should not trigger any transfer, the DMA counts should be zero here
  Wait_us(1); //+
  return 0; // no call back, next job right away
}

static u32 SPI_Stop(u32 u, u32 BitMask) {

  u8 n;
  SPI_MasterHW_t* S = (SPI_MasterHW_t*) u;
  for(n=0;n<16;n++) { // put low all the selected pins
    if(S->NSSs[n]==0) break;
    
    if(BitMask & (1<<n)) // if no, continue
        IO_PinSetHigh(S->NSSs[n]);
  };
  S->SPI->CR1 &= ~((u16)SPI_CR1_SPE);//#SPI_Cmd(S->SPI, DISABLE);
  DMA_Interrupt(u, DISABLE);
  Wait_us(1);
  return 0; // no call back, next job right away
}


// this code has been optimized for speed, it represent the time delay between DMA block transfers
// further optimization would be to use double buffer scheme...
#define RESERVED_MASK           (uint32_t)0x0F7D0F7D  
static void DMA_TX_Set(u32 u, u32 blockstart, u16 nbofbytes) {

  SPI_MasterHW_t* S = (SPI_MasterHW_t*) u; 
  DMA_Stream_TypeDef* TX_Stream = S->DMA_TX->Stream;
// first we have to clear all flags
//  if(DMA_GetFlagStatus(DMA_TX_Stream, DMA_RX_Flags)!=DMA_TX_TC_Flag)
//    while(1) __NOP();
  //DMA_ClearFlag(TX_Stream, S->DMA_TX->Flags);
  if(S->DMA_TX->fn_ct_index & 4) {
    S->DMA_TX->DMA->HIFCR = (u32)(S->DMA_TX->Flags & RESERVED_MASK);;
  }else{
    S->DMA_TX->DMA->LIFCR = (u32)(S->DMA_TX->Flags & RESERVED_MASK);;
  }
  
  //DMA_Cmd(TX_Stream, DISABLE);
  TX_Stream->CR &= ~(u32)DMA_SxCR_EN;
  
  //while(DMA_GetCmdStatus(TX_Stream)==ENABLE) __NOP(); // wait for the DMA to be completely disabled
  while(TX_Stream->CR & (u32)DMA_SxCR_EN) ;
  
//  S->DMA_TX_InitStruct.DMA_BufferSize = nbofbytes;
//  S->DMA_TX_InitStruct.DMA_Memory0BaseAddr = blockstart;
  //DMA_Init(TX_Stream, &S->DMA_TX_InitStruct);
  TX_Stream->M0AR = blockstart;
  TX_Stream->NDTR = nbofbytes;
  
  //DMA_Cmd(TX_Stream, ENABLE);
  TX_Stream->CR |= (u32)DMA_SxCR_EN;
  //=========------------>
  //SPI_I2S_DMACmd(S->SPI, SPI_I2S_DMAReq_Tx, ENABLE);
  S->SPI->CR2 |= SPI_I2S_DMAReq_Tx;
}

static void DMA_RX_Set(u32 u, u32 blockstart, u16 nbofbytes) {
  
  SPI_MasterHW_t* S = (SPI_MasterHW_t*) u; 
  DMA_Stream_TypeDef* RX_Stream = S->DMA_RX->Stream;
// first we have to clear all flags
//  if(DMA_GetFlagStatus(DMA_TX_Stream, DMA_RX_Flags)!=DMA_TX_TC_Flag)
//    while(1) __NOP();
  
//  DMA_ClearFlag(RX_Stream, S->DMA_RX->Flags);
  if(S->DMA_RX->fn_ct_index & 4) {
    S->DMA_RX->DMA->HIFCR = (u32)(S->DMA_RX->Flags & RESERVED_MASK);;
  }else{
    S->DMA_RX->DMA->LIFCR = (u32)(S->DMA_RX->Flags & RESERVED_MASK);;
  }
  
  
//  DMA_Cmd(RX_Stream, DISABLE);
  RX_Stream->CR &= ~(u32)DMA_SxCR_EN;
  
//  while(DMA_GetCmdStatus(RX_Stream)==ENABLE) __NOP(); // wait for the DMA to be completely disabled
  while(RX_Stream->CR & (u32)DMA_SxCR_EN) ;
  //=========------->
  
  //SPI_I2S_ReceiveData(S->SPI);  // in case clear any SPI RX flag before DMA RX kicks in!
  Dummy[0] = S->SPI->DR;
  //=========------->
  
//  S->DMA_RX_InitStruct.DMA_BufferSize = nbofbytes;
//  S->DMA_RX_InitStruct.DMA_Memory0BaseAddr = blockstart;
//  DMA_Init(RX_Stream, &S->DMA_RX_InitStruct);
  RX_Stream->M0AR = blockstart;
  RX_Stream->NDTR = nbofbytes;
  
//  DMA_Cmd(RX_Stream, ENABLE);
  RX_Stream->CR |= (u32)DMA_SxCR_EN;

//  SPI_I2S_DMACmd(S->SPI, SPI_I2S_DMAReq_Rx, ENABLE);
  S->SPI->CR2 |= SPI_I2S_DMAReq_Rx;
}

static u32 SPI_Move(u32 u, u32 Param1, u32 Param2, u32 Param3) {
  
//  SPI_MasterHW_t* S = (SPI_MasterHW_t*) u;
  
  // if the TX adr is null, no bytes to transmit, disable MOSI, and transmit dummy things instead
  if(Param1==0) {
    if(Param3>sizeof(Dummy)) while(1); // buffer is too small
    SPI_Disable_MOSI(u);
    Param1 = (u32)&Dummy[0];
  }else // enable MOSI
    SPI_Enable_MOSI(u);

  // if the RX adr is null, no bytes to receive, feed a dummy zone with it. (or disable the RX DMA? no side effect?)
  if(Param2==0) {
    if(Param3>sizeof(Dummy)) while(1); // buffer is too small
    Param2 = (u32) &Dummy[0];
  };
  
  if(Param3==0) while(1); // no bytes to transfer? weird!

  DMA_RX_Set(u, Param2, Param3);
  DMA_TX_Set(u, Param1, Param3); // this is to generate the SPI clocks to get the data in...     
  return 1; // callback armed!
}

//==============================================================
// SEQUENCER COMPATIBLE FUNCTION START
// This is the single instruction process
// These functions can be used in sequencers
// from 3 param down to 1
u32 sq_SPI_MHW_StartJob(u32 u) {
  u32* p = (u32*) u;
  return SPI_Start(p[0], p[1]);//Param1);//1 parameter
}

// from 3 param down to 1
u32 sq_SPI_MHW_StopJob(u32 u){
  u32* p = (u32*) u;
  return SPI_Stop(p[0], p[1]);//Param1);
}

u32 sq_SPI_MHW_MoveJob(u32 u){
  u32* p = (u32*) u;
  return SPI_Move(p[0], p[1], p[2], p[3]);
}

u32 sq_SPI_MHW_DMA_Interrupt(u32 u){
  u32* p = (u32*) u;
  DMA_Interrupt(p[0], (FunctionalState)p[1]);
  return 0;
}
//==== Let's try to see what would look like an example doing 1 simple sequence manually
//==== We assume the interrupt hooks are already in place.
// This will be called when SA(Jobs) is non empty OR by SPI interrupt events to read possibly next item in the list







