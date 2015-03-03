
#include "SebEngine.h"
#include "SebTimer.h"

// This is the 16 bit 4 channels timer, TIM3 and TIM4 only timers are 16 bit 4 channels timers on STM32F437
// This also support the 16 bit 2 channels timers TIM9&12 (2ch), TIM10&11&13&14 (1ch), basic timers 6&7

static u32 Timer_CountDownModeIRQHandler(u32 u) {
  
  u8 n;
  u32 AnyCountdown = 0;
  Timer_t* Timer = (Timer_t*) u;

  Timer->TIM->SR &= ~1; // clear the pending bit
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
    Timer->TIM->DIER &= ~0x0001;// Disable INT
  }

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
  Timer->Max = MCU_Timer_GetMax_by_PPP((u32)T);
  Timer->SR_ValidFlags = MCU_Timer_GetSR_ValidFlags_by_PPP((u32)T);
  
  
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
  Timer->fnCountDown[n] = fn;
  Timer->ctCountDown[n] = ct;
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

#endif
