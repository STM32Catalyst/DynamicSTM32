
// This is the tentative code to replace the Excel wizard

#include "SebEngine.h"
#include "ClockTree.h"

// The clock tree is pretty much fixed, so we declare them here
//  InHz / Min / Coef / Max / ClosestCoef / MinHz / Hz / MaxHz / TargetHz / ClosestHz / ErrorHz;
PLL_Node_t Mm; // FreqM = In / M [2..63] => Freq [1..2] MHz 2MHz preffered for lower jitter
PLL_Node_t Nm; // VCO = FreqM * N [2..511] => Freq [192..432] MHz
PLL_Node_t Pm; // FreqP = VCO / P [2,4,6,8] => Freq < 180 MHz
PLL_Node_t Qm; // FreqQ = VCO / Q [2..15] => Freq ... 
PLL_Clocks_t MainPLL;

// for the I2S PLL:
PLL_Node_t Ni, Ri, Qi;
PLL_Clocks_t I2S_PLL;
// for the SAI PLL:
PLL_Node_t Ns, Rs, Qs;
PLL_Clocks_t SAI_PLL;

// a PLL is made of clock node. Each clock node will either multiply or divide the clock (linear step)

void Init_PLL_Node(PLL_Node_t* X, s32 Step, u32 InHz, s32 Min, s32 Max, u32 MinHz, u32 MaxHz, u32 TargetHz) {

  X->Step = Step;
  X->InHz = InHz;
  X->Min = Min;
  X->Max = Max;
  if(Step>0) {
    X->InitCoef = X->Coef = Min-Step; // first itteration will start by step the count. We pre-unstep it
  }else{
    X->InitCoef = X->Coef = Max-Step;
  };
  X->MinHz = MinHz; X->MaxHz = MaxHz;// [1..2] MHz
  X->TargetHz = TargetHz; // target is closest to highest possible frequency
  X->ErrorHz = ~(u32)0;
  X->ClosestCoef = -1;
  X->ClosestHz = 0;
}

void PLL_Init(PLL_Clocks_t* PLL, PLL_Node_t* M, PLL_Node_t* N, PLL_Node_t* P, PLL_Node_t* Q, PLL_Node_t* R) {
  
  if(N==0) while(1); // cannot do anything, it's a base PLL feature
  // The global divider
  if(M) Init_PLL_Node(M, -1, MHzToHz(24), 2, 63, MHzToHz(1), MHzToHz(2), MHzToHz(2));
  // This is the multiplier frequency in the VCO
  Init_PLL_Node(N, 1, MHzToHz(12), 2, 511, MHzToHz(192), MHzToHz(432), 0); // no target frequency
  // First derivative is the P prescaler output clock
  if(P) Init_PLL_Node(P, -2, MHzToHz(12), 2, 8, MHzToHz(1), MHzToHz(180), MHzToHz(96));   // want 96MHz output if possible
  // Second derivative is the Q prescaler output clock
  if(Q) Init_PLL_Node(Q, -1, MHzToHz(12), 2, 15, MHzToHz(1), MHzToHz(48), MHzToHz(48));  
  // Third derivative is the Q prescaler output clock
  if(R) Init_PLL_Node(R, -1, MHzToHz(12), 2, 7, MHzToHz(1), MHzToHz(48), MHzToHz(192));  
  
  // tie PLL with its nodes
  PLL->M = M;
  PLL->N = N;
  PLL->P = P;
  PLL->Q = Q;
  PLL->R = R; // no R here
  
  // the links between the nodes. In->M->N->(P,Q,R)
  if(M) N->pInHz = &M->Hz; // M->N
  if(P) P->pInHz = &N->Hz; // N->P
  if(Q) Q->pInHz = &N->Hz; // N->Q
  if(R) R->pInHz = &N->Hz; // N->R
}

