
#include "SebEngine.h"

KeyGroup_t myKeyGroup;
IO_Pin_t UpSwitch, DownSwitch, LeftSwitch, RightSwitch, SelectSwitch;

typedef struct {
  
  u32 keymask;
  const char* Name;
} KeyDesc_t;

const KeyDesc_t KeyDesc[] = {
  
  {1<<0, "Up"},
  {1<<1, "Down"},
  {1<<2, "Left"},
  {1<<3, "Right"},
  {1<<4, "Select"},
  
};


// [   4  ][  3  ][  2 ][  1 ][ 0]
// [SELECT][RIGHT][LEFT][DOWN][UP]


u32 ButtonsTest(void) {

  KeyGroup_t* K = &myKeyGroup;
    
  NewKeyGroup(K, DIGITAL_KEYS); // Simple digital keyboards
  // initialize all pins (these are the package pins, could be direct keys, could be not)
  AddKeyPin(K, NewIO_Pin(&UpSwitch,PG11));
  AddKeyPin(K, NewIO_Pin(&DownSwitch,PB8));            
  AddKeyPin(K, NewIO_Pin(&LeftSwitch,PE1));
  AddKeyPin(K, NewIO_Pin(&RightSwitch,PG10));
  AddKeyPin(K, NewIO_Pin(&SelectSwitch,PE0));    
  SetKeyGroup_Timings(&myKeyGroup); // clock related
  ConfigureKeyGroup(&myKeyGroup);
  EnableKeyGroup(&myKeyGroup);

  // we are going to simulate a simple keyboard scan first, without the sequencer.
  while(1) {
    Wait_ms(50);
    KeysProcessInformation(K);
  }

  return 0;
}
