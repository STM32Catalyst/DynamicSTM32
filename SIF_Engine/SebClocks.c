
#include "SebEngine.h"

#if 0
MCUClocks_t MCUClocks;

void MCUInitClocks(void) {
  
  // for now, this is hardcoded
  MCUClocks.HSE_Hz = 24000000; // if zero means no clock feed from external oscin/oscout
  MCUClocks.HSI_Hz = 0; // internal oscillator, if put zero here means it is not allowed to use it (ETH/USB/...) (16MHz)(16000000)

  MCUClocks.LSE_Hz = 0; // same here
  MCUClocks.LSI_Hz = 32000; // same (32kHz)(32000)

  MCUClocks.SysClk_Hz = 96000000;
  MCUClocks.CoreClk_Hz = 96000000;
  MCUClocks.APB1Clk_Hz = 24000000;
  MCUClocks.APB2Clk_Hz = 48000000;
  MCUClocks.SAIClk_Hz = 0;
  MCUClocks.I2SClk_Hz = 0;
  MCUClocks.Pll_Clk_Hz = 96000000;

  MCUClocks.ETH_Clk_Hz = 0;
  MCUClocks.OTG_Clk_Hz = 0;
}
#else
void ClockTreeInit(MCU_Clocks_t* C, u32 InHSE_Hz, u32 InLSE_Hz, u32 I2S_CKIN_Hz, u32 InETH_Clk_Hz, u32 InOTG_Clk_Hz );

void MCUInitClocks(void) {
  
  // for now, this is hardcoded
  ClockTreeInit(&MCU_Clocks, MHzToHz(24), 0, 0, 0, 0 );
  
  MCU_Clocks.InHSI_Hz = 0; // internal oscillator, if put zero here means it is not allowed to use it (ETH/USB/...) (16MHz)(16000000)

  MCU_Clocks.OutSysClk_Hz.Value = 96000000;
  MCU_Clocks.OutCoreClk_Hz.Value = 96000000;
  MCU_Clocks.OutAPB1Clk_Hz.Value = 24000000;
  MCU_Clocks.OutAPB2Clk_Hz.Value = 48000000;
  MCU_Clocks.OutSAIClk_Hz.Value = 0;
  MCU_Clocks.OutI2SClk_Hz.Value = 0;
  MCU_Clocks.OutMainClk_Hz.Value = 96000000;

  MCU_Clocks.InETH_Clk_Hz = 0;
  MCU_Clocks.InOTG_Clk_Hz = 0;
}


#endif


#if 0
MCUClocks_t* GetMCUClockTree(void) {
  
  return &MCUClocks;
}
#else
MCU_Clocks_t* GetMCUClockTree(void) {
  
  return &MCU_Clocks;
}
#endif


#if 0
u32 GetClockHz(MCO_ClockSource_t S) {
  
  switch(S) {
  case HSI_CLOCK: return MCUClocks.HSI_Hz;
  case HSE_CLOCK: return MCUClocks.HSE_Hz;
  case LSE_CLOCK: return MCUClocks.LSE_Hz;
  case PLL_CLOCK: return MCUClocks.Pll_Clk_Hz;
  case SYS_CLOCK: return MCUClocks.SysClk_Hz;
  case I2S_CLOCK: return MCUClocks.I2SClk_Hz;
  default: return 0; // no clock
  };
}
#else
u32 GetClockHz(MCO_ClockSource_t S) {
  
  switch(S) {
  case HSI_CLOCK: return MCU_Clocks.InHSI_Hz;
  case HSE_CLOCK: return MCU_Clocks.InHSE_Hz;
  case LSE_CLOCK: return MCU_Clocks.InLSE_Hz;
  case PLL_CLOCK: return MCU_Clocks.OutMainClk_Hz.Value;
  case SYS_CLOCK: return MCU_Clocks.OutSysClk_Hz.Value;
  case I2S_CLOCK: return MCU_Clocks.OutI2SClk_Hz.Value;
  default: return 0; // no clock
  };
}
#endif


