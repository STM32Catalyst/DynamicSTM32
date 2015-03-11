
#ifndef _CELL_INT_H_
#define _CELL_INT_H_

void HookEXTIn(u32 n, u32 fn, u32 ct);

void EXTI_SetEdgesEnable(u32 PinName, u8 RisingEnabled, u8 FallingEnabled);

void EXTI_SetTrigger(u32 n, FunctionalState Enable); // protected
void EXTI_SetIRQ(u32 n, FunctionalState Enable); // protected
void EXTI_SetEvent(u32 n, FunctionalState Enable); // DAC, ADC use for example

u32 EXTI_IsAnyIRQHooked(void);
u32 EXTI_IsIRQHooked(u32 n);

void NVIC_EXTIsEnable(FunctionalState Enable);

u32 EXTI_PinShareSameNVIC(u32 PinName1, u32 PinName2);

u32 EXTI_Interrupt(u32 n, FunctionalState Enable);

u32 sq_EXTI_Interrupt(u32 u);

//=========================
void BookEXTI(u32 PinName);
void FreeEXTI(u32 PinName);

void BookAF(u32 PinName, u32 AF);
void FreeAF(u32 PinName);
#endif
