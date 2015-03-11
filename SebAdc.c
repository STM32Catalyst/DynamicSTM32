
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
    if(cy>ADC_Cycles[n]) break;
  }
  
  if(n) n--; // take the just below cy possible value, unless it is the minimum already! If did not find solutions take the biggest possible
  
  return n; // return the index (which is what to write to the register)
}



typedef struct {
  
  Signal_t Signal;
  PinNameDef PinName;
  ADC_TypeDef* ADCx;
  u16       Channel;
  
} ADC_Info_t;


ADC_Info_t ADCI[] = {
  
  {ADC1_IN0, PA0, ADC1, ADC_Channel_0},
  {ADC2_IN0, PA0, ADC2, ADC_Channel_0},
  {ADC3_IN0, PA0, ADC3, ADC_Channel_0},
  
  {ADC1_IN1, PA1, ADC1, ADC_Channel_1},
  {ADC2_IN1, PA1, ADC2, ADC_Channel_1},
  {ADC3_IN1, PA1, ADC3, ADC_Channel_1},
  
  {ADC1_IN2, PA2, ADC1, ADC_Channel_2},
  {ADC2_IN2, PA2, ADC2, ADC_Channel_2},
  {ADC3_IN2, PA2, ADC3, ADC_Channel_2},
  
  {ADC1_IN3, PA3, ADC1, ADC_Channel_3},
  {ADC2_IN3, PA3, ADC2, ADC_Channel_3},
  {ADC3_IN3, PA3, ADC3, ADC_Channel_3},
  
  {ADC1_IN4, PA4, ADC1, ADC_Channel_4},
  {ADC2_IN4, PA4, ADC2, ADC_Channel_4},
  {ADC3_IN4, PF6, ADC3, ADC_Channel_4},
  
  {ADC1_IN5, PA5, ADC1, ADC_Channel_5},
  {ADC2_IN5, PA5, ADC2, ADC_Channel_5},
  {ADC3_IN5, PF7, ADC3, ADC_Channel_5},
  
  {ADC1_IN6, PA6, ADC1, ADC_Channel_6},
  {ADC2_IN6, PA6, ADC2, ADC_Channel_6},
  {ADC3_IN6, PF8, ADC3, ADC_Channel_6},
  
  {ADC1_IN7, PA7, ADC1, ADC_Channel_7},
  {ADC2_IN7, PA7, ADC2, ADC_Channel_7},
  {ADC3_IN7, PF9, ADC3, ADC_Channel_7},
  
  {ADC1_IN8, PB0, ADC1, ADC_Channel_8},
  {ADC2_IN8, PB0, ADC2, ADC_Channel_8},
  {ADC3_IN8, PF10, ADC3, ADC_Channel_8},
  
  {ADC1_IN9, PB1, ADC1, ADC_Channel_9},
  {ADC2_IN9, PB1, ADC2, ADC_Channel_9},
  {ADC3_IN9, PF3, ADC3, ADC_Channel_9},
  
  {ADC1_IN10, PC0, ADC1, ADC_Channel_10},
  {ADC2_IN10, PC0, ADC2, ADC_Channel_10},
  {ADC3_IN10, PC0, ADC3, ADC_Channel_10},
  
  {ADC1_IN11, PC1, ADC1, ADC_Channel_11},
  {ADC2_IN11, PC1, ADC2, ADC_Channel_11},
  {ADC3_IN11, PC1, ADC3, ADC_Channel_11},
  
  {ADC1_IN12, PC2, ADC1, ADC_Channel_12},
  {ADC2_IN12, PC2, ADC2, ADC_Channel_12},
  {ADC3_IN12, PC2, ADC3, ADC_Channel_12},
  
  {ADC1_IN13, PC3, ADC1, ADC_Channel_13},
  {ADC2_IN13, PC3, ADC2, ADC_Channel_13},
  {ADC3_IN13, PC3, ADC3, ADC_Channel_13},
  
  {ADC1_IN14, PC4, ADC1, ADC_Channel_14},
  {ADC2_IN14, PC4, ADC2, ADC_Channel_14},
  {ADC3_IN14, PF4, ADC3, ADC_Channel_14},
  
  {ADC1_IN15, PC5, ADC1, ADC_Channel_15},
  {ADC2_IN15, PC5, ADC2, ADC_Channel_15},
  {ADC3_IN15, PF5, ADC3, ADC_Channel_15},

// internal signals connected to ADCs  
  {ADC1_VTEMP, 0, ADC1, ADC_Channel_TempSensor},//16
  {ADC2_VTEMP, 0, ADC2, ADC_Channel_TempSensor},
  {ADC3_VTEMP, 0, ADC3, ADC_Channel_TempSensor},
  
  {ADC1_VREF, 0, ADC1, ADC_Channel_Vrefint},//17
  {ADC2_VREF, 0, ADC2, ADC_Channel_Vrefint},
  {ADC3_VREF, 0, ADC3, ADC_Channel_Vrefint},
  
  {ADC1_VBAT, 0, ADC1, ADC_Channel_Vbat},//18
  {ADC2_VBAT, 0, ADC2, ADC_Channel_Vbat},
  {ADC3_VBAT, 0, ADC3, ADC_Channel_Vbat},
  
};

