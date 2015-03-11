
#include "SebEngine.h"

Buttons_t myKeys;
IO_Pin_t UpKey,DownKey,LeftKey,RightKey,SelectKey,BootKey;
                
u32 ButtonsTest(void) {
    
  Buttons_t* K = &myKeys;

  // initialize all pins
  K->Button[0].Key = IO_PinInit(&UpKey,PG11);
  K->Button[1].Key = IO_PinInit(&DownKey,PB8);            
  K->Button[2].Key = IO_PinInit(&LeftKey,PE1);
  K->Button[3].Key = IO_PinInit(&RightKey,PG10);
  K->Button[4].Key = IO_PinInit(&SelectKey,PE0);    
  InitButtons(K);

  // we are going to simulate a simple keyboard scan first, without the sequencer.
  while(1) {
  
  Wait_ms(50);
  KeysProcessInformation(K);
  
  }

  return 0;
}
