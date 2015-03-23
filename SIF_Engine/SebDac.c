
#include "SebEngine.h"
#include "math.h"

//const u32 DACn_DHR_Adr[3] = { 0, DAC_DHR12R1_ADDRESS, DAC_DHR12R2_ADDRESS };
const u32 DACn_DHR_Adr[3] = { 0, DAC_DHR12L1_ADDRESS, DAC_DHR12L2_ADDRESS };
const u32 DAC_Channels[3] = { 0, DAC_Channel_1, DAC_Channel_2 };

static u32 DAC_Get_n_From_PinName(PinNameDef N) {
  
  if(N==PA4) return 1;
  if(N==PA5) return 2;
  
  while(1); // this is NOT a DAC output pin
  
}


void NewDAC_Pin_Buffer_VRef_mV(DAC_t* D, u32 PPP, IO_Pin_t* DacPin, FunctionalState BufferEnable, u16 VRef_mV) {

  if(PPP==0) while(1);
  if(DacPin==0) while(1); // no DAC output pin?

  // Initialise the structure
  D->VRef_mV = VRef_mV;
  D->PPP = PPP;     
  D->n = DAC_Get_n_From_PinName(DacPin->Name);
  D->AnalogOutPin = DacPin;
  
  ClockGateEnable_PPP((u32)D->PPP, ENABLE); // Clock enable the peripheral

  DAC_DeInit();  
  DAC_StructInit(&D->InitStructure); // Initialize the structure, can be edited outside this function
  D->InitStructure.DAC_OutputBuffer = BufferEnable ? DAC_OutputBuffer_Enable : DAC_OutputBuffer_Disable;  
  D->InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
}

void SetDAC_Waveform(DAC_t* D, u32 Adr, u32 Size) {
  
  u8 n = D->n;

  if(Size>1) { // DMA will be used for the transfer
    D->SampleAdr = Adr;
    D->SampleSize = Size;

    ClockGateEnable_PPP((u32)DMA1, ENABLE); // Clock enable DMA here
    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    DMA_StreamChannelInfo_t* DMA_Info = Get_pStreamChannelForPPP_Signal((u32)DAC, DAC1_ANALOG, DMA_DIR_MemoryToPeripheral);
    D->DMA_TX = Get_pDMA_Info(DMA_Info->Stream);
    D->TX_Channel = DMA_Info->Channel;

    DMA_DeInit(DMA_Info->Stream); // DACn
    DMA_InitTypeDef DMAI;  
    DMAI.DMA_Channel = DMA_Info->Channel;  // DACn
    DMAI.DMA_PeripheralBaseAddr = (u32)DACn_DHR_Adr[n];
    DMAI.DMA_Memory0BaseAddr = (u32)D->SampleAdr; // n = 1 or 2
    DMAI.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMAI.DMA_BufferSize = D->SampleSize;
    DMAI.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMAI.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMAI.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMAI.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMAI.DMA_Mode = DMA_Mode_Circular;
    DMAI.DMA_Priority = DMA_Priority_High;
    DMAI.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMAI.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMAI.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMAI.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA_Info->Stream, &DMAI);
    return;
  }
  
  while(1); // not supported for now
}

void UseDAC_Trigger(DAC_t* D, IO_Pin_t* Trigger, u32 InternalTrigger) {  //  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T3_TRGO;
  
  if(Trigger) { // Physical Trigger Pin (can be probed by oscilloscope)
    if((Trigger->Name & 0xF)!=9) while(1);
    D->TriggerPin = Trigger;
    InternalTrigger = DAC_Trigger_Ext_IT9; // override the internal trigger
  };
  
  D->InitStructure.DAC_Trigger = InternalTrigger;
}

