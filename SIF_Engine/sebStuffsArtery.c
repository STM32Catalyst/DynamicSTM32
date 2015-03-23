
#include "sebEngine.h"



u32 HookSA_NoLongerEmpty(StuffsArtery_t* SA, u32 (*fn)(u32), u32 ct) {
  
  SA->fnNoLongerEmpty = fn;
  SA->ctNoLongerEmpty = ct;
  return 0;
}

u32 HookSA_Emptied(StuffsArtery_t* SA, u32 (*fn)(u32), u32 ct) {
  
  SA->fnEmptied = fn;
  SA->ctEmptied = ct;
  return 0;
}

//==========================================================
// StrandCreation
StuffsArtery_t* NewSA(StuffsArtery_t* SA, u32 begin, s32 size) { // size is the number of u32 in the buffer which starts in begin.

  if(size==0)
    while(1); // no memory for it?

  SA->Table = (u32*)begin; // memory location of the table
  //-SA->pbLowest = 0; // left is decreasing memory address with rollover circular buffer
  SA->pbHighest = (size - 1); // tight is increasing memory address with rollover circular buffer inclusive pointer
  
  { // valid SRAM space
    SA->bCountLimit = size; // we validate the strand size (action can occur)
    SA->bCount = 0;
    return SA;
  }
  
//  SA->bCount = SA->bCountLimit = 0; // no strand possible
//  return begin;
}


//=========================================================
u32 GetSA_Up(StuffsArtery_t* SA) {
  SA->Out = SA->Table[SA->pbUp]; // we create one more bit space on the left (lower memory)
  return SA->Out;
}

u32 GetSA_Down(StuffsArtery_t* SA) {
  SA->Out = SA->Table[SA->pbDown]; // we create one more bit space on the left (lower memory)
  return SA->Out;
}


//==========================================================
// manage the left side
u32 GlueSA_Down(StuffsArtery_t* SA) {

  if(SA->bCountLimit==0) while(1); // error
  
  if(SA->bCount==0) { // if strand empty: Create the first bit
    SA->pbDown = SA->pbUp = 0;//-SA->pbLowest; // arbitrary left is the start creation side
  }
  else {  // the strand exist, check fullness
    if(SA->bCount>=SA->bCountLimit)
      while(1); // too big! improve memory allocation, flag, roll over etc... depends on scheme

    // we create one more bit space on the left (lower memory)
    SA->pbDown--;
    if(SA->pbDown>SA->pbHighest) //rolloever from zero is positive overvflow! if(SA->pbDown<SA->pbLowest) // rollover if out of range
      SA->pbDown = SA->pbHighest;
  };
  
  // write in memory and increase bit counter safely
  SA->Table[SA->pbDown] = SA->In;
  SA->bCount++;
  if(SA->bCount>SA->bCountMax) SA->bCountMax = SA->bCount;// statistics
  
  if(SA->bCount==1)
    if(SA->fnNoLongerEmpty) {// if the strand not empty, tell someone?
    return SA->fnNoLongerEmpty(SA->ctNoLongerEmpty);
  }else{
    SA->FlagNoLongerEmpty = 1;
  };
  
  return 0;
}

u32 ClipSA_Down(StuffsArtery_t* SA) { // 0 or 1, returns the bit pointer

//  if(bCountLimit==0) while(1); // error
  
  if(SA->bCount==0) // if strand empty, nothing to read from it, error
    while(1); // error, nothing on this strand, check its size is non zero first!

  SA->bCount--;  // strand not empty. Get the left bit first, reduce strand size
  SA->Out = SA->Table[SA->pbDown]; // we create one more bit space on the left (lower memory)
  
  // point to the new left side
  SA->pbDown++;
  if(SA->pbDown>SA->pbHighest) // circular memory space bottom reached.
    SA->pbDown = 0;//-SA->pbLowest; // jump to higher end
  
  // all write done, now doing actions and pending flags
  if(SA->bCount==0)
    if(SA->fnEmptied) {// if the strand turns empty, tell someone?
      return SA->fnEmptied(SA->ctEmptied);
    }else{
      SA->FlagEmptied = 1;
    };

  return 0;// return read valid bit
}

//==========================================================
// manage the right side

u32 GlueSA_Up(StuffsArtery_t* SA) {

  if(SA->bCountLimit==0) while(1); // error
  
  if(SA->bCount==0) { // if strand empty: Create the first bit
    SA->pbUp = SA->pbDown = 0;//- SA->pbLowest; // arbitrary left is the start creation side
  }
  else {  // the strand exist, check fullness
    if(SA->bCount>=SA->bCountLimit)
      while(1); // too big! improve memory allocation, flag, roll over etc... depends on scheme

    // we create one more bit space on the left (lower memory)
    SA->pbUp++;
    if(SA->pbUp>SA->pbHighest) // rollover if out of range
      SA->pbUp = 0;//-SA->pbLowest;
  };
  
  // write in memory and increase bit counter safely
  SA->Table[SA->pbUp] = SA->In;
  SA->bCount++;
  if(SA->bCount>SA->bCountMax) SA->bCountMax = SA->bCount;
  
  if(SA->bCount==1)
      if(SA->fnNoLongerEmpty) {// if the strand not empty, tell someone?
      return SA->fnNoLongerEmpty(SA->ctNoLongerEmpty); //!!! this can activate interrupt IRQ...
    }else{
      SA->FlagNoLongerEmpty = 1;
    };

  return 0;
}

u32 ClipSA_Up(StuffsArtery_t* SA) { // 0 or 1

//  if(SA->bCountLimit==0) while(1); // error
  
  if(SA->bCount==0) // if strand empty, nothing to read from it, error
    while(1); // error, nothing on this strand, check its size is non zero first!

  SA->bCount--;  // strand not empty. Get the left bit first, reduce strand size
  SA->Out = SA->Table[SA->pbUp]; // we create one more bit space on the left (lower memory)
  
  // point to the new left side
  SA->pbUp--;
  if(SA->pbUp>SA->pbHighest)//- -underflow if(SA->pbUp<SA->pbLowest) // circular memory space bottom reached.
    SA->pbUp = SA->pbHighest; // jump to higher end

  if(SA->bCount==0)
    if(SA->fnEmptied) {// if the strand turns empty, tell someone?
      return SA->fnEmptied(SA->ctEmptied);
    }else{
      SA->FlagEmptied = 1;
    };

  return 0;// return read valid bit
}


//============================================
// Direct FIFO management

u32 AddToSA(StuffsArtery_t* SA, u32 In) {
  
  SA->In = In;
  return GlueSA_Up(SA);
}


