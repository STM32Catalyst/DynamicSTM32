
#ifndef _SPI_MASTER_IO_H_
#define _SPI_MASTER_IO_H_

void SPIIO_Test(void);

void SPIIO_SendByte(u8 byte);
u8 SPIIO_ReadByte(void);
u8 SPIIO_Dummy(void);
void SPIIO_Start(void);
void SPIIO_Stop(void);

void SPIIO_SetDelay(u8 delay);
u8 SPIIO_GetDelay(void);

//===============================-------------------------------------------->

// This master SPI is monodirectional only by GPIOs. To be used by polling method (blocking)

typedef struct {

  IO_Pin_t* MISO; // we need the pointer to the pin
  IO_Pin_t* MOSI; // we need the pointer to the pin
  IO_Pin_t* SCK; // we need the pointer to the pin
  IO_Pin_t* NSSs[16]; // list of NSS pins, can be more than one (max 16 pins)  

  // simplified DMA, both TX and RX? (does not support both directions, supposed to be bidirectional, with other direction point to 0000 or fail.
  u32 TX;
  u32 RX;
  u32 bCount;

  Timer_t* Timer; // this will be to control time ticks
  u8 Cn; //  a number between 0 and 3

  u32 (*fnWaitMethod)(u32);
  u32 ctWaitMethod;
  u32 WaitParam;
  
  RangedValue_t Bps; // input
  MCU_Clocks_t* Clocks;
  
  StuffsArtery_t* SA; // this points to Job feeding

} SPI_MasterIO_t;


void NewSPI_MasterIO(SPI_MasterIO_t* M, IO_Pin_t* MISO, IO_Pin_t* MOSI, IO_Pin_t* SCK, IO_Pin_t* NSS0); // this will be the first NSS[0]
void SetSPI_MasterIO_Timings(SPI_MasterIO_t* M, u32 MinBps, u32 MaxBps ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB
void SetSPI_MasterIO_Format(SPI_MasterIO_t* M, u32 CPol, u32 CPha, u32 FirstBit );
void ConfigureSPI_MasterIO(SPI_MasterIO_t* M);
void EnableSPI_MasterIO(SPI_MasterIO_t* M);

// Available SPI Functions For Sequencer use
u32 sq_SPI_MIO_StartJob(u32 u); // SPI_MasterHW* and bitmask for NSS to go low
u32 sq_SPI_MIO_StopJob(u32 u); // same as start, bitmask which NSS will go high.
u32 sq_SPI_MIO_MoveJob(u32 u); // SPI_MasterHW* and RX_Adr, TX_Adr, sizeof (4 parameters needed)
u32 sq_SPI_MIO_DMA_Interrupt(u32 u);



#endif
