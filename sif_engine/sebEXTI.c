
#include "sebEngine.h"

// An interrupt cell is naturally hooked to some interrupt handlers and have a specific type
// this cell is passive (not affecting other cells)

#define NVIC_ReleasePendingIRQ(IRQn) NVIC->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)) // Clear pending interrupt
#define EXTI_Counts 16
IRQn_Type chToIRQn[EXTI_Counts] = {
EXTI0_IRQn, // 0
EXTI1_IRQn, // 1
EXTI2_IRQn, // 2
EXTI3_IRQn, // 3
EXTI4_IRQn, // 4
EXTI9_5_IRQn, // 5
EXTI9_5_IRQn, // 6
EXTI9_5_IRQn, // 7
EXTI9_5_IRQn, // 8
EXTI9_5_IRQn, // 9
EXTI15_10_IRQn, // 10
EXTI15_10_IRQn, // 11
EXTI15_10_IRQn, // 12
EXTI15_10_IRQn, // 13
EXTI15_10_IRQn, // 14
EXTI15_10_IRQn, // 15
};

// EXTI Interrupt enable is 16 channels (1 channel going to one GPIO port)
// The NVIC is less channels, some are grouped together and share the same interrupt line 0,1,2,3,4,5..9,10..15
// n is a EXTI channel 0..15
// all EXTI functions pointers which jump straight to void or handler if interrupt is enabled
//u32   (*fnEXTI_0)(u32); // fnReturn fnFault

void EXTI_SetIRQ(u32 n, FunctionalState Enable) {

  ClearAndSetShiftedBits(EXTI->IMR, 1, (Enable==DISABLE)?0:1, n);
}

u32 EXTI_Interrupt(u32 n, FunctionalState Enable) {
  
  EXTI->PR = 1<<n; // clear the pending bit
  EXTI_SetIRQ(n, Enable); // at least one hook in place, make sure interrupt is enabled  
  return Enable; // if enable, will cause an interrupt later
}

void HookEXTIn(u32 n, u32 fn, u32 ct) { // if adr = 0, unhook a function from the EXIT handler. if same, will just clear the pending flags and IRQ(?) n=0..15

  u32   oldfn;

  // if 0: Disable the feature
  // if same: Clear the pending
  // if new, Enable the feature
  if(n>countof(fnEXTIs)) while(1); // index too big, this EXTI does not exist

  oldfn = fnEXTIs[n];

  if(oldfn && fn) while(1); // can't hook a function if it is already hooked to another one already! Unhook first with "0"

  EXTI_Interrupt(n, DISABLE); // we disable any interrupt before manipulating the hook
  EXTI->PR = 1<<n; // clear the pending bit

  if(oldfn==fn)
    return; // no change

  fnEXTIs[n] = fn;//0 here
  ctEXTIs[n] = ct;

}


void EXTI_SetEdgesEnable(u32 PinName, u8 RisingEnabled, u8 FallingEnabled) {

  u32 n = PinName & 0xF; // the pin index 0..15
  u32 p = (PinName >> 4) & 0xF; // the port name index 0..15 (ABCDEFGHIJKL...)
  
  BookEXTI(PinName);
  // configure the right GPIO (we should have a check if is was already assigned... for this we need RAM flags...
  ClearAndSetShiftedBits(SYSCFG->EXTICR[n>>2], 15, p, ((n%4)<<2));  // 32 bit = 8 pin x 4 bit field: Which GPIOx for this EXTI channel?

  EXTI->PR = 1<<n; // clear the EXTI channel pending bit (0..15) // may not be needed
  ClearAndSetShiftedBits(EXTI->RTSR, 1, ((RisingEnabled)?1:0), n);
  ClearAndSetShiftedBits(EXTI->FTSR, 1, ((FallingEnabled)?1:0), n);
  EXTI->PR = 1<<n; // clear the EXTI channel pending bit (0..15) // should we do this disabling interrupts?
}


void NVIC_EXTIsEnable(FunctionalState Enable) {
  // all the interrupt handlers exist and clear flags by default
  // call this only when final activation of the EXTI interrupt generation mecanism
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = Enable;
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
NVIC_ReleasePendingIRQ(NVIC_InitStructure.NVIC_IRQChannel);  
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
NVIC_ReleasePendingIRQ(NVIC_InitStructure.NVIC_IRQChannel);  
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
NVIC_ReleasePendingIRQ(NVIC_InitStructure.NVIC_IRQChannel);  
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
NVIC_ReleasePendingIRQ(NVIC_InitStructure.NVIC_IRQChannel);  
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
NVIC_ReleasePendingIRQ(NVIC_InitStructure.NVIC_IRQChannel);  
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
NVIC_ReleasePendingIRQ(NVIC_InitStructure.NVIC_IRQChannel);  
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
NVIC_ReleasePendingIRQ(NVIC_InitStructure.NVIC_IRQChannel);  
  NVIC_Init(&NVIC_InitStructure);
}

/*
void EXTI_SetTrigger(u32 n, FunctionalState Enable) {

  ClearAndSetShiftedBits(EXTI->EMR, 1, (Enable==DISABLE)?0:1, n);
}
*/

/*
u32 EXTI_IsIRQHooked(u32 n) {

  u32 IRQ = chToIRQn[n];
  u8 i;
  // several channels can share the same IRQ... let's check if all of them are not hooked (0)
  for(i=0;i<15;i++) { // sweep all 16 channels

      if(       (IRQ==chToIRQn[i]) // if it is the right IRQ...
        &&      (fnEXTIs[i]!=0) ) return TRUE; // ... and it is hooked, then stop right there, it's hooked!
  }

  return FALSE;
}
*/
/*
u32 EXTI_IsAnyIRQHooked(void) {

  u8 i;
  // several channels can share the same IRQ... let's check if all of them are not hooked (0)
  for(i=0;i<15;i++) { // sweep all 16 channels

      if(       (1) // if it is the right IRQ...
        &&      (fnEXTIs[i]!=0) ) return TRUE; // ... and it is hooked, then stop right there, it's hooked!
  }

  return FALSE;
}
*/

u32 EXTI_PinShareSameNVIC(u32 PinName1, u32 PinName2) {

  return (chToIRQn[PinName1&0xF]==chToIRQn[PinName2&0xF]);
}

//========================--------------------->

//==============-------------> Sequencer compatible functions

u32 sq_EXTI_Interrupt(u32 u) { // (n, fn, ct); n can be the pin name this is only for the GPIOs. For non GPIOs, a separate function should be created.
  u32* p = (u32*) u;
  IO_Pin_t* Pin = (IO_Pin_t*)p[0];
  EXTI_Interrupt(Pin->Name & 0xF, (FunctionalState) p[1]);
  return p[1]; // if 0, continue, if 1 an interrupt will come, so return to the top.
}


//===============================================
// Resource overbooking sanity check

u8 BookedEXTI[16];

void BookEXTI(u32 PinName) {
  
  if(PinName>MAX_PACKAGE_PIN)
    while(1); // not possible

  if(BookedEXTI[PinName % 16]!=0)
    while(1); // this EXTI is already used
  
  BookedEXTI[PinName % 16] = PinName>>4; // which GPIO is used for this EXTI
}

void FreeEXTI(u32 PinName) {
  
  if(PinName>MAX_PACKAGE_PIN)
    while(1); // not possible

  BookedEXTI[PinName % 16] = 0; // which GPIO is used for this EXTI  
}



