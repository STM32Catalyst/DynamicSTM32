
#ifndef _IO_PIN_H_
#define _IO_PIN_H_

typedef enum
{
  PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
  PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
  PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,
  PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,
  PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,
  PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,
  PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,
  PH0,PH1,PH2,PH3,PH4,PH5,PH6,PH7,PH8,PH9,PH10,PH11,PH12,PH13,PH14,PH15,
  PI0,PI1,PI2,PI3,PI4,PI5,PI6,PI7,PI8,PI9,PI10,PI11,PI12,PI13,PI14,PI15,
  PJ0,PJ1,PJ2,PJ3,PJ4,PJ5,PJ6,PJ7,PJ8,PJ9,PJ10,PJ11,PJ12,PJ13,PJ14,PJ15,
  PK0,PK1,PK2,PK3,PK4,PK5,PK6,PK7, 
  MAX_PACKAGE_PIN
}PinNameDef;

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
} IO_PinTypeDef; // 40 bytes per fast GPIO in Fast access should use RAM.


void IO_PinInit(IO_PinTypeDef* Pin, PinNameDef Name);

void IO_PinClockEnable(IO_PinTypeDef* Pin);
void IO_PinSetHigh(IO_PinTypeDef* Pin);
void IO_PinSetLow(IO_PinTypeDef* Pin);
void IO_PinSet(IO_PinTypeDef* Pin, u32 value);
s32 IO_PinGet(IO_PinTypeDef* Pin);
void IO_PinToggle(IO_PinTypeDef* Pin);
void IO_PinSetSpeedMHz(IO_PinTypeDef* Pin, u32 MHz);
void IO_PinEnablePullUp(IO_PinTypeDef* Pin, FunctionalState Enable);
void IO_PinEnablePullDown(IO_PinTypeDef* Pin, FunctionalState Enable);
void IO_PinEnableHighDrive(IO_PinTypeDef* Pin, FunctionalState Enable);
void IO_PinConfiguredAs(IO_PinTypeDef* Pin, u32 signal);

u32 IO_PinGetPR(IO_PinTypeDef* Pin);
void IO_PinClearPR(IO_PinTypeDef* Pin);

#define IO_PinSetInput(p) IO_PinConfiguredAs((p), GPIO_AF16_DIGITAL_INPUT)
#define IO_PinSetOutput(p) IO_PinConfiguredAs((p), GPIO_AF17_DIGITAL_OUTPUT)
#define IO_PinSetAnalog(p) IO_PinConfiguredAs((p), GPIO_AF18_ANALOG)
#define IO_PinSetAF(p) IO_PinConfiguredAs((p), (p->AF))
#define IO_PinRead(p) IO_PinGet((p))


// helper function to deal with signals and alternate functions on pins
u32 GetPinAF(PinNameDef PinName, u32 PPP_Adr);

#endif
