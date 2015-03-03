
#include "sebEngine.h"


u32 HookBV_NoLongerEmpty(ByteVein_t* BV, u32 (*fn)(u32), u32 ct) {
  
  BV->fnNoLongerEmpty = fn;
  BV->ctNoLongerEmpty = ct;
  return 0;
}

u32 HookBV_Emptied(ByteVein_t* BV, u32 (*fn)(u32), u32 ct) {
  
  BV->fnEmptied = fn;
  BV->ctEmptied = ct;  
  return 0;
}

//==========================================================
// StrandCreation
u32 NewBV(ByteVein_t* BV, u32 begin, s32 size) {

  if(size==0)
    while(1); // no memory for it?

  BV->pbLowest = begin; // left is decreasing memory address with rollover circular buffer
  BV->pbHighest = begin + (size - 1); // tight is increasing memory address with rollover circular buffer inclusive pointer
  
//  if(           (((u32)BV->pbLowest)>=0x00000000) // memory range check
//     &&         (((u32)BV->pbHighest)<=0xFFFFFFFF) 
//                 )
  { // valid SRAM space
    BV->bCountLimit = size; // we validate the strand size (action can occur)
    BV->bCount = 0;
    return begin;
  }
  
//  BV->bCount = BV->bCountLimit = 0; // no strand possible
//  return begin;
}

//==========================================================
// manage the left side
u32 GlueBV_Down(ByteVein_t* BV) {

  if(BV->bCountLimit==0) while(1); // error
  
  if(BV->bCount==0) { // if strand empty: Create the first bit
    BV->pbDown = BV->pbUp = BV->pbLowest; // arbitrary left is the start creation side
  }
  else {  // the strand exist, check fullness
    if(BV->bCount>=BV->bCountLimit)
      while(1); // too big! improve memory allocation, flag, roll over etc... depends on scheme

    // we create one more bit space on the left (lower memory)
    BV->pbDown--;
    if(BV->pbDown<BV->pbLowest) // rollover if out of range
      BV->pbDown = BV->pbHighest;
  };
  
  // write in memory and increase bit counter safely
  *(u8*)BV->pbDown = BV->In;
  BV->bCount++;
  if(BV->bCount>BV->bCountMax) BV->bCountMax = BV->bCount;// statistics
  
  if(BV->bCount==1)
    if(BV->fnNoLongerEmpty) {// if the strand not empty, tell someone?
      BV->fnNoLongerEmpty(BV->ctNoLongerEmpty);
    }else{
      BV->FlagNoLongerEmpty = 1;
    };

// one item was added, check if someone is ready to empty it
//TBD  if(BV->fnOut) ((u32(*)(u32))BV->fnOut)(BV->ctOut);
  
  return BV->pbDown;
}

u32 ClipBV_Down(ByteVein_t* BV) { // 0 or 1, returns the bit pointer

//  if(bCountLimit==0) while(1); // error
  
  if(BV->bCount==0) // if strand empty, nothing to read from it, error
    while(1); // error, nothing on this strand, check its size is non zero first!

  BV->bCount--;  // strand not empty. Get the left bit first, reduce strand size
  BV->Out = *(u8*) BV->pbDown; // we create one more bit space on the left (lower memory)
  
  // point to the new left side
  BV->pbDown++;
  if(BV->pbDown>BV->pbHighest) // circular memory space bottom reached.
    BV->pbDown = BV->pbLowest; // jump to higher end
  
  if(BV->bCount==0)
    if(BV->fnEmptied) {// if the strand turns empty, tell someone?
      BV->fnEmptied(BV->ctEmptied);
    }else{
      BV->FlagEmptied = 1;
    };

  // one item was added, check if someone is ready to empty it
//TBD  if(BV->fnIn) ((u32(*)(u32))BV->fnIn)(BV->ctIn);
  
  return BV->Out;// return read valid bit
}

