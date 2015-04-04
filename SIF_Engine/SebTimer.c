
#include "SebEngine.h"
#include "SebTimer.h"


static void TimerOutputCC_SetDMA(Timer_t* Timer, u32 n);
static DMA_StreamChannelInfo_t* Get_TimerDMA_InfoByPPP_n(u32 PPP, u32 n, u32 Direction);

static TimerRelatedSignals_t* FindBySignal(SignalName_t Signal);
static TimerRelatedSignals_t* FindByTIM_n(TIM_TypeDef* TIMx, u8 n );
// This is the 16 bit 4 channels timer, TIM3 and TIM4 only timers are 16 bit 4 channels timers on STM32F437
// This also support the 16 bit 2 channels timers TIM9&12 (2ch), TIM10&11&13&14 (1ch), basic timers 6&7

u32 GetRatio(u32 In, u32 InMax, u32 OutMax) {
  
  u32 Out = (In*OutMax)/InMax; // it might saturate... to revisit later
  return Out;
}

static const u32 TimerCCFlags[] = {
  TIM_FLAG_Update, // 0 // order forced by TIM_DMA_n HW register setting. N/A should be FF then?
  TIM_FLAG_CC1, // 1
  TIM_FLAG_CC2, // 2
  TIM_FLAG_CC3, // 3
  TIM_FLAG_CC4, // 4
};

static const u32 TimerCC_Channels[] = {
  0,
  TIM_Channel_1, // 1
  TIM_Channel_2, // 2
  TIM_Channel_3, // 3
  TIM_Channel_4, // 4
};

static u32 TIM_DMA_n[] = {
  TIM_DMA_Update, // 0
  TIM_DMA_CC1, // 1
  TIM_DMA_CC2, // 2
  TIM_DMA_CC3, // 3
  TIM_DMA_CC4, // 4
  TIM_DMA_COM, // 5
  TIM_DMA_Trigger, // 6 
};


//============
u32 IsTimerHighEnd(TIM_TypeDef* TIMx) {
  // to be replaced by a lookup table flag list
  return ((TIMx==TIM1)||(TIMx==TIM8));
}

void TIM_OCnInit(TIM_TypeDef* TIMx, u32 n, TIM_OCInitTypeDef* OC) {

  switch(n) {
  case 1:    
    TIM_OC1Init(TIMx, OC);
    if(IsTimerHighEnd(TIMx)) // extra job to activate the output compare on the high end timer (not yet scalable)
      TIM_CCxCmd(TIMx, TIM_Channel_1, TIM_CCx_Enable);
  break;
  case 2:    
    TIM_OC2Init(TIMx, OC);
    if(IsTimerHighEnd(TIMx)) // extra job to activate the output compare on the high end timer (not yet scalable)
      TIM_CCxCmd(TIMx, TIM_Channel_2, TIM_CCx_Enable);
  break;
  case 3:    
    TIM_OC3Init(TIMx, OC);
    if(IsTimerHighEnd(TIMx)) // extra job to activate the output compare on the high end timer (not yet scalable)
      TIM_CCxCmd(TIMx, TIM_Channel_3, TIM_CCx_Enable);
  break;
  case 4:    
    TIM_OC4Init(TIMx, OC);
    if(IsTimerHighEnd(TIMx)) // extra job to activate the output compare on the high end timer (not yet scalable)
      TIM_CCxCmd(TIMx, TIM_Channel_4, TIM_CCx_Enable);
  break;
  default:while(1);
  };
  
}
//============
void TIM_OCnPreloadConfig(TIM_TypeDef* TIMx, u32 n, u32 Preload) {  // Enable preload feature *  // no shadow registers please

  switch(n) {
  case 1:    
    TIM_OC1PreloadConfig(TIMx, Preload);  // Enable or Disable preload feature *  // no shadow registers please
  break;
  case 2:    
    TIM_OC2PreloadConfig(TIMx, Preload);  // Enable or Disable preload feature *  // no shadow registers please  
    break;
  case 3:    
    TIM_OC3PreloadConfig(TIMx, Preload);  // Enable or Disable preload feature *  // no shadow registers please  
    break;
  case 4:    
    TIM_OC4PreloadConfig(TIMx, Preload);  // Enable or Disable preload feature *  // no shadow registers please
  break;
  default:while(1);
  };
  
}
//============



