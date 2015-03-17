
#ifndef _SEB_ADC_H_
#define _SEB_ADC_H_

// THIS IS THE SAR ADC. (Sigma delta ADC to be created later, as it does not exist in STM32F437 initial test vehicle)

// F437 bugged std lib?
typedef enum {
  ADC_VRef,
  ADC_VBat,
  ADC_Temp } ADC_Internal_Signals;


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
  ADC_TRIGGERED,
  ADC_INJECTED_AS_NORMAL, // the injected scan is tied to normal channel's
} ADC_ScanScheme;


typedef struct {
  
  ADC_TypeDef* ADCx;
  ADC_InitTypeDef ADCI;
  
  u32       VRef_mV; // The ADC supply voltage in mV
  u32       FeedClockHz;
  
  IO_Pin_t* NormalAnalogInPins[16]; // this correspond to the normal sequence
  u8        NormalChannel[18];
  u8        NormalUsedChannelCount;
  IO_Pin_t* NormalTriggerPin;
  
  u16       NormalSampleTime_cy[18];
  u32       NormalTotalCycle_cy;
  u32       NormalMaxSamplingRate_Hz;
  
  u32       NormalInternalTrigger;
  u32       NormalSamplesAdr; // DMA
  u32       NormalSamplesCount; // DMA if 1+
  ADC_ScanScheme       NormalScheme;
  
  IO_Pin_t* InjectedAnalogInPins[4]; // this correspond to the injected (normal interruptable by injected priority channel)
  u8        InjectedChannel[4];
  u8        InjectedUsedChannelCount;
  u32       InjectedInternalTrigger;    
  IO_Pin_t* InjectedTriggerPin;
  
  u16       InjectedSampleTime_cy[4];
  u32       InjectedTotalCycle_cy;
  u32       InjectedMaxSamplingRate_Hz;
  ADC_ScanScheme       InjectedScheme;
  
  u16       MinThreshold_mV;  //
  u16       MaxThreshold_mV;  // 
  IO_Pin_t* ThresholdMonitoredPin; // NULL = function on all pins
  u32       fnThreshold;
  u32       ctThreshold;
  
  u32       fnNormalDone;
  u32       ctNormalDone;
  u32       fnInjectedDone;
  u32       ctInjectedDone;
  
  // this is for debug only
  u16       Normal_Lsb[18]; // if no RAM buffer allocated for more than a single normal channel, this one will be used instead. It will also have its conversion in mV for debugging and bringup
  u16       Normal_mV[18]; // this can be updated by interrupts or manually
  s16       Injected_Lsb[4];
  s16       Injected_mV[4];
  
  s16       Temp_degC_x10;
  s16       MeasuredVdd_mV; // estimation of Vdd from Vref
  
  u8        NormalDone : 1;
  u8        InjectedDone : 1;
  u8        Threshold : 1;
  u8        Overflow : 1;
} ADC_t;

/* ADC_CommonInitTypeDef */

u32 NewADC(ADC_t* A, ADC_TypeDef* ADCx, u32 Vdd_mV, MCUClocks_t * Tree );
u32 NewADC_NormalChannel(ADC_t* A, IO_Pin_t* P, u32 SampleTime_cy);
u32 NewADC_NormalChannelInternal(ADC_t* A, ADC_Internal_Signals Channel);
u32 NewADC_InjectedChannel(ADC_t* A, IO_Pin_t* P, u32 SampleTime_cy);
u32 NewADC_InjectedChannelInternal(ADC_t* A, ADC_Internal_Signals Channel);
u32 UseADC_NormalTrigger(ADC_t* A, IO_Pin_t* T, u32 InternalTrigger);
u32 UseADC_InjectedTrigger(ADC_t* A, IO_Pin_t* T, u32 InternalTrigger);

void SetADC_Waveform(ADC_t* A, u32 Adr, u32 Size);
u32 SetADC_Threshold_Pin_Min_Max_mV(ADC_t* A, IO_Pin_t* P, u32 MinThreshold_mV, u32 MaxThreshold_mV);

void ConfigureADC(ADC_t* A, ADC_ScanScheme NormalScheme, ADC_ScanScheme InjectedScheme);
void HookADC(ADC_t* A, u16 ADC_IT, u32 fn, u32 ct);
void ADC_InterruptEnable(ADC_t* A, u16 ADC_IT, FunctionalState NewState);
void EnableADC(ADC_t* A);
void NVIC_ADCsEnable(FunctionalState Enable);


void StartADC_NormalConversion(ADC_t* A); // SW Trigger
void StartADC_InjectedConversion(ADC_t* A); // SW Trigger

// Helper function
u32 ADC_BackupInjected(ADC_t* A);
u32 ADC_LsbTo_mV(ADC_t* A, u32 Lsb);
u32 ADC_ConvertNormalTo_mV(ADC_t* A);
u32 ADC_ConvertInjectedTo_mV(ADC_t* A);

s32 ADC_Convert_VRefByLsb(ADC_t* A, u32 Lsb);
s32 ADC_FeedbackVdd(ADC_t* A, u32 Vdd_mV);


#endif