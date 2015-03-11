
#include "sebEngine.h"

// This is the mecanics for the state machine to work, let's perfect it so it becomes easier to manage

// this is the model function to be inserted in features code directly for speed reason.
u32 STMA_Run(u32 u) {

  STateMAchine* STMA = (STateMAchine*) u;// type it
  u32 result;

  // should the pre-post be inside the while or outside? Let's practise
//  if(STMA->prefn) ((u32(*)(u32))STMA->prefn)(STMA->ct); // call either an empty function or a hooked one with desired context predefined (if context missing... error)

    // run the current state piece of code
//    if(STMA->State >= STMA->StatesCount) while(1); // out of range
    if(STMA->fnStates[STMA->State]) result = ((u32(*)(u32))STMA->fnStates[STMA->State])(STMA->ct); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);// error, the state machine is going nowhere

    // identify the next state based on the result field
//    STMA->State = STMA->St2St[result & 0x1F]; // maximum 32 states, the other 2 bits are for state machine 4 type of behaviour
//    STMA->State &= 0x1F;
    STMA->State = result;

//  if(STMA->postfn) ((u32(*)(u32))STMA->postfn)(STMA->ct); // call either an empty function or a hooked one with desired context predefined (if context missing... error)

  return u;
}




//============================================== (this is the pre, there is no post yet)