void TestPLL(void) {
  
  TestClockStrategy();  while(1);
  
  u32 OkMain, OkI2S, OkSAI;
  
  PLL_Init(&MainPLL, &Mm, &Nm, &Pm, &Qm, 0); // default initial values
  PLL_Init(&I2S_PLL, 0, &Ni, 0, &Qi, &Ri); // default initial values, M prescaler is the mainPLL node one, no P but Ri style output
  PLL_Init(&SAI_PLL, 0, &Ns, 0, &Qs, &Rs); // default initial values

  // set the MainPLL targets
  Pm.TargetHz = MHzToHz(96); // I want sys clock of 180 MHz
  Qm.TargetHz = MHzToHz(48);
  
  // set the I2C Pll targets
  Qi.TargetHz = MHzToHz(1); // 1 MHz for the ADC sample output on MCO pin
  Ri.TargetHz = MHzToHz(1); // 1 MHz for the I2C target (for test only)
  Ni.pInHz = &Mm.ClosestHz; // the output of the M main PLL prescaler is the clock feed for the I2C PLL's N multiplier
  
  // set the SAI Pll targets, say is used for the LCD drive on R output, and SAI drive on the Q output
  Qs.TargetHz = MHzToHz(2); // 2 MHz for SAI
  Rs.TargetHz = MHzToHz(20); // XGA target (1024+32)*(768+32)*(24Hz) = 1056*800*24 = 20MHz
  Ns.pInHz = &Mm.ClosestHz; // source of PLL is main's M prescaled clock and feeds the N multiplier stage
  
  OkMain = 0;
  OkI2S = 0;
  OkSAI = 0;
//  do{
    SolvePLL(&MainPLL,OkMain++,TRUE); // we settle with the best option for MainPLL first
    SolvePLL(&I2S_PLL,OkI2S++,TRUE); // could not find a solution
    SolvePLL(&SAI_PLL,OkSAI++,TRUE); // could not find a solution
    // found a solution!
    NOPs(1);
//    NbOfSolutions++;
//  }
  
  while(1);
}

//  InHz / Min / Coef / Max / ClosestCoef / MinHz / Hz / MaxHz / TargetHz / ClosestHz / ErrorHz;

static void CaptureThisSolution(PLL_Node_t* X) {

  if(X==0) return;
  X->ClosestHz = X->Hz;
  X->ClosestCoef = X->Coef;
  X->ErrorHz = Absolute_Difference(X->TargetHz, X->Hz);;  
}

static void UseThisSolution(PLL_Node_t* X) {
  
  if(X==0) return;
  X->Hz = X->ClosestHz;
  X->Coef = X->ClosestCoef;
}

// returns TRUE=1 if same (as good as best choice) or TRUE=2 if strictly better
static u32 IsSameOrBetterCoef(PLL_Node_t* X) {

  if(X==0) return 1; // combined with others will yield true
  if(X->Done) return FALSE; // coef out of range
  if(X->TargetHz == 0) { X->ErrorHz = 0; return 1;}; // anything within range will do, (same)
  // check how far we are from target, being within range
  u32 ErrorHz = Absolute_Difference(X->TargetHz, X->Hz);
  if(ErrorHz > X->ErrorHz) return FALSE; // not same or better
  if(ErrorHz == X->ErrorHz) return 1; // same!
//  X->ErrorHz = ErrorHz;
  return 2; // better!
}

static u32 StartNextPossiblePLLCoef(PLL_Node_t* T) {
  
  if(T==0)return 0;
    
  T->Coef = T->InitCoef;
  T->Done = FALSE;
  
  return 0;
}

static u32 GetNextPossiblePLLCoef(PLL_Node_t* T) { // step = +1, +2 if numerator, -1 -2 if denominator
  
  if(T==0) return FALSE; // not contributor to ending the overall sweep
  
  if(T->Step==0) while(1); // error
  
  if(T->Done) return FALSE; // no itteration until the sweep is restarted
  
  if(T->pInHz) T->InHz = *(T->pInHz); // link directly to the calculated parent. (+)

  while(T->Step<0) { // prescaler, increasing frequency by decreasing coefficient
    
      T->Coef += T->Step;// try higher frequency    
      if(T->Coef < T->Min) {
        T->Done = TRUE;
        return FALSE; // finished sweeping
      };

      T->Hz = T->InHz / T->Coef;
      if(T->Hz > T->MaxHz) {
        T->Done = TRUE;
        return FALSE; // too high frequency, finished!      
      };
      if(T->Hz >= T->MinHz) return TRUE; // found ok coef
  }
  
  while(T->Step>0) { // multiplier, increasing frequency by increasing coefficient
    
      T->Coef += T->Step;// try higher frequency
      if(T->Coef > T->Max) {
        T->Done = TRUE;
        return FALSE; // finished sweeping
      };

      T->Hz = T->InHz * T->Coef;
      if(T->Hz > T->MaxHz) {
        T->Done = TRUE;
        return FALSE;
      }; // too high frequency, finished!      
      
      if(T->Hz >= T->MinHz) return TRUE; // found ok coef
  }
  
  return 1;
}

