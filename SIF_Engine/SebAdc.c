
#include "SebEngine.h"
#include "SebAdc.h"

// First, we need to match the pin name with the ADC number and its channel
const u16 ADC_Cycles[] = {
3, //#define ADC_SampleTime_3Cycles                    ((uint8_t)0x00)
15, //#define ADC_SampleTime_15Cycles                   ((uint8_t)0x01)
28, //#define ADC_SampleTime_28Cycles                   ((uint8_t)0x02)
56, //#define ADC_SampleTime_56Cycles                   ((uint8_t)0x03)
84, //#define ADC_SampleTime_84Cycles                   ((uint8_t)0x04)
112, //#define ADC_SampleTime_112Cycles                  ((uint8_t)0x05)
144, //#define ADC_SampleTime_144Cycles                  ((uint8_t)0x06)
480, //#define ADC_SampleTime_480Cycles                  ((uint8_t)0x07)
}; // this will output the number of cycles for each possible channel configuration 3 bit values

u32 ADC_CyclesToSampletime(u32 cy) {
  u32 n;
  for(n=0;n<countof(ADC_Cycles);n++) {
    if(cy<=ADC_Cycles[n]) break;
  }
  if(n) n--; // take the just below cy possible value, unless it is the minimum already! If did not find solutions take the biggest possible
  return n; // return the index (which is what to write to the register)
}


//============================== The public functions to build your system mecanics

u32 NewADC(ADC_t* A, ADC_TypeDef* ADCx, u32 VRef_mV, MCUClocks_t * Tree ) {
  
  ClockGateEnable_PPP((u32) ADCx, ENABLE); // Enable ADC Clocks
  if(VRef_mV==0) while(1);
  A->VRef_mV = VRef_mV;
  A->ADCx = ADCx;
  A->FeedClockMHz = Tree->APB2Clk_Hz;
  
  ADC_StructInit(&A->ADCI);
  
  ADC_CommonInitTypeDef ADCCI;
  ADC_CommonStructInit(&ADCCI);
  ADC_CommonInit(&ADCCI); // default, maybe can be hacked before the enable?
  
  return 0;
}

static void SetADC_Pin(IO_Pin_t* P) {
  
  IO_PinClockEnable(P);
  IO_PinSetInput(P);
  IO_PinSetLow(P);
  IO_PinSetSpeedMHz(P, 1);
  IO_PinEnablePullUpDown(P, DISABLE, DISABLE);// try mid voltage if it works
  IO_PinEnableHighDrive(P, DISABLE);
  IO_PinSetAnalog(P);  
}

static void SetADC_PinTrigger(IO_Pin_t* P) {

  IO_PinClockEnable(P);
  IO_PinSetInput(P);
  IO_PinSetLow(P);
  IO_PinSetSpeedMHz(P, 1);
  IO_PinEnablePullUpDown(P, ENABLE, DISABLE);
  IO_PinEnableHighDrive(P, DISABLE);
  IO_PinSetInput(P);
}

// Temperature Sensor, Vrefint and VBAT management functions ******************
//void ADC_TempSensorVrefintCmd(FunctionalState NewState);
//void ADC_VBATCmd(FunctionalState NewState);

u32 NewADC_NormalChannel(ADC_t* A, IO_Pin_t* P, u32 SampleTime_cy) {
  
  u32 n = A->NormalUsedChannelCount;
  // Configure the pin as analog input
  if(P==0) while(1); // no pin = problem!
  if(n==16) while(1); // no more slots available!
  
  A->NormalAnalogInPins[n] = P; // remember this pin
  A->NormalSampleTime_cy[n] = SampleTime_cy;

  ADC_Info_t* ADCI = ADC_GetByPPP_PinName(A->ADCx, P->Name);
  A->NormalChannel[n] = ADCI->Channel;
  
  n++;
  A->NormalUsedChannelCount = n;
  return 0;
}

