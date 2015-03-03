
#include "sebEngine.h"

// This is the sequencer... work in progress!

// The sequencer will run called by interrupt to increment the states (PC)


u32 JobToDo(u32 u) { // this can be called by others OR by the DMA interrupt of this SPI
  
  u32 CallbackArmed = 0;
  StuffsArtery_t* SA = (StuffsArtery_t*) u;
  
  do {
    ClipSA_Down(SA); // this will trigger empty fifo?
    OneJob_t* Job = (OneJob_t*)SA->Out;
    if(Job->fnJob) {
      CallbackArmed = Job->fnJob((u32)Job->ctJobs);//, Job->Param1, Job->Param2, Job->Param3);
    }else{
      while(1); // error, no function to call; Did you initialize the SA before your periph_Init() function? Otherwise, it is still 00000 inside...
    };
    
    if(CallbackArmed) return CallbackArmed; // a task taking time is now done by HW, once finished, it will return back to this function (the interrupt handler will return to this function)
  }while(SA->bCount);
  
  return 0;
}


// For sequences not using interrups (bitbanging I2C/SPI), call directly JobToDo (no return)  
u32 StartJobToDoInBackground(u32 u) { // this can be called by others OR by the DMA interrupt of this SPI

  StuffsArtery_t* SA = (StuffsArtery_t*) u;
  
  if(u==0) while(1);
  if(SA->bCount==0) while(1);
  SA->FlagEmptied = 0;
  
  u32 Primask = __get_PRIMASK(); // get current primask (in the future, it should be to raise the priority to be the same as the IRQ handlers dealing with this sequence... to look like an interrupt
  
  __set_PRIMASK(Primask | 1); // if we don't do this, the function won't be over that an interrupt will also call it.... messing the bCount and the FIFO. Non blocking though, prevents useless stack depth
  JobToDo(u);
  __set_PRIMASK(Primask);
  
  return 0;
}
  
// For sequences not using interrups (bitbanging I2C/SPI), call directly JobToDo (no return)  
u32 StartJobToDoInForeground(u32 u) { // this can be called by others OR by the DMA interrupt of this SPI

  StuffsArtery_t* SA = (StuffsArtery_t*) u;

  if(u==0) while(1);
  if(SA->bCount==0) while(1);
  SA->FlagEmptied = 0;
  
  JobToDo(u);
  
  return 0;
}
