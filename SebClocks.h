
#ifndef _CLOCKS_H_
#define _CLOCKS_H_



typedef struct {

u32 HSE_Hz; // if zero means no clock feed from external oscin/oscout
u32 HSI_Hz; // internal oscillator, if put zero here means it is not allowed to use it (ETH/USB/...) (16MHz)(16000000)

u32 LSE_Hz; // same here
u32 LSI_Hz; // same (32kHz)(32000)

u32 SysClk_Hz;
u32 CoreClk_Hz;
u32 APB1Clk_Hz;
u32 APB2Clk_Hz;
u32 SAIClk_Hz;
u32 I2SClk_Hz;

u32 ETH_Clk_Hz; // external feed
u32 OTG_Clk_Hz; // external feed

//  IO_Pin_t* SDA; // we need the pointer to the pin
//  u32 (*fnWaitMethod)(u32);
//  u32 ctWaitMethod;
  
} MCUClocks_t; // there should be only one globally for this one

void MCUInitClocks(void);

MCUClocks_t* GetMCUClockTree(void);

//
void FTM_Generate48MHzOnMCO2_PC9(void);
#endif