u32 NewADC_InjectedChannel(ADC_t* A, IO_Pin_t* P, u32 SampleTime_cy) {

  u32 n = A->InjectedUsedChannelCount;
  // Configure the pin as analog input
  if(P==0) while(1); // no pin = problem!
  if(n==4) while(1); // no more slots available!
  
  A->InjectedAnalogInPins[n] = P; // remember this pin
  A->InjectedSampleTime_cy[n] = SampleTime_cy;

  ADC_Info_t* ADCI = ADC_GetByPPP_PinName(A->ADCx, P->Name);
  A->InjectedChannel[n] = ADCI->Channel;
  
  n++;
  A->InjectedUsedChannelCount = n;
  return 0;
}  


u32 UseADC_NormalTrigger(ADC_t* A, IO_Pin_t* T, u32 InternalTrigger) {
  
  A->NormalInternalTrigger = InternalTrigger;
  A->NormalTriggerPin = T;
  A->ADCI.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
  
  if(T) { // Physical Trigger Pin (can be probed by oscilloscope)
    if((T->Name & 0xF)!=11) while(1);
    A->NormalInternalTrigger = ADC_ExternalTrigConv_Ext_IT11;
  };

  return 0;
}

u32 UseADC_InjectedTrigger(ADC_t* A, IO_Pin_t* T, u32 InternalTrigger) {
  
  A->InjectedInternalTrigger = InternalTrigger;
  A->InjectedTriggerPin = T;
  A->ADCI.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
  
  if(T) { // Physical Trigger Pin (can be probed by oscilloscope)
    if((T->Name & 0xF)!=15) while(1);
    A->InjectedInternalTrigger = ADC_ExternalTrigInjecConv_Ext_IT15;
  };
  
  return 0;
}

// Regular Channels DMA Configuration functions *******************************
//void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState);
//void ADC_DMARequestAfterLastTransferCmd(ADC_TypeDef* ADCx, FunctionalState NewState);

void SetADC_Waveform(ADC_t* A, u32 Adr, u32 Size) {

  A->NormalSamplesAdr = Adr;
  A->NormalSamplesCount = Size;
}

// This will setup the DMA in circular mode for the normal channels
void SetADC_DMA(ADC_t* A, u32 Adr, u32 Size) {

  DMA_InitTypeDef       DMA_InitStructure;
  u32 Signal;
  if(A->ADCx == ADC1) Signal = ADC1_ANALOG;
  else
  if(A->ADCx == ADC2) Signal = ADC2_ANALOG;
  else
  if(A->ADCx == ADC3) Signal = ADC3_ANALOG;
  else
    while(1); // error!

  DMA_StreamChannelInfo_t* DSCI = Get_pStreamChannelForPPP_Signal((u32)A->ADCx, Signal, DMA_DIR_PeripheralToMemory);
  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DSCI->Channel;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&A->ADCx->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr = Adr;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = Size;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // Enable DMA clock (if used)
  DMA_Init(DSCI->Stream, &DMA_InitStructure);
  DMA_Cmd(DSCI->Stream, ENABLE);
  BookDMA_Stream(DSCI->Stream);
}




