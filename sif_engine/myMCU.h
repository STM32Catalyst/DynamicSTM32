
#ifndef _MY_MCU_H_
#define _MY_MCU_H_
 
typedef enum { // all the manipulating signal names which can cause an interrupt
  
  ucNoSignal, // 0
  
//===---->
  ucGPIOA,
  ucGPIOB,
  ucGPIOC,
  ucGPIOD,
  ucGPIOE,
  ucGPIOF,
  ucGPIOG,
  ucGPIOH,
  ucGPIOI,
  ucGPIOJ,
  ucGPIOK,
  
  ucEXTI0,
  ucEXTI1,
  ucEXTI2,
  ucEXTI3,
  ucEXTI4,
  ucEXTI5,
  ucEXTI6,
  ucEXTI7,
  ucEXTI8,
  ucEXTI9,
  ucEXTI10,
  ucEXTI11,
  ucEXTI12,
  ucEXTI13,
  ucEXTI14,
  ucEXTI15,
  
  ucDMA1,
  ucDMA2,
  
  ucDMA1S0,
  ucDMA1S1,
  ucDMA1S2,
  ucDMA1S3,
  ucDMA1S4,
  ucDMA1S5,
  ucDMA1S6,
  ucDMA1S7,
  ucDMA2S0,
  ucDMA2S1,
  ucDMA2S2,
  ucDMA2S3,
  ucDMA2S4,
  ucDMA2S5,
  ucDMA2S6,
  ucDMA2S7,
  
  ucSPI1,
  ucSPI2,
  ucSPI3,
  ucSPI4,
  ucSPI5,
  ucSPI6,
  
  ucUSART1,
  ucUSART2,
  ucUSART3,
  ucUART4,
  ucUART5,
  ucUSART6,
  ucUSART7,
  ucUSART8,
  
  ucTIM6,
  ucTIM7,
//===---->  
} MCU_NodeList;




typedef struct {

  MCU_NodeList       Node;
  u32                PPP_Adr;
  void                  (*fnClk) (u32,FunctionalState);
  u32                 ctClk;
  u32                  IRQn;
  u32                  fnIRQ;
  u32                  ctIRQ;
  
} MCU_NodeDependencyType; // this points to a const data // this is one entry

//extern const MCU_SignalDependencyType Signal2PPPnIRQ[];

u32 ClockGateEnable_PPP(u32 PPP_Adr, FunctionalState Enable);

u32 HookIRQ_PPP(u32 PPP_Adr, u32 fn, u32 ct);

#endif