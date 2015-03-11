
#ifndef _TIMER_H_
#define _TIMER_H_

#define TIMER_MAX_COUNTDOWN 4
#define TIMER_MAX_CC 5 // 0 is unused, we count CC from 1 to 4 unfortunately

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

//======== Now we need to take care of the CC (capture compare) which can be 0 to 4 of them in each timer.
  IO_Pin_t* PinCC[TIMER_MAX_CC];
  u32* CCR[TIMER_MAX_CC]; // pointer to each valid CCR register. 0 means not available (only with pointers!)
  u32 EdgesSize[TIMER_MAX_CC]; // 00: no, 01 = single value, which will be stored in RegCC, >1: RegCC becomes pointer to RAM buffer with DMA transfer?
  u32 EdgesTableAdr[TIMER_MAX_CC]; // used to point to the RAM area that will be used by DMA or interrupt (ideally depends on HW availability)
  u32 RegCC[TIMER_MAX_CC];  // The CC registers, backed-up from HW timers (being output compare or input capture)
  u32 fnCC[TIMER_MAX_CC]; // optional hook for each CC
  u32 ctCC[TIMER_MAX_CC]; // optional hook for each CC
  u8 FlagCC[TIMER_MAX_CC]; // set when CC occurs (we should add overflow case here to speed up MCU)
  
} Timer_t;

void ArmTimerCountdown(Timer_t* Timer, u32 n, u32 ticks);
void HookTimerCountdown(Timer_t* Timer, u32 n, u32 fn, u32 ct);
void ReArmTimerCountdown(Timer_t* Timer, u32 n);
void NVIC_TimersEnable(FunctionalState Enable);

void NewTimer_us(Timer_t* Timer, TIM_TypeDef* T, u32 Period_us, MCUClocks_t * Tree);

void SetTimerScheme(Timer_t* Timer, u32 Scheme);
void SetCC_Scheme(Timer_t* Timer, u32 n, u32 Scheme);

u32 sq_HookTimerCountdown(u32 u);
u32 sq_ArmTimerCountdown(u32 u);
u32 sq_ReArmTimerCountdown(u32 u);

void EnableFreeRunTimer(Timer_t* Timer);
void SetTimerOutputCC_SingleEdge(Timer_t* Timer, u32 n, u32 Value_lsb);
void SetTimerOutputCC_MultiEdges(Timer_t* Timer, u32 n, u32 Adr, u32 Size);
void EnableOneShotTimerCC_Triggered(Timer_t* Timer, u32 n);

u32 SetTimerInputCC(u32 u, u32 n, IO_Pin_t* Pin, FunctionalState Enable);
u32 EnableTimerCC_Interrupt(u32 u, u32 n, FunctionalState Enable);

typedef struct {
  Signal_t Signal;
  TIM_TypeDef* TIMx;
  u8 n;
} TimerRelatedSignals_t;

#endif
