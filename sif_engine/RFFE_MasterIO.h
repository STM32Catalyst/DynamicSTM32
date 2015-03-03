
#ifndef _RFFE_MASTER_IO_H_
#define _RFFE_MASTER_IO_H_

typedef struct {
  
  IO_Pin_t* SCLK;
  IO_Pin_t* SDATA;
  
  // simplified DMA, both TX and RX? (does not support both directions, supposed to be bidirectional, with other direction point to 0000 or fail.
  u32 TX;
  u32 RX;
  u32 bCount;
  
  u32 MaxBps; // this is the maximum required baudrate
//  u32 ActualBps; // HW bit rate (does not include delays between bytes)
//  u32 AverageBps; // Averate bit rate per job
//  u32 FeedClockMHz; // SPI Clock feed. Internally only plays with PSC = 2^(n+1)
  
  StuffsArtery_t* SA; // this points to Job feeding
  
  Timer_t* Timer; // this will be to control time ticks
  u8 Cn; //  a number between 0 and 3

  u32 (*fnWaitMethod)(u32);
  u32 ctWaitMethod;
  u32 WaitParam;
  
  u8 ParityError : 1;
  
} RFFE_MasterIO_t;

  

void NewRFFE_MasterIO_RX_TX(RFFE_MasterIO_t* M);
u32 SetRFFE_MasterIO_Timings(RFFE_MasterIO_t* M, u32 MaxBps, MCUClocks_t* T ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB
// GPIOs?

// Available SPI Functions For Sequencer use
u32 sq_RFFE_MIO_StartJob(u32 u); // SPI_MasterHW_t* and bitmask for NSS to go low
u32 sq_RFFE_MIO_StopJob(u32 u); // same as start, bitmask which NSS will go high.
u32 sq_RFFE_MIO_MoveJob(u32 u); // SPI_MasterHW_t* and RX_Adr, TX_Adr, sizeof (4 parameters needed)
u32 sq_RFFE_MIO_DMA_Interrupt(u32 u);

void RFFE_Test(void);
#endif