static u32 Timer_CountDownModeIRQHandler(u32 u) {
  
  u8 n;
  u32 AnyCountdown = 0;
  u32 AnyFlag = 0;
  Timer_t* Timer = (Timer_t*) u;

  AnyFlag = Timer->TIM->SR;
  AnyFlag &= Timer->TIM->DIER;
  if( AnyFlag & TIM_FLAG_Update) { // Overflow event detected
  
    Timer->TIM->SR &= ~TIM_FLAG_Update; // clear the pending bit
    Timer->Ticks++;
    // Countdown0
    for(n=0;n<TIMER_MAX_COUNTDOWN;n++) { // unlooped by compiler when needed
      if(Timer->CountDown[n])
        if(--Timer->CountDown[n]==0) // times up!
          if(Timer->fnCountDown[n]) {// call the hook
            ((u32(*)(u32))Timer->fnCountDown[n])(Timer->ctCountDown[n]);
          }else{ // if no hook, then set a flag for polling purpose
            Timer->CountDownDone[n] = 1;
          }
      AnyCountdown |= Timer->CountDown[n];
    };
    // if all countdowns are zero... what do we do? Self disable all? Or it's inside each hook?
    
    if(AnyCountdown==0) { // we can stop the interrupt, easier to debug this way
      Timer->TIM->DIER &= ~TIM_FLAG_Update;// Disable INT
    }
    
  };
  
  AnyFlag = Timer->TIM->SR;
  AnyFlag &= Timer->TIM->DIER;
  // For those timers who have CC support, we check here
  if(AnyFlag & (TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4)) {

    for(n=1;n<TIMER_MAX_CC;n++) {
      // check for CCn
      if((Timer->CCR[n]!=0) && (AnyFlag & TimerCCFlags[n])) {
        Timer->TIM->SR &= ~TimerCCFlags[n]; // clear the pending flag
        Timer->RegCC[n] = *Timer->CCR[n]; // capture the HW CC register (useful when using Input capture)

        if(Timer->fnCC[n])  ((u32(*)(u32)) Timer->fnCC[n])(Timer->ctCC[n]);
        else
          Timer->FlagCC[n] = 1;
      }
    };
    
  };

  return 0;
}

void NewTimer(Timer_t* Timer, TIM_TypeDef* T/*, u32 Period_us, MCUClocks_t * Tree*/) {// if Period_us = 0, disable the timer

  // the structure should be edited by caller and only the specifics done here.
  Timer->TIM = T;
  // Here we load the capability
  MCU_TimerCapabilities_t* C = MCU_GetMCU_TimerCapabilitiesByPPP((u32)T);
  Timer->Max = C->Max;
  Timer->SR_ValidFlags = C->SR_ValidFlags;

  // This tells how many CC is available for this timer with quick access to member variable
  Timer->CCR[0] = Timer->CCR[1] = Timer->CCR[2] = Timer->CCR[3] = Timer->CCR[4] = 0;
  if(C->SR_ValidFlags & TIM_FLAG_CC1)
    Timer->CCR[1] = (u32*) &Timer->TIM->CCR1;
  if(C->SR_ValidFlags & TIM_FLAG_CC2)
    Timer->CCR[2] = (u32*) &Timer->TIM->CCR2;
  if(C->SR_ValidFlags & TIM_FLAG_CC3)
    Timer->CCR[3] = (u32*) &Timer->TIM->CCR3;
  if(C->SR_ValidFlags & TIM_FLAG_CC4)
    Timer->CCR[4] = (u32*) &Timer->TIM->CCR4;
  
  ClockGateEnable_PPP((u32)T, ENABLE);
  
  HookIRQ_PPP((u32)T, (u32) Timer_CountDownModeIRQHandler, (u32) Timer);
/* this will be done when a non zero countdown comes in  
  if(Period_us) { // Enable interrupt source
    Timer->TIM->DIER |= 0x0001; // Enable INT
  }else{ // Disable interrupt source
    Timer->TIM->DIER &= ~0x0001;// Disable INT
  }
*/  
//-  Timer->TIM->CR1 = 5; // Event enabled, no shadowing, freerun [ENABLE TIMER NOW]
  
}