//==========================================================
// manage the right side
u32 GlueBV_Up(ByteVein_t* BV) {

  if(BV->bCountLimit==0) while(1); // error
  
  if(BV->bCount==0) { // if strand empty: Create the first bit
    BV->pbUp = BV->pbDown = BV->pbLowest; // arbitrary left is the start creation side
  }
  else {  // the strand exist, check fullness
    if(BV->bCount>=BV->bCountLimit)
      while(1); // too big! improve memory allocation, flag, roll over etc... depends on scheme

    // we create one more bit space on the left (lower memory)
    BV->pbUp++;
    if(BV->pbUp>BV->pbHighest) // rollover if out of range
      BV->pbUp = BV->pbLowest;
  };
  
  // write in memory and increase bit counter safely
  *(u8*)BV->pbUp = BV->In;
  BV->bCount++;
  if(BV->bCount>BV->bCountMax) BV->bCountMax = BV->bCount;
  
  if(BV->bCount==1)
    if(BV->fnNoLongerEmpty) { // if the strand not empty, tell someone?
      BV->fnNoLongerEmpty(BV->ctNoLongerEmpty); //!!! this can activate interrupt IRQ...
    }else{
      BV->FlagNoLongerEmpty = 1;
    };

  // one item was added, check if someone is ready to empty it
//TBD  if(BV->fnOut) ((u32(*)(u32))BV->fnOut)(BV->ctOut);

  return BV->pbUp;
}

u32 ClipBV_Up(ByteVein_t* BV) { // 0 or 1

//  if(BV->bCountLimit==0) while(1); // error
  
  if(BV->bCount==0) // if strand empty, nothing to read from it, error
    while(1); // error, nothing on this strand, check its size is non zero first!

  BV->bCount--;  // strand not empty. Get the left bit first, reduce strand size
  BV->Out = *(u8*) BV->pbUp; // we create one more bit space on the left (lower memory)
  
  // point to the new left side
  BV->pbUp--;
  if(BV->pbUp<BV->pbLowest) // circular memory space bottom reached.
    BV->pbUp = BV->pbHighest; // jump to higher end

  if(BV->bCount==0)
    if(BV->fnEmptied) {// if the strand turns empty, tell someone?
      BV->fnEmptied(BV->ctEmptied);
    }else{
      BV->FlagEmptied = 1;
    };

  // one item was added, check if someone is ready to empty it
//TBD  if(BV->fnIn) ((u32(*)(u32))BV->fnIn)(BV->ctIn);
  
  return BV->Out;// return read valid bit
}
/*
u32 SetBV_Down(ByteVein* BV) {
  *(u8*)BV->pbDown = BV->In; // we write Down (tail)
  return BV->In;
}

u32 SetBV_Up(ByteVein* BV) {
  *(u8*)BV->pbUp = BV->In;  
  return BV->In;
}

u32 GetBV_Down(ByteVein* BV) {
  BV->Out = *(u8*)BV->pbDown ; // we write Down (tail)
  return BV->Out;
}

u32 GetBV_Up(ByteVein* BV) {

  BV->Out = *(u8*)BV->pbUp ; // we write Down (tail)
  return BV->Out;
}
*/

u32 AddToBV(ByteVein_t* BV, u32 In) {
  
  BV->In = In;
  return GlueBV_Up(BV);
}


// reading the strand content: The strand does not kill the bits when cut!
#ifdef ADD_EXAMPLES_TO_PROJECT
static ByteVein_t BV; // for test program
static u8 BVR[512]; // the assigned buffer for this byte strand


void BV_Test(void) {
  
  u32 n;
  NewBV(&BV, (u32)BVR, (s32)sizeof(BVR));
    
  for(n=0;n<32;n++)
    AddToBV(&BV, n); 

  for(n=0;n<32;n++)
    ClipBV_Down(&BV);
  
  for(n=0;n<32;n++)
    AddToBV(&BV, 100 + n);
  
  for(n=0;n<32;n++) 
    ClipBV_Down(&BV);

  for(n=0;n<32;n++) {
    BV.In = 200 + n; GlueBV_Down(&BV);
  };
  
  for(n=0;n<32;n++) 
    ClipBV_Up(&BV);
  
  while(1);
}
#endif

