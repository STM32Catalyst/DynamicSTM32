/*\
| | Keyboard Header File
| | -------------------------------------------------------------------
| | Prototype accessible functions in keyboard.c
| |
| |	Copyright © 1998-2004, STMicroelectronics, Inc.
| |	All rights reserved.
| | -------------------------------------------------------------------
\*/

// History of changes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// WHO        WHEN      WHAT
// NICKNAME   MMDDYY    MAIN CHANGES REASON
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FAE_TWN    090203    Created Header

// This was a stable code which was compatible with analog keyboards (use ADC to reuse the pincount, a key change the resistor divider...)
// Revisited to be used by Events (either Timer or EXTI)

#ifndef _SEB_BUTTONS_H_
#define _SEB_BUTTONS_H_


// Keyboard scan, Buttons
typedef struct {
  IO_Pin_t* Key; 

  u32 Time;// u8 = 4 sec max, u16 no limitation
  u32 RepeatCountdown;
}Button_t;

/*
typedef struct {
  IO_Pin_t* Key; 
  // Analog Keys only, with ADC_Max non zero
  u16 ADC_Min;
  u16 ADC_Max; // if MAX is zero, it's a digital key

  u32 Time;// u8 = 4 sec max, u16 no limitation
  u32 RepeatCountdown;
} AnalogButton_t;
*/

typedef struct {

  u32 DebounceTime;//	1	// in loops (timer overflow 16 msec)
// Program here the time to repeat a key in 16msec unit
  u32 RepeatStartTime;//		10	// in loops (timer overflow 16 msec)
// Program here the normal speed repetition
  u32 RepeatSlowTime;//	3
// Program here the corresponding slow time (from Key Pressed)
  u32 RepeatSlowTimeDuration;//	20
// Program here the fast speed repetition
  u32 RepeatFastTime;//	1
// Time Key is setup for 4 seconds (must not be the maximum FF-1 value)
  u32 HotKeyTime;//	40 // 0.1 sec x 50 = 4 seconds

//  StuffsArtery_t* SA; // this points to Job feeding
  
//  Timer_t* Timer; // this will be to control time ticks
//  u8 Cn; //  a number between 0 and 3

//  u32 (*fnWaitMethod)(u32);
//  u32 ctWaitMethod;
//  u32 WaitParam;  
  
  Button_t Button[16]; // max 16 buttons in this implementation
//  AnalogButton_t A_Button[16]; // analog buttons, max 16 (to be finished)
  
  u32 PreviousKeysMask;
  u32 KeysMask; // each bit is a key
  u32 KeysPressed;
  u32 KeysHot;
  u32 KeysReleased;
  u32 KeysRepeated;

  u32 (*fnKeyEvent)(u32); // will be called if non zero AND some flags are set (Hot/Released/Repeated)
  u32 ctKeyEvent;
  
} Buttons_t;

u32 KeysProcessInformation (Buttons_t* K);
u32 KeysInstantSweep(Buttons_t* K);
u32 KeyDown(Buttons_t* K, u32 Key);
u32 KeyPressed(Buttons_t* K, u32 Key);
u32 KeyRepeated(Buttons_t* K, u32 Key);
u32 KeyHot(Buttons_t* K, u32 Key);
u32 KeyReleased(Buttons_t* K, u32 Key);
u32 KeysClearFlags(Buttons_t* K, u32 Key);
u32 InitButton(Button_t* B);
u32 InitButtons(Buttons_t* K);

#endif

