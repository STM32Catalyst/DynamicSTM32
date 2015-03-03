
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