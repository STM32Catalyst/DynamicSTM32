
#include "sebEngine.h"

const DMA_StreamInfoType DMA_StreamInfo[] = {

  {DMA1, DMA1_Stream0, 0, DMA1_Stream0_IRQn, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_FEIF0, DMA_FLAG_TCIF0 },
  {DMA1, DMA1_Stream1, 1, DMA1_Stream1_IRQn, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_FEIF1, DMA_FLAG_TCIF1 },
  {DMA1, DMA1_Stream2, 2, DMA1_Stream2_IRQn, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_FEIF2, DMA_FLAG_TCIF2 },
  {DMA1, DMA1_Stream3, 3, DMA1_Stream3_IRQn, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3 | DMA_FLAG_TEIF3 | DMA_FLAG_DMEIF3 | DMA_FLAG_FEIF3, DMA_FLAG_TCIF3 },
  {DMA1, DMA1_Stream4, 4, DMA1_Stream4_IRQn, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4 | DMA_FLAG_TEIF4 | DMA_FLAG_DMEIF4 | DMA_FLAG_FEIF4, DMA_FLAG_TCIF4 },
  {DMA1, DMA1_Stream5, 5, DMA1_Stream5_IRQn, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5, DMA_FLAG_TCIF5 },
  {DMA1, DMA1_Stream6, 6, DMA1_Stream6_IRQn, DMA_FLAG_TCIF6 | DMA_FLAG_HTIF6 | DMA_FLAG_TEIF6 | DMA_FLAG_DMEIF6 | DMA_FLAG_FEIF6, DMA_FLAG_TCIF6 },
  {DMA1, DMA1_Stream7, 7, DMA1_Stream7_IRQn, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7, DMA_FLAG_TCIF7 },
  
  {DMA2, DMA2_Stream0, 8, DMA2_Stream0_IRQn, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_FEIF0, DMA_FLAG_TCIF0 },
  {DMA2, DMA2_Stream1, 9, DMA2_Stream1_IRQn, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_FEIF1, DMA_FLAG_TCIF1 },
  {DMA2, DMA2_Stream2, 10, DMA2_Stream2_IRQn, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2 | DMA_FLAG_TEIF2 | DMA_FLAG_DMEIF2 | DMA_FLAG_FEIF2, DMA_FLAG_TCIF2 },
  {DMA2, DMA2_Stream3, 11, DMA2_Stream3_IRQn, DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3 | DMA_FLAG_TEIF3 | DMA_FLAG_DMEIF3 | DMA_FLAG_FEIF3, DMA_FLAG_TCIF3 },
  {DMA2, DMA2_Stream4, 12, DMA2_Stream4_IRQn, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4 | DMA_FLAG_TEIF4 | DMA_FLAG_DMEIF4 | DMA_FLAG_FEIF4, DMA_FLAG_TCIF4 },
  {DMA2, DMA2_Stream5, 13, DMA2_Stream5_IRQn, DMA_FLAG_TCIF5 | DMA_FLAG_HTIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_FEIF5, DMA_FLAG_TCIF5 },
  {DMA2, DMA2_Stream6, 14, DMA2_Stream6_IRQn, DMA_FLAG_TCIF6 | DMA_FLAG_HTIF6 | DMA_FLAG_TEIF6 | DMA_FLAG_DMEIF6 | DMA_FLAG_FEIF6, DMA_FLAG_TCIF6 },
  {DMA2, DMA2_Stream7, 15, DMA2_Stream7_IRQn, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7, DMA_FLAG_TCIF7 },
};

u32 Get_pDMA_Info(DMA_Stream_TypeDef* DMA_Stream) {
  
  u8 n;
  
  for(n=0;n<countof(DMA_StreamInfo);n++) {
    
      if(DMA_Stream==DMA_StreamInfo[n].Stream) // the stream is a memory location, it is unique
        return (u32) &DMA_StreamInfo[n];
  }
  
  return 0; // failed to find it
}


// The next table is about the request DMA mapping table
/*
typedef struct {

  DMA_TypeDef* DMA;
  DMA_Stream_TypeDef* Stream;
  u32 fn_ct_index; // 0..7 for DMA1 stream 0..7, 8..15 for DMA2.
  u32 Channel;
  u32 PPP_Adr;
  SignalList Signal;
  u32 Direction;
  
} DMA_StreamChannelInfoType; // this points to a const data
*/
//#define DMA_DIR_PeripheralToMemory        ((uint32_t)0x00000000)
//#define DMA_DIR_MemoryToPeripheral        ((uint32_t)0x00000040) 

#ifndef DAC1
#define DAC1 0
#define DAC2 0

//#define ADC1 0
//#define ADC2 0
//#define ADC1 0
#endif

#ifndef SAI1
#define SAI1 0
#endif

