
#ifndef _RS232_HW_H_
#define _RS232_HW_H_

typedef struct {
  
  USART_TypeDef* USART; // The corresponding HW peripheral
  // DMAStream* xxx
  ByteVein* BV_TX; // data flowing to TX pin of MCU
  ByteVein* BV_RX; // data flowing to RX pin of MCU
  
  IO_PinTypeDef* TX;
  IO_PinTypeDef* RX; // The receive pin (which can be woken up if EXTI compatible)
  
  IO_PinTypeDef* CTS; // if 0, not used
  IO_PinTypeDef* RTS; // if 0, not used
  
//  u32 (*fnOut)(u32); // What to do when ready to send (TX)
//  u32 ctOut;

//  u32 (*fnIn)(u32); // What to do when received data (RX)
//  u32 ctIn;
  
} SerialRs232;

void NewRs232HW(SerialRs232* RS, USART_TypeDef* USART);
void NVIC_Rs232sEnable(FunctionalState Enable);
void SetRs232BVs(SerialRs232* RS, ByteVein* BV_TX, ByteVein*BV_RX);
void SetRs232Timings(SerialRs232* RS, u32 BaudRate, u32 Parity2, u32 StopBits);
#endif