// MCO pins are used to generate output frequency clocks
IO_Pin_t MCO2_PC9;
void FTM_Generate48MHzOnMCO2_PC9(void) {

  IO_Pin_t* MCO2 = &MCO2_PC9;
  NewIO_Pin(MCO2, PC9);
  
  //RCC_MCO2Config(RCC_MCO2Source_SYSCLK,RCC_MCO2Div_4);//24Mhz now. RCC_MCO2Div_2);// 96Mhz/2 = 48 MHz
  RCC_MCO2Config(RCC_MCO2Source_HSE,RCC_MCO2Div_4); // 24MHz/4=6MHz
  IO_PinClockEnable(MCO2);
  IO_PinSetHigh(MCO2);
  IO_PinSetOutput(MCO2);  
  IO_PinEnablePullUpDown(MCO2, DISABLE, DISABLE);
  IO_PinEnableHighDrive(MCO2, ENABLE);  
  IO_PinSetSpeedMHz(MCO2, 48);
  IO_PinConfiguredAs(MCO2, GPIO_AF_MCO);
  return;
/*  
  GPIO_InitTypeDef GPIO_InitStructure;
// We also activate MCO2 to output the 48 MHz signal for debug purpose (vs TIMER mode)
  // MCO2/PC9
  RCC_MCO2Config(RCC_MCO2Source_SYSCLK,RCC_MCO2Div_4);//24Mhz now. RCC_MCO2Div_2);// 96Mhz/2 = 48 MHz
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, 0); // PC9/AF0-SYS
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;//GPIO_Speed_100MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  */
}


typedef struct { // ROM
  
  SignalName_t Signal;
  MCO_ClockSource_t CS;
  u8 Div;
  
  void (*fn)(u32,u32);
  u32 RCC_MCOxSource;
  u32 RCC_MCOxDiv;
} MCO_Info_t;


typedef struct { // RAM
  
  IO_Pin_t* MCO;
  u32 Desired_Hz;
  u32 Real_Hz;
  MCO_Info_t* MCOI;
  PinAlternateDescription_t* PAD;
  
} ClockOut_t;

  
//void RCC_MCO1Config(uint32_t RCC_MCO1Source, uint32_t RCC_MCO1Div);

