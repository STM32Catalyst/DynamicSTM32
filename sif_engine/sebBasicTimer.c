
#include "sebEngine.h"

// a Basic Timer has only Overflow interrupt and optionally an output trigger and DMA trigger. TIM6/7 on STM32F437
// We use these Timers to generate a timestamp. The timers are freerun, their beat rate is programmable and each timer has 4 countdown which can trigger a hook(fn,ct)
//Disable the interrupt when all countdown are reaching zero, and to enable when any countdown become non zero (must be manipulated by functions only)

static u32 BasicTimer_CountDownModeIRQHandler(u32 u) {
  
  u8 n;
  u32 AnyCountdown = 0;
  BasicTimer* BT = (BasicTimer*) u;

  BT->TIM->SR &= ~1; // clear the pending bit
  BT->Ticks++;
  // Countdown0
  for(n=0;n<BT_MAX_COUNTDOWN;n++) { // unlooped by compiler when needed
    if(BT->CountDown[n])
      if(--BT->CountDown[n]==0) // times up!
        if(BT->fnCountDown[n]) {// call the hook
          ((u32(*)(u32))BT->fnCountDown[n])(BT->ctCountDown[n]);
        }else{ // if no hook, then set a flag for polling purpose
          BT->CountDownDone[n] = 1;
        }
    AnyCountdown |= BT->CountDown[n];
  };
  // if all countdowns are zero... what do we do? Self disable all? Or it's inside each hook?
  
  if(AnyCountdown==0) { // we can stop the interrupt, easier to debug this way
    BT->TIM->DIER &= ~0x0001;// Disable INT
  }

  return 0;
}

void NewBasicTimer_us(BasicTimer* BT, TIM_TypeDef* T, u32 Period_us, MCUClockTree * Tree) {// if Period_us = 0, disable the timer

  // the structure should be edited by caller and only the specifics done here.
  BT->FeedClockMHz = Tree->APB1Clk_Hz; // all basic timers are using this one.
  u32 cy,psc,arr;
  if(Period_us==0) while(1); // invalid choice

  if(BT->OverflowPeriod_us)
    while(1); // already booked! Free the timer first before reusing it!
  
  BT->TIM = T;
  BT->OverflowPeriod_us = Period_us;

  // we only need to guarantee the period in us with lowest timer clock for it
  cy = BT->FeedClockMHz * Period_us; // if 1 us: r = 96 cycles at 96MHz
  // now let's cook the timer prescaler and autoreload values...
  psc = 0;
  arr = cy;

  while(arr>0xFFFF) { // in this scheme we want the lowest psc value (not best for power)
     psc++;
     arr = cy / (psc+1);
     if(psc>0xFFFF) while(1); // not possible, input clock too high or period too big
  }

  ClockGateEnable_PPP((u32)T, ENABLE);
  
  HookIRQ_PPP((u32)T, (u32) BasicTimer_CountDownModeIRQHandler, (u32) BT);

  BT->TIM->CR2 = 2<<4;// update event as trigger output

  BT->TIM->PSC = psc; // This is the prescaler
  BT->TIM->ARR = arr; // The autoreload value
  
  // We hook up the NVIC directly to the unique IRQ handler (scheme fixed)
  BT->TIM->SR &= ~1; // clear the pending bit
/* this will be done when a non zero countdown comes in  
  if(Period_us) { // Enable interrupt source
    BT->TIM->DIER |= 0x0001; // Enable INT
  }else{ // Disable interrupt source
    BT->TIM->DIER &= ~0x0001;// Disable INT
  }
*/  
  BT->TIM->CR1 = 5; // Event enabled, no shadowing, freerun [ENABLE TIMER NOW]
  
}


void HookBasicTimerCountdown(BasicTimer* BT, u32 n, u32 fn, u32 ct) {
  // hook first!
  BT->fnCountDown[n] = fn;
  BT->ctCountDown[n] = ct;
}

void ArmBasicTimerCountdown(BasicTimer* BT, u32 n, u32 ticks) {
  
  BT->CountDownDone[n] = 0; // clear this flag
  BT->CountDown[n] = BT->InitialCountDown[n] = ticks+1; //rounding up this will run wild as soon as written!
  BT->TIM->DIER |= 0x0001; // Enable INT
}

void ReArmBasicTimerCountdown(BasicTimer* BT, u32 n) {

  BT->CountDown[n] = BT->InitialCountDown[n]; 
  BT->TIM->DIER |= 0x0001; // Enable INT
}