void SetTimerTimings_us(Timer_t* Timer, u32 Period_us ) {
  // there is no constrain on timers, we assume the APB1 and APB2 clock are above 1MHz (1us clock feed)
  // the structure should be edited by caller and only the specifics done here.
  u32 Hz;

  if(Period_us==0) while(1); // invalid choice

  if(Timer->OverflowPeriod_us)
    while(1); // already booked! Free the timer first before reusing it!
          
  MCU_NodeDependency_t* I = GetSignal2InfoBy_PPP((u32)Timer->TIM);

  if(I->fnClk == RCC_APB1PeriphClockCmd)
    Hz = (Timer->Clocks->OutAPB1Clk_Hz.Value * 2); // Timers have double the clock speed of APB
  else
    Hz = (Timer->Clocks->OutAPB2Clk_Hz.Value * 2); // Timers have double the clock speed of APB
    
  u32 cy,psc,arr;
  
  Timer->OverflowPeriod_us = Period_us;
  
  // we only need to guarantee the period in us with lowest timer clock for it
  cy = (Hz / 1000000) * Period_us; // if 1 us: r = 96 cycles at 96MHz (divide by 1000000?)
  // now let's cook the timer prescaler and autoreload values...
  psc = 0;
  arr = cy;

  while(arr>Timer->Max) { // in this scheme we want the lowest psc value (not best for power)
     psc++;
     arr = cy / (psc+1);
     if(psc>0xFFFF) while(1); // not possible, input clock too high or period too big
  }

//==============
  
  Timer->TIM->CR2 = 2<<4;// update event as trigger output

  Timer->TIM->PSC = psc; // This is the prescaler
  Timer->TIM->ARR = arr; // The autoreload value
  
  // We hook up the NVIC directly to the unique IRQ handler (scheme fixed)
  Timer->TIM->SR &= ~1; // clear the pending bit
}

void ConfigureTimer(Timer_t* T) {
  
  // For timers, it is done during the Timings as it's time we play with
}


void HookTimerCountdown(Timer_t* Timer, u32 n, u32 fn, u32 ct) {
  // hook first!
  Timer->ctCountDown[n] = ct;
  Timer->fnCountDown[n] = fn;
}

void ArmTimerCountdown(Timer_t* Timer, u32 n, u32 ticks) {
  
  Timer->CountDownDone[n] = 0; // clear this flag
  Timer->CountDown[n] = Timer->InitialCountDown[n] = ticks+1; //rounding up this will run wild as soon as written!
  Timer->TIM->DIER |= 0x0001; // Enable INT
}

void ReArmTimerCountdown(Timer_t* Timer, u32 n) {

  Timer->CountDown[n] = Timer->InitialCountDown[n]; 
  Timer->TIM->DIER |= 0x0001; // Enable INT
}

void NVIC_TimersEnable(FunctionalState Enable) {
  // all the interrupt handlers exist and clear flags by default
  // call this only when final activation of the EXTI interrupt generation mecanism
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = Enable;

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_Init(&NVIC_InitStructure);  
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM8_TRG_COM_TIM14_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  // These IRQ can be always enabled as the timer has its own interrupt enable bit.
}
//=============-----> END OF REFERENCE CODE

//==--->
//=====----->
//========-------->
//===========---------->
//=============-------------> Sequencer compatible functions

u32 sq_HookTimerCountdown(u32 u) {// (Timer* Timer, u32 n, u32 fn, u32 ct) // Timer, countdown_nb, fn to call, ct to call

  u32* p = (u32*) u;
  HookTimerCountdown((Timer_t*)p[0], p[1], p[2], p[3]);
  return 0;
}

u32 sq_ArmTimerCountdown(u32 u) { //(Timer* Timer, u32 n, u32 ticks)
  
  u32* p = (u32*) u;
  ArmTimerCountdown((Timer_t*)p[0], p[1], p[2]);
  return 1; // this will cause an interrupt later
}

u32 sq_ReArmTimerCountdown(u32 u) { //(Timer* Timer, u32 n) 

  u32* p = (u32*) u;
  ReArmTimerCountdown((Timer_t*)p[0], p[1]);
  return 1; // this will cause an interrupt later
}

//======================== Dynamic allocation of a timer countdown

u32 GetFreeTimerCountdownIndex(u32 u) {
  
  u32 n;
  Timer_t* Timer = (Timer_t*) u;
  
  for(n=0;n<TIMER_MAX_COUNTDOWN;n++)
    if(Timer->InitialCountDown[n]==0)
      return n;
  
  return -1; // no more resource available
}

