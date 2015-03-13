
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
u16 DAC1_Wave[DAC_WAVE_SAMPLE_SIZE];
u16 DAC2_Wave[DAC_WAVE_SAMPLE_SIZE];

// EXAMPLE HERE
static IO_Pin_t Dac1_PA4, Dac2_PA5;
static IO_Pin_t DacTriggerPH9;
static DAC_t myDac1, myDac2;

void DAC_WaveGeneratePredefined(u16* pu16, u8 index);

void DAC_Test(void) {
  
  DAC_t* Dac1 = &myDac1;
//  DAC_t* Dac2 = &myDac2;
  
  DAC_WaveGeneratePredefined(DAC1_Wave, 11);// How many sine periods in the wave RAM
//  DAC_WaveGeneratePredefined(DAC2_Wave, 13);// How many sine periods in the wave RAM

  NewDAC_Pin_Buffer(Dac1, (u32)DAC, 1, NewIO_Pin(&Dac1_PA4, PA4), ENABLE); // DAC1 on PA4, DAC2 on PA5
//  NewDAC_Pin_Buffer(Dac2, (u32)DAC, 2, NewIO_Pin(&Dac2_PA5, PA5), ENABLE); // DAC1 on PA4, DAC2 on PA5  
  
#if 1
  
  UseDAC_Trigger(Dac1, NewIO_Pin(&DacTriggerPH9, PH9), 0); // other option
  SetDAC_Waveform(Dac1, (u32)&DAC1_Wave[0], DAC_WAVE_SAMPLE_SIZE);
  EnableDAC(Dac1);
  
  FTM_Generate48MHzOnMCO2_PC9();
  while(1);
  
#else // S/W manual update without HW trigger
  
  u16 n;
  EnableDAC(Dac1);
  
  while(1)  {
    for(n=0;n<DAC_WAVE_SAMPLE_SIZE;n++)
      *(u32*)DACn_DHR_Adr[1] = DAC1_Wave[n];
  };

#endif  
}

#define myPI 3.14159265359
void DAC_WaveGeneratePredefined(u16* pu16, u8 index) {
  
  u16 i;
  u16 data;
  
  for(i=0;i<DAC_WAVE_SAMPLE_SIZE;i++) {
    
    switch(index) {
      
    case 0: data = 0;break;
    case 1: data = i*4096/DAC_WAVE_SAMPLE_SIZE;break; // rising values
    case 2: data = 4095-i*4096/DAC_WAVE_SAMPLE_SIZE;break; // falling values
    case 11: data = 2048+2047*sin(1*2*i*myPI/DAC_WAVE_SAMPLE_SIZE);break;
    case 12: data = 2048+2047*sin(2*2*i*myPI/DAC_WAVE_SAMPLE_SIZE);break;
    case 13: data = 2048+2047*sin(3*2*i*myPI/DAC_WAVE_SAMPLE_SIZE);break;
    case 14: data = 2048+2047*sin(4*2*i*myPI/DAC_WAVE_SAMPLE_SIZE);break;
    case 15: data = 2048+2047*sin(5*2*i*myPI/DAC_WAVE_SAMPLE_SIZE);break;
    case 16: data = 2048+2047*sin(6*2*i*myPI/DAC_WAVE_SAMPLE_SIZE);break;
    case 17: data = 2048+2047*sin(7*2*i*myPI/DAC_WAVE_SAMPLE_SIZE);break;
    case 18: data = 2048+2047*sin(8*2*i*myPI/DAC_WAVE_SAMPLE_SIZE);break;
    default:data = (i<(DAC_WAVE_SAMPLE_SIZE>>1))?1024:2048; // square wave, not full amplitude
    }
    
    pu16[i] = data;
    
  }
}


//==== One case study
// If you want to debug a fast changing variable, the DAC can be a solution, convert the variable into a high speed analog output
// Most sensors have both a legacy analog output and an I2C bus digital interface.
// For digital filter, it can be good to combine ADC + Digital processing + DAC output for debugging (instead of using only a digital bus for audio out)



