
#ifndef _CLOCKTREE_H_
#define _CLOCKTREE_H_

void TestClockStrategy(void);

typedef struct {

  u32* pInHz; // pointing to another's node frequency
  u32 InHz; // the incoming frequency 
  // coefficient related (the variable knob)
  s32 Min; 
  s32 Coef;  
  s32 Max;
  s32 Step;  
  s32 ClosestCoef;
  s32 InitCoef;
  
  // frequency related (the resulting freq)
  u32 MinHz;
  u32 Hz;
  u32 MaxHz;
  
  u32 TargetHz;
  u32 ClosestHz;
  u32 ErrorHz;
  
  u8 Done;
  
} PLL_Node_t;

typedef struct {
  // the frequency domains...
  PLL_Node_t* M; // FreqM = In / M [2..63] => Freq [1..2] MHz 2MHz preffered for lower jitter
  PLL_Node_t* N; // VCO = FreqM * N [2..511] => Freq [192..432] MHz
  PLL_Node_t* P; // FreqP = VCO / P [2,4,6,8] => Freq < 180 MHz
  PLL_Node_t* Q; // FreqQ = VCO / Q [2..15] => Freq ... 
  PLL_Node_t* R; // FreqR = VCO / R [2..7] => Freq <192 MHz max error of 0.3%
  
} PLL_Clocks_t;

void Init_PLL_Node(PLL_Node_t* X, s32 Step, u32 InHz, s32 Min, s32 Max, u32 MinHz, u32 MaxHz, u32 TargetHz);
void PLL_Init(PLL_Clocks_t* PLL, PLL_Node_t* M, PLL_Node_t* N, PLL_Node_t* P, PLL_Node_t* Q, PLL_Node_t* R);
u32 SolvePLL(PLL_Clocks_t* PLL, u8 Skip, u8 SweepAll);
void TestPLL(void);

//=========================================== 

typedef struct {
  
  u32 Gear;
  u32 FlashWaitState;
  u32 Core_Hz;
  u32 OverDrive;
  u32 DataCacheEnable;
  
} Gear_t;
/*
typedef struct {
  u32 Min;
  u32 Max;
  u32 Value;
} RangedValue_t;
*/
typedef struct {

Gear_t Gears[5]; // all 5 gears must exist, even if it's the same cloned 5 times.
u32 CurrentGear;
u32 MaxGears; // how many real gears we can use.

PLL_Clocks_t* MainPLL; // NULL = PLL not to be used
PLL_Clocks_t* I2C_PLL; // NULL = PLL not to be used
PLL_Clocks_t* SAI_PLL; // NULL = PLL not to be used

RangedValue_t Vdd_mV; // if Vdd, no voltage information is available, we will take the most conservative option then. To override it, put 3.3V
RangedValue_t MCU_mA; // the current core estimated consumption in mA
RangedValue_t MCU_mW;

u32 InHSE_Hz; // if zero means no clock feed from external oscin/oscout
u32 InHSI_Hz; // internal oscillator, if put zero here means it is not allowed to use it (ETH/USB/...) (16MHz)(16000000)

u32 InLSE_Hz; // same here
u32 InLSI_Hz; // same (32kHz)(32000)

u32 InI2SClk_Hz; // external clock feed, 0 if not available
u32 InETH_Clk_Hz; // external feed, 0 if not available
u32 InOTG_Clk_Hz; // external feed, 0 if not available

u32 ADC_Max_Hz; // Single ADC Max conversion rate (Vdd dependent)
u32 DAC_Max_Hz; // Single DAC Max conversion rate (?)

RangedValue_t OutSysClk_Hz;
RangedValue_t OutCoreClk_Hz;
RangedValue_t OutAPB1Clk_Hz;
RangedValue_t OutAPB2Clk_Hz;
RangedValue_t OutSAIClk_Hz;
RangedValue_t OutI2SClk_Hz; // to cook
RangedValue_t OutMainClk_Hz; // to cook
RangedValue_t Out48MHzClk_Hz;

RangedValue_t OutMCO1_Hz;
RangedValue_t OutMCO2_Hz;
RangedValue_t OutRTC_Hz;

//  IO_Pin_t* SDA; // we need the pointer to the pin
//  u32 (*fnWaitMethod)(u32);
//  u32 ctWaitMethod;
  
} MCU_Clocks_t; // there should be only one globally for this one

extern MCU_Clocks_t MCU_Clocks; // this is the global clock tree

void SetMCU_Gear(MCU_Clocks_t* C, u32 Gear);
void MCU_GearUp(MCU_Clocks_t* C);
void MCU_GearDown(MCU_Clocks_t* C);

#endif
