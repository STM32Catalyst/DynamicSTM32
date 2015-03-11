
#include "SebEngine.h"
#include "math.h"

const u32 DACn_DHR_Adr[3] = { 0, DAC_DHR12R1_ADDRESS, DAC_DHR12R2_ADDRESS };
const u32 DAC_Channels[3] = { 0, DAC_Channel_1, DAC_Channel_2 };

void NewDAC_Pin_Buffer(DAC_t* D, u32 PPP, u32 n, IO_Pin_t* DacPin, FunctionalState BufferEnable) {
  
  if(n==0) while(1);
  if(n>2) while(1); // should be in LUT config table, not hardcoded
  if(PPP==0) while(1);
  if(DacPin==0) while(1); // no DAC output pin?

  // Initialise the structure
  D->PPP = PPP;     
  D->n = n;
  
  ClockGateEnable_PPP((u32)D->PPP, ENABLE); // Clock enable the peripheral

  D->AnalogOutPin[n] = DacPin;
  IO_PinClockEnable(DacPin);
  IO_PinSetInput(DacPin);
  IO_PinSetLow(DacPin);
  IO_PinSetSpeedMHz(DacPin, 1);
  IO_PinEnablePullUpDown(DacPin, DISABLE, DISABLE);
  IO_PinEnableHighDrive(DacPin, DISABLE);
  IO_PinSetAnalog(DacPin);

  DAC_DeInit();  
  DAC_StructInit(&D->InitStructure[n]); // Initialize the structure, can be edited outside this function
  D->InitStructure[n].DAC_OutputBuffer = BufferEnable ? DAC_OutputBuffer_Enable : DAC_OutputBuffer_Disable;  
  D->InitStructure[n].DAC_WaveGeneration = DAC_WaveGeneration_None;
}

void SetDAC_Waveform(DAC_t* D, u32 Adr, u32 Size) {
  
  u32 n = D->n;

  if(Size>1) { // DMA will be used for the transfer
    D->SampleAdr[n] = Adr;
    D->SampleSize[n] = Size;

    ClockGateEnable_PPP((u32)DMA1, ENABLE); // Clock enable the peripheral
    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    DMA_StreamChannelInfo_t* DMA_Info = Get_pStreamChannelForPPP_Signal((u32)DAC, DAC1_ANALOG, DMA_DIR_MemoryToPeripheral);

    DMA_DeInit(DMA_Info->Stream); // DACn
    DMA_InitTypeDef DMAI;  
    DMAI.DMA_Channel = DMA_Info->Channel;  // DACn
    DMAI.DMA_PeripheralBaseAddr = (u32)DACn_DHR_Adr[n];
    DMAI.DMA_Memory0BaseAddr = (u32)D->SampleAdr[n]; // n = 1 or 2
    DMAI.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMAI.DMA_BufferSize = D->SampleSize[n];
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
  
  u32 n = D->n;
  u32 TriggerMode = InternalTrigger;
  
  if(Trigger) { // Physical Trigger Pin (can be probed by oscilloscope)
    if((Trigger->Name & 0xF)!=9) while(1);
    D->TriggerPin[n] = Trigger;
    IO_PinClockEnable(Trigger);
    IO_PinSetInput(Trigger);
    IO_PinSetLow(Trigger);
    IO_PinSetSpeedMHz(Trigger, 1);
    IO_PinEnablePullUpDown(Trigger, DISABLE, DISABLE);
    IO_PinEnableHighDrive(Trigger, DISABLE);
    IO_PinSetInput(Trigger);

    EXTI_SetEdgesEnable(Trigger->Name, ENABLE, DISABLE); // Rising Edge is the trigger/event
    EXTI_SetEvent(Trigger->Name & 0xF, ENABLE); // no checks on overbooking here...
    
    TriggerMode = DAC_Trigger_Ext_IT9;
  };
  
  D->InitStructure[n].DAC_Trigger = TriggerMode;
}

void EnableDAC(DAC_t* D) { // Will configure the DAC (parameters can be overriden by higher level function if they know how to do it
  
  u32 n = D->n;

  DMA_StreamChannelInfo_t* DMA_Info = Get_pStreamChannelForPPP_Signal((u32)DAC, DAC1_ANALOG, DMA_DIR_MemoryToPeripheral);

  DAC_Init(DAC_Channels[n], &D->InitStructure[n]);
  if(D->SampleSize[n]>1) DMA_Cmd(DMA_Info->Stream, ENABLE);
  DAC_Cmd(DAC_Channels[n], ENABLE);
  if(D->SampleSize[n]>1) DAC_DMACmd(DAC_Channels[n], ENABLE);
}

