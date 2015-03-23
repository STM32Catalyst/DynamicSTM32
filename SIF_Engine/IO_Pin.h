
#ifndef _IO_PIN_H_
#define _IO_PIN_H_

extern GPIO_TypeDef* GPIOs[];

#define Check_Pin(Name) if(Name>=MAX_PACKAGE_PIN) while(1);

#if 1
#define ClearAndSetShiftedBits(reg, clr, set, shift) { (reg) &= ~((clr)<<(shift)); (reg) |= ((set)<<(shift));}
#else
extern u32 before,middle,after;
#define ClearAndSetShiftedBits(reg, clr, set, shift) {before=(reg);middle = (reg) &= ~((clr)<<(shift)); (reg) |= ((set)<<(shift));after = (reg);}
#endif


typedef enum
{
GPIO_AF16_DIGITAL_INPUT = 0x10,
GPIO_AF17_DIGITAL_OUTPUT = 0x11,
GPIO_AF18_ANALOG = 0x12,
GPIO_AF19_EXTI = 0x13,
GPIO_AF20_DISABLED = 0x14
}Extra_AF_Typedef;

// this could be in ROM but would take access time.
// better keep this in RAM for performance, and keep option to power down the flash.
// Function pointers are challenging for linkers...
typedef struct {
  PinNameDef   Name; // with the pin name we can precook the bitband addresses below
  GPIO_TypeDef* GPIOx;
  u32    BitMask;
  u8    AF; // the chosen alternate function (difficult to manage right now)
} IO_Pin_t; // 40 bytes per fast GPIO in Fast access should use RAM.

typedef struct {
  
  u8 PinName;
  u32 PPP_Base;
  SignalName_t SignalName;
  u8 AF;
  char* SignalNameString;
  
} PinAlternateDescription_t;

PinAlternateDescription_t* GetSignalDescription(PinNameDef PinName, u32 PPP_Adr);

IO_Pin_t* NewIO_Pin(IO_Pin_t* Pin, PinNameDef Name);

void IO_PinClockEnable(IO_Pin_t* Pin);
void IO_PinSetHigh(IO_Pin_t* Pin);
void IO_PinSetLow(IO_Pin_t* Pin);
void IO_PinSet(IO_Pin_t* Pin, u32 value);
s32 IO_PinGet(IO_Pin_t* Pin);
void IO_PinToggle(IO_Pin_t* Pin);
void IO_PinSetSpeedMHz(IO_Pin_t* Pin, u32 MHz);
void IO_PinEnablePullUp(IO_Pin_t* Pin, FunctionalState Enable);
void IO_PinEnablePullDown(IO_Pin_t* Pin, FunctionalState Enable);
void IO_PinEnablePullUpDown(IO_Pin_t* Pin, FunctionalState UpEnable, FunctionalState DownEnable);
void IO_PinEnableHighDrive(IO_Pin_t* Pin, FunctionalState Enable);
void IO_PinConfiguredAs(IO_Pin_t* Pin, u32 AF);

u32 IO_PinGetPR(IO_Pin_t* Pin);
void IO_PinClearPR(IO_Pin_t* Pin);

#define IO_PinSetInput(p) IO_PinConfiguredAs((p), GPIO_AF16_DIGITAL_INPUT)
#define IO_PinSetOutput(p) IO_PinConfiguredAs((p), GPIO_AF17_DIGITAL_OUTPUT)
#define IO_PinSetAnalog(p) IO_PinConfiguredAs((p), GPIO_AF18_ANALOG)
#define IO_PinSetAF(p) IO_PinConfiguredAs((p), (p->AF))
#define IO_PinRead(p) IO_PinGet((p))


// helper function to deal with signals and alternate functions on pins
u32 GetPinAF(PinNameDef PinName, u32 PPP_Adr);

//==========================================
// Sequencer compatible functions
u32 sq_PinSetLowJob(u32 u);
u32 sq_PinSetHighJob(u32 u);

//=============================================================================
// Resource allocation, conflict detection
// These functions should be embedded in the clock enable or creation of entities
// If all pins of a GPIO are freed, the clock enable can be turned off for power saving
// Eventually the clock enable function will be replaced by this.
void BookPin(u32 PinName);
u32 IsPinLocked(u32 PinName);
void FreePin(u32 PinName);

// helper functions (for code size and rationalization)
u32 ConfigurePinAsAnalog(IO_Pin_t* P);
u32 ConfigurePinAsInputTrigger(IO_Pin_t* P);
u32 ConfigurePinAsPushPullOutputPD(IO_Pin_t* P);
u32 ConfigurePinAsPushPullOutputPU(IO_Pin_t* P);
u32 ConfigurePinAsPushPullOutput(IO_Pin_t* P);
u32 ConfigurePinAsOpenDrainPU(IO_Pin_t* P); // this pull up is very weak. it does not replace an external pull up for I2C bus-like interfaces

// These functions for now are global, they are unique resources.


#endif
