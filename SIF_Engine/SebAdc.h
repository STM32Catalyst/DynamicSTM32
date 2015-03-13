
#ifndef _SEB_ADC_H_
#define _SEB_ADC_H_

// THIS IS THE SAR ADC. (Sigma delta ADC to be created later, as it does not exist in STM32F437 initial test vehicle)

// Here is the reasoning for the implementation:
// Normal channels have only one single data register.
// We have a RAM info for the buffer (Adr,Size)
// 1 or more channel, use DMA if calling the Waveform function
// Adr = 0: Use ADC internal buffer
// 2+ normal channel: For this one, DMA will be used and the DMA will loop through the size. The size will be in sample count?

// Injected channels have their own individual registers, so these ones will be used without DMA. As interrupting the normal channels, they are irregular coming events...

void ADC_Config(void);
void ADC_Streams_Init(void);

// These are all the enables ways for normal and injected channels
// All the config fields that is in the ADC_t structure should be setup before calling this function
// 0 = disabled
// 1 = continuous (you'll bringup with this)
// 2 = one shot (you'll tune with this)
// 3 = triggered (you'll build your complex system and optimize it with this
// 4 = your scheme (you'll want to try something unique)
typedef enum {
  ADC_DISABLED,
  ADC_CONTINUOUS,
  ADC_ONCE,
  ADC_TRIGGERED,
  ADC_YOUR_WAY
} ADC_ScanScheme;


typedef struct {
  
  ADC_TypeDef* ADCx;
  ADC_InitTypeDef ADCI;
  
  u32       VRef_mV; // The ADC supply voltage in mV
  u32       FeedClockMHz;
  
  IO_Pin_t* NormalAnalogInPins[16]; // this correspond to the normal sequence
  u32       NormalChannel[18];
  u32       NormalUsedChannelCount;
  IO_Pin_t* NormalTriggerPin;
  u32       NormalSampleTime_cy[18];
  u32       NormalInternalTrigger;
  u32       NormalSamplesAdr; // DMA
  u32       NormalSamplesCount; // DMA if 1+
  ADC_ScanScheme       NormalScheme;
  
  IO_Pin_t* InjectedAnalogInPins[4]; // this correspond to the injected (normal interruptable by injected priority channel)
  u32       InjectedChannel[4];
  u32       InjectedUsedChannelCount;
  u32       InjectedInternalTrigger;    
  IO_Pin_t* InjectedTriggerPin;
  u32       InjectedSampleTime_cy[4];
  ADC_ScanScheme       InjectedScheme;
  
  u32       MaxSamplingRate_Hz;
  
  u16       MinThreshold_mV;  //
  u16       MaxThreshold_mV;  // 
  IO_Pin_t* ThresholdMonitoredPin; // NULL = function on all pins
  u32       fnThreshold;
  u32       ctThreshold;
  
  u32       fnDone;
  u32       ctDone;
  u32       fnInjectedDone;
  u32       ctInjectedDone;
  
  // this is for debug only
  u16       ADC_Values_Lsb[24]; // if no RAM buffer allocated for more than a single normal channel, this one will be used instead. It will also have its conversion in mV for debugging and bringup
  u16       ADC_Values_mV[24];
} ADC_t;

/*
ADC_CommonInitTypeDef
*/

u32 NewADC(ADC_t* A, ADC_TypeDef* ADCx, u32 Vdd_mV, MCUClocks_t * Tree );
u32 NewADC_Channel(ADC_t* A, IO_Pin_t* P, u32 SampleTime_cy);
u32 NewADC_InjectedChannel(ADC_t* A, IO_Pin_t* P, u32 SamplingRate_Hz);
u32 UseADC_Trigger(ADC_t* A, IO_Pin_t* T, u32 InternalTrigger);
u32 UseADC_InjectedTrigger(ADC_t* A, IO_Pin_t* T, u32 InternalTrigger);

void SetADC_Waveform(ADC_t* A, u32 Adr, u32 Size);

void NVIC_ADC_Enable(ADC_t* A, FunctionalState Enable);


void ADC_Demo(void);

#endif