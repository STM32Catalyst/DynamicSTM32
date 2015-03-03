
#ifndef _TIMER_H_
#define _TIMER_H_

#define TIMER_MAX_COUNTDOWN 4

typedef struct {

  TIM_TypeDef* TIM; //#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
  u32 Max; // the max value the timer can count
  u32 SR_ValidFlags;  // the capabilities of this timer
//  u32 scheme; // the way the timer is being used...

//===--- These are enablers for slave function to work or not, for I2C spy to work or not.
  u32 fnCountDown[TIMER_MAX_COUNTDOWN]; // First Function to call when an overflow occured
  u32 ctCountDown[TIMER_MAX_COUNTDOWN];
  
  u32 FeedClockMHz; // for debug only, not really needed
  u32 OverflowPeriod_us;

  u32 InitialCountDown[TIMER_MAX_COUNTDOWN];
  u32 CountDown[TIMER_MAX_COUNTDOWN];               // Countdown (in overflows unit)
  u32 Ticks; // autoincremented every event
  
  u8 CountDownDone[TIMER_MAX_COUNTDOWN]; // This can be used if main loop polling scheme is used. Set by Interrupt, cleared by main loop or OS.
  
} Timer_t;

void Timer_T1_T2_Test(void);
void Timer_T3_T4_Test(void);
void Timer_T5_T6_Test(void);
void Timer_T7_T8_Test(void);
void Timer_T9_T10_Test(void);
void Timer_T11_T12_Test(void);
void Timer_T13_T14_Test(void);

void ArmTimerCountdown(Timer_t* Timer, u32 n, u32 ticks);
void HookTimerCountdown(Timer_t* Timer, u32 n, u32 fn, u32 ct);
void ReArmTimerCountdown(Timer_t* Timer, u32 n);
void NVIC_TimersEnable(FunctionalState Enable);

void NewTimer_us(Timer_t* Timer, TIM_TypeDef* T, u32 Period_us, MCUClocks_t * Tree);

u32 sq_HookTimerCountdown(u32 u);
u32 sq_ArmTimerCountdown(u32 u);
u32 sq_ReArmTimerCountdown(u32 u);

#endif