static u32 IsNotIsDoneYetNextPossiblePLLCoef(PLL_Node_t* T) {
  
  if(T==0) return FALSE;
  return (T->Done==FALSE);
}


static u32 SolvePLL(PLL_Clocks_t* PLL, u8 Skip, u8 SweepAll) {
  
  u32 NbOfSolutions = 0;
  u32 FoundOneSolution = FALSE;
  
  // brute force, to find all the possible combinations?
//  while(1) {
  if(PLL==0) while(1); // the PLL does not exist, error
  
    if(!Skip) StartNextPossiblePLLCoef(PLL->M);
    do{// We found a possible M solution, now we need a possible N solution
      if(!Skip) GetNextPossiblePLLCoef(PLL->M);
      if(!Skip) StartNextPossiblePLLCoef(PLL->N); // Start sweeping all options
      do{// We found a possible (M,N) solution, now we need a possible P solution
        if(!Skip) GetNextPossiblePLLCoef(PLL->N);
        if(!Skip) StartNextPossiblePLLCoef(PLL->P);
        do{// We found a possible (M,N,P) solution, now we need a possible Q solution
          if(!Skip) GetNextPossiblePLLCoef(PLL->P);
          if(!Skip) StartNextPossiblePLLCoef(PLL->Q);
          do{ // did we also find an R solution (if there)
            if(!Skip) GetNextPossiblePLLCoef(PLL->Q);
            if(!Skip) StartNextPossiblePLLCoef(PLL->R);
            do{
              if(!Skip) GetNextPossiblePLLCoef(PLL->R);
              //==== did we find a viable solution?
              if(!Skip) FoundOneSolution = 1; // all the existing coeff together yield a better common result
              if(!Skip) FoundOneSolution *= IsSameOrBetterCoef(PLL->Q);
              if(!Skip) FoundOneSolution *= IsSameOrBetterCoef(PLL->P);
              if(!Skip) FoundOneSolution *= IsSameOrBetterCoef(PLL->R);
              if(!Skip) FoundOneSolution *= IsSameOrBetterCoef(PLL->N);
              if(!Skip) FoundOneSolution *= IsSameOrBetterCoef(PLL->M);
              
              if(!Skip) if(FoundOneSolution>1) { // all coef as good as best AND one is better
                NbOfSolutions++; // at least closer than previous set
                CaptureThisSolution(PLL->M);
                CaptureThisSolution(PLL->N);
                CaptureThisSolution(PLL->P);
                CaptureThisSolution(PLL->Q);
                CaptureThisSolution(PLL->R);
                NOPs(1); // here we should return
                if(SweepAll==FALSE) return NbOfSolutions;
              };
              Skip = 0; // this is to return back here from an itteration
            } while(IsNotIsDoneYetNextPossiblePLLCoef(PLL->R));
          } while(IsNotIsDoneYetNextPossiblePLLCoef(PLL->Q));
        } while(IsNotIsDoneYetNextPossiblePLLCoef(PLL->P));
      } while(IsNotIsDoneYetNextPossiblePLLCoef(PLL->N));
    } while(IsNotIsDoneYetNextPossiblePLLCoef(PLL->M));
//  };// while
  
  // return Gears[PLL->N->Coef][PLL->M->Coef]; // for linker to map the structure for debugging
  UseThisSolution(PLL->M);
  UseThisSolution(PLL->N);
  UseThisSolution(PLL->P);
  UseThisSolution(PLL->Q);
  UseThisSolution(PLL->R);
  return 0; // all trials completed
}