static ADC_Info_t* ADC_GetByPPP_PinName(ADC_TypeDef* ADCx, PinNameDef PinName) { // if there are multiple choice
  // if ADCx is null, we find the first match
  // if ADCx is non null, we use it for match check
  u32 n;
  
  for(n=0;n<countof(ADCI);n++)    
    if(ADCI[n].PinName == PinName)
      if((ADCx==0)||(ADCI[n].ADCx == ADCx))
        return &ADCI[n];
      
  while(1); // error, nothing found!
}


#if 0
static ADC_StreamChannelInfo_t* Get_TimerDMA_InfoByPPP_n(u32 PPP, u32 n) {

  Signal_t Signal = FindByADC_n((ADC_TypeDef*) PPP, n )->Signal;
  return Get_pStreamChannelForPPP_Signal((u32)PPP, Signal, DMA_DIR_PeripheralToMemory);
}
#endif









u32 NewADC(ADC_t* A, ADC_TypeDef* ADCx, MCUClocks_t * Tree ) {
  
  ClockGateEnable_PPP((u32) ADCx, ENABLE); // Enable ADC Clocks
  
  A->ADCx = ADCx;
  A->FeedClockMHz = Tree->APB2Clk_Hz;
  
  return 0;
}


static void SetADC_Pin(IO_Pin_t* P) {
  
  IO_PinClockEnable(P);
  IO_PinSetInput(P);
  IO_PinSetLow(P);
  IO_PinSetSpeedMHz(P, 1);
  IO_PinEnablePullUpDown(P, DISABLE, DISABLE);
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

u32 NewADC_Channel(ADC_t* A, IO_Pin_t* P, u32 SampleTime_cy) {
  
  u32 n;
  // Configure the pin as analog input
  if(P==0) while(1); // no pin = problem!
  
  for(n=0;n<16;n++) // find an empty slot
    if(A->AnalogInPins[n]==0) break;
  
  if(n==16) while(1); // no more slots available!
  
  A->AnalogInPins[n] = P; // remember this pin
  A->ChannelSamplingRate_Hz[n] = SampleTime_cy;
  SetADC_Pin(P);
  
  ADC_Info_t* ADCI = ADC_GetByPPP_PinName(A->ADCx, P->Name);
  
  ADC_RegularChannelConfig(A->ADCx, ADCI->Channel, n, ADC_CyclesToSampletime(SampleTime_cy));
  
  return 0;
}

u32 NewADC_InjectedChannel(ADC_t* A, IO_Pin_t* P, u32 SampleTime_cy) {

  u32 n;
  // Configure the pin as analog input
  if(P==0) while(1); // no pin = problem!
  
  for(n=0;n<4;n++) // find an empty slot
    if(A->InjectedAnalogInPins[n]==0) break;
  
  if(n==4) while(1); // no more slots available!
  
  A->InjectedAnalogInPins[n] = P; // remember this pin
  A->InjectedChannelSamplingRate_Hz[n] = SampleTime_cy;
  SetADC_Pin(P);
  
  ADC_Info_t* ADCI = ADC_GetByPPP_PinName(A->ADCx, P->Name);
  
  ADC_InjectedChannelConfig(A->ADCx, ADCI->Channel, n, ADC_CyclesToSampletime(SampleTime_cy));
  
  return 0;
}

u32 UseADC_Trigger(ADC_t* A, IO_Pin_t* T, u32 InternalTrigger) {
  
  if(T==0) while(1);

  u32 TriggerMode = InternalTrigger;
  
  if(T) { // Physical Trigger Pin (can be probed by oscilloscope)
    if((T->Name & 0xF)!=11) while(1);
    A->TriggerPin = T;
    SetADC_PinTrigger(T);
    TriggerMode = ADC_ExternalTrigConv_Ext_IT11;
  };
  
  //?????A->ADCI->ADC_ExternalTrigConv = TriggerMode;
  return 0;
}

u32 UseADC_InjectedTrigger(ADC_t* A, IO_Pin_t* T, u32 InternalTrigger) {
  
  if(T==0) while(1);

  u32 TriggerMode = InternalTrigger;
  
  if(T) { // Physical Trigger Pin (can be probed by oscilloscope)
    if((T->Name & 0xF)!=15) while(1);
    A->TriggerPin = T;
    SetADC_PinTrigger(T);
    TriggerMode = ADC_ExternalTrigInjecConv_Ext_IT15;
  };
  
//???  A->ADCI->ADC_InjectedExternalTrigConv = TriggerMode;
  return 0;
}

void SetADC_Waveform(ADC_t* A, u32 Adr, u32 Size) {
  
  
}

void EnableADC(DAC_t* D) {
  
}

void NVIC_ADCsEnable(FunctionalState Enable) {
  
  
}



void SetADC_DMA(ADC_t* A, u32 Adr, u32 Size) {

  DMA_InitTypeDef       DMA_InitStructure;

  /* DMA2 Stream0 channel2 configuration **************************************/
//  DMA_InitStructure.DMA_Channel = DMA_CHANNELx;  
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADCx_DR_ADDRESS;
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADCxConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // Enable DMA clock (if used)
//  DMA_Init(DMA_STREAMx, &DMA_InitStructure);
//  DMA_Cmd(DMA_STREAMx, ENABLE);
  
  
}



















//============================= FTSC2 CODE TO MIGRATE TO THE NEW STRUCTURE ====================>


/*
This example shows how to use the ADC3 and DMA to transfer continuously 
converted data from ADC3 to memory.
The ADC3 is configured to convert continuously channel7(for STM324xG-EVAL
and STM32437I-EVAL)/channel8(for STM324x9I-EVAL RevB).
Each time an end of conversion occurs the DMA transfers, in circular mode, the
converted data from ADC3 DR register to the uhADCxConvertedValue variable.

In this example, the system clock is 144MHz, APB2 = 72MHz and ADC clock = APB2/2. 
Since ADC3 clock is 36 MHz and sampling time is set to 3 cycles, the conversion 
time to 12bit data is 12 cycles so the total conversion time is (12+3)/36= 0.41us(2.4Msps).

User can vary the ADC3 channel7(for STM324xG-EVAL and STM32437I-EVAL)/channel8
(for STM324x9I-EVAL RevB) voltage using the Eval Board potentiometer.
The converted voltage is displayed on the Eval Board LCD (when the define USE_LCD
is enabled in main.h file)
*/
  #define ADCx                     ADC3
  #define ADC_CHANNEL              ADC_Channel_7
  #define ADCx_CLK                 RCC_APB2Periph_ADC3
  #define ADCx_CHANNEL_GPIO_CLK    RCC_AHB1Periph_GPIOF
  #define GPIO_PIN                 GPIO_Pin_9
  #define GPIO_PORT                GPIOF
  #define DMA_CHANNELx             DMA_Channel_2
  #define DMA_STREAMx              DMA2_Stream0
  #define ADCx_DR_ADDRESS          0x4001224C

#define ADC_SAMPLING_WAVE_SIZE 8192
extern u16 ADC1_Wave[ADC_SAMPLING_WAVE_SIZE];
extern u16 ADC2_Wave[ADC_SAMPLING_WAVE_SIZE];
extern u16 ADC3_Wave[ADC_SAMPLING_WAVE_SIZE];
//===== 8>< --------------------------
// FTSC2 
// ADC1 = PC1 channel 11
// ADC2 = PC2 channel 12
// ADC3 = PC3 channel 13
// ADC Trigger = PG11 (or Timer6?) Rising Edge launch the conversion
// 
//#define ADC_SAMPLING_WAVE_SIZE 8192
u16 ADC1_Wave[ADC_SAMPLING_WAVE_SIZE];
u16 ADC2_Wave[ADC_SAMPLING_WAVE_SIZE];
u16 ADC3_Wave[ADC_SAMPLING_WAVE_SIZE];

#define ADC1_DR_ADDRESS          ((uint32_t)0x4001204C)
#define ADC2_DR_ADDRESS          ((uint32_t)0x4001214C)
#define ADC3_DR_ADDRESS          ((uint32_t)0x4001224C)

void ADC_Streams_Init(void) {

  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;

  /* Enable ADCx, DMA and GPIO clocks ****************************************/ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOG, ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
  

  /* DMA1 Stream0 channel0 ADC1 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1_Wave;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = ADC_SAMPLING_WAVE_SIZE;
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
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);
  
  // ADC2 DMA2 Stream 3 Channel 1 Stream 3 ADC2 modified
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC2_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC2_Wave;
  DMA_Init(DMA2_Stream3, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream3, ENABLE);

  // ADC3 DMA2 Stream 1 Channel 2 Stream 0 modified
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3_Wave;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream1, ENABLE);
  
  /* Configure ADC1 Channel11/12/13 PC1/2/3 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //========================================
  // trigger pin PG11
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Configure PG9 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* Connect EXTI Line11 to PG11 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource11);

  /* Configure EXTI Line11 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;//EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //EXTI_Trigger_Falling
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI11_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
  
  
//====  
  
  
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//4/6/8
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5..20
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1/2/3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//10/8/6
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;//ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_Ext_IT11;// ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_Init(ADC2, &ADC_InitStructure);
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC1/2/3 regular channel1/12/13 configuration **************************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_3Cycles);// 3/15/28/56/84/112/144/480
  ADC_RegularChannelConfig(ADC2, ADC_Channel_12, 2, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 3, ADC_SampleTime_3Cycles);

 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

  /* Enable ADC1/2/3 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  ADC_DMACmd(ADC2, ENABLE);
  ADC_DMACmd(ADC3, ENABLE);

  /* Enable ADC1/2/3 */
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  ADC_Cmd(ADC3, ENABLE);


}

__IO uint16_t uhADCxConvertedValue = 0;
__IO uint32_t uwADCxConvertedVoltage = 0;

void ADC_Stream_Main(void) {
  
  /* ADC configuration */
  ADC_Config();

  /* Start ADC Software Conversion */ 
  ADC_SoftwareStartConv(ADCx);  

  while (1)
  {
    uwADCxConvertedVoltage = uhADCxConvertedValue *3300/0xFFF;
  };
}

/**
  * @brief  ADC3 channel07 with DMA configuration
  * @note   This function Configure the ADC peripheral  
            1) Enable peripheral clocks
            2) DMA2_Stream0 channel2 configuration
            3) Configure ADC Channel7 pin as analog input
            4) Configure ADC3 Channel7 
  * @param  None
  * @retval None
  */
void ADC_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADCx, DMA and GPIO clocks ****************************************/ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_AHB1PeriphClockCmd(ADCx_CHANNEL_GPIO_CLK, ENABLE);  
  RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);
  

  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_CHANNELx;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADCx_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADCxConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA_STREAMx, &DMA_InitStructure);
  DMA_Cmd(DMA_STREAMx, ENABLE);

  /* Configure ADC3 Channel7 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADCx, &ADC_InitStructure);

  /* ADC3 regular channel7 configuration **************************************/
  ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);

 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADCx, ENABLE);

  /* Enable ADC3 DMA */
  ADC_DMACmd(ADCx, ENABLE);

  /* Enable ADC3 */
  ADC_Cmd(ADCx, ENABLE);
}

