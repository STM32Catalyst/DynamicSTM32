
#include "upgrade.h"
#if 1 //def SIF_ENGINE
#include "fast_io.h"
#include "bit_capilar.h"

// I don't know how to make the bit stream friendly, generic, simple and fast.
// too many checks for a single bit write.
// that's why we arbitrary talk 1,4,8,16,24,32 bits... or even more!
// bit stream engines

u32 pbLowest; // inclusive
u32 pbHighest;  // inclusive
u32 bMax; // this exist for the rollover
u32 bIn; // this exist for the rollover
u32 pbDown;
u32 pbUp;
//u32 pbSweep;

void (*OnStrandNoLongerEmpty)(void);
void (*OnStrandEmptied)(void);

//BITBAND_SRAM((u32)pu8,0);

void WhenStrandNoLongerEmpty( void (*fn)(void)) {
  
  OnStrandNoLongerEmpty = fn;
}

void WhenStrandEmptied( void (*fn)(void)) {
  
  OnStrandEmptied = fn;
}


u32 IsStrandEmpty(void) {
  
  return (bIn==0);
}

u32 IsStrandNotEmpty(void) {
  
  return (bIn!=0);
}

//==========================================================
// StrandCreation
u32 CreateEmptyStrand(u32 begin, s32 bitsize) {

  if(bitsize==0) {
      while(1); // no memory for it?
//      return NULL;
  }

  pbLowest = begin; // left is decreasing memory address with rollover circular buffer
  pbHighest = begin + (bitsize - 1)*4; // tight is increasing memory address with rollover circular buffer inclusive pointer
  
  if(           (((u32)pbLowest)>=BITBAND_SRAM_REF)
     &&         (((u32)pbHighest)<=BITBAND_SRAM(0x2004FFFF,7)) 
                 )
  { // valid SRAM space
    bMax = bitsize; // we validate the strand size (action can occur)
    bIn = 0;
    return begin;
  }
  
  bIn = bMax = 0; // no strand possible
  return begin;
}

u32 CreateFullStrand(u32 begin, s32 bitsize) { // the strand won't be empty size, it will be full size

  if(bitsize==0) {
      while(1); // no memory for it?
//      return NULL;
  }

  pbDown = pbLowest = begin; // left is decreasing memory address with rollover circular buffer
  pbUp = pbHighest = begin + (bitsize - 1)*4; // tight is increasing memory address with rollover circular buffer inclusive pointer
  
  if(           (((u32)pbLowest)>=BITBAND_SRAM_REF)
     &&         (((u32)pbHighest)<=BITBAND_SRAM(0x2004FFFF,7)) 
                 )
  { // valid SRAM space
    bIn = bMax = bitsize; // we validate the strand size (action can occur)
    return begin;
  }
  
  bIn = bMax = 0;
  return begin;
}

//==========================================================
// manage the left side
u32 GlueBitDown(s32 bit) {
  
  if(bMax==0) while(1); // error
  
  if(bIn==0) { // if strand empty: Create the first bit
    pbDown = pbUp = pbLowest; // arbitrary left is the start creation side
  }
  else {  // the strand exist, check fullness
    if(bIn>=bMax) {
      while(1); // too big! improve memory allocation, flag, roll over etc... depends on scheme
    };

    // we create one more bit space on the left (lower memory)
    pbDown-=4;
    if(pbDown<pbLowest) // rollover if out of range
      pbDown = pbHighest;
  };
  
  // write in memory and increase bit counter safely
  *(u8*)pbDown = bit;
  bIn++;
  
  if((bIn==1)&&(OnStrandNoLongerEmpty)) // if the strand not empty, tell someone?
    OnStrandNoLongerEmpty();
  
  return pbDown;
}

u32 ClipBitDown(void) { // 0 or 1, returns the bit pointer
  
//  if(bMax==0) while(1); // error
  
  if(bIn==0) { // if strand empty, nothing to read from it, error
    while(1); // error, nothing on this strand, check its size is non zero first!
  }

  bIn--;  // strand not empty. Get the left bit first, reduce strand size
  u32 bit = *(u8*) pbDown; // we create one more bit space on the left (lower memory)
//  u32 pBit = pbDown; // we create one more bit space on the left (lower memory)
  
  // point to the new left side
  pbDown+=4;
  if(pbDown>pbHighest) // circular memory space bottom reached.
    pbDown = pbLowest; // jump to higher end
  
  if((bIn==0)&&(OnStrandEmptied)) // if the strand turns empty, tell someone?
    OnStrandEmptied();
  
  return bit;// return read valid bit
}

//==========================================================
// manage the right side
u32 GlueBitUp(u32 bit) {
  
  if(bMax==0) while(1); // error
  
  if(bIn==0) { // if strand empty: Create the first bit
    pbUp = pbDown = pbLowest; // arbitrary left is the start creation side
  }
  else {  // the strand exist, check fullness
    if(bIn>=bMax) {
      while(1); // too big! improve memory allocation, flag, roll over etc... depends on scheme
    };

    // we create one more bit space on the left (lower memory)
    pbUp+=4;
    if(pbUp>pbHighest) // rollover if out of range
      pbUp = pbLowest;
  };
  
  // write in memory and increase bit counter safely
  *(u8*)pbUp = bit;
  bIn++;
  
  if((bIn==1)&&(OnStrandNoLongerEmpty)) // if the strand not empty, tell someone?
    OnStrandNoLongerEmpty();

  return pbUp;
}

u32 ClipBitUp(void) { // 0 or 1
  
//  if(bMax==0) while(1); // error
  
  if(bIn==0) { // if strand empty, nothing to read from it, error
    while(1); // error, nothing on this strand, check its size is non zero first!
  }

  bIn--;  // strand not empty. Get the left bit first, reduce strand size
  s32 bit = *(u8*) pbUp; // we create one more bit space on the left (lower memory)
  //s32 pBit = pbUp; // we create one more bit space on the left (lower memory)
  
  // point to the new left side
  pbUp-=4;
  if(pbUp<pbLowest) // circular memory space bottom reached.
    pbUp = pbHighest; // jump to higher end

  if((bIn==0)&&(OnStrandEmptied)) // if the strand turns empty, tell someone?
    OnStrandEmptied();
  
  return bit;// return read valid bit
}

//======================
//  if(           (((u32)pu8)>=BITBAND_PERI_REF)
//     &&         (((u32)pu8)<=BITBAND_PERI(0x4007FFFF,7)) 
//======================

// reading the strand content: The strand does not kill the bits when cut!

u32 SDA_BS[1];
u32 SCL_BS[1];


void StrandTest(void) {

  CreateEmptyStrand(BITBAND_SRAM((u32)SDA_BS,0), sizeof(SDA_BS)*8);
//  CreateEmptyStrand(BITBAND_SRAM((u32)SCL_BS,0), sizeof(SCL_BS)*8);
  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);
  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);
  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);
  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);
  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();
  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();
  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();
  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();
  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);
  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);
  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);
  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);  GlueBitUp(1);
  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();
  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();
  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();
  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();  ClipBitDown();

  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);
  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);
  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);
  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);  GlueBitDown(1);
  ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();
  ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();
  ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();
  ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();ClipBitUp();
  while(1);
}



#endif