//========================================================================
// How to calculate which prescaler, which sampling time for each ADC?
/*
u32 ADC_ComputeCommonClocks(ADC_t* A, ADC_CommonInitTypeDef* ADCCI) {
  // We pass one ADC at a time, a
  // Started from:  ADC_CommonStructInit(ADCCI); // ADC_Mode_Independent, ADC_Prescaler_Div2 (4,6,8), ADC_DMAAccessMode_Disabled, ADC_TwoSamplingDelay_5Cycles
  u32 BaseClock_Hz = A->FeedClockMHz*1000000; // based on prescaler
  
  // we assume the worst case, where normal and injected channels are continuously running (and assume we don't scan the same channel more than once in a single sequence)
  u32 SamplingTime_cy = 3; // min
  u32 ConversionTime_cy = 12; // 12 bit
  u32 NbOfChannels = 0;
  u32 TotalNeededCycles = 0;
  u32 TotalSpareCycles = 0;
  u32 SamplingRate_Hz = 0;
  u32 n;
  u32 MaxAskedSamplingRate_Hz = 0;
  for(n=0;n<16;n++) {
    if(A->AnalogInPins[n]) {
      NbOfChannels++;
      TotalNeededCycles += 15; // 12 cycles for 12 bit conversion, 3 cycles for sampling time (minimum)
//      if(A->ChannelSamplingRate_Hz[n]>MaxAskedSamplingRate_Hz) MaxAskedSamplingRate_Hz = A->ChannelSamplingRate_Hz[n]; // max sampling rate
    };
    if((n<4)&&(A->InjectedAnalogInPins[n])) {
      NbOfChannels++;
      TotalNeededCycles += 15; // 12 cycles for 12 bit conversion, 3 cycles for sampling time (minimum)
//      if(A->InjectedChannelSamplingRate_Hz[n]>MaxAskedSamplingRate_Hz) MaxAskedSamplingRate_Hz = A->InjectedChannelSamplingRate_Hz[n]; // max sampling rate      
    }
  };
  
  // now we know the max total time to sweep through all channels.
  // Let's compare it with the min sampling rate for each channel
  SamplingRate_Hz = BaseClock_Hz / TotalNeededCycles;
  if(SamplingRate_Hz>MaxAskedSamplingRate_Hz) while(1); // too many channels! can't meet the required sampling rate for all of them!
  // Then there are spare time to spread to different channels evenly?
  // Here we will have to use the spare cycles for the prescaler (global)
  // In a future improvement of this implementation
  
  return 0;
}
*/

void ConfigureADC(ADC_t* A, ADC_ScanScheme NormalScheme, ADC_ScanScheme InjectedScheme) {
  
  u32 n;

  // Now we setup the common structure
  A->NormalScheme = NormalScheme;
  A->InjectedScheme = InjectedScheme;
  
  if(A->NormalSamplesCount) { // if the number of normal samples to aquires is non zero, we use DMA, otherwise, it's a single channel which will be used (discontinuous mode?)

    u32 Adr = A->NormalSamplesAdr;
    u32 Size = A->NormalSamplesCount;
    
    if(Adr==0) { // If no address, use the ADC's RAM limited RAM buffer to its fullest
      Adr = (u32)&A->ADC_Values_Lsb[0];
      Size = countof(A->ADC_Values_Lsb); // use the whole RAM space
    }
    
    SetADC_DMA(A, Adr, Size); // one channel only, DMA will rollover the specified RAM area
    ADC_DMARequestAfterLastTransferCmd(A->ADCx, ENABLE);
    ADC_DMACmd(A->ADCx, ENABLE);
  }
  
  // Initialize the GPIOs, channels and triggers
  // more than 1 channel is used, we run the circular mode in a sequence (discontinous?) method
  // Normal Channels
  for(n=0;n<A->NormalUsedChannelCount;n++) {
    if(A->NormalAnalogInPins[n]==0) while(1); //abnormal
    ADC_RegularChannelConfig(A->ADCx, A->NormalChannel[n], n+1, ADC_CyclesToSampletime(A->NormalSampleTime_cy[n]));
    SetADC_Pin(A->NormalAnalogInPins[n]);
  };

  // Injected Channels
  for(n=0;n<A->InjectedUsedChannelCount;n++) {
    if(A->InjectedAnalogInPins[n]==0) break; //abnormal
    ADC_InjectedChannelConfig(A->ADCx, A->InjectedChannel[n], n+1, ADC_CyclesToSampletime(A->InjectedSampleTime_cy[n]));
    SetADC_Pin(A->InjectedAnalogInPins[n]);
  };
  
  // Normal Trigger (we need to check for double booking as same trigger may be shared by multiple ADCs)
  if(A->NormalTriggerPin && (A->NormalInternalTrigger!=SHARED)) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SetADC_PinTrigger(A->NormalTriggerPin);
    EXTI_SetEdgesEnable(A->NormalTriggerPin->Name, ENABLE, DISABLE); // Rising Edge is the trigger/event
    EXTI_SetEvent(A->NormalTriggerPin->Name & 0xF, ENABLE); // no checks on overbooking here...
  };
  
  // Injected Trigger (we need to check for double booking as same trigger may be shared by multiple ADCs)
  if(A->InjectedTriggerPin && (A->InjectedInternalTrigger!=SHARED)) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SetADC_PinTrigger(A->InjectedTriggerPin);
    EXTI_SetEdgesEnable(A->InjectedTriggerPin->Name, ENABLE, DISABLE); // Rising Edge is the trigger/event
    EXTI_SetEvent(A->InjectedTriggerPin->Name & 0xF, ENABLE); // no checks on overbooking here...
  };
  

