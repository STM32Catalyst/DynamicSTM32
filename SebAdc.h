
#ifndef _SEB_ADC_H_
#define _SEB_ADC_H_

// THIS IS THE SAR ADC. (Sigma delta ADC to be created later, as it does not exist in STM32F437 initial test vehicle)

void ADC_Config(void);
void ADC_Streams_Init(void);

typedef struct {

  u32 VDDA_mV; // The ADC supply voltage in mV
  u32       Mode;
  u32       FeedClockMHz;
  
  ADC_TypeDef* ADCx;
  ADC_InitTypeDef ADCI;

  IO_Pin_t* AnalogInPins[16]; // this correspond to the normal sequence
  IO_Pin_t* InjectedAnalogInPins[4]; // this correspond to the injected (normal interruptable by injected priority channel)
  IO_Pin_t* TriggerPin;
  IO_Pin_t* IntectedTriggerPin;
  u16       ADC_Channel_Lsb[18];
  u16       ADC_Channel_mV[18];
  u32       ChannelSamplingRate_Hz[18]; // The max sampling frequency for each channel in Hz
  u32       InjectedChannelSamplingRate_Hz[4];
  u32       MaxSamplingRate_Hz;
  u16       MinThreshold;
  u16       MaxThreshold;
  u32       fnThreshold;
  u32       ctThreshold;
  u32       fnDone;
  u32       ctDone;
  u32       fnInjectedDone;
  u32       ctInjectedDone;
} ADC_t;

/*
ADC_CommonInitTypeDef
*/

u32 NewADC(ADC_t* A, ADC_TypeDef* ADCx, MCUClocks_t * Tree );
u32 NewADC_Channel(ADC_t* A, IO_Pin_t* P, u32 SampleTime_cy);
u32 NewADC_InjectedChannel(ADC_t* A, IO_Pin_t* P, u32 SamplingRate_Hz);
u32 UseADC_Trigger(ADC_t* A, IO_Pin_t* T, u32 InternalTrigger);
u32 UseADC_InjectedTrigger(ADC_t* A, IO_Pin_t* T, u32 InternalTrigger);

void SetADC_Waveform(ADC_t* A, u32 Adr, u32 Size);
void EnableADC(DAC_t* D);
void NVIC_ADCsEnable(FunctionalState Enable);

#endif