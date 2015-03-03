
#ifndef _STUFF_ARTERY_H_
#define _STUFF_ARTERY_H_

typedef struct {
  // by playing with head and tail, any emulation is possible like FIFO, STACK and any exotic things including duplicates, like DNA Strands
//- zero  u16 pbLowest; // points to assigned SRAM start address (telomere)
  u16 pbHighest;  // pointer to assigned SRAM end address (inclusive) (telomere)
  u16 bCountLimit; // This is the SRAM buffer size
  u16 bCount; // This is the used SRAM buffer size
  u16 bCountMax; // This is for stats, to know how much we use the memory in real case over time
  u16 pbDown; // current tail (low address)
  u16 pbUp; // current head (high address)

  u32 In; // the item (pointer to it) to glue
  u32 Out; // the item (pointer to it) that was clipped
  
  u32* Table; // points to an array of 32 bit.... which can be pointers...
  
//TBD  u32 (*fnOut)(u32); // This hook is called when an item is added
//TBD  u32 ctOut;

//TBD  u32 (*fnIn)(u32); // This hook is called when an item is removed
//TBD  u32 ctIn;
// these below are for higher end use, later on...
  u32 (*fnNoLongerEmpty)(u32);
  u32 ctNoLongerEmpty;
  u32 (*fnEmptied)(u32);
  u32 ctEmptied;
  
  u8 FlagNoLongerEmpty : 1; // unused
  u8 FlagEmptied : 1; // unused
//  u8 FlagFull : 1; // unused
//  u8 FlagNoLongerFull : 1; // unused
  
} StuffsArtery_t;


u32 NewSA(StuffsArtery_t* SA, u32 begin, s32 size);

u32 GetSA_Down(StuffsArtery_t* SA);
u32 GetSA_Up(StuffsArtery_t* SA);

u32 GlueSA_Down(StuffsArtery_t* SA);
u32 ClipSA_Down(StuffsArtery_t* SA);
u32 GlueSA_Up(StuffsArtery_t* SA);
u32 ClipSA_Up(StuffsArtery_t* SA);

u32 AddToSA(StuffsArtery_t* SA, u32 In);
// the big question is should we create an array of pointer+size, pointer hooks, etc...
// can we run a sequence with this?

#endif