#if 0
typedef struct 
{
  uint32_t ADC_Mode;                      /*!< Configures the ADC to operate in 
                                               independent or multi mode. 
                                               This parameter can be a value of @ref ADC_Common_mode */                                              
  uint32_t ADC_Prescaler;                 /*!< Select the frequency of the clock 
                                               to the ADC. The clock is common for all the ADCs.
                                               This parameter can be a value of @ref ADC_Prescaler */
  uint32_t ADC_DMAAccessMode;             /*!< Configures the Direct memory access 
                                              mode for multi ADC mode.
                                               This parameter can be a value of 
                                               @ref ADC_Direct_memory_access_mode_for_multi_mode */
  uint32_t ADC_TwoSamplingDelay;          /*!< Configures the Delay between 2 sampling phases.
                                               This parameter can be a value of 
                                               @ref ADC_delay_between_2_sampling_phases */
  
}ADC_CommonInitTypeDef;
#endif

//========================================================================
// How to calculate which prescaler, which sampling time for each ADC?
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
      if(A->ChannelSamplingRate_Hz[n]>MaxAskedSamplingRate_Hz) MaxAskedSamplingRate_Hz = A->ChannelSamplingRate_Hz[n]; // max sampling rate
    };
    if((n<4)&&(A->InjectedAnalogInPins[n])) {
      NbOfChannels++;
      TotalNeededCycles += 15; // 12 cycles for 12 bit conversion, 3 cycles for sampling time (minimum)
      if(A->InjectedChannelSamplingRate_Hz[n]>MaxAskedSamplingRate_Hz) MaxAskedSamplingRate_Hz = A->InjectedChannelSamplingRate_Hz[n]; // max sampling rate      
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


//==================== Build a demonstration code for various modes
ADC_t myAdc1, myAdc2, myAdc3;
void ADC_Demo(void) {
  
  ADC_t* Adc1 = &myAdc1;
  ADC_t* Adc2 = &myAdc2;
  ADC_t* Adc3 = &myAdc3;
  
  NewADC(Adc1, ADC1, GetMCUClockTree() );
  NewADC(Adc2, ADC2, GetMCUClockTree() );
  NewADC(Adc3, ADC3, GetMCUClockTree() );
  
  ADC_CommonInitTypeDef ADCCI;
  ADC_CommonStructInit(&ADCCI);
  ADC_CommonInit(&ADCCI);
  
}