// Now we need to identify the gearing
// APB1: RCC_HCLK_Div1
// APB2: (same)<<3
// HPRE: RCC_SYSCLK_Div1
const u32 Gears[5][5] = { 
  { (RCC_SYSCLK_Div1 | RCC_HCLK_Div1 | (RCC_HCLK_Div1<<3)), 0, 0, 0, 0 }, // no gear because all dividers are /1
  { (RCC_SYSCLK_Div2 | RCC_HCLK_Div1 | (RCC_HCLK_Div1<<3)), (RCC_SYSCLK_Div1 | RCC_HCLK_Div2 | (RCC_HCLK_Div2<<3)), 0, 0, 0 },
  { (RCC_SYSCLK_Div4 | RCC_HCLK_Div1 | (RCC_HCLK_Div1<<3)), (RCC_SYSCLK_Div2 | RCC_HCLK_Div2 | (RCC_HCLK_Div2<<3)), (RCC_SYSCLK_Div1 | RCC_HCLK_Div4 | (RCC_HCLK_Div4<<3)), 0, 0 },
  { (RCC_SYSCLK_Div8 | RCC_HCLK_Div1 | (RCC_HCLK_Div1<<3)), (RCC_SYSCLK_Div4 | RCC_HCLK_Div2 | (RCC_HCLK_Div2<<3)), (RCC_SYSCLK_Div2 | RCC_HCLK_Div4 | (RCC_HCLK_Div4<<3)), (RCC_SYSCLK_Div1 | RCC_HCLK_Div8 | (RCC_HCLK_Div8<<3)), 0 },
  { (RCC_SYSCLK_Div16 | RCC_HCLK_Div1 | (RCC_HCLK_Div1<<3)),(RCC_SYSCLK_Div8 | RCC_HCLK_Div2 | (RCC_HCLK_Div2<<3)), (RCC_SYSCLK_Div4 | RCC_HCLK_Div4 | (RCC_HCLK_Div4<<3)), (RCC_SYSCLK_Div2 | RCC_HCLK_Div8 | (RCC_HCLK_Div8<<3)), (RCC_SYSCLK_Div1 | RCC_HCLK_Div16 | (RCC_HCLK_Div16<<3)) },
};


void SetMCU_Gear(MCU_Clocks_t* C, u32 Gear) { // Gear 1..5 MaxGear 1..5

  // This will keep the peripherals at constant speed, only the core speed will be affected.
  if(Gear==0) while(1); // not yet supported
  C->CurrentGear = Max(Gear, C->MaxGears); // don't exceed max gear, store the new setting
  
  u32 tmpreg = RCC->CFGR;
  tmpreg &= ~RCC_CFGR_HPRE; // Clear HPRE[3:0] bits
  tmpreg &= ~RCC_CFGR_PPRE1;// Clear PPRE1[2:0] bits
  tmpreg &= ~RCC_CFGR_PPRE2;// Clear PPRE2[2:0] bits
  tmpreg |= Gears[C->MaxGears-1][Gear-1];
  RCC->CFGR = tmpreg;/* Store the new value */
}

void MCU_GearUp(MCU_Clocks_t* C) {
  SetMCU_Gear(C, C->CurrentGear+1);
};

void MCU_GearDown(MCU_Clocks_t* C) {
  if(C->CurrentGear==1) return; // cannot go to gear 0 for now
  SetMCU_Gear(C, C->CurrentGear-1);
};


//======================================== 8>< ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Ok, now we can generate solutions for our needs.
// First, we need to check all used peripherals and estimate what is the minimum APB1 and APB2 frequencies.
// Second, we need to check the max CPU Core frequency which depends on Vdd
// Third, we check how many gears we can get (5 up to 5) 
// Fourth calculate the wait states, etc... for each gear. (could be the same pointed gear :-)
// The gears will be first used manually, and later from feedback about performance.
// We assume that all peripheral have prescalers in 2's power which will be tuned statically (because they can't be all changed in single cycle as they are not on a single register)


MCU_Clocks_t MCU_Clocks;

void ClockTreeInit(MCU_Clocks_t* C, u32 InHSE_Hz, u32 InLSE_Hz, u32 I2S_CKIN_Hz, u32 InETH_Clk_Hz, u32 InOTG_Clk_Hz ) {

//======= The inputs fixed by the system ========  
  C->InHSE_Hz = InHSE_Hz;
  C->InHSI_Hz = 16000000;
  C->InLSI_Hz = 32000;
  C->InLSE_Hz = InLSE_Hz;
  C->InI2SClk_Hz = I2S_CKIN_Hz;
  C->InETH_Clk_Hz = InETH_Clk_Hz;
  C->InOTG_Clk_Hz = InOTG_Clk_Hz;
}