u32 SetFreeTimerCountdownIndex(u32 u, u32 n) {
  
  Timer_t* Timer = (Timer_t*) u;
  Timer->InitialCountDown[n] = 0;
  return 0;
}

//====================== Capture Compare Initialization functions ================================ 8><8 =======

u32 EnableTimerCC_Interrupt(u32 u, u32 n, FunctionalState Enable) { // n = [1..4]
  
  Timer_t* Timer = (Timer_t*) u;
  
  if(n==0) while(1); // not allowed
  if(n>=TIMER_MAX_CC) while(1); // not available CCn
  if(Timer->CCR[n]==0) while(1); // not available

  // Clear any pending flags first
  Timer->TIM->SR &= TimerCCFlags[n];
  
  if(Enable) Timer->TIM->DIER |= TimerCCFlags[n];     
  else       Timer->TIM->DIER &= ~TimerCCFlags[n];
    
  return 0;
  
}

u32 SetTimerInputCC(u32 u, u32 n, IO_Pin_t* Pin, FunctionalState Enable) {
  
  Timer_t* Timer = (Timer_t*) u;
  
  if(n==0) while(1); // not allowed
  if(n>=TIMER_MAX_CC) while(1); // not available CCn
  if(Timer->CCR[n]==0) while(1); // not available
  // Enable is not used yet....
  TIM_ICInitTypeDef IC;
  TIM_ICStructInit(&IC);
  // For now... should be passed as parameter later on. Rising edge, no prescaler, no filtering
  IC.TIM_Channel = TimerCC_Channels[n];
  IC.TIM_ICPolarity = TIM_ICPolarity_Rising;
  IC.TIM_ICSelection = TIM_ICSelection_DirectTI;
  IC.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  IC.TIM_ICFilter = 0;  
  TIM_ICInit(Timer->TIM, &IC);

  if(Pin) {
    Timer->PinCC[n] = Pin;
    IO_PinClockEnable(Pin);
    IO_PinSetHigh(Pin);  
    IO_PinSetInput(Pin);  
    IO_PinEnablePullUpDown(Pin, ENABLE, DISABLE);
    IO_PinEnableHighDrive(Pin, ENABLE);
    IO_PinConfiguredAs(Pin,  GetPinAF(Pin->Name,(u32)Timer->TIM));   
  };
  
//  EnableTimerCC_Interrupt(u, n, Enable);
  return 0;
}


u32 HookTimerCC(u32 u, u32 n, u32 fn, u32 ct) {

  Timer_t* Timer = (Timer_t*) u;

  if(n==0) while(1); // not allowed
  if(n>=TIMER_MAX_CC) while(1); // not available CCn
  if(Timer->CCR[n]==0) while(1); // not available
  
  Timer->ctCC[n] = ct;
  Timer->fnCC[n] = fn;
  
  return 0;
}

void TIM_DMA_Activate(Timer_t* Timer) {

  u32 n;  
  
  for(n=1;n<4;n++) {
    if(Timer->EdgesSize[n]<=1) continue;
    // This CC is hooked up to a DMA, let's enable them
    DMA_StreamChannelInfo_t* DSCI = Get_TimerDMA_InfoByPPP_n((u32)Timer->TIM, n, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral);
    DMA_Cmd(DSCI->Stream/*DMA1_Stream4*/, ENABLE);  // DMA enable
    BookDMA_Stream(DSCI->Stream);
    TIM_DMACmd(Timer->TIM, TIM_DMA_n[n], ENABLE);  // TIM2 Update DMA Request enable 
  };
  
}



// In this scheme, the timer is reset, wait for Ch2 rising edge, then starts one pulse mode 
// For example, the output compares can generate pulses or edge which can trigger time shifted ADC, DAC, Analog Switch behaviours
// Using external triggers is easier to probe, hence easier debug, faster move forward job
void EnableOneShotTimerCC_Triggered(Timer_t* Timer, u32 n) {

  TIM_DMA_Activate(Timer); // Will sweep through all DMA possible interaction
  
  TIM_SelectOnePulseMode(Timer->TIM, TIM_OPMode_Single); // One Pulse Mode selection

  if(n==1)
    TIM_SelectInputTrigger(Timer->TIM, TIM_TS_TI1FP1); // Input Timer Trigged on CH1
  else
    if(n==2) TIM_SelectInputTrigger(Timer->TIM, TIM_TS_TI2FP2); // Input Timer Trigged on CH2
      else
        while(1);

  // Slave Mode selection: Trigger Mode (the trigger will set the Timer enable bit)
  TIM_SelectSlaveMode(Timer->TIM, TIM_SlaveMode_Trigger);  
}

