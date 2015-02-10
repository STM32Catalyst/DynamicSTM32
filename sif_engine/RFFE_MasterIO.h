
#ifndef _RFFE_MASTER_IO_H_
#define _RFFE_MASTER_IO_H_

typedef struct {
  
  IO_PinTypeDef* SCLK;
  IO_PinTypeDef* SDATA;
  
  // simplified DMA, both TX and RX? (does not support both directions, supposed to be bidirectional, with other direction point to 0000 or fail.
  u32 TX;
  u32 RX;
  u32 bCount;
  
  u32 MaxBps; // this is the maximum required baudrate
  u32 ActualBps; // HW bit rate (does not include delays between bytes)
//  u32 AverageBps; // Averate bit rate per job
  u32 FeedClockMHz; // SPI Clock feed. Internally only plays with PSC = 2^(n+1)
  
  StuffsArtery* SA; // this points to Job feeding
  
  BasicTimer* BT; // this will be to control time ticks
  u8 BTn; //  a number between 0 and 3

  u32 (*fnWaitMethod)(u32);
//  u32 ctWaitMethod;
  u32 WaitParam;
  
  u8 ParityError : 1;
  
} RFFE_MasterIO;

  

void NewRFFE_MasterIO_RX_TX(RFFE_MasterIO* S, DMA_Stream_TypeDef* RX_Stream, u32 DMA_RX_Channel, DMA_Stream_TypeDef* TX_Stream, u32 DMA_TX_Channel);
u32 SetRFFE_MasterIO_Timings(RFFE_MasterIO* S, u32 MaxBps, u32 CPol, u32 CPha, u32 FirstBit ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB
// GPIOs?

// Available SPI Functions For Sequencer use
u32 sq_RFFE_MIO_StartJob(u32 u); // SPI_MasterHW* and bitmask for NSS to go low
u32 sq_RFFE_MIO_StopJob(u32 u); // same as start, bitmask which NSS will go high.
u32 sq_RFFE_MIO_MoveJob(u32 u); // SPI_MasterHW* and RX_Adr, TX_Adr, sizeof (4 parameters needed)
u32 sq_RFFE_MIO_DMA_Interrupt(u32 u);

void RFFE_Test(void);
#endif
