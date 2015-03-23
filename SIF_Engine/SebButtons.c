
#include "SebEngine.h"
#include "SebButtons.h"

u32 KeysProcessInformation (Buttons_t* K)
{
  u8 n;
  u32 Mask = 1;
  
  K->PreviousKeysMask = K->KeysMask; // for all keys at once
  KeysClearFlags(K, ~0UL); // clear all flags (processed)
  KeysInstantSweep(K); // KeysMask will show all keys pressed or not this instant
  
  for(n=0;n<countof(K->Button);n++, Mask<<=1) {

    if(K->Button[n].Key == 0) break; // no more keys (contiguous assumption, otherwise, continue)
    
    if((K->KeysMask & Mask)==0) { // Key is not pressed
      if(K->PreviousKeysMask & Mask) { // Key was pressed before
        KeysClearFlags(K, Mask);
        K->KeysReleased |= Mask; // Key has just been released
      };
      
      K->Button[n].Time = 0; // reset the pressed counter
      continue;
      
    }else{
        K->Button[n].Time++; // increased the pressed counter
    }

    // check for hot key (time key)
    if(K->Button[n].Time==K->HotKeyTime) 
      K->KeysHot |= Mask;
    
    // Check if it has reached the debouncing delay
    if(K->Button[n].Time==K->DebounceTime) {
      K->KeysPressed |= Mask;
      K->Button[n].RepeatCountdown = K->RepeatStartTime;
      
    }else{
      // Key repeat events using countdown (can't use _32msec: key repeat will stop after 4 seconds = 0xFF)
      if(K->Button[n].RepeatCountdown) {
          
        K->Button[n].RepeatCountdown--;

        if(K->Button[n].RepeatCountdown==0) {	// repeat event reached
            
          K->KeysRepeated |= Mask;
          
          // prepare for next repeat
          if(K->Button[n].Time<K->RepeatSlowTimeDuration) 	// if key pressed for less than 200x16ms = 2.6 sec			
            K->Button[n].RepeatCountdown = K->RepeatSlowTime;
          else
            K->Button[n].RepeatCountdown = K->RepeatFastTime;
        }
      }
    };

  }; // for loop
  
  if((K->KeysPressed)||(K->KeysRepeated)||(K->KeysReleased)||(K->KeysHot))
    if(K->fnKeyEvent) ((u32(*)(u32))(K->fnKeyEvent))((K->ctKeyEvent));
  
  return 0;
}


// here we check for all pins and tell if key is pressed or not
u32 KeysInstantSweep(Buttons_t* K) {
  
  u8 n;
  for(n=0;n<countof(K->Button);n++) {
    if(K->Button[n].Key==0) break; // no more keys (contiguous assumption, otherwise, continue)
    if(0){//K->Button[n].ADC_Max != 0) {
      // analog keyboard not yet supported
      while(1);
    }else{
      // digital keyboard supported (later on, we will also support IO Expander remote keys for low pincount MCUs)
      if(IO_PinRead(K->Button[n].Key)==1) {
        K->KeysMask &= ~1<<n;
      }else{
        K->KeysMask |= 1<<n;
      };
    };
  };

  return 0;
}

//========== These are to be used by GUIs or Human interface level functions

u32 KeyDown(Buttons_t* K, u32 Key) {
  
  return K->KeysMask & Key;
}

u32 KeyPressed(Buttons_t* K, u32 Key) {
  
  return K->KeysPressed & Key;
}

u32 KeyRepeated(Buttons_t* K, u32 Key) {
  
  return K->KeysRepeated & Key;
}

u32 KeyHot(Buttons_t* K, u32 Key) {
  
  return K->KeysHot & Key;
}

u32 KeyReleased(Buttons_t* K, u32 Key) {
  
  return K->KeysReleased & Key;
}

u32 KeysClearFlags(Buttons_t* K, u32 Key) {
  
  K->KeysMask &= ~Key;
  K->KeysPressed &= ~Key;
  K->KeysRepeated &= ~Key;
  K->KeysHot &= ~Key;
  K->KeysReleased &= ~Key;
  
  return 0;
}

u32 InitButton(Button_t* B) {

  B->RepeatCountdown = 0;	// no repeat
  B->Time             = 0;	// 
  
  // Init the GPIO
          if(0){//B->ADC_Max) {
    while(1); // analog input: Not yet supported
  }else{
    // initialize the pin as digital input with low bandwidth and internal pull-up enabled
    IO_PinClockEnable(B->Key);
//    IO_PinSetHigh(B->Key);  
    IO_PinSetInput(B->Key);  
    IO_PinEnablePullUpDown(B->Key, ENABLE, DISABLE);
    IO_PinEnableHighDrive(B->Key, ENABLE);
    IO_PinSetSpeedMHz(B->Key, 0);    
  }
  
  return 0;
}
  
u32 InitButtons (Buttons_t* K)
{
  u8 n;
  for(n=0;n<countof(K->Button);n++){
    if(K->Button[n].Key==0) break;  // no more keys (contiguous assumption, otherwise, continue)
    InitButton(&K->Button[n]);
  };
               
  KeysClearFlags(K, ~0UL); // clear all keys
          
  K->DebounceTime = 2;
  K->RepeatStartTime = 10;
  K->RepeatSlowTime = 3;
  K->RepeatSlowTimeDuration = 20;
  K->RepeatFastTime = 1;
  K->HotKeyTime = 40;
  
  return 0;
}

    
// This is to test the keyboard (digital input mode for now)

