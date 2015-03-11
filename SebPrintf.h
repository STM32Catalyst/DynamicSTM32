
#ifndef _SEB_PRINTF_H_
#define _SEB_PRINTF_H_

// This is a printf function which should work with any serial interface or RAM strings. No more "multiple copies" of printf in the code.

typedef struct {
  u32 fnPutChar, ctPutChar;
} PrintfHk_t;

u32 u8_To_u32_BitFieldFormat(u8 byte);

u32 SebPrintf(PrintfHk_t* T, const char *str,...);

#endif