// this will depend on few things
void SetMaxSysClkFromVdd_mV(MCU_Clocks_t* C, u32 Vdd_mV) {
  
  if(Vdd_mV==0) Vdd_mV = 1700; // if no supply specified, assume the lowest
  C->Vdd_mV.Value = Vdd_mV; // save Vdd_mV in context

//======= Frequencies Vdd dependent are set here ======
  
  if(Vdd_mV<1700) while(1); // out of range supply
  if((Vdd_mV>=1700)&&(Vdd_mV<2100))  { C->ADC_Max_Hz = 1200000; C->OutCoreClk_Hz.Max = C->OutSysClk_Hz.Max = MHzToHz(168);};
  if((Vdd_mV>=2100)&&(Vdd_mV<2400))  { C->ADC_Max_Hz = 1200000; C->OutCoreClk_Hz.Max = C->OutSysClk_Hz.Max = MHzToHz(180);};
  if((Vdd_mV>=2400)&&(Vdd_mV<2700))  { C->ADC_Max_Hz = 2400000; C->OutCoreClk_Hz.Max = C->OutSysClk_Hz.Max = MHzToHz(180);};
  if((Vdd_mV>=2700)&&(Vdd_mV<=3600)) { C->ADC_Max_Hz = 2400000; C->OutCoreClk_Hz.Max = C->OutSysClk_Hz.Max = MHzToHz(180);};
  if(Vdd_mV>3600) while(1); // out of range supply
  
//======================================================
  
}

// call this function BEFORE solving the PLLs
void SetMinSysClkAPB1APB2_Hz(MCU_Clocks_t* C, u32 SysClkMin_Hz, u32 APB1Min_Hz, u32 APB2Min_Hz) {
  
  // Also set the min/max frequencies for APB1/2
  if(APB1Min_Hz>MHzToHz(45)) while(1); // out of range
  C->OutAPB1Clk_Hz.Min = APB1Min_Hz;
  C->OutAPB1Clk_Hz.Max = MHzToHz(45);
  
  if(APB2Min_Hz>MHzToHz(90)) while(1); // out of range
  C->OutAPB2Clk_Hz.Min = Max(APB2Min_Hz,APB1Min_Hz*2); // APB2 min is at least 2x the APB1 min frequency
  C->OutAPB2Clk_Hz.Max = MHzToHz(90);
  
  C->OutCoreClk_Hz.Min = C->OutSysClk_Hz.Min = Max(Max(SysClkMin_Hz,C->OutAPB2Clk_Hz.Min*2), C->OutAPB1Clk_Hz.Min*4); // min freq is user's choice, no lower than 2xAPB2 min frequency for sure
}

// call this function AFTER solving the PLLs
// we find out how many gears we can implement
void SetMCU_Gears(MCU_Clocks_t* C) {
  
  u32 ratio = C->OutSysClk_Hz.Value / C->OutSysClk_Hz.Min;
  if(ratio>=16) C->MaxGears = 5;
  else
    if(ratio>=8) C->MaxGears = 4;
  else
    if(ratio>=4) C->MaxGears = 3;
  else
    if(ratio>=2) C->MaxGears = 2;
  else
    C->MaxGears = 1; 
  
  C->OutAPB2Clk_Hz.Value = C->OutSysClk_Hz.Value/2;
  C->OutAPB1Clk_Hz.Value = C->OutSysClk_Hz.Value/4;
}

void TestClockStrategy(void) {
  
  ClockTreeInit(&MCU_Clocks, MHzToHz(24), 0, 0, 0, MHzToHz(24)); // external crystal for USB
  
  MCU_Clocks.MainPLL = &MainPLL;
  MCU_Clocks.I2C_PLL = &I2S_PLL;
  MCU_Clocks.SAI_PLL = &SAI_PLL;
  
  SetMaxSysClkFromVdd_mV(&MCU_Clocks, 3000); // 3.0V supply voltage
  SetMinSysClkAPB1APB2_Hz(&MCU_Clocks, MHzToHz(24), MHzToHz(16), MHzToHz(32));
  // we use USB so we need to feed the PLLs with HSE
  
  
  u32 OkMain, OkI2S, OkSAI;
  
  PLL_Init(&MainPLL, &Mm, &Nm, &Pm, &Qm, 0); // default initial values
  PLL_Init(&I2S_PLL, 0, &Ni, 0, &Qi, &Ri); // default initial values, M prescaler is the mainPLL node one, no P but Ri style output
  PLL_Init(&SAI_PLL, 0, &Ns, 0, &Qs, &Rs); // default initial values

  Mm.pInHz = &MCU_Clocks.InHSE_Hz; // use crystal 24MHz as input (USB require precise clock)
  
  // set the MainPLL targets
  Pm.MinHz =  MCU_Clocks.OutCoreClk_Hz.Min;
  Pm.TargetHz = MCU_Clocks.OutSysClk_Hz.Max;//MHzToHz(96); // I want sys clock of 180 MHz
  Qm.TargetHz = MHzToHz(48); // for USB
  
  // set the I2C Pll targets
  Qi.TargetHz = MHzToHz(1); // 1 MHz for the ADC sample output on MCO pin
  Ri.TargetHz = MHzToHz(1); // 1 MHz for the I2C target (for test only)
  Ni.pInHz = &Mm.ClosestHz; // the output of the M main PLL prescaler is the clock feed for the I2C PLL's N multiplier
  
  // set the SAI Pll targets, say is used for the LCD drive on R output, and SAI drive on the Q output
  Qs.TargetHz = MHzToHz(2); // 2 MHz for SAI
  Rs.TargetHz = MHzToHz(20); // XGA target (1024+32)*(768+32)*(24Hz) = 1056*800*24 = 20MHz
  Ns.pInHz = &Mm.ClosestHz; // source of PLL is main's M prescaled clock and feeds the N multiplier stage

  OkMain = 0;
  OkI2S = 0;
  OkSAI = 0;
//  do{
  SolvePLL(&MainPLL,OkMain++,TRUE); // we settle with the best option for MainPLL first
  SolvePLL(&I2S_PLL,OkI2S++,TRUE); // could not find a solution
  SolvePLL(&SAI_PLL,OkSAI++,TRUE); // could not find a solution
  
  MCU_Clocks.OutCoreClk_Hz.Value = MCU_Clocks.OutSysClk_Hz.Value = Pm.Hz;
  MCU_Clocks.OutAPB1Clk_Hz.Value = Pm.Hz / 4; // this is the closest value derived from sysclock (each periph can divide further)
  MCU_Clocks.OutAPB2Clk_Hz.Value = Pm.Hz / 2; // this is the closest value derived from sysclock (each periph can divide further)
  SetMCU_Gears(&MCU_Clocks);
  
}

