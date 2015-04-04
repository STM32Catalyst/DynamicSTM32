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
  IO_Pin_t* Key;  // can be NULL means it's not an IO (or it's dead or remote update)
  u32 Time;// u8 = 4 sec max, u16 no limitation
  u32 RepeatCountdown;
}DigitalSwitch_t;


typedef enum {
  UNKNOWN_KEYS,
  DIGITAL_KEYS,
  MATRIX_KEYS,
  TRIANGULAR_KEYS,
  VIRTUAL_KEYS,
  ANALOG_KEYS,  
  COUNT_OF_KEY_TYPES
} KeyStyle_t;


#define KEYGROUP_MAX_KEYS 16

typedef struct {
  
  KeyStyle_t KeyStyle;
  u32 Time;// u8 = 4 sec max, u16 no limitation
  
  u32 RepeatCountdown;  
  u8 DebounceTime;// how many confirmed scans to consider the key pressed to be stable //	1	// in loops (timer overflow 16 msec)
  u16 RepeatStartTime;// Program here the time to repeat a key in 16msec unit//		10	// in loops (timer overflow 16 msec)
  u8 RepeatSlowTime;// Program here the normal speed repetition//	3
  u16 RepeatSlowTimeDuration;// Program here the corresponding slow time (from Key Pressed)//	20
  u8 RepeatFastTime;// Program here the fast speed repetition//	1
  u16 HotTime;// Time Key is setup for 4 seconds (must not be the maximum FF-1 value)//	40 // 0.1 sec x 50 = 4 seconds

//  StuffsArtery_t* SA; // this points to Job feeding
  
//  Timer_t* Timer; // this will be to control time ticks
//  u8 Cn; //  a number between 0 and 3

//  u32 (*fnWaitMethod)(u32);
//  u32 ctWaitMethod;
//  u32 WaitParam;  
  
  IO_Pin_t* Pins[KEYGROUP_MAX_KEYS]; // all keyboards, except remote or virtual ones will have a list of pins
  u8 PinCount;
  u32 pKeys; // this points to information used to generate the bitmask of keys from the pins. Beware, there can be more keys than pins!
  
//  u32 (*fnScanMethod)(u32);
//  u32 ctScanMethod;  // pointer to the structure of switches to scan (be analog, digital, remote, RAM, etc...), this structure should have all info needed. The function returns a bitfield, one bit per key
  
  u32 PreviousMask;
  u32 Mask; // each bit is a key
  
  u8 Pressed : 1;
  u8 Hot : 1;
  u8 Released : 1;
  u8 Repeated : 1;
  u8 ScanDone : 1; // in case the scan is done separately asynchroneously (mask to be updated and processed only when this flag is set)

  u32 (*fnKeyEvent)(u32); // will be called if non zero AND some flags are set (Hot/Released/Repeated)
  u32 ctKeyEvent;
  
} KeyGroup_t; // you can clone this as many times as needed


u32 NewKeyGroup(KeyGroup_t* K, KeyStyle_t KeyStyle); // Simple digital keyboards
u32 AddKeyPin(KeyGroup_t* K, IO_Pin_t* P);
u32 SetKeyGroup_Timings(KeyGroup_t* K); // clock related
u32 ConfigureKeyGroup(KeyGroup_t* K);
u32 EnableKeyGroup(KeyGroup_t* K);



u32 KeysProcessInformation (KeyGroup_t* K);
u32 KeysInstantSweep(KeyGroup_t* K);
//u32 KeyDown(KeyGroup_t* K, u32 Key);
u32 CheckKeyPressed(KeyGroup_t* K, u32 Key);
u32 CheckKeyRepeated(KeyGroup_t* K, u32 Key);
u32 CheckKeyHot(KeyGroup_t* K, u32 Key);
u32 CheckKeyReleased(KeyGroup_t* K, u32 Key);
u32 KeysClearFlags(KeyGroup_t* K);

//==============================
// Friendly functions


  
  
  
#endif