// ADC Init related call  
  A->ADCI.ADC_Resolution = ADC_Resolution_12b;
  A->ADCI.ADC_ScanConvMode = ENABLE; // Always
  A->ADCI.ADC_ContinuousConvMode = (NormalScheme==ADC_CONTINUOUS)?ENABLE:DISABLE;
  A->ADCI.ADC_ExternalTrigConvEdge = (NormalScheme==ADC_TRIGGERED)?ADC_ExternalTrigConvEdge_Rising:ADC_ExternalTrigConvEdge_None;
  A->ADCI.ADC_ExternalTrigConv = A->NormalInternalTrigger;
  A->ADCI.ADC_DataAlign = ADC_DataAlign_Left; // always 16 bit value regardless of the number of significant bits
  // for now we only use normal channels.
  A->ADCI.ADC_NbrOfConversion = A->NormalUsedChannelCount; // Number of conversion in the sequence (for normal?)
  // how to deal with injected?
  ADC_Init(A->ADCx, &A->ADCI);
}


void EnableADC(ADC_t* A) {

  ADC_Cmd(A->ADCx, ENABLE);
  NOPs(100);
  
  if((A->NormalScheme==ADC_TRIGGERED)||(A->NormalScheme==ADC_ONCE)) // the S/W trigger will be done later
    return;
  
  ADC_SoftwareStartConv(A->ADCx); // kick in (trigger it) continuous or one shot mode
}

void StartADC_NormalConversion(ADC_t* A) {
  
  ADC_SoftwareStartConv(A->ADCx); // kick in (trigger it) continuous or one shot mode
}

//==================== Build a demonstration code for various modes

//===== 8>< --------------------------
// FTSC2 
// ADC1 = PC1 channel 11
// ADC2 = PC2 channel 12
// ADC3 = PC3 channel 13
// ADC Trigger = PG11 (or Timer6?) Rising Edge launch the conversion
// 
#define ADC_SAMPLING_WAVE_SIZE 8192
u16 ADC1_Wave[ADC_SAMPLING_WAVE_SIZE];
u16 ADC2_Wave[ADC_SAMPLING_WAVE_SIZE];
u16 ADC3_Wave[ADC_SAMPLING_WAVE_SIZE];


ADC_t myAdc1, myAdc2, myAdc3;
IO_Pin_t myPA4, myPC1; 
IO_Pin_t myPH11, myPH15; // PH11=normal PH15=injected

