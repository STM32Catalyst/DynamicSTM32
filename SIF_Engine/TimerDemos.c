
#include "SebEngine.h"

// This file is part of the Timer.c file, it deals with Capture/Compare management


//=============-----> This is to test the timer functionality

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
  EnableFreeRunTimer(&Timer1);
  EnableFreeRunTimer(&Timer2);
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
  EnableFreeRunTimer(&Timer3);
  EnableFreeRunTimer(&Timer4);
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
  EnableFreeRunTimer(&Timer5);
  EnableFreeRunTimer(&Timer6);
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
  EnableFreeRunTimer(&Timer7);
  EnableFreeRunTimer(&Timer8);
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
  EnableFreeRunTimer(&Timer9);
  EnableFreeRunTimer(&Timer10);
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
  EnableFreeRunTimer(&Timer11);
  EnableFreeRunTimer(&Timer12);
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
  EnableFreeRunTimer(&Timer13);
  EnableFreeRunTimer(&Timer14);
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

//========================================================================================================================


u32 NewTimerOutputCC(u32 u, u32 n, IO_Pin_t* Pin) {
  
  Timer_t* Timer = (Timer_t*) u;
  
  if(n==0) while(1); // not allowed
  if(n>=TIMER_MAX_CC) while(1); // not available CCn
  if(Timer->CCR[n]==0) while(1); // not available
  
  if(Pin) {  
    Timer->PinCC[n] = Pin;
    IO_PinClockEnable(Pin);
    IO_PinSetHigh(Pin);  
    IO_PinSetOutput(Pin);  
    IO_PinEnablePullUpDown(Pin, DISABLE, DISABLE);
    IO_PinEnableHighDrive(Pin, ENABLE);
    IO_PinConfiguredAs(Pin,  GetPinAF(Pin->Name,(u32)Timer->TIM));  
  };
  
  return 0;
}

//============== Testing Input Captures and Output Compares In Interrupt and PWM (no interrupt) modes ====================
static IO_Pin_t T3c1_PC6, T3c2_PC7;
static IO_Pin_t T5c1_PH10, T5c2_PH11, T5c3_PH12;
static vu8 loop = 1;

static u16 MultiOC[] = { 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000, 0x9000, 0xa000, 0xb000, 0xc000, 0xd000, 0xe000};

void Timer_IC_OC_PWM_Test(void) {
  
  //=========== TIMER 3 related
  // Configure the corresponding pins
  NewTimer_us(&Timer3, TIM3, 10000, GetMCUClockTree());// 0.01s overflow tick period
  
  // We will later give simple function to configure the capture and the timer behavious
  // For now all input capture are rising edge, no prescale, no filter, pin to channel, no special.
#if 0
  NewTimerOutputCC((u32) &Timer3, 1, NewIO_Pin(&T3c1_PC6, PC6));  
  SetTimerOutputCC_SingleEdge(&Timer3, 1, GetRatio(30, 100, Timer3.TIM->ARR));// I want 30% duty cycle on PC6_T3.1
#else // multiple pulses by DMA
  NewTimerOutputCC((u32) &Timer3, 1, NewIO_Pin(&T3c1_PC6, PC6));   
  SetTimerOutputCC_MultiEdges(&Timer3, 1, (u32)MultiOC, 4);// I want 30% duty cycle on PC6_T3.1
#endif  
  
//  SetTimerInputCC((u32)&Timer3, 1, NewIO_Pin(&T3c1_PC6, PC6), ENABLE);
  SetTimerInputCC((u32)&Timer3, 2, NewIO_Pin(&T3c2_PC7, PC7), ENABLE);
  EnableOneShotTimerCC_Triggered(&Timer3, 2); // Use T3.2 as trigger input
//  EnableFreeRunTimer(&Timer3);

  // For now the output compare are in PWM mode with interrupt (optional)
  // Later there will be some configurability of the output compare available
  
  EnableTimerCC_Interrupt((u32)&Timer3, 1, DISABLE);//output compare ENABLE);
  EnableTimerCC_Interrupt((u32)&Timer3, 2, DISABLE);

  //========== TIMER 5 related
  NewTimer_us(&Timer5, TIM5, 1000000, GetMCUClockTree());// 1s overflow tick period

  SetTimerInputCC((u32)&Timer5, 1, NewIO_Pin(&T5c1_PH10, PH10), ENABLE);
  SetTimerInputCC((u32)&Timer5, 2, NewIO_Pin(&T5c2_PH11, PH11), ENABLE);
  SetTimerInputCC((u32)&Timer5, 3, NewIO_Pin(&T5c3_PH12, PH12), ENABLE);
  EnableFreeRunTimer(&Timer5);
  
  EnableTimerCC_Interrupt((u32)&Timer5, 1, ENABLE);
  EnableTimerCC_Interrupt((u32)&Timer5, 2, ENABLE);
  EnableTimerCC_Interrupt((u32)&Timer5, 3, ENABLE);
  NVIC_TimersEnable(ENABLE);
  while(loop); // it will continue by debugger (manually set statement to next line of code which will be compile and not removed by compiler)
  
  // output compare trial
}
