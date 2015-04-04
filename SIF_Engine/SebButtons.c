
#include "SebEngine.h"
#include "SebButtons.h"

//==== Below are the different method to generate buttons/switch report as a 32 bit bitfield
// It will count how many times the bitfield is stable, and use it to report debounce, repeat, hot key (pressed long enough) with a calling function or a flag set.
// If a pin should be independent (like a power key) or have unusual behaviour, and work all the time, create a separate keygroup! (in RAM)

// This is the simplest digital keyboard, using one digital input per switch
// The switch is pressed causes the GPIO to be a 0 (GND), to scan, it only needs a pin list which last entry is the NULL pointer (like the NULL char in a string! :-)
// + Fast Scan
// + Key combinations
// + Clock Wakeup possible
// - many pins

u32 ConfigureDigitalKeys(u32 u) {

  KeyGroup_t* K = (KeyGroup_t*) u;
  u8 n;
    
  for(n=0;n<K->PinCount;n++) {
    // initialize the pin as digital input with low bandwidth and internal pull-up enabled
    IO_PinClockEnable(K->Pins[n]);
    IO_PinSetHigh(K->Pins[n]);  //-
    IO_PinSetInput(K->Pins[n]);  
    IO_PinEnablePullUpDown(K->Pins[n], ENABLE, DISABLE);
    IO_PinEnableHighDrive(K->Pins[n], ENABLE);
    IO_PinSetSpeedMHz(K->Pins[n], 0);    
  }
  
  return 0;
}

u32 ScanDigitalKeys(u32 u) { // for generic hook call
  // we browse through all the keys in the list until we have a null-pointer or reach 32 bit quantity
  KeyGroup_t* K = (KeyGroup_t*) u;
  u32 bm = 0x00000000;
  for(u8 n=0;n<K->PinCount;n++) {
    if(IO_PinRead(K->Pins[n])==0) // when the key is GND, key is pressed
      bm |= 1<<n;
  }
  
  return bm;
}

// This is the analog keyboard
// There is a resistor divider made of several resistors
// Between each resistor, a switch can force ground, changing the node voltage, which is sensed by an ADC
// This way, 5-9 keys can be supported with one pin.
// If need key combination, create multiple analog keyboards, and combine them at higher level.
// The key that forces GND can be used to wake up the MCU with EXTI. Not the others.
// An array of ADC values in mV or LSB with 2 values for each key (tolerances), ends when the values reach 0 (0 min and 0 max)
// + 1 pin
// - No key combination
// - one switch only for clock wakeup
// - slower, need ADC conversion

u32 AnalogKeyScan(u32 u) {
  
  return 0;
}


// This is the matrix keyboard
// Keys are placed between pins in a matrix row and column fashion
// By following a scan sequence which changes pin from input to output, 
// all the keys can be scanned
// + less pins
// + Key combinations
// 0 normal speed
// - Limited clock wakeup

u32 MatrixKeyScan(u32 u) {
  
  return 0;
}

// This is the triangular keyboard scan
// The layout of the keys follow a triangular pattern
// It's like the matrix keyboard, with more keys with less pins. (22 keys for 8 IOs)

u32 TriangularKeyScan(u32 u) {
  
  return 0;
}

// Virtual keyboard (using IO expander or serial communication to emulate a keyboard: Updated by field and flag for "process it"
u32 VirtualKeyScan(u32 u) {
  
  return 0;
}


//========================================================================================

u32 KeysProcessInformation (KeyGroup_t* K)
{
  K->PreviousMask = K->Mask; // for all keys at once  
  KeysClearFlags(K); // clear all flags (processed)
  K->Mask = ScanDigitalKeys((u32)K);

  //=========== Is the time for pressed key combination increasing or not? or reset?
  
  // no keys, as previously? Nothing to do
  if((K->Mask==0)&&(K->PreviousMask==0))
    return 0;
  
  // If previous key was pressed, or key changed
  // at least some key pressed, and it's same as before, inc the time
  if(K->PreviousMask!=K->Mask) { // same 
    if(K->PreviousMask) { // a Release event occured
      KeysClearFlags(K); // clear all
      K->Released = 1; // Key has just been released
      K->Time = 0; // reset the pressed counter
      return 0; // finished. (we start counting next time)
    }else{ // before no key was pressed
      return 0;
    }
  };
  
  //=========== We reach here only if keys are stable and pressed
  
  K->Time++; // time this key combi is stable pressed
    
  //============= now let's analyse Time in details

    // check for hot key (long pressed time key)
    if(K->Time==K->HotTime) 
      K->Hot = 1;
    
    // Check if it has reached the debouncing delay
    if(K->Time==K->DebounceTime) {
      K->Pressed = 1;
      K->RepeatCountdown = K->RepeatStartTime;
      
    }else{
      // Key repeat events using countdown (can't use _32msec: key repeat will stop after 4 seconds = 0xFF)
      if(K->RepeatCountdown) {
          
        K->RepeatCountdown--;

        if(K->RepeatCountdown==0) {	// repeat event reached
            
          K->Repeated = 1;
          
          // prepare for next repeat
          if(K->Time<K->RepeatSlowTimeDuration) 	// if key pressed for less than 200x16ms = 2.6 sec			
            K->RepeatCountdown = K->RepeatSlowTime;
          else
            K->RepeatCountdown = K->RepeatFastTime;
        }
      }
    };

  if((K->Pressed)||(K->Repeated)||(K->Released)||(K->Hot))
    if(K->fnKeyEvent) ((u32(*)(u32))(K->fnKeyEvent))((K->ctKeyEvent));
  
return 0;
}

//========== These are to be used by GUIs or Human interface level functions
/*
u32 IsKeyDown(KeyGroup_t* K, u32 Key) {
  
  return (K->Mask == Key);
}
*/
u32 CheckKeyPressed(KeyGroup_t* K, u32 Key) {
  
  return (K->Pressed&&(K->Mask == Key));
}

u32 CheckKeyRepeated(KeyGroup_t* K, u32 Key) {
  
  return (K->Repeated && (K->Mask == Key));
}

u32 CheckKeyHot(KeyGroup_t* K, u32 Key) {
  
  return (K->Hot && (K->Mask == Key));
}

u32 CheckKeyReleased(KeyGroup_t* K, u32 Key) {
  
  return (K->Released && (K->Mask == Key));
}

u32 KeysClearFlags(KeyGroup_t* K) {
  
  K->Pressed = 0;
  K->Repeated = 0;
  K->Hot = 0;
  K->Released = 0;
  return 0;
}


// for now, we don't have the keyboard hook set in place.


u32 NewKeyGroup(KeyGroup_t* K, KeyStyle_t KeyStyle) {
  
  K->KeyStyle = KeyStyle;
  K->PinCount = 0;
  

  KeysClearFlags(K); // clear all keys
  K->RepeatCountdown = 0;	// no repeat

  return 0;
}

u32 AddKeyPin(KeyGroup_t* K, IO_Pin_t* P) {
  
  if(K->PinCount>=KEYGROUP_MAX_KEYS) while(1); // maximum bitmask size
  K->Pins[K->PinCount++] = P;
  
  return 0;
}

u32 SetKeyGroup_Timings(KeyGroup_t* K) {

  K->Time = 0;	// 

  // these are timings
  K->DebounceTime = 2;
  K->RepeatStartTime = 10;
  K->RepeatSlowTime = 3;
  K->RepeatSlowTimeDuration = 20;
  K->RepeatFastTime = 1;
  K->HotTime = 40;
  
  return 0;
}

u32 ConfigureKeyGroup(KeyGroup_t* K) {

  if(K->KeyStyle==DIGITAL_KEYS)
    ConfigureDigitalKeys((u32)K);
  else while(1); // unsupported
  
  return 0;
}

u32 EnableKeyGroup(KeyGroup_t* K) {
  
  return 0;
}


