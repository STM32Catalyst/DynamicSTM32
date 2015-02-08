
#ifndef _SPI_MASTER_HW_H_
#define _SPI_MASTER_HW_H_
// This defined the structure and variables needed for SPI Master Peripheral HW cell management
// For now, DMA is not used.

// This system can be improved to support blocking and non blocking modes.
// For the non blocking mode, the I2C speed must be lower, and all the transitions for SDA and SCL must use the
// HW Timer delay with a callback. Then it should be able to use slow I2C as background, only using IOs.
// Beware that current is consumed while SDA or SCL are low (pull-up resistors current). Favour non 50% duty cycles...

// SPI Master overall. Beware: You must have DMA Stream to run this. Otherwise, we need to emulate byte by byte interrupt pseudo DMA which does not exist yet.
typedef struct {
  
  IO_PinTypeDef* MISO; // we need the pointer to the pin (MISO=MOSI or it fails)
  IO_PinTypeDef* MOSI; // we need the pointer to the pin 
  IO_PinTypeDef* SCK; // we need the pointer to the pin
  IO_PinTypeDef* NSSs[16]; // list of NSS pins, can be more than one (max 16 pins)
  
  u32 TX_Channel;
  DMA_StreamInfoType* DMA_TX; // this point to a const structure based on datasheet info
  
  u32 RX_Channel;
  DMA_StreamInfoType* DMA_RX; // this point to a const structure based on datasheet info
  SPI_TypeDef* SPI; // debug

  u32 MaxBps; // this is the maximum required baudrate
  u32 ActualBps; // HW bit rate (does not include delays between bytes)
//  u32 AverageBps; // Averate bit rate per job
  u32 FeedClockMHz; // SPI Clock feed. Internally only plays with PSC = 2^(n+1)
  
  StuffsArtery* SA; // this points to Job feeding

} SPI_MasterHW;


void NewSPI_MasterHW_RX_TX(SPI_MasterHW* S, DMA_Stream_TypeDef* RX_Stream, u32 DMA_RX_Channel, DMA_Stream_TypeDef* TX_Stream, u32 DMA_TX_Channel);
u32 SetSPI_MasterHW_Timings(SPI_MasterHW* S, u32 MaxBps, u32 CPol, u32 CPha, u32 FirstBit ); // 1200000, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_FirstBit_MSB
// GPIOs?

// Available SPI Functions For Sequencer use
u32 sq_SPI_MHW_StartJob(u32 u); // SPI_MasterHW* and bitmask for NSS to go low
u32 sq_SPI_MHW_StopJob(u32 u); // same as start, bitmask which NSS will go high.
u32 sq_SPI_MHW_MoveJob(u32 u); // SPI_MasterHW* and RX_Adr, TX_Adr, sizeof (4 parameters needed)
u32 sq_SPI_MHW_DMA_Interrupt(u32 u);

#endif