static void TimerOutputCC_SetDMA(Timer_t* Timer, u32 n) {
  
  DMA_InitTypeDef DMAI;
  u32 DataSize;
  
  if(Timer->Max > 0x0000FFFF)
    DataSize = DMA_PeripheralDataSize_Word;
  else
    DataSize = DMA_PeripheralDataSize_HalfWord;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 , ENABLE);

  DMA_StreamChannelInfo_t* DSCI = Get_TimerDMA_InfoByPPP_n((u32)Timer->TIM, n, DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral);
  DMA_DeInit(DSCI->Stream);//(DMA1_Stream4);
  DMAI.DMA_Channel = DSCI->Channel;//DMA_Channel_5;  
  DMAI.DMA_PeripheralBaseAddr = (u32)Timer->CCR[n];
  DMAI.DMA_Memory0BaseAddr = (u32)Timer->EdgesTableAdr[n];
  DMAI.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMAI.DMA_BufferSize = Timer->EdgesSize[n];
  DMAI.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMAI.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMAI.DMA_PeripheralDataSize = DataSize;
  DMAI.DMA_MemoryDataSize = DataSize;
  DMAI.DMA_Mode = DMA_Mode_Circular;
  DMAI.DMA_Priority = DMA_Priority_High;
  DMAI.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMAI.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMAI.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMAI.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(DSCI->Stream/*DMA1_Stream4*/, &DMAI);
}



void SetTimerOutputCC_SingleEdge(Timer_t* Timer, u32 n, u32 Value_lsb) {
 
  Timer->EdgesSize[n] = 0; // Non zero triggers the DMA mode for multiple pulses
  Timer->EdgesTableAdr[n] = 0;

  TIM_OCInitTypeDef OC;
  TIM_OCStructInit(&OC);
  
  OC.TIM_OCMode = TIM_OCMode_PWM2;
  OC.TIM_OutputState = TIM_OutputState_Enable;
  OC.TIM_Pulse = Value_lsb;
  OC.TIM_OCPolarity = TIM_OCPolarity_Low;  
  
  TIM_OCnInit(Timer->TIM, n, &OC);
}


void SetTimerOutputCC_MultiEdges(Timer_t* Timer, u32 n, u32 Adr, u32 Size) {
 
  Timer->EdgesSize[n] = Size; // Non zero triggers the DMA mode for multiple pulses
  Timer->EdgesTableAdr[n] = Adr;

  TIM_OCInitTypeDef OC;
  TIM_OCStructInit(&OC);  
  
  OC.TIM_OCMode = TIM_OCMode_Toggle;
  OC.TIM_OutputState = TIM_OutputState_Enable;
  OC.TIM_Pulse = 1;
  OC.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCnInit(Timer->TIM, n, &OC);

  //CC1 only for now
  TimerOutputCC_SetDMA(Timer, n);
  TIM_OCnPreloadConfig(Timer->TIM, n, TIM_OCPreload_Disable);  // Enable preload feature *  // no shadow registers please
}

void EnableFreeRunTimer(Timer_t* Timer) {
  
  //u32 n;
  // Simnple timer mode
  if(Timer->EdgesSize[1]<2) { // DMA needed here
    TIM_Cmd(Timer->TIM, ENABLE);      
    return;
  }
  //=======================================================
  // DMA transfer is required here (CC1 hard code for now
  TIM_DMA_Activate(Timer);
  TIM_Cmd(Timer->TIM, ENABLE); // let's run the Timer at last
}

///=================================
// Signal name to Timer and local index of resource


