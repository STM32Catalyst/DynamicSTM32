
#include "SebEngine.h"

void ADCx_IRQHandler(u32 u);
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

static u32 ADC_CyclesToSampletime(u32 cy) {
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
  A->FeedClockHz = Tree->APB2Clk_Hz;
  
  ADC_StructInit(&A->ADCI);
  
  ADC_CommonInitTypeDef ADCCI;
  ADC_CommonStructInit(&ADCCI);
  ADC_CommonInit(&ADCCI); // default, maybe can be hacked before the enable?
  
  // we hook the NVIC interrupt handler
  HookIRQ_PPP((u32)A->ADCx, (u32) ADCx_IRQHandler, (u32) A);
  
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
  
  u8 n = A->NormalUsedChannelCount;
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

  u8 n = A->InjectedUsedChannelCount;
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

//===== special internal channels

u32 NewADC_NormalChannelInternal(ADC_t* A, ADC_Internal_Signals Channel) {
  
  u8 n = A->NormalUsedChannelCount;
  u8 ADC_Channel;
  
  if(A->ADCx!=ADC1) while(1); // Error, these channels are only connected to ADC1, they have no meaning in ADC2 and ADC3
  
  switch(Channel) {
  case ADC_Temp: //                      ((uint8_t)ADC_Channel_16)
    ADC_Channel = ADC_Channel_16;
    ADC_TempSensorVrefintCmd(ENABLE);
    break;
  case ADC_VRef: //                         ((uint8_t)ADC_Channel_17)
    ADC_Channel = ADC_Channel_17;
    ADC_TempSensorVrefintCmd(ENABLE);
    break;
  case ADC_VBat: //                            ((uint8_t)ADC_Channel_18)  
    ADC_Channel = ADC_Channel_18;
    ADC_VBATCmd(ENABLE);
    break;
  default:while(1);
  };
  
  A->NormalSampleTime_cy[n] = 1000;
  A->NormalChannel[n] = ADC_Channel;
  
  n++;
  A->NormalUsedChannelCount = n;
  return 0;
}

u32 NewADC_InjectedChannelInternal(ADC_t* A, ADC_Internal_Signals Channel) {
  
  u8 n = A->InjectedUsedChannelCount;
  u8 ADC_Channel;
  
  if(A->ADCx!=ADC1) while(1); // Error, these channels are only connected to ADC1, they have no meaning in ADC2 and ADC3
  
  switch(Channel) {
  case ADC_Temp: //                      ((uint8_t)ADC_Channel_16)
    ADC_Channel = ADC_Channel_18;
    ADC_TempSensorVrefintCmd(ENABLE);
    break;
  case ADC_VRef: //                         ((uint8_t)ADC_Channel_17)
    ADC_Channel = ADC_Channel_17;
    ADC_TempSensorVrefintCmd(ENABLE);
    break;
  case ADC_VBat: //                            ((uint8_t)ADC_Channel_18)  
    ADC_Channel = ADC_Channel_18;
    ADC_VBATCmd(ENABLE);
    break;
  default:while(1);
  };
  
  A->InjectedSampleTime_cy[n] = 1000;
  A->InjectedChannel[n] = ADC_Channel;
  
  n++;
  A->InjectedUsedChannelCount = n;
  return 0;
}  

//=====



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
static void SetADC_DMA(ADC_t* A, u32 Adr, u32 Size) {

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
static void CalculateADC_Timings(ADC_t* A) {
  u8 n;
  
  A->NormalTotalCycle_cy=A->InjectedMaxSamplingRate_Hz=0;
  // we check how long it takes to sweep through all the defined normal and injected channels
  for(n=0;n<A->NormalUsedChannelCount;n++) // ADC 12 bits takes 
    A->NormalTotalCycle_cy += (12 + A->NormalSampleTime_cy[n]);
  
  for(n=0;n<A->InjectedUsedChannelCount;n++)
    A->InjectedTotalCycle_cy += 12 + A->InjectedSampleTime_cy[n];
  
  // we assume the APB2 div2 prescaler mode is used

  if(A->NormalTotalCycle_cy)
    A->NormalMaxSamplingRate_Hz = (A->FeedClockHz / 2) / A->NormalTotalCycle_cy;
  
  if(A->InjectedTotalCycle_cy)
    A->InjectedMaxSamplingRate_Hz = (A->FeedClockHz / 2) / A->InjectedTotalCycle_cy;
}


void ConfigureADC(ADC_t* A, ADC_ScanScheme NormalScheme, ADC_ScanScheme InjectedScheme) {
  
  u32 n;

  // Now we setup the common structure
  A->NormalScheme = NormalScheme;
  A->InjectedScheme = InjectedScheme;
  
  if(A->NormalSamplesCount) { // if the number of normal samples to aquires is non zero, we use DMA, otherwise, it's a single channel which will be used (discontinuous mode?)

    u32 Adr = A->NormalSamplesAdr;
    u32 Size = A->NormalSamplesCount;
    
    if(Adr==0) { // If no address, use the ADC's RAM limited RAM buffer to its fullest
      Adr = (u32)&A->Normal_Lsb[0];
      if(Size>countof(A->Normal_Lsb)) while(1); // use the whole RAM space
      if(Size==0) Size = countof(A->Normal_Lsb); // if no size is given, we use the buffer size
    }
    
    SetADC_DMA(A, Adr, Size); // one channel only, DMA will rollover the specified RAM area
    ADC_DMARequestAfterLastTransferCmd(A->ADCx, ENABLE);
    ADC_DMACmd(A->ADCx, ENABLE);
  }
  
  // Initialize the GPIOs, channels and triggers
  // more than 1 channel is used, we run the circular mode in a sequence (discontinous?) method
  // Normal Channels
  for(n=0;n<A->NormalUsedChannelCount;n++) {
    ADC_RegularChannelConfig(A->ADCx, A->NormalChannel[n], n+1, ADC_CyclesToSampletime(A->NormalSampleTime_cy[n]));
    if(A->NormalAnalogInPins[n])
      SetADC_Pin(A->NormalAnalogInPins[n]);
  };

  // Injected Channels
  for(n=0;n<A->InjectedUsedChannelCount;n++) {
    ADC_InjectedChannelConfig(A->ADCx, A->InjectedChannel[n], n+1, ADC_CyclesToSampletime(A->InjectedSampleTime_cy[n]));
    if(A->InjectedAnalogInPins[n])
      SetADC_Pin(A->InjectedAnalogInPins[n]);
  };
  
  A->ADCx->JSQR = 0; // re-do it better
  if(A->InjectedUsedChannelCount) {
    for(n=0;n<A->InjectedUsedChannelCount;n++)
      ClearAndSetShiftedBits(A->ADCx->JSQR, 0xF, A->InjectedChannel[n], (4-A->InjectedUsedChannelCount)*5+n*5);
    ClearAndSetShiftedBits(A->ADCx->JSQR, 0x3, A->InjectedUsedChannelCount-1, 20);      
  }
  
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
  
  // we have to activate the injected trigger if it is selected
  if(InjectedScheme == ADC_TRIGGERED) { // setup the triggered pin mode
//void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv);
    ADC_ExternalTrigInjectedConvConfig(A->ADCx, ADC_ExternalTrigInjecConv_Ext_IT15);
//void ADC_ExternalTrigInjectedConvEdgeConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConvEdge);    
    ADC_ExternalTrigInjectedConvEdgeConfig(A->ADCx, ADC_ExternalTrigInjecConvEdge_Rising);
  }
  
  CalculateADC_Timings(A); // calculate the sweep time in cycles and Hz
  
  // If you want to hack the configuration, put the alteration after calling this function and the EnableADC();
}


void EnableADC(ADC_t* A) {

  ADC_Cmd(A->ADCx, ENABLE);
  NOPs(100);
  
  // Normal channels, kick in the continuous if it is selected
  if(A->NormalScheme==ADC_CONTINUOUS) // the S/W trigger will be done later
    ADC_SoftwareStartConv(A->ADCx); // kick in (trigger it) continuous or one shot mode
  
  
  if(A->InjectedScheme==ADC_INJECTED_AS_NORMAL) { // When normal uses trigger pin, same trigger pin can be used for injected...
    ADC_AutoInjectedConvCmd(A->ADCx, ENABLE); // The injected channels will be converted following normal conversions
  }
  
  // Injected channels, kick in the continuous if it is selected
  if(A->InjectedScheme==ADC_CONTINUOUS) {
    if(A->NormalScheme!=ADC_CONTINUOUS) while(1); // it is not possible
    ADC_AutoInjectedConvCmd(A->ADCx, ENABLE); // The injected channels will be converted following normal conversions
    ADC_SoftwareStartInjectedConv(A->ADCx);
    return;
  }
  
  // Injected channels, kick in the continuous if it is selected
  if(A->InjectedScheme==ADC_TRIGGERED) { // this will use the Px15 pin as possible trigger
    return;
  }

  
}

void StartADC_NormalConversion(ADC_t* A) {
  
  ADC_SoftwareStartConv(A->ADCx); // kick in (trigger it) continuous or one shot mode
}

void StartADC_InjectedConversion(ADC_t* A) {
  
  ADC_SoftwareStartInjectedConv(A->ADCx); // kick in (trigger it) continuous or one shot mode
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
// The possible interrupts generated by this would be conversion complete by normal or injected channels
// and the watchdog

// Interrupts and flags management functions **********************************
//void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);
//FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
//void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
//ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT);
//void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT);

void NVIC_ADCsEnable(FunctionalState Enable) {
  
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = Enable;

  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_Init(&NVIC_InitStructure);  
}

// here we will put the ISR for the ADC channels
void HookADC(ADC_t* A, u16 ADC_IT, u32 fn, u32 ct) {

  if(ADC_IT == ADC_IT_EOC) { A->fnNormalDone = fn; A->ctNormalDone = ct; };
  if(ADC_IT == ADC_IT_AWD) { A->fnThreshold = fn; A->ctThreshold = ct; };
  if(ADC_IT == ADC_IT_JEOC) { A->fnInjectedDone = fn; A->ctInjectedDone = ct; };
  if(ADC_IT == ADC_IT_OVR) { while(1); }; // no hook for this case

}

//            @arg ADC_IT_EOC: End of conversion interrupt mask
//            @arg ADC_IT_AWD: Analog watchdog interrupt mask
//            @arg ADC_IT_JEOC: End of injected conversion interrupt mask
//            @arg ADC_IT_OVR: Overrun interrupt enable   

//            @arg ADC_FLAG_AWD: Analog watchdog flag
//            @arg ADC_FLAG_EOC: End of conversion flag
//            @arg ADC_FLAG_JEOC: End of injected group conversion flag
//            @arg ADC_FLAG_JSTRT: Start of injected group conversion flag
//            @arg ADC_FLAG_STRT: Start of regular group conversion flag
//            @arg ADC_FLAG_OVR: Overrun flag    
void ADC_InterruptEnable(ADC_t* A, u16 ADC_IT, FunctionalState NewState) {
  
  u32 Flag = 0;
  if(ADC_IT == ADC_IT_EOC) Flag |= ADC_FLAG_EOC;
  if(ADC_IT == ADC_IT_AWD) Flag |= ADC_FLAG_AWD;
  if(ADC_IT == ADC_IT_JEOC) Flag |= ADC_FLAG_JEOC;
  if(ADC_IT == ADC_IT_OVR) Flag |= ADC_FLAG_OVR;
    
  ADC_ClearFlag(A->ADCx, Flag); // clear any pending flags
  ADC_ClearITPendingBit(A->ADCx, ADC_IT); // clear any pending interrupt request
  ADC_ITConfig(A->ADCx, ADC_IT, NewState); // enable/disable the interrupt request
}
u32 ADC_DR;
void ADCx_IRQHandler(u32 u) {
  
  ADC_t* A = (ADC_t *) u;
  ADC_DR = A->ADCx->SR;
  //Toggle_LED_4(); // for debug purpose
  // First, we need to find out which interrupt we have to deal with
  if(ADC_GetITStatus(A->ADCx, ADC_IT_EOC)==SET) {
    A->NormalDone = 1;
    if(A->fnNormalDone) ((u32(*)(u32))A->fnNormalDone)(A->ctNormalDone);
    ADC_ClearITPendingBit(A->ADCx, ADC_IT_EOC);    
  }

  if(ADC_GetITStatus(A->ADCx, ADC_IT_JEOC)==SET) {
    A->InjectedDone = 1;
    if(A->fnInjectedDone) ((u32(*)(u32))A->fnInjectedDone)(A->ctInjectedDone);
    ADC_ClearITPendingBit(A->ADCx, ADC_IT_JEOC);    
  }

  if(ADC_GetITStatus(A->ADCx, ADC_IT_AWD)==SET) {
    A->Threshold = 1;
    if(A->fnThreshold) ((u32(*)(u32))A->fnThreshold)(A->ctThreshold);
    ADC_ClearITPendingBit(A->ADCx, ADC_IT_AWD);    
  }

  if(ADC_GetITStatus(A->ADCx, ADC_IT_OVR)==SET) {
    A->Overflow = 1;
    ADC_ClearITPendingBit(A->ADCx, ADC_IT_OVR);
    while(1); // error
  }
  
}

// helper functions
// Helper function
u32 ADC_BackupInjected(ADC_t* A) {
  
    A->Injected_Lsb[0] = A->ADCx->JDR1; // copy all faster than test
    A->Injected_Lsb[1] = A->ADCx->JDR2;
    A->Injected_Lsb[2] = A->ADCx->JDR3;
    A->Injected_Lsb[3] = A->ADCx->JDR4;
    return 0;
}

u32 ADC_Normal_LsbTo_mV(ADC_t* A, u32 Lsb) {

  u32 mV = Interpolate_s32 (0, 0xFFF0, 0, A->VRef_mV, Lsb);  
  return mV;
}

u32 ADC_Injected_LsbTo_mV(ADC_t* A, u32 Lsb) {

  u32 mV = Interpolate_s32 (0, (s32)0x7FF8, 0, A->VRef_mV, (s32)(s16)Lsb);  
  return mV;
}


u32 ADC_ConvertNormalTo_mV(ADC_t* A) {

  u8 n;
  for(n=0;n<countof(A->Normal_Lsb);n++) {
    A->Normal_mV[n] = ADC_Normal_LsbTo_mV(A, A->Normal_Lsb[n]);
  }
  return n;
}

u32 ADC_ConvertInjectedTo_mV(ADC_t* A) {

  u8 n;
  for(n=0;n<countof(A->Injected_Lsb);n++) {
    A->Injected_mV[n] = ADC_Injected_LsbTo_mV(A, A->Injected_Lsb[n]);
  }
  return n;
}

s32 ADC_Convert_mV_to_DegC_x10(ADC_t* A, u32 mV) {
  
  s32 Deg30Lsb = (s32)(*((u16*) 0x1FFF7A2C));
  s32 Deg110Lsb = (s32)(*((u16*) 0x1FFF7A2E));
  
  // we first convert the LSB calibrated Flash values to mV (let's remove the sign issue between normal and injected channels)
  u32 Deg30mV = Interpolate_s32 (0, 0x0FFF, 0, A->VRef_mV, Deg30Lsb);
  u32 Deg110mV = Interpolate_s32 (0, 0x0FFF, 0, A->VRef_mV, Deg110Lsb);
  u32 DegC_x10 = Interpolate_s32 (Deg30mV, Deg110mV, 300, 1100, (s32)(s16)mV);
  A->Temp_degC_x10 = DegC_x10; // capture the value for debugging or reference
  
  return DegC_x10;
}

s32 ADC_Convert_VRefByLsb(ADC_t* A, u32 Lsb) {
  
  s32 VRefLsb3p3V_Lsb = (s32)(*((u16*) 0x1FFF7A2A)); // this contains the ADC 12 bit right aligned injected raw data for Vref LSB at 30C and 3.3V
  u32 Vdd_mV = (3300*(VRefLsb3p3V_Lsb<<3))/Lsb;
  A->MeasuredVdd_mV = Vdd_mV;

  return 0;
}

s32 ADC_FeedbackVdd(ADC_t* A, u32 Vdd_mV) {
  
  A->VRef_mV = A->MeasuredVdd_mV; // hmm....
}