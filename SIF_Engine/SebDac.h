
#ifndef _SEB_DAC_H_
#define _SEB_DAC_H_

#define DAC_DHR12R1_ADDRESS    0x40007408
#define DAC_DHR12L1_ADDRESS    0x4000740C

#define DAC_DHR12R2_ADDRESS    0x40007414
#define DAC_DHR12L2_ADDRESS    0x40007418

typedef struct {

  u16 VRef_mV;
  
  MCU_Clocks_t* Clocks; // This includes everything about clocks, and Vdd.
//  DMA_InitTypeDef DMA_InitStructure[4];
  u32 PPP; // same base address for both...
  u8 n; // 1 or 2   
  
  DAC_InitTypeDef InitStructure;
  IO_Pin_t* AnalogOutPin;
  IO_Pin_t* TriggerPin;
  
  u16 SampleSize; // if zero, it means static output and WaveAdr is the DAC value directly, if 1, static output with WaveAdr being the pointer to the value, >1 DMA will be used
  u32 SampleAdr;

  u32 TX_Channel; // for future dynamic S/W DAC updates
  DMA_StreamInfo_t* DMA_TX; // this point to a const structure based on datasheet info, for future dynamic S/W DAC updates
  
  StuffsArtery_t* SA; // this points to Job feeding
  
} DAC_t;

void NewDAC_Pin_Buffer_VRef_mV(DAC_t* D, u32 PPP, IO_Pin_t* DacPin, FunctionalState BufferEnable, u16 VRef_mV);
void SetDAC_Waveform(DAC_t* D, u32 Adr, u32 Size);
void UseDAC_Trigger(DAC_t* D, IO_Pin_t* Trigger, u32 InternalTrigger);
void ConfigureDAC(DAC_t* D); // HW configuration job
void EnableDAC(DAC_t* D); // activate the DAC's mechanics

// helper functions
void SetDAC_Lsb(DAC_t* D, u32 Lsb);
u16 DAC_mV_ToLsb(DAC_t* D, u32 mV);
void DAC_WaveGeneratePredefined(u16* pu16, u8 WaveName, u16 size);

#endif