const MCO_Info_t MCOI[] = {
  // MCO1 options Prescale 1 to 5
//#define RCC_MCO1Source_HSI               ((uint32_t)0x00000000)
//#define RCC_MCO1Source_LSE               ((uint32_t)0x00200000)
//#define RCC_MCO1Source_HSE               ((uint32_t)0x00400000)
//#define RCC_MCO1Source_PLLCLK            ((uint32_t)0x00600000)
  // PA8
  { MCO1, HSI_CLOCK, 1, RCC_MCO1Config, RCC_MCO1Source_HSI, RCC_MCO1Div_1 },
  { MCO1, HSI_CLOCK, 2, RCC_MCO1Config, RCC_MCO1Source_HSI, RCC_MCO1Div_2 },
  { MCO1, HSI_CLOCK, 3, RCC_MCO1Config, RCC_MCO1Source_HSI, RCC_MCO1Div_3 },
  { MCO1, HSI_CLOCK, 4, RCC_MCO1Config, RCC_MCO1Source_HSI, RCC_MCO1Div_4 },
  { MCO1, HSI_CLOCK, 5, RCC_MCO1Config, RCC_MCO1Source_HSI, RCC_MCO1Div_5 },
  
  { MCO1, LSE_CLOCK, 1, RCC_MCO1Config, RCC_MCO1Source_LSE, RCC_MCO1Div_1 },
  { MCO1, LSE_CLOCK, 2, RCC_MCO1Config, RCC_MCO1Source_LSE, RCC_MCO1Div_2 },
  { MCO1, LSE_CLOCK, 3, RCC_MCO1Config, RCC_MCO1Source_LSE, RCC_MCO1Div_3 },
  { MCO1, LSE_CLOCK, 4, RCC_MCO1Config, RCC_MCO1Source_LSE, RCC_MCO1Div_4 },
  { MCO1, LSE_CLOCK, 5, RCC_MCO1Config, RCC_MCO1Source_LSE, RCC_MCO1Div_5 },  
  
  { MCO1, HSE_CLOCK, 1, RCC_MCO1Config, RCC_MCO1Source_HSE, RCC_MCO1Div_1 },
  { MCO1, HSE_CLOCK, 2, RCC_MCO1Config, RCC_MCO1Source_HSE, RCC_MCO1Div_2 },
  { MCO1, HSE_CLOCK, 3, RCC_MCO1Config, RCC_MCO1Source_HSE, RCC_MCO1Div_3 },
  { MCO1, HSE_CLOCK, 4, RCC_MCO1Config, RCC_MCO1Source_HSE, RCC_MCO1Div_4 },
  { MCO1, HSE_CLOCK, 5, RCC_MCO1Config, RCC_MCO1Source_HSE, RCC_MCO1Div_5 },
  
  { MCO1, PLL_CLOCK, 1, RCC_MCO1Config, RCC_MCO1Source_PLLCLK, RCC_MCO1Div_1 },
  { MCO1, PLL_CLOCK, 2, RCC_MCO1Config, RCC_MCO1Source_PLLCLK, RCC_MCO1Div_2 },
  { MCO1, PLL_CLOCK, 3, RCC_MCO1Config, RCC_MCO1Source_PLLCLK, RCC_MCO1Div_3 },
  { MCO1, PLL_CLOCK, 4, RCC_MCO1Config, RCC_MCO1Source_PLLCLK, RCC_MCO1Div_4 },
  { MCO1, PLL_CLOCK, 5, RCC_MCO1Config, RCC_MCO1Source_PLLCLK, RCC_MCO1Div_5 },

  // MCO2 Options
//#define RCC_MCO2Source_SYSCLK            ((uint32_t)0x00000000)
//#define RCC_MCO2Source_PLLI2SCLK         ((uint32_t)0x40000000)
//#define RCC_MCO2Source_HSE               ((uint32_t)0x80000000)
//#define RCC_MCO2Source_PLLCLK            ((uint32_t)0xC0000000)  
  // PC9
  { MCO2, SYS_CLOCK, 1, RCC_MCO2Config, RCC_MCO2Source_SYSCLK, RCC_MCO2Div_1 },  
  { MCO2, SYS_CLOCK, 2, RCC_MCO2Config, RCC_MCO2Source_SYSCLK, RCC_MCO2Div_2 },  
  { MCO2, SYS_CLOCK, 3, RCC_MCO2Config, RCC_MCO2Source_SYSCLK, RCC_MCO2Div_3 },  
  { MCO2, SYS_CLOCK, 4, RCC_MCO2Config, RCC_MCO2Source_SYSCLK, RCC_MCO2Div_4 },  
  { MCO2, SYS_CLOCK, 5, RCC_MCO2Config, RCC_MCO2Source_SYSCLK, RCC_MCO2Div_5 },    
  
  { MCO2, I2S_CLOCK, 1, RCC_MCO2Config, RCC_MCO2Source_PLLI2SCLK, RCC_MCO2Div_1 },  
  { MCO2, I2S_CLOCK, 2, RCC_MCO2Config, RCC_MCO2Source_PLLI2SCLK, RCC_MCO2Div_2 },  
  { MCO2, I2S_CLOCK, 3, RCC_MCO2Config, RCC_MCO2Source_PLLI2SCLK, RCC_MCO2Div_3 },  
  { MCO2, I2S_CLOCK, 4, RCC_MCO2Config, RCC_MCO2Source_PLLI2SCLK, RCC_MCO2Div_4 },  
  { MCO2, I2S_CLOCK, 5, RCC_MCO2Config, RCC_MCO2Source_PLLI2SCLK, RCC_MCO2Div_5 },  
  
  { MCO2, HSE_CLOCK, 1, RCC_MCO2Config, RCC_MCO2Source_HSE, RCC_MCO2Div_1 },  
  { MCO2, HSE_CLOCK, 2, RCC_MCO2Config, RCC_MCO2Source_HSE, RCC_MCO2Div_2 },  
  { MCO2, HSE_CLOCK, 3, RCC_MCO2Config, RCC_MCO2Source_HSE, RCC_MCO2Div_3 },  
  { MCO2, HSE_CLOCK, 4, RCC_MCO2Config, RCC_MCO2Source_HSE, RCC_MCO2Div_4 },  
  { MCO2, HSE_CLOCK, 5, RCC_MCO2Config, RCC_MCO2Source_HSE, RCC_MCO2Div_5 },  
  
  { MCO2, PLL_CLOCK, 1, RCC_MCO2Config, RCC_MCO2Source_PLLCLK, RCC_MCO2Div_1 },  
  { MCO2, PLL_CLOCK, 2, RCC_MCO2Config, RCC_MCO2Source_PLLCLK, RCC_MCO2Div_2 },  
  { MCO2, PLL_CLOCK, 3, RCC_MCO2Config, RCC_MCO2Source_PLLCLK, RCC_MCO2Div_3 },  
  { MCO2, PLL_CLOCK, 4, RCC_MCO2Config, RCC_MCO2Source_PLLCLK, RCC_MCO2Div_4 },  
  { MCO2, PLL_CLOCK, 5, RCC_MCO2Config, RCC_MCO2Source_PLLCLK, RCC_MCO2Div_5 },  
  
};