IO_Pin_t myPA0, myPA1; // main connector
IO_Pin_t myPF4, myPF5, myPF6, myPF7, myPF8, myPF9; // main connector
IO_Pin_t myPE11;
// PA4 ADC12ch4
// PC1 ADC123ch1
void ADC_Demo(void) {
  
  ADC_t* Adc1 = &myAdc1;
  ADC_t* Adc2 = &myAdc2;
  ADC_t* Adc3 = &myAdc3;

  NewADC(Adc1, ADC1, 3300, GetMCUClockTree() );
  NewADC(Adc2, ADC2, 3300, GetMCUClockTree() );
  NewADC(Adc3, ADC3, 3300, GetMCUClockTree() );
  
  // for now all channels are NORMAL
  NewADC_NormalChannel(Adc1, NewIO_Pin(&myPA4, PA4), 100); 
  
  NewADC_NormalChannel(Adc2, NewIO_Pin(&myPC1, PC1), 40);
  
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPA0, PA0), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPA1, PA1), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF4, PF4), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF5, PF5), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF6, PF6), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF7, PF7), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF8, PF8), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF9, PF9), 4); 
  
  UseADC_NormalTrigger(Adc1, NewIO_Pin(&myPH11, PH11), 0);
  UseADC_NormalTrigger(Adc2, 0, ADC_ExternalTrigConv_Ext_IT11); // won't rebook the pin
  UseADC_NormalTrigger(Adc3, 0, ADC_ExternalTrigConv_Ext_IT11); // won't rebook the pin 

  SetADC_Waveform(Adc1, (u32)ADC1_Wave, countof(ADC1_Wave));
  SetADC_Waveform(Adc2, (u32)ADC2_Wave, countof(ADC2_Wave));
  SetADC_Waveform(Adc3, (u32)ADC3_Wave, countof(ADC3_Wave));
  
  ConfigureADC(Adc1, ADC_CONTINUOUS, ADC_DISABLED);
  ConfigureADC(Adc2, ADC_CONTINUOUS, ADC_DISABLED);
  ConfigureADC(Adc3, ADC_ONCE, ADC_DISABLED);
  
  EnableADC(Adc1);
  EnableADC(Adc2);
  EnableADC(Adc3);  
 // NVIC_ADC_Enable(ENABLE);  
  while(1) {
    StartADC_NormalConversion(Adc3);
    Wait_ms(1);
  }
}

/*
// Initialization and Configuration functions *********************************
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_CommonInit(ADC_CommonInitTypeDef* ADC_CommonInitStruct);
void ADC_CommonStructInit(ADC_CommonInitTypeDef* ADC_CommonInitStruct);
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);

// Regular Channels Configuration functions ***********************************
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_SoftwareStartConv(ADC_TypeDef* ADCx);
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx);
void ADC_EOCOnEachRegularChannelCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ContinuousModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number);
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
uint32_t ADC_GetMultiModeConversionValue(void);

// Injected channels Configuration functions **********************************
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, uint8_t Length);
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset);
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv);
void ADC_ExternalTrigInjectedConvEdgeConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConvEdge);
void ADC_SoftwareStartInjectedConv(ADC_TypeDef* ADCx);
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx);
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel);
*/

//============================
// THRESHOLD SETUP FUNCTIONS
// Analog Watchdog configuration functions ************************************
//void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog);
//void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold,uint16_t LowThreshold);
//void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel);

u32 SetADC_Threshold_Pin_Min_Max_mV(ADC_t* A, IO_Pin_t* P, u32 MinThreshold_mV, u32 MaxThreshold_mV) {
  
  if(A==0) while(1);
  if(A->VRef_mV==0) while(1); // we need this to convert to LSB!

  A->MinThreshold_mV = MinThreshold_mV;
  A->MaxThreshold_mV = MaxThreshold_mV;
  A->ThresholdMonitoredPin = P;

  ADC_Info_t* ADCI = ADC_GetByPPP_PinName(A->ADCx, P->Name);
  
  if(MaxThreshold_mV==0) {
    ADC_AnalogWatchdogCmd(A->ADCx, ADC_AnalogWatchdog_None); // disable the feature
    return 0;
  }

  u32 MinLsb = Interpolate_s32 (0, A->VRef_mV, 0, 0xFFF, MinThreshold_mV ); // convert to a positive 12 bit quantity
  u32 MaxLsb = Interpolate_s32 (0, A->VRef_mV, 0, 0xFFF, MaxThreshold_mV ); // convert to a positive 12 bit quantity
  
  ADC_AnalogWatchdogThresholdsConfig(A->ADCx, MaxLsb, MinLsb);
  ADC_AnalogWatchdogSingleChannelConfig(A->ADCx, ADCI->Channel);
  ADC_AnalogWatchdogCmd(A->ADCx, ADC_AnalogWatchdog_SingleRegOrInjecEnable);
  
  return 0;
}

//===============================
// Interrupt related functions

// Interrupts and flags management functions **********************************
//void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);
//FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
//void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
//ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT);
//void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT);
/*
void NVIC_ADC_Enable(ADC_t A, FunctionalState Enable) {
  
}
*/
// here we will put the ISR for the ADC channels