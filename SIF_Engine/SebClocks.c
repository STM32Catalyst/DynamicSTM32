
#include "SebEngine.h"

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

  MCUClocks.ETH_Clk_Hz = 0;
  MCUClocks.OTG_Clk_Hz = 0;
}

MCUClocks_t* GetMCUClockTree(void) {
  
  return &MCUClocks;
}


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
