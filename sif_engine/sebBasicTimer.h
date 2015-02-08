
#ifndef _SEBBASICTIMER_H_
#define _SEBBASICTIMER_H_

#define BT_MAX_COUNTDOWN 4
// this is the code for basic timers resource in the Seb's way
typedef struct {

  TIM_TypeDef* TIM; //#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
  u32 scheme; // the way the timer is being used...

//===--- These are enablers for slave function to work or not, for I2C spy to work or not.
  u32 fnCountDown[BT_MAX_COUNTDOWN]; // First Function to call when an overflow occured
  u32 ctCountDown[BT_MAX_COUNTDOWN];
  
  u32 FeedClockMHz;
  u32 OverflowPeriod_us;

  u32 InitialCountDown[BT_MAX_COUNTDOWN];
  u32 CountDown[BT_MAX_COUNTDOWN];               // Countdown (in overflows unit)
  u32 Ticks; // autoincremented every event
  
  u8 CountDownDone[BT_MAX_COUNTDOWN]; // This can be used if main loop polling scheme is used. Set by Interrupt, cleared by main loop or OS.
  
} BasicTimer;

// This structure can be used on global variables and assigned to HW Timers. TIM6/7 of STM32F437 for example.
// Hardcoded SYSCLK = 96MHz for now.

void NewBasicTimer_us(BasicTimer* BT, TIM_TypeDef* T, u32 Period_us); // initialize the structure and corresponding HW
void HookBasicTimerCountdown(BasicTimer* BT, u32 n, u32 fn, u32 ct);// make use of countdowns to trigger a hook with programmable delay. This scheme can generate auto repeat event.
void ArmBasicTimerCountdown(BasicTimer* BT, u32 n, u32 ticks);
void ReArmBasicTimerCountdown(BasicTimer* BT, u32 n);
void NVIC_BasicTimersEnable(FunctionalState Enable); // This must be done to activate once for all the NVIC for timers. Timers will have its own INT enable bits that we will dynamically use later.

u32 sq_HookBasicTimerCountdown(u32 u); // (BasicTimer* BT, u32 n, u32 fn, u32 ct) // BT, countdown_nb, fn to call, ct to call
u32 sq_ArmBasicTimerCountdown(u32 u); //(BasicTimer* BT, u32 n, u32 ticks)
u32 sq_ReArmBasicTimerCountdown(u32 u); //(BasicTimer* BT, u32 n)
#endif