void ConfigureDAC(DAC_t* D) {
  
  u8 n = D->n;
// configure the analog output pin  
  ConfigurePinAsAnalog(D->AnalogOutPin);
  
// configure the external trigger pin, if non NULL  
  if(D->TriggerPin) {
    ConfigurePinAsInputTrigger(D->TriggerPin);
    EXTI_SetEdgesEnable(D->TriggerPin->Name, ENABLE, DISABLE); // Rising Edge is the trigger/event
    EXTI_SetEvent(D->TriggerPin->Name & 0xF, ENABLE); // no checks on overbooking here...
  };
  
  DAC_Init(DAC_Channels[n], &D->InitStructure);
  if(D->DMA_TX) { // if DMA is defined (needed)
    DMA_Cmd(D->DMA_TX->Stream, ENABLE);
    BookDMA_Stream(D->DMA_TX->Stream);
    DAC_DMACmd(DAC_Channels[n], ENABLE);  
  };
}

void EnableDAC(DAC_t* D) { // Finally activate the DAC (HW configuration can be altered between Configure and Enable
  
  u8 n = D->n;
  DAC_Cmd(DAC_Channels[n], ENABLE);
}

// helper functions

void SetDAC_Lsb(DAC_t* D, u32 Lsb) {
  
  u32 n = D->n;
  u16* DR = (u16*) DACn_DHR_Adr[n];
  *DR = Lsb;
  DAC->SWTRIGR = 1<<(n-1); // push the data out right now
}


u16 DAC_mV_ToLsb(DAC_t* D, u32 mV) {
  
  u32 Lsb = Interpolate_s32 (0, D->VRef_mV, 0, 0xFFF0, mV); 
  return mV;
}

#define myPI 3.14159265359
void DAC_WaveGeneratePredefined(u16* pu16, u8 WaveName, u16 size) {
  
  u16 i;
  u16 data;
  
  for(i=0;i<size;i++) {
    
    switch(WaveName) {
      
    case 0: data = 0;break;
    case 1: data = i*4096/size;break; // rising values
    case 2: data = 4095-i*4096/size;break; // falling values
    case 11: data = (u16)(2048+2047*sin(1*2*i*myPI/size));break;
    case 12: data = (u16)(2048+2047*sin(2*2*i*myPI/size));break;
    case 13: data = (u16)(2048+2047*sin(3*2*i*myPI/size));break;
    case 14: data = (u16)(2048+2047*sin(4*2*i*myPI/size));break;
    case 15: data = (u16)(2048+2047*sin(5*2*i*myPI/size));break;
    case 16: data = (u16)(2048+2047*sin(6*2*i*myPI/size));break;
    case 17: data = (u16)(2048+2047*sin(7*2*i*myPI/size));break;
    case 18: data = (u16)(2048+2047*sin(8*2*i*myPI/size));break;
    default:data = (i<(size>>1))?1024:2048; // square wave, not full amplitude
    }
    
    pu16[i] = data<<4; // make it 16 bit format
    
  }
}


//#define DAC_Trigger_None                   ((uint32_t)0x00000000) /*!< Conversion is automatic once the DAC1_DHRxxxx register */
//#define DAC_Trigger_T2_TRGO                ((uint32_t)0x00000024) /*!< TIM2 TRGO selected as external conversion trigger for DAC channel */
//#define DAC_Trigger_T4_TRGO                ((uint32_t)0x0000002C) /*!< TIM4 TRGO selected as external conversion trigger for DAC channel */
//#define DAC_Trigger_T5_TRGO                ((uint32_t)0x0000001C) /*!< TIM5 TRGO selected as external conversion trigger for DAC channel */
//#define DAC_Trigger_T6_TRGO                ((uint32_t)0x00000004) /*!< TIM6 TRGO selected as external conversion trigger for DAC channel */
//#define DAC_Trigger_T7_TRGO                ((uint32_t)0x00000014) /*!< TIM7 TRGO selected as external conversion trigger for DAC channel */
//#define DAC_Trigger_T8_TRGO                ((uint32_t)0x0000000C) /*!< TIM8 TRGO selected as external conversion trigger for DAC channel */                                                                       
//#define DAC_Trigger_Ext_IT9                ((uint32_t)0x00000034) /*!< EXTI Line9 event selected as external conversion trigger for DAC channel */
//#define DAC_Trigger_Software               ((uint32_t)0x0000003C) /*!< Conversion started by software trigger for DAC channel */