// Core power estimation
typedef struct {
  u32 Hz;
  u32 mA;
} HzTo_mA_t;

const HzTo_mA_t HzTo_mA[] = { // typical use with Flash at 25C
  {   0,   1 },
  {   2,   3 },
  {   4,   5 },
  {   8,   7 },
  {  16,  12 },
  {  25,  20 },
  {  30,  20 },
  {  60,  37 },
  {  90,  54 },
  { 120,  66 },
  { 144,  85 },
  { 150,  87 },
  { 168,  98 },
  { 180, 103 }
};

#ifndef _SEB_MATH_H_
static s32 Interpolate_s32 (s32 x0, s32 x1, s32 y0, s32 y1, s32 x)
{ 
// code for 32 bit mcu
    s32 dwQ;
        dwQ = ((y1-y0))*x+(x1*y0)-(x0*y1);	// overflow not checked yet
	dwQ = dwQ / (x1-x0);// we can also do roundings here
  
	return dwQ;
}
#endif
void EstimatePower(MCU_Clocks_t* C) {
  
  u32 Hz = C->OutSysClk_Hz.Value;
  u32 mA,mW;
  if(Hz==0) while(1); // error, can't be clock stopped
  
  for(u32 n=1;n<countof(HzTo_mA); n++) {
    
    if(Hz>=HzTo_mA[n].Hz){ // We reached or crossed the Hz in the table
      mA =  Interpolate_s32 (HzTo_mA[n].Hz, HzTo_mA[n-1].Hz, HzTo_mA[n].mA, HzTo_mA[n-1].mA, Hz);
      C->MCU_mA.Value = mA;
      if(C->Vdd_mV.Value==0) while(1); // this one must be either default non zero or measured by ADC
      mW = mA * C->Vdd_mV.Value;
      C->MCU_mW.Value = mW;
      break;
    }
  }

// for now we only use this, later we can add the peripherals and GPIO power depending on output speed?  
}

// Sometime we have a maximum current which will limit the frequency...
void FindMaxSysClkFromMax_mA(MCU_Clocks_t* C, u32 Max_mA) {

  u32 Hz;
  if(Max_mA<1) while(1); // too small, out of the table
  Max_mA = Min(Max_mA, HzTo_mA[countof( HzTo_mA )-1].mA );
  C->MCU_mA.Max = Max_mA;
  
  for(u32 n=1;n<countof(HzTo_mA); n++) {
    
    if(Max_mA>=HzTo_mA[n].mA){ // We reached or crossed the Hz in the table
        Hz =  Interpolate_s32 (HzTo_mA[n].mA, HzTo_mA[n-1].mA, HzTo_mA[n].Hz, HzTo_mA[n-1].Hz, Max_mA);
        C->OutSysClk_Hz.Max = Hz;
        break;
    }
  }
  
}