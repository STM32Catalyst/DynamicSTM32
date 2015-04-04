
#ifndef _CLOCKS_H_
#define _CLOCKS_H_

#define MHzToHz(a) (((u32)a)*1000000)
#if 0
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

u32 Pll_Clk_Hz;

//  IO_Pin_t* SDA; // we need the pointer to the pin
//  u32 (*fnWaitMethod)(u32);
//  u32 ctWaitMethod;

} MCUClocks_t; // there should be only one globally for this one
#endif
void MCUInitClocks(void);
#if 0
MCUClocks_t* GetMCUClockTree(void);
#else
MCU_Clocks_t* GetMCUClockTree(void);
#endif
//
void FTM_Generate48MHzOnMCO2_PC9(void);

// MCO related functions
typedef enum {
  
  NO_CLOCK,
  HSI_CLOCK,
  HSE_CLOCK,
  LSE_CLOCK,
  PLL_CLOCK,
  SYS_CLOCK,
  I2S_CLOCK,
  ANY_CLOCK, // required smarter code searching for best option
  COUNTOF_CLOCK // the number of clock sources
} MCO_ClockSource_t;

u32 GetClockHz(MCO_ClockSource_t S);

void Clocks_Demo(void);

#endif