u32 NewClockOutHW_Hz_Src(ClockOut_t* C, IO_Pin_t* MCO, u32 Desired_Hz, MCO_ClockSource_t CS) {

  s32 ErrorHz, MinErrorHz;
  u32 Hz;
  s16 n,closest_n;
  
  if(MCO==0) while(1); // choose the pin first
  if(Desired_Hz==0) while(1); // choose the clock frequency first
  
  C->MCO = MCO;
  C->Desired_Hz = Desired_Hz;
  closest_n = -1;

  if(CS==NO_CLOCK) while(1); // for now... error
  // now let's see what is the closest clock frequency
  C->PAD = (PinAlternateDescription_t*) GetSignalDescription(MCO->Name, (u32) RCC); // will hang if no solution
  
  // now we have to find the best match
  MinErrorHz = 1E9;
  for(n=0;n<countof(MCOI);n++) {
    
    if(MCOI[n].Signal != C->PAD->SignalName) continue; // not for this signal
    if((CS!=ANY_CLOCK)&&(MCOI[n].CS!=CS)) continue; // it is not the clock source we want
    
    Hz = GetClockHz(MCOI[n].CS)/MCOI[n].Div; // this is the Hz this clock source can do.
    ErrorHz = Absolute_Difference(Desired_Hz,Hz);
    if((Hz)&&(ErrorHz<MinErrorHz)) { // 0Hz is not an option!
      MinErrorHz = ErrorHz;
      closest_n = n;
      C->Real_Hz = Hz;
    }
  }

  if(closest_n==-1) while(1); // no solution found
  
  // now let's take the best solution
  C->MCOI = (MCO_Info_t*) &MCOI[closest_n];
  
  return 0;
}

void ConfigureClockOut(ClockOut_t* C) {
  
  ConfigurePinAsPushPullOutput(C->MCO);
  IO_PinSetSpeedMHz(C->MCO, C->Real_Hz/1000000);
  
  C->MCOI->fn(C->MCOI->RCC_MCOxSource,C->MCOI->RCC_MCOxDiv);
}


void EnableClockOut(ClockOut_t* C) {
  
  IO_PinConfiguredAs(C->MCO, GetPinAF(C->MCO->Name, (u32)RCC)); // this will activate the output clock
}

static IO_Pin_t myPA8,myPC9;
static ClockOut_t myC1,myC2;
void Clocks_Demo(void) {
  
  NewClockOutHW_Hz_Src(&myC1, NewIO_Pin(&myPA8, PA8), 5000000, HSE_CLOCK);
  ConfigureClockOut(&myC1);
  EnableClockOut(&myC1);

  NewClockOutHW_Hz_Src(&myC2, NewIO_Pin(&myPC9, PC9), 1000000, ANY_CLOCK);
  ConfigureClockOut(&myC2);
  EnableClockOut(&myC2);
  
  while(1);
}