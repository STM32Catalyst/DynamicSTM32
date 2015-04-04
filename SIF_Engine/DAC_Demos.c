
#include "SebEngine.h"

//=====================================
//=== WAVEFORM RELATED
//  #define DAC_DHR8R1_ADDRESS     0x40007410

//=========== 8>< ----------------
// Board details:
// DAC trigger pin is PH9
// DAC1 output on PA4
// DAC2 output on PA5
//=========== 8>< ----------------

// FTSC2 DAC trigger sources can be:
// Mode 0: PG9 for each sample sweep
// Mode 1: Timer 3 samplng rate

#define DAC_WAVE_SAMPLE_SIZE 8192
static u16 DAC1_Wave[DAC_WAVE_SAMPLE_SIZE];
static u16 DAC2_Wave[DAC_WAVE_SAMPLE_SIZE];

// EXAMPLE HERE
static IO_Pin_t Dac1_PA4, Dac2_PA5;
static IO_Pin_t DacTriggerPH9;
static DAC_t myDac1, myDac2;

void DAC_Test(void) {
  
  DAC_t* Dac1 = &myDac1;
//  DAC_t* Dac2 = &myDac2; // untested, that pin is used as USB ULPI and can't be re-routed.
  
  DAC_WaveGeneratePredefined(DAC1_Wave, 11, countof(DAC1_Wave));// How many sine periods in the wave RAM
//  DAC_WaveGeneratePredefined(DAC2_Wave, 13);// How many sine periods in the wave RAM

  Dac1->Clocks = &MCU_Clocks;
  NewDAC_Pin_Buffer_VRef_mV(Dac1, (u32)DAC, NewIO_Pin(&Dac1_PA4, PA4), ENABLE, 3300); // DAC1 on PA4, DAC2 on PA5
  //Dac2->Clocks = &MCU_Clocks;
//  NewDAC_Pin_Buffer_VRef_mV(Dac2, (u32)DAC, NewIO_Pin(&Dac2_PA5, PA5), ENABLE, 3300); // DAC1 on PA4, DAC2 on PA5  
  
#if 0
  
  UseDAC_Trigger(Dac1, NewIO_Pin(&DacTriggerPH9, PH9), 0); // other option
  SetDAC_Waveform(Dac1, (u32)DAC1_Wave, countof(DAC1_Wave));
  ConfigureDAC(Dac1);
  EnableDAC(Dac1);
  
  FTM_Generate48MHzOnMCO2_PC9();
  while(1);
  
#else // S/W manual update without HW trigger
  
  u16 n;
  
  UseDAC_Trigger(Dac1, 0, DAC_Trigger_Software); // override, the trigger pin mode  
  ConfigureDAC(Dac1);
  EnableDAC(Dac1);
  
  while(1)  {
    for(n=0;n<countof(DAC1_Wave);n++)
      SetDAC_Lsb(Dac1, DAC1_Wave[n]);
  };

#endif  
}



//==== One case study
// If you want to debug a fast changing variable, the DAC can be a solution, convert the variable into a high speed analog output
// Most sensors have both a legacy analog output and an I2C bus digital interface.
// For digital filter, it can be good to combine ADC + Digital processing + DAC output for debugging (instead of using only a digital bus for audio out)



