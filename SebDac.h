
#ifndef _SEB_DAC_H_
#define _SEB_DAC_H_

#define DAC_DHR12R1_ADDRESS    0x40007408
#define DAC_DHR12R2_ADDRESS    0x40007414

typedef struct {

//  DMA_InitTypeDef DMA_InitStructure[4];
  u32 PPP; // same base address for both...
  u32 n; // 1 or 2   
  
  DAC_InitTypeDef InitStructure[4];
  IO_Pin_t* AnalogOutPin[4];
  IO_Pin_t* TriggerPin[4];
  
  u32 SampleSize[4]; // if zero, it means static output and WaveAdr is the DAC value directly, if 1, static output with WaveAdr being the pointer to the value, >1 DMA will be used
  u32 SampleAdr[4];
} DAC_t;

void NewDAC_Pin_Buffer(DAC_t* D, u32 PPP, u32 n, IO_Pin_t* DacPin, FunctionalState BufferEnable);
void SetDAC_Waveform(DAC_t* D, u32 Adr, u32 Size);
void UseDAC_Trigger(DAC_t* D, IO_Pin_t* Trigger, u32 InternalTrigger);
void EnableDAC(DAC_t* D);

#endif
