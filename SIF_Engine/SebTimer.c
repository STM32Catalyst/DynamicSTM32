
#include "SebEngine.h"
#include "SebTimer.h"

// This is the 16 bit 4 channels timer, TIM3 and TIM4 only timers are 16 bit 4 channels timers on STM32F437
// This also support the 16 bit 2 channels timers TIM9&12 (2ch), TIM10&11&13&14 (1ch), basic timers 6&7

u32 GetRatio(u32 In, u32 InMax, u32 OutMax) {
  
  u32 Out = (In*OutMax)/InMax; // it might saturate... to revisit later
  return Out;
}

static const u32 TimerCCFlags[] = {
  TIM_FLAG_Update, // 0
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

void NewTimer_us(Timer_t* Timer, TIM_TypeDef* T, u32 Period_us, MCUClocks_t * Tree) {// if Period_us = 0, disable the timer

  // the structure should be edited by caller and only the specifics done here.
  MCU_NodeDependency_t* I = GetSignal2InfoBy_PPP((u32)T);
          
  if(I->fnClk == RCC_APB1PeriphClockCmd)
    Timer->FeedClockMHz = (Tree->APB1Clk_Hz * 2) / 1000000; // Timers have double the clock speed of APB
  else
    Timer->FeedClockMHz = (Tree->APB2Clk_Hz * 2) / 1000000; // Timers have double the clock speed of APB
    
  u32 cy,psc,arr;
  if(Period_us==0) while(1); // invalid choice

  if(Timer->OverflowPeriod_us)
    while(1); // already booked! Free the timer first before reusing it!
  
  Timer->TIM = T;
  Timer->OverflowPeriod_us = Period_us;

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
  
  
  // we only need to guarantee the period in us with lowest timer clock for it
  cy = Timer->FeedClockMHz * Period_us; // if 1 us: r = 96 cycles at 96MHz
  // now let's cook the timer prescaler and autoreload values...
  psc = 0;
  arr = cy;

  while(arr>Timer->Max) { // in this scheme we want the lowest psc value (not best for power)
     psc++;
     arr = cy / (psc+1);
     if(psc>0xFFFF) while(1); // not possible, input clock too high or period too big
  }

  ClockGateEnable_PPP((u32)T, ENABLE);
  
  HookIRQ_PPP((u32)T, (u32) Timer_CountDownModeIRQHandler, (u32) Timer);

  Timer->TIM->CR2 = 2<<4;// update event as trigger output

  Timer->TIM->PSC = psc; // This is the prescaler
  Timer->TIM->ARR = arr; // The autoreload value
  
  // We hook up the NVIC directly to the unique IRQ handler (scheme fixed)
  Timer->TIM->SR &= ~1; // clear the pending bit
/* this will be done when a non zero countdown comes in  
  if(Period_us) { // Enable interrupt source
    Timer->TIM->DIER |= 0x0001; // Enable INT
  }else{ // Disable interrupt source
    Timer->TIM->DIER &= ~0x0001;// Disable INT
  }
*/  
  Timer->TIM->CR1 = 5; // Event enabled, no shadowing, freerun [ENABLE TIMER NOW]
  
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

u32 SetTimerOutputCC(u32 u, u32 n, IO_Pin_t* Pin, u32 InitValue, FunctionalState Enable) {
  
  Timer_t* Timer = (Timer_t*) u;
  
  if(n==0) while(1); // not allowed
  if(n>=TIMER_MAX_CC) while(1); // not available CCn
  if(Timer->CCR[n]==0) while(1); // not available
  
  *Timer->CCR[n] = Timer->RegCC[n] = InitValue;

  // This should be passing parameter later
  TIM_OCInitTypeDef OC;
  TIM_OCStructInit(&OC);
  if(Enable)
    OC.TIM_OCMode = TIM_OCMode_PWM2;
  else
    OC.TIM_OCMode = TIM_OCMode_Inactive;
  
  OC.TIM_OutputState = TIM_OutputState_Enable;
  OC.TIM_Pulse = InitValue;
  OC.TIM_OCPolarity = TIM_OCPolarity_Low;//High; // Pulse generation positive pulse, negative polarity
  
  switch(n) {
  case 1:    TIM_OC1Init(Timer->TIM, &OC);      break;
  case 2:    TIM_OC2Init(Timer->TIM, &OC);      break;
  case 3:    TIM_OC3Init(Timer->TIM, &OC);      break;
  case 4:    TIM_OC4Init(Timer->TIM, &OC);      break;
  default:while(1);
  };

  if(Pin) {  
    Timer->PinCC[n] = Pin;
    IO_PinClockEnable(Pin);
    IO_PinSetHigh(Pin);  
    IO_PinSetOutput(Pin);  
    IO_PinEnablePullUpDown(Pin, DISABLE, DISABLE);
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


//=============-----> This is to test the timer functionality
#ifdef ADD_EXAMPLES_TO_PROJECT
static u32 ToggleLED1(u32 u) {
  Timer_t* Timer = (Timer_t*) u;
// we directly use u as a parameter, not a pointer of structure as here it is very simple and fixed
  Toggle_LED_1();
  ReArmTimerCountdown(Timer,1);
  return 0;
}

static u32 ToggleLED2(u32 u) {
  Timer_t* Timer = (Timer_t*) u;
// we directly use u as a parameter, not a pointer of structure as here it is very simple and fixed
  Toggle_LED_2();
  ReArmTimerCountdown(Timer,2);
  return 0;
}

static u32 ToggleLED3(u32 u) {
  Timer_t* Timer = (Timer_t*) u;
// we directly use u as a parameter, not a pointer of structure as here it is very simple and fixed
  Toggle_LED_3();
  ReArmTimerCountdown(Timer,1);
  return 0;
}

static u32 ToggleLED4(u32 u) {
  Timer_t* Timer = (Timer_t*) u;
// we directly use u as a parameter, not a pointer of structure as here it is very simple and fixed
  Toggle_LED_4();
  ReArmTimerCountdown(Timer,2);
  return 0;
}

// how about it hooks itself? Also the timers could be local variables and destroyed later (after releasing them)!
// this would also dynamic HW sharing... with optimized RAM allocation.

static Timer_t Timer1, Timer2, Timer3, Timer4, Timer5, Timer6, Timer7, Timer8, Timer9, Timer10, Timer11, Timer12, Timer13, Timer14;

void Timer_T1_T2_Test(void) { // ...
  NewTimer_us(&Timer1, TIM1, 1000, GetMCUClockTree());// 1ms tick period
  NewTimer_us(&Timer2, TIM2, 10, GetMCUClockTree());// 10us period
  NVIC_TimersEnable(ENABLE);
  
  HookTimerCountdown(&Timer1, 1, (u32)ToggleLED1, (u32)&Timer1); 
  ArmTimerCountdown(&Timer1, 1, 500);
  HookTimerCountdown(&Timer1, 2, (u32)ToggleLED2, (u32)&Timer1); 
  ArmTimerCountdown(&Timer1, 2, 1000);
  
  HookTimerCountdown(&Timer2, 1, (u32)ToggleLED3, (u32)&Timer2);
  ArmTimerCountdown(&Timer2, 1, 500);
  HookTimerCountdown(&Timer2, 2, (u32)ToggleLED4, (u32)&Timer2);
  ArmTimerCountdown(&Timer2, 2, 1000);
  while(1);
}

void Timer_T3_T4_Test(void) { // basic timers
  NewTimer_us(&Timer3, TIM3, 1000, GetMCUClockTree());// 1ms tick period
  NewTimer_us(&Timer4, TIM4, 10, GetMCUClockTree());// 10us period
  NVIC_TimersEnable(ENABLE);
  
  HookTimerCountdown(&Timer3, 1, (u32)ToggleLED1, (u32)&Timer3); 
  ArmTimerCountdown(&Timer3, 1, 500);
  HookTimerCountdown(&Timer3, 2, (u32)ToggleLED2, (u32)&Timer3); 
  ArmTimerCountdown(&Timer3, 2, 1000);
  
  HookTimerCountdown(&Timer4, 1, (u32)ToggleLED3, (u32)&Timer4);
  ArmTimerCountdown(&Timer4, 1, 500);
  HookTimerCountdown(&Timer4, 2, (u32)ToggleLED4, (u32)&Timer4);
  ArmTimerCountdown(&Timer4, 2, 1000);
  while(1);
}

void Timer_T5_T6_Test(void) { // basic timers
  NewTimer_us(&Timer5, TIM5, 1000, GetMCUClockTree());// 1ms tick period
  NewTimer_us(&Timer6, TIM6, 10, GetMCUClockTree());// 10us period
  NVIC_TimersEnable(ENABLE);
  
  HookTimerCountdown(&Timer5, 1, (u32)ToggleLED1, (u32)&Timer5); 
  ArmTimerCountdown(&Timer5, 1, 500);
  HookTimerCountdown(&Timer5, 2, (u32)ToggleLED2, (u32)&Timer5); 
  ArmTimerCountdown(&Timer5, 2, 1000);
  
  HookTimerCountdown(&Timer6, 1, (u32)ToggleLED3, (u32)&Timer6);
  ArmTimerCountdown(&Timer6, 1, 500);
  HookTimerCountdown(&Timer6, 2, (u32)ToggleLED4, (u32)&Timer6);
  ArmTimerCountdown(&Timer6, 2, 1000);
  while(1);
}

void Timer_T7_T8_Test(void) { // basic timers
  NewTimer_us(&Timer7, TIM7, 1000, GetMCUClockTree());// 1ms tick period
  NewTimer_us(&Timer8, TIM8, 10, GetMCUClockTree());// 10us period
  NVIC_TimersEnable(ENABLE);
  
  HookTimerCountdown(&Timer7, 1, (u32)ToggleLED1, (u32)&Timer7); 
  ArmTimerCountdown(&Timer7, 1, 500);
  HookTimerCountdown(&Timer7, 2, (u32)ToggleLED2, (u32)&Timer7); 
  ArmTimerCountdown(&Timer7, 2, 1000);
  
  HookTimerCountdown(&Timer8, 1, (u32)ToggleLED3, (u32)&Timer8);
  ArmTimerCountdown(&Timer8, 1, 500);
  HookTimerCountdown(&Timer8, 2, (u32)ToggleLED4, (u32)&Timer8);
  ArmTimerCountdown(&Timer8, 2, 1000);
  while(1);
}

void Timer_T9_T10_Test(void) { // T9.2, T10.1
  NewTimer_us(&Timer9, TIM9, 1000, GetMCUClockTree());// 1ms tick period
  NewTimer_us(&Timer10, TIM10, 10, GetMCUClockTree());// 10us period
  NVIC_TimersEnable(ENABLE);
  
  HookTimerCountdown(&Timer9, 1, (u32)ToggleLED1, (u32)&Timer9); 
  ArmTimerCountdown(&Timer9, 1, 500);
  HookTimerCountdown(&Timer9, 2, (u32)ToggleLED2, (u32)&Timer9); 
  ArmTimerCountdown(&Timer9, 2, 1000);
  HookTimerCountdown(&Timer10, 1, (u32)ToggleLED3, (u32)&Timer10);
  ArmTimerCountdown(&Timer10, 1, 500);
  HookTimerCountdown(&Timer10, 2, (u32)ToggleLED4, (u32)&Timer10);
  ArmTimerCountdown(&Timer10, 2, 1000);
  while(1);
}

void Timer_T11_T12_Test(void) { // T11.1 T12.2
  NewTimer_us(&Timer11, TIM11, 1000, GetMCUClockTree());// 1ms tick period
  NewTimer_us(&Timer12, TIM12, 10, GetMCUClockTree());// 10us period
  NVIC_TimersEnable(ENABLE);
  
  HookTimerCountdown(&Timer11, 1, (u32)ToggleLED1, (u32)&Timer11); 
  ArmTimerCountdown(&Timer11, 1, 500);
  HookTimerCountdown(&Timer11, 2, (u32)ToggleLED2, (u32)&Timer11); 
  ArmTimerCountdown(&Timer11, 2, 1000);
  HookTimerCountdown(&Timer12, 1, (u32)ToggleLED3, (u32)&Timer12);
  ArmTimerCountdown(&Timer12, 1, 500);
  HookTimerCountdown(&Timer12, 2, (u32)ToggleLED4, (u32)&Timer12);
  ArmTimerCountdown(&Timer12, 2, 1000);
  while(1);
}

void Timer_T13_T14_Test(void) { // T13.1 T14.1
  NewTimer_us(&Timer13, TIM13, 1000, GetMCUClockTree());// 1ms tick period
  NewTimer_us(&Timer14, TIM14, 10, GetMCUClockTree());// 10us period
  NVIC_TimersEnable(ENABLE);
  
  HookTimerCountdown(&Timer13, 1, (u32)ToggleLED1, (u32)&Timer13); 
  ArmTimerCountdown(&Timer13, 1, 500);
  HookTimerCountdown(&Timer13, 2, (u32)ToggleLED2, (u32)&Timer13); 
  ArmTimerCountdown(&Timer13, 2, 1000);
  HookTimerCountdown(&Timer14, 1, (u32)ToggleLED3, (u32)&Timer14);
  ArmTimerCountdown(&Timer14, 1, 500);
  HookTimerCountdown(&Timer14, 2, (u32)ToggleLED4, (u32)&Timer14);
  ArmTimerCountdown(&Timer14, 2, 1000);
  while(1);
}

//============== Testing Input Captures and Output Compares In Interrupt and PWM (no interrupt) modes ====================
static IO_Pin_t T3c1_PC6, T3c2_PC7;
static IO_Pin_t T5c1_PH10, T5c2_PH11, T5c3_PH12;
static vu8 loop = 1;
void Timer_IC_OC_PWM_Test(void) {
  
  // Configure the corresponding pins
  NewTimer_us(&Timer3, TIM3, 1000000, GetMCUClockTree());// 1s overflow tick period
  NewTimer_us(&Timer5, TIM5, 1000000, GetMCUClockTree());// 1s overflow tick period
  
  // We will later give simple function to configure the capture and the timer behavious
  // For now all input capture are rising edge, no prescale, no filter, pin to channel, no special.
//  SetTimerInputCC((u32)&Timer3, 1, IO_PinInit(&T3c1_PC6, PC6), ENABLE);
  SetTimerOutputCC((u32) &Timer3, 1, IO_PinInit(&T3c1_PC6, PC6), GetRatio(30, 100, Timer3.TIM->ARR), ENABLE); // I want 30% duty cycle
  
  SetTimerInputCC((u32)&Timer3, 2, IO_PinInit(&T3c2_PC7, PC7), ENABLE);
  
  SetTimerInputCC((u32)&Timer5, 1, IO_PinInit(&T5c1_PH10, PH10), ENABLE);
  
  SetTimerInputCC((u32)&Timer5, 2, IO_PinInit(&T5c2_PH11, PH11), ENABLE);
  
  SetTimerInputCC((u32)&Timer5, 3, IO_PinInit(&T5c3_PH12, PH12), ENABLE);

  // For now the output compare are in PWM mode with interrupt (optional)
  // Later there will be some configurability of the output compare available
  
  EnableTimerCC_Interrupt((u32)&Timer3, 1, ENABLE);
  EnableTimerCC_Interrupt((u32)&Timer3, 2, ENABLE);
  
  EnableTimerCC_Interrupt((u32)&Timer5, 1, ENABLE);
  EnableTimerCC_Interrupt((u32)&Timer5, 2, ENABLE);
  EnableTimerCC_Interrupt((u32)&Timer5, 3, ENABLE);
  NVIC_TimersEnable(ENABLE);
  while(loop); // it will continue by debugger (manually set statement to next line of code which will be compile and not removed by compiler)
  
  // output compare trial
  
}


#endif