static TimerRelatedSignals_t TRS[] = {
  
  {TIM1_UP,  TIM1,   0 },
  {TIM2_UP,  TIM2,   0 },
  {TIM3_UP,  TIM3,   0 },
  {TIM4_UP,  TIM4,   0 },
  {TIM5_UP,  TIM5,   0 },
  {TIM6_UP,  TIM6,   0 },
  {TIM7_UP,  TIM7,   0 },
  {TIM8_UP,  TIM8,   0 },
  {TIM9_UP,  TIM9,   0 },
  {TIM10_UP,  TIM10,  0 },
  {TIM11_UP,  TIM11,  0 },
  {TIM12_UP,  TIM12,  0 },
  {TIM13_UP,  TIM13,  0 },
  {TIM14_UP,  TIM14,  0 },


  {TIM1_CH1,  TIM1,  1},
  {TIM1_CH2,  TIM2,  2},
  {TIM1_CH3,  TIM3,  3},
  {TIM1_CH4,  TIM4,  4},
  {TIM1_CH1N, TIM5,  0},
  {TIM1_CH2N, TIM6,  0},
  {TIM1_CH3N, TIM7,  0},
  {TIM1_BKIN, TIM8,  0},
  {TIM1_ETR,  TIM9,  0},
//---- TIM2
  {TIM2_CH1,  TIM2,  1},
  {TIM2_CH2,  TIM2,  2},
  {TIM2_CH3,  TIM2,  3},
  {TIM2_CH4,  TIM2,  4},
  {TIM2_ETR,  TIM2,  0},
//---- TIM3
  {TIM3_CH1,  TIM3,  1},
  {TIM3_CH2,  TIM3,  2},
  {TIM3_CH3,  TIM3,  3},
  {TIM3_CH4,  TIM3,  4},
  {TIM3_ETR,  TIM3,  0},
//---- TIM4
  {TIM4_CH1,  TIM4,  1},
  {TIM4_CH2,  TIM4,  2},
  {TIM4_CH3,  TIM4,  3},
  {TIM4_CH4,  TIM4,  4},
  {TIM4_ETR,  TIM4,  0},
//---- TIM5
  {TIM5_CH1,  TIM5,  1},
  {TIM5_CH2,  TIM5,  2},
  {TIM5_CH3,  TIM5,  3},
  {TIM5_CH4,  TIM5,  4},
  {TIM5_ETR,  TIM5,  0},
//---- TIM6
  {TIM6_ETR,  TIM6,  0},
//---- TIM7
  {TIM7_ETR,  TIM7,  0},
//---- TIM8
  {TIM8_CH1,  TIM8,  1},
  {TIM8_CH2,  TIM8,  2},
  {TIM8_CH3,  TIM8,  3},
  {TIM8_CH4,  TIM8,  4},
  {TIM8_CH1N,  TIM8,  0},
  {TIM8_CH2N,  TIM8,  0},
  {TIM8_CH3N,  TIM8,  0},
  {TIM8_BKIN,  TIM8,  0},
  {TIM8_ETR,  TIM8,  0},
//---- TIM9
  {TIM9_CH1,  TIM9,  1},
  {TIM9_CH2,  TIM9,  2},
  {TIM9_ETR,  TIM9,  0},
//---- TIM10
  {TIM10_CH1,  TIM10,  1},
  {TIM10_ETR,  TIM10,  0},
//---- TIM11
  {TIM11_CH1,  TIM11,  1},
  {TIM11_ETR,  TIM11,  0},
//---- TIM12
  {TIM12_CH1,  TIM12,  1},
  {TIM12_CH2,  TIM12,  2},
  {TIM12_ETR,  TIM12,  0},
//---- TIM13
  {TIM13_CH1,  TIM13,  1},
  {TIM13_ETR,  TIM13,  0},
//---- TIM14
  {TIM14_CH1,  TIM14,  1},
  {TIM14_ETR,  TIM14,  0},
};


static TimerRelatedSignals_t* FindBySignal(SignalName_t Signal) {

  u32 i;

  for(i=0;i<countof(TRS);i++)
    if(Signal==TRS[i].Signal)
      return &TRS[i];

  while(1);
}

static TimerRelatedSignals_t* FindByTIM_n(TIM_TypeDef* TIMx, u8 n ) {

  u32 i;

  for(i=0;i<countof(TRS);i++)
    if((TIMx==TRS[i].TIMx)&&(n==TRS[i].n))
      return &TRS[i];

  while(1);
}

// Now we need to find the channel and stream given the PPP and the index using the signal
static DMA_StreamChannelInfo_t* Get_TimerDMA_InfoByPPP_n(u32 PPP, u32 n, u32 Direction) {

  SignalName_t Signal = FindByTIM_n((TIM_TypeDef*) PPP, n )->Signal;
  return Get_pStreamChannelForPPP_Signal((u32)PPP, Signal, Direction); // DMA_DIR_PeripheralToMemory | DMA_DIR_MemoryToPeripheral
}

