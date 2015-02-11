
#ifndef _BYTE_VEIN_H_
#define _BYTE_VEIN_H_

// this can be used for FIFO, STACK, etc... and can be used within interrupt or main loop, or between main loop and interrupts. It's a time queue, within a limited RAM segment, a data snake
// can grow up from its head, or from its tail, and like the game, the snake can get longer or shorter from either head or tail.
// Once it becomes empty, a hook can be triggered, or a flag set
// Once it is no longer empty, same phylosophy
// TODO: Full, nolongerfull management
// The bit version is called bitcapillar
// The pointer version is called StuffsArtery

typedef struct {
  // by playing with head and tail, any emulation is possible like FIFO, STACK and any exotic things including duplicates, like DNA Strands
  u32 pbLowest; // points to assigned SRAM start address (telomere)
  u32 pbHighest;  // pointer to assigned SRAM end address (inclusive) (telomere)
  u32 bCountLimit; // This is the SRAM buffer size
  u32 bCount; // This is the used SRAM buffer size
  u32 bCountMax; // This is for stats, to know how much we use the memory in real case over time
  u32 pbDown; // current tail (low address)
  u32 pbUp; // current head (high address)

  u32 In; // the byte to glue
  u32 Out; // the byte that was clipped
  
//TBD  u32 (*fnOut)(u32); // This hook is called when an item is added
//TBD  u32 ctOut;

//TBD  u32 (*fnIn)(u32); // This hook is called when an item is removed
//TBD  u32 ctIn;
// these below are for higher end use, later on...
  u32 (*fnNoLongerEmpty)(u32);
  u32 ctNoLongerEmpty;
  u32 (*fnEmptied)(u32);
  u32 ctEmptied;
  
//  u32 (*fnFull)(u32);
//  u32 ctFull;
//  u32 (*fnNoLongerFull)(u32);
//  u32 ctNoLongerFull;

  u8 FlagNoLongerEmpty : 1; // unused
  u8 FlagEmptied : 1; // unused
//  u8 FlagFull : 1; // unused
//  u8 FlagNoLongerFull : 1; // unused
  
} ByteVein;


u32 NewBV(ByteVein* BV, u32 begin, s32 size);
u32 HookBV_NoLongerEmpty(ByteVein* BV, u32 (*fn)(u32), u32 ct);
u32 HookBV_Emptied(ByteVein* BV, u32 (*fn)(u32), u32 ct);
u32 GlueBV_Down(ByteVein* BV);
u32 ClipBV_Down(ByteVein* BV);
u32 GlueBV_Up(ByteVein* BV);
u32 ClipBV_Up(ByteVein* BV);
//---------- to access the head and tail
/*
u32 SetBV_Down(ByteVein* BV);
u32 SetBV_Up(ByteVein* BV);
u32 GetBV_Down(ByteVein* BV);
u32 GetBV_Up(ByteVein* BV);
*/

u32 AddToBV(ByteVein* BV, u32 In);


#endif
