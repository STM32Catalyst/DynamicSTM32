
#ifndef _RFFE_MASTER_IO_H_
#define _RFFE_MASTER_IO_H_

typedef struct {
  
  IO_Pin_t* SCLK;
  IO_Pin_t* SDATA;
  
  // simplified DMA, both TX and RX? (does not support both directions, supposed to be bidirectional, with other direction point to 0000 or fail.
  u32 TX;
  u32 RX;
  u32 bCount;
  
  RangedValue_t Bps; // input
  MCU_Clocks_t* Clocks; // This includes everything about clocks, and Vdd.
  
  Timer_t* Timer; // this will be to control time ticks
  u8 Cn; //  a number between 0 and 3

  u32 (*fnWaitMethod)(u32);
  u32 ctWaitMethod;
  u32 WaitParam;

  StuffsArtery_t* SA; // this points to Job feeding
  
  u8 ParityError : 1;
  
} RFFE_MasterIO_t;

  

void NewRFFE_MasterIO_RX_TX_SCLK_SDATA(RFFE_MasterIO_t* M, IO_Pin_t* SCLK, IO_Pin_t* SDATA);
void SetRFFE_MasterIO_Timings(RFFE_MasterIO_t* M, u32 MinBps, u32 MaxBps ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB
void SetRFFE_MasterIO_Format(RFFE_MasterIO_t* M);
void ConfigureRFFE_MasterIO(RFFE_MasterIO_t* M);
void EnableRFFE_MasterIO(RFFE_MasterIO_t* M);

// Available SPI Functions For Sequencer use
u32 sq_RFFE_MIO_StartJob(u32 u); // SPI_MasterHW_t* and bitmask for NSS to go low
u32 sq_RFFE_MIO_StopJob(u32 u); // same as start, bitmask which NSS will go high.
u32 sq_RFFE_MIO_MoveJob(u32 u); // SPI_MasterHW_t* and RX_Adr, TX_Adr, sizeof (4 parameters needed)
u32 sq_RFFE_MIO_DMA_Interrupt(u32 u);

#endif
