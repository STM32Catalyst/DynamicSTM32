
#include "SebEngine.h"

// Seb ADC Demos here

//==================== Build a demonstration code for various modes
u32 ADC_NormalCompleted(u32 u) {
  
  ADC_t* A = (ADC_t*) u;  
  Toggle_LED_1();
  ADC_ConvertNormalTo_mV(A); // this will work only if the Normal_Lsb is used by the DMA (must have Adr=0 when setup the waveform)
  return 0;
}

u32 ADC_InjectedCompleted(u32 u) {
  
  ADC_t* A = (ADC_t*) u;
  Toggle_LED_2();
  ADC_BackupInjected(A); // copy the injected DR values in the structure
  ADC_ConvertInjectedTo_mV(A); // convert them to the mV and save in the structure for easy debug live watch monitoring
  return 0;
}

u32 ADC_Threshold(u32 u) {
  
  Toggle_LED_3();
  return 0;
}

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
  
  // for simpler coding and porting of the example
  ADC_t* Adc1 = &myAdc1; 
  ADC_t* Adc2 = &myAdc2;
  ADC_t* Adc3 = &myAdc3;

  // Set ADC1
  NewADC(Adc1, ADC1, 3300, GetMCUClockTree() );
  NewADC_NormalChannel(Adc1, NewIO_Pin(&myPA4, PA4), 100); 
  UseADC_NormalTrigger(Adc1, NewIO_Pin(&myPH11, PH11), 0);
  SetADC_Waveform(Adc1, (u32)ADC1_Wave, countof(ADC1_Wave));
  SetADC_Threshold_Pin_Min_Max_mV(Adc1, &myPA4, 1000, 2500);
  
  NewADC_InjectedChannelInternal(Adc1, ADC_Temp);  
  NewADC_InjectedChannelInternal(Adc1, ADC_VRef);
  
  // Set ADC2
  NewADC(Adc2, ADC2, 3300, GetMCUClockTree() );  
  NewADC_NormalChannel(Adc2, NewIO_Pin(&myPC1, PC1), 40);
  UseADC_NormalTrigger(Adc2, 0, ADC_ExternalTrigConv_Ext_IT11); // won't rebook the pin
  SetADC_Waveform(Adc2, (u32)ADC2_Wave, countof(ADC2_Wave));
  
  // Set ADC3
  NewADC(Adc3, ADC3, 3300, GetMCUClockTree() );
  UseADC_NormalTrigger(Adc3, 0, ADC_ExternalTrigConv_Ext_IT11); // won't rebook the pin 
  UseADC_InjectedTrigger(Adc3, NewIO_Pin(&myPH15, PH15), 0);  
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPA0, PA0), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPA1, PA1), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF4, PF4), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF5, PF5), 4); 
  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF6, PF6), 4); 
//  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF7, PF7), 4); 
//  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF8, PF8), 4); 
//  NewADC_NormalChannel(Adc3, NewIO_Pin(&myPF9, PF9), 4); 

//  NewADC_InjectedChannel(Adc3, NewIO_Pin(&myPF7, PF7), 4); // What happen if same channel is used in normal AND injected? (same sample time anyway)
//  NewADC_InjectedChannel(Adc3, NewIO_Pin(&myPF8, PF8), 4); 
  NewADC_InjectedChannel(Adc3, NewIO_Pin(&myPF9, PF9), 4); 
  SetADC_Waveform(Adc3, (u32)ADC3_Wave, countof(ADC3_Wave));

  // Configure all ADC HW for the following schemes:
  ConfigureADC(Adc1, ADC_TRIGGERED, ADC_TRIGGERED);
  ConfigureADC(Adc2, ADC_CONTINUOUS, ADC_INJECTED_AS_NORMAL);
  ConfigureADC(Adc3, ADC_CONTINUOUS, ADC_INJECTED_AS_NORMAL);//ADC_DISABLED);//ADC_INJECTED_AS_NORMAL

// here you can alter the config of each ADC before they are running...  
  
  EnableADC(Adc1);
  EnableADC(Adc2);
  EnableADC(Adc3);  
  
  HookADC(Adc1, ADC_IT_EOC, (u32) ADC_NormalCompleted, (u32)Adc1);
  HookADC(Adc1, ADC_IT_JEOC, (u32) ADC_InjectedCompleted, (u32)Adc1);
  HookADC(Adc1, ADC_IT_AWD, (u32) ADC_Threshold, (u32)Adc1);
  ADC_InterruptEnable(Adc1, ADC_IT_EOC, ENABLE); // if you want interrupt for EOC with DMA in continuous mode, it won't work. Interrupts will come, but EOF flag will be cleared by DMA so fast!
  ADC_InterruptEnable(Adc1, ADC_IT_JEOC, ENABLE);
//  ADC_InterruptEnable(Adc1, ADC_IT_AWD, ENABLE);
  
  NVIC_ADCsEnable(ENABLE);  
  while(1) {
    //StartADC_NormalConversion(Adc3);
    Adc1->InjectedDone = 0;    
    StartADC_InjectedConversion(Adc1);
    while(Adc1->InjectedDone==0) ;
    ADC_Convert_mV_to_DegC_x10(Adc1, Adc1->Injected_mV[0]);
    ADC_Convert_VRefByLsb(Adc1, Adc1->Injected_Lsb[1]); // Vdd independent
  }
}