void NVIC_BasicTimersEnable(FunctionalState Enable) {
  // all the interrupt handlers exist and clear flags by default
  // call this only when final activation of the EXTI interrupt generation mecanism
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = Enable;
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  
  // These IRQ can be always enabled as the timer has its own interrupt enable bit.
}
//=============-----> END OF REFERENCE CODE

//==--->
//=====----->
//========-------->
//===========---------->
//=============-------------> Sequencer compatible functions

u32 sq_HookBasicTimerCountdown(u32 u) {// (BasicTimer* BT, u32 n, u32 fn, u32 ct) // BT, countdown_nb, fn to call, ct to call

  u32* p = (u32*) u;
  HookBasicTimerCountdown((BasicTimer*)p[0], p[1], p[2], p[3]);
  return 0;
}

u32 sq_ArmBasicTimerCountdown(u32 u) { //(BasicTimer* BT, u32 n, u32 ticks)
  
  u32* p = (u32*) u;
  ArmBasicTimerCountdown((BasicTimer*)p[0], p[1], p[2]);
  return 1; // this will cause an interrupt later
}

u32 sq_ReArmBasicTimerCountdown(u32 u) { //(BasicTimer* BT, u32 n) 

  u32* p = (u32*) u;
  ReArmBasicTimerCountdown((BasicTimer*)p[0], p[1]);
  return 1; // this will cause an interrupt later
}

//======================== Dynamic allocation of a timer countdown

u32 GetFreeTimerCountdownIndex(u32 u) {
  
  u32 n;
  BasicTimer* BT = (BasicTimer*) u;
  
  for(n=0;n<BT_MAX_COUNTDOWN;n++)
    if(BT->InitialCountDown[n]==0)
      return n;
  
  return -1; // no more resource available
}

u32 SetFreeTimerCountdownIndex(u32 u, u32 n) {
  
  BasicTimer* BT = (BasicTimer*) u;
  BT->InitialCountDown[n] = 0;
  return 0;
}


//=============-----> This is to test the timer functionality
#ifdef ADD_EXAMPLES_TO_PROJECT
static u32 ToggleLED1(u32 u) {
  BasicTimer* T = (BasicTimer*) u;
// we directly use u as a parameter, not a pointer of structure as here it is very simple and fixed
  Toggle_LED_1();
  ReArmBasicTimerCountdown(T,1);
  return 0;
}

static u32 ToggleLED2(u32 u) {
  BasicTimer* T = (BasicTimer*) u;
// we directly use u as a parameter, not a pointer of structure as here it is very simple and fixed
  Toggle_LED_2();
  ReArmBasicTimerCountdown(T,2);
  return 0;
}

static u32 ToggleLED3(u32 u) {
  BasicTimer* T = (BasicTimer*) u;
// we directly use u as a parameter, not a pointer of structure as here it is very simple and fixed
  Toggle_LED_3();
  ReArmBasicTimerCountdown(T,1);
  return 0;
}

static u32 ToggleLED4(u32 u) {
  BasicTimer* T = (BasicTimer*) u;
// we directly use u as a parameter, not a pointer of structure as here it is very simple and fixed
  Toggle_LED_4();
  ReArmBasicTimerCountdown(T,2);
  return 0;
}

// how about it hooks itself? Also the timers could be local variables and destroyed later (after releasing them)!
// this would also dynamic HW sharing... with optimized RAM allocation.
static BasicTimer Timer6, Timer7;
void fnTIM67_Test(void) {
  
  NewBasicTimer_us(&Timer6, TIM6, 1000, GetMCUClockTree());// 1ms tick period
  NewBasicTimer_us(&Timer7, TIM7, 10, GetMCUClockTree());// 10us period
  NVIC_BasicTimersEnable(ENABLE);
  
  HookBasicTimerCountdown(&Timer6, 1, (u32)ToggleLED1, (u32)&Timer6); 
  ArmBasicTimerCountdown(&Timer6, 1, 500);
  HookBasicTimerCountdown(&Timer6, 2, (u32)ToggleLED2, (u32)&Timer6); 
  ArmBasicTimerCountdown(&Timer6, 2, 1000);
  HookBasicTimerCountdown(&Timer7, 1, (u32)ToggleLED3, (u32)&Timer7);
  ArmBasicTimerCountdown(&Timer7, 1, 500);
  HookBasicTimerCountdown(&Timer7, 2, (u32)ToggleLED4, (u32)&Timer7);
  ArmBasicTimerCountdown(&Timer7, 2, 1000);
  while(1);
}
#endif