const DMA_StreamChannelInfoType DMA_StreamChannelInfo[] = {
//========================= DMA 1 ======================================================================================  
  {DMA1, DMA2_Stream0, 0, DMA_Channel_0, (u32)SPI3, SPI3_RX, DMA_DIR_PeripheralToMemory},
  {DMA1, DMA2_Stream0, 0, DMA_Channel_1, (u32)I2C1, I2C1_RX, DMA_DIR_PeripheralToMemory},
  {DMA1, DMA2_Stream0, 0, DMA_Channel_2, (u32)TIM4, TIM4_CH1, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream0, 0, DMA_Channel_3, (u32)I2S3ext, I2S3_EXT_RX, DMA_DIR_PeripheralToMemory}, // ???
  {DMA1, DMA2_Stream0, 0, DMA_Channel_4, (u32)UART5, UART5_RX, DMA_DIR_PeripheralToMemory},
  {DMA1, DMA2_Stream0, 0, DMA_Channel_5, (u32)0/*UART8*/, 0/*UART8_RX*/, DMA_DIR_PeripheralToMemory}, // ???
  {DMA1, DMA2_Stream0, 0, DMA_Channel_6, (u32)TIM5, TIM5_CH3, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream0, 0, DMA_Channel_6, (u32)TIM5, TIM5_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA1, DMA2_Stream0, 0, DMA_Channel_7, (u32)UART5, 0, DMA_DIR_PeripheralToMemory},

//  {DMA, DMA2_Stream, 0, DMA_Channel_0, (u32)0, 0, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA, DMA2_Stream, 0, DMA_Channel_1, (u32)0, 0, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA, DMA2_Stream, 0, DMA_Channel_2, (u32)0, 0, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream1, 0, DMA_Channel_3, (u32)TIM2, TIM2_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral}, // ???
  {DMA1, DMA2_Stream1, 0, DMA_Channel_3, (u32)TIM5, TIM5_CH4, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},  
  {DMA1, DMA2_Stream1, 0, DMA_Channel_4, (u32)USART3, USART3_TX, DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream1, 0, DMA_Channel_5, (u32)0/*UART7*/, 0/*UART7_TX*/, DMA_DIR_MemoryToPeripheral}, // ???
  {DMA1, DMA2_Stream1, 0, DMA_Channel_6, (u32)TIM5, TIM5_CH4, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream1, 0, DMA_Channel_6, (u32)TIM5, TIM5_TRIG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},  
  {DMA1, DMA2_Stream1, 0, DMA_Channel_7, (u32)TIM5, TIM5_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},

  {DMA1, DMA2_Stream2, 0, DMA_Channel_0, (u32)SPI3, SPI3_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream2, 0, DMA_Channel_1, (u32)TIM7, TIM7_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream2, 0, DMA_Channel_2, (u32)I2S3ext, I2S3_EXT_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream2, 0, DMA_Channel_3, (u32)I2C3, I2C3_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream2, 0, DMA_Channel_4, (u32)UART4, UART4_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream2, 0, DMA_Channel_5, (u32)TIM3, TIM3_CH4, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream2, 0, DMA_Channel_5, (u32)TIM3, TIM3_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},  
  {DMA1, DMA2_Stream2, 0, DMA_Channel_6, (u32)TIM5, TIM5_CH1, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream2, 0, DMA_Channel_7, (u32)I2C2, I2C2_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},

  {DMA1, DMA2_Stream3, 0, DMA_Channel_0, (u32)SPI2, SPI2_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA1, DMA2_Stream3, 0, DMA_Channel_1, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream3, 0, DMA_Channel_2, (u32)TIM4, TIM4_CH2, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream3, 0, DMA_Channel_3, (u32)I2S2ext, I2S2_EXT_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream3, 0, DMA_Channel_4, (u32)USART3, USART3_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream3, 0, DMA_Channel_5, (u32)0/*UART7*/,0/*UART7_RX*/ , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream3, 0, DMA_Channel_6, (u32)TIM5, TIM5_CH4, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream3, 0, DMA_Channel_6, (u32)TIM5, TIM5_TRIG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream3, 0, DMA_Channel_7, (u32)I2C2, I2C2_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
  {DMA1, DMA2_Stream4, 0, DMA_Channel_0, (u32)SPI2, SPI2_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream4, 0, DMA_Channel_1, (u32)TIM7, TIM7_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream4, 0, DMA_Channel_2, (u32)I2S2ext, I2S2_EXT_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream4, 0, DMA_Channel_3, (u32)I2C3, I2C3_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream4, 0, DMA_Channel_4, (u32)UART4, UART4_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream4, 0, DMA_Channel_5, (u32)TIM3, TIM3_CH1, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream4, 0, DMA_Channel_5, (u32)TIM3, TIM3_TRIG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream4, 0, DMA_Channel_6, (u32)TIM5, TIM5_CH2, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream4, 0, DMA_Channel_7, (u32)USART3, USART3_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
  {DMA1, DMA2_Stream5, 0, DMA_Channel_0, (u32)SPI3, SPI3_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream5, 0, DMA_Channel_1, (u32)I2C1, I2C1_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream5, 0, DMA_Channel_2, (u32)I2S3ext, I2S3_EXT_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream5, 0, DMA_Channel_3, (u32)TIM2, TIM2_CH1, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream5, 0, DMA_Channel_4, (u32)USART2, USART2_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream5, 0, DMA_Channel_5, (u32)TIM3, TIM3_CH2, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA1, DMA2_Stream5, 0, DMA_Channel_6, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream5, 0, DMA_Channel_7, (u32)DAC1, DAC1_ANALOG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
//  {DMA1, DMA2_Stream6, 0, DMA_Channel_0, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream6, 0, DMA_Channel_1, (u32)I2C1, I2C1_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream6, 0, DMA_Channel_2, (u32)TIM4, TIM4_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream6, 0, DMA_Channel_3, (u32)TIM2, TIM2_CH2, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream6, 0, DMA_Channel_3, (u32)TIM2, TIM2_CH4, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},  
  {DMA1, DMA2_Stream6, 0, DMA_Channel_4, (u32)USART2, USART2_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream6, 0, DMA_Channel_5, (u32)0/*UART8*/,0/*UART8_RX*/ , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream6, 0, DMA_Channel_6, (u32)TIM5, TIM5_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream6, 0, DMA_Channel_7, (u32)DAC2, DAC2_ANALOG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
  {DMA1, DMA2_Stream7, 0, DMA_Channel_0, (u32)SPI3, SPI3_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream7, 0, DMA_Channel_1, (u32)I2C1, I2C1_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream7, 0, DMA_Channel_2, (u32)TIM4, TIM4_CH3, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream7, 0, DMA_Channel_3, (u32)TIM2, TIM2_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream7, 0, DMA_Channel_3, (u32)TIM2, TIM2_CH4, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},  
  {DMA1, DMA2_Stream7, 0, DMA_Channel_4, (u32)UART5, UART5_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream7, 0, DMA_Channel_5, (u32)TIM3, TIM3_CH3, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA1, DMA2_Stream7, 0, DMA_Channel_6, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA1, DMA2_Stream7, 0, DMA_Channel_7, (u32)I2C2, I2C2_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
//========================== DMA 2 ===============================================================================  
  
  {DMA2, DMA2_Stream0, 0, DMA_Channel_0, (u32)ADC1, ADC1_ANALOG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream0, 0, DMA_Channel_1, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream0, 0, DMA_Channel_2, (u32)ADC3, ADC3_ANALOG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream0, 0, DMA_Channel_3, (u32)SPI1, SPI1_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream0, 0, DMA_Channel_4, (u32)SPI4, SPI4_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream0, 0, DMA_Channel_5, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream0, 0, DMA_Channel_6, (u32)TIM1, TIM1_TRIG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream0, 0, DMA_Channel_7, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
  {DMA2, DMA2_Stream1, 0, DMA_Channel_0, (u32)SAI1, SAI1_A, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream1, 0, DMA_Channel_1, (u32)DCMI, DCMI_BUS, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream1, 0, DMA_Channel_2, (u32)ADC3, ADC3_ANALOG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream1, 0, DMA_Channel_3, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream1, 0, DMA_Channel_4, (u32)SPI4, SPI4_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream1, 0, DMA_Channel_5, (u32)USART6, USART6_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream1, 0, DMA_Channel_6, (u32)TIM1, TIM1_CH1, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream1, 0, DMA_Channel_7, (u32)TIM8, TIM8_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
  {DMA2, DMA2_Stream2, 0, DMA_Channel_0, (u32)TIM8, TIM8_CH1, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream2, 0, DMA_Channel_0, (u32)TIM8, TIM8_CH2, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream2, 0, DMA_Channel_0, (u32)TIM8, TIM8_CH3, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream2, 0, DMA_Channel_1, (u32)ADC2, ADC2_ANALOG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream2, 0, DMA_Channel_2, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream2, 0, DMA_Channel_3, (u32)SPI1, SPI1_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream2, 0, DMA_Channel_4, (u32)USART1, USART1_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream2, 0, DMA_Channel_5, (u32)USART6, USART6_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream2, 0, DMA_Channel_6, (u32)TIM1, TIM1_CH2, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream2, 0, DMA_Channel_7, (u32)TIM8, TIM8_CH1, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
  {DMA2, DMA2_Stream3, 0, DMA_Channel_0, (u32)SAI1, SAI1_A, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream3, 0, DMA_Channel_1, (u32)ADC2, ADC2_ANALOG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream3, 0, DMA_Channel_2, (u32)SPI5, SPI5_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream3, 0, DMA_Channel_3, (u32)SPI1, SPI1_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream3, 0, DMA_Channel_4, (u32)SDIO, SDIO_BUS, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream3, 0, DMA_Channel_5, (u32)SPI4, SPI4_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream3, 0, DMA_Channel_6, (u32)TIM1, TIM1_CH1, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream3, 0, DMA_Channel_7, (u32)TIM8, TIM8_CH2, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
  {DMA2, DMA2_Stream4, 0, DMA_Channel_0, (u32)ADC1, ADC1_ANALOG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream4, 0, DMA_Channel_1, (u32)SAI1, SAI1_B, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream4, 0, DMA_Channel_2, (u32)SPI5, SPI5_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream4, 0, DMA_Channel_3, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream4, 0, DMA_Channel_4, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream4, 0, DMA_Channel_5, (u32)SPI4, SPI4_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream4, 0, DMA_Channel_6, (u32)TIM1, TIM1_CH4, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream4, 0, DMA_Channel_6, (u32)TIM1, TIM1_TRIG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream4, 0, DMA_Channel_6, (u32)TIM1, TIM1_COM, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream4, 0, DMA_Channel_7, (u32)TIM8, TIM8_CH3, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
  {DMA2, DMA2_Stream5, 0, DMA_Channel_0, (u32)SAI1, SAI1_B, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream5, 0, DMA_Channel_1, (u32)SPI6, SPI6_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream5, 0, DMA_Channel_2, (u32)CRYP, CRYP_OUT, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream5, 0, DMA_Channel_3, (u32)SPI1, SPI1_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream5, 0, DMA_Channel_4, (u32)USART1, USART1_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream5, 0, DMA_Channel_5, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream5, 0, DMA_Channel_6, (u32)TIM1, TIM1_UP, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream5, 0, DMA_Channel_7, (u32)SPI5, SPI5_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
  {DMA2, DMA2_Stream6, 0, DMA_Channel_0, (u32)TIM1, TIM1_CH1, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream6, 0, DMA_Channel_0, (u32)TIM1, TIM1_CH2, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},  
  {DMA2, DMA2_Stream6, 0, DMA_Channel_0, (u32)TIM1, TIM1_CH3, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},  
  {DMA2, DMA2_Stream6, 0, DMA_Channel_1, (u32)SPI6, SPI6_RX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream6, 0, DMA_Channel_2, (u32)CRYP, CRYP_IN, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream6, 0, DMA_Channel_3, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream6, 0, DMA_Channel_4, (u32)SDIO, SDIO_BUS, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream6, 0, DMA_Channel_5, (u32)USART6, USART6_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream6, 0, DMA_Channel_6, (u32)TIM1, TIM1_CH3, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream6, 0, DMA_Channel_7, (u32)SPI5, SPI5_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
//  {DMA2, DMA2_Stream7, 0, DMA_Channel_0, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream7, 0, DMA_Channel_1, (u32)DCMI, DCMI_BUS, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream7, 0, DMA_Channel_2, (u32)HASH, HASH_IN, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream7, 0, DMA_Channel_3, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream7, 0, DMA_Channel_4, (u32)USART1, USART1_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream7, 0, DMA_Channel_5, (u32)USART6, USART6_TX, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
//  {DMA2, DMA2_Stream7, 0, DMA_Channel_6, (u32), , DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream7, 0, DMA_Channel_7, (u32)TIM8, TIM8_CH4, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream7, 0, DMA_Channel_7, (u32)TIM8, TIM8_TRIG, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  {DMA2, DMA2_Stream7, 0, DMA_Channel_7, (u32)TIM8, TIM8_COM, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral},
  
};


u32 Get_pStreamChannelForPPP_Signal(u32 PPP_Adr, u32 Signal, u32 Direction) {
  
  u32 n;
  
  for(n=0;n<countof(DMA_StreamChannelInfo);n++) {
    if(DMA_StreamChannelInfo[n].PPP_Adr == PPP_Adr)
      if(DMA_StreamChannelInfo[n].Signal == Signal)
        if(DMA_StreamChannelInfo[n].Direction == Direction)
          return (u32)(&DMA_StreamChannelInfo[n]);
  };
  
  return 0; // not found!
}

// we also need to have a conflict or solver function... need to have inventory of how many solutions are possible...
// to run recursively with the right functions.