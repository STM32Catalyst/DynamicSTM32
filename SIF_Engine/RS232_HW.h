
#ifndef _RS232_HW_H_
#define _RS232_HW_H_

typedef struct {
  
  USART_TypeDef* USART; // The corresponding HW peripheral
  
  USART_InitTypeDef UI; // the USART configuration
  // DMAStream* xxx
  ByteVein_t* BV_TX; // data flowing to TX pin of MCU
  ByteVein_t* BV_RX; // data flowing to RX pin of MCU
  
  IO_Pin_t* TX;
  IO_Pin_t* RX; // The receive pin (which can be woken up if EXTI compatible)
  
  IO_Pin_t* CTS; // if 0, not used
  IO_Pin_t* RTS; // if 0, not used
  
//  u32 (*fnOut)(u32); // What to do when ready to send (TX)
//  u32 ctOut;

//  u32 (*fnIn)(u32); // What to do when received data (RX)
//  u32 ctIn;
  
  PrintfHk_t Print;

  RangedValue_t Bps; // input
  MCU_Clocks_t* Clocks; // This includes everything about clocks, and Vdd.
  
} Rs232_t;

void NewRs232HW(Rs232_t* RS, USART_TypeDef* USART, IO_Pin_t* RX, IO_Pin_t* TX);
void SetRs232Timings(Rs232_t* RS, u32 BaudRate, u32 Parity2, u32 StopBits);
void SetRs232Format(Rs232_t* RS);
void SetRs232BVs(Rs232_t* RS, ByteVein_t* BV_TX, ByteVein_t* BV_RX);

void ConfigureRs232HW(Rs232_t* R);
void EnableRs232HW(Rs232_t* R);
void NVIC_Rs232sEnable(FunctionalState Enable);

u32 RS232_putchar(Rs232_t* RS, u32 c);

//=================================
// Self clock management
u32 GetRs232HW_Timings( Rs232_t* RS );

#endif
