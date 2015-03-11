
#ifndef _SEB_DMA_H_
#define _SEB_DMA_H_

typedef struct {

  DMA_TypeDef* DMA;
  DMA_Stream_TypeDef* Stream;
  u32 fn_ct_index; // 0..7 for DMA1 stream 0..7, 8..15 for DMA2.
  u32 Stream_IRQn;
  u32 Flags;
  u32 TC_Flag;
  
} DMA_StreamInfo_t; // this points to a const data

u32 Get_pDMA_Info(DMA_Stream_TypeDef* DMA_Stream);

//==================----------------->
// Direction field, 32 bit
//#define DMA_DIR_PeripheralToMemory        ((uint32_t)0x00000000)
//#define DMA_DIR_MemoryToPeripheral        ((uint32_t)0x00000040) 
//#define DMA_DIR_MemoryToMemory            ((uint32_t)0x00000080)

// DMA stream assignment... 
typedef struct {

  DMA_TypeDef* DMA;
  DMA_Stream_TypeDef* Stream;
  u32 fn_ct_index; // 0..7 for DMA1 stream 0..7, 8..15 for DMA2.
  u32 Channel;
  u32 PPP_Adr;
  Signal_t Signal;
  u32 Direction;
  
} DMA_StreamChannelInfo_t; // this points to a const data

DMA_StreamChannelInfo_t* Get_pStreamChannelForPPP_Signal(u32 PPP_Adr, u32 Signal, u32 Direction);

#endif
