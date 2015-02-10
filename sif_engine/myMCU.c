
#include "SebEngine.h"
#include "myMCU.h"

/*
typedef struct {

  MCU_SignalList        KeySignalName;
  u32                   PPP_Adr;
  u32                   PPP_fnClk;
  u32                   PPP_ctClk;
  u32                   IRQn;
  u32                   fnIRQ;
  u32                   ctIRQ;
  
} MCU_SignalDepedencyType; // this points to a const data // this is one entry
*/
extern const MCU_NodeDependencyType Signal2Info[] = {

{ ucNoSignal, 0, 0, 0, 0, 0},

{ ucGPIOA, (u32)GPIOA, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOA, 0, 0, 0 },
{ ucGPIOB, (u32)GPIOB, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOB, 0, 0, 0 },
{ ucGPIOC, (u32)GPIOC, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOC, 0, 0, 0 },
{ ucGPIOD, (u32)GPIOD, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOD, 0, 0, 0 },
{ ucGPIOE, (u32)GPIOE, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOE, 0, 0, 0 },
{ ucGPIOF, (u32)GPIOF, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOF, 0, 0, 0 },
{ ucGPIOG, (u32)GPIOG, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOG, 0, 0, 0 },
{ ucGPIOH, (u32)GPIOH, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOH, 0, 0, 0 },
{ ucGPIOI, (u32)GPIOI, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOI, 0, 0, 0 },
//{ ucGPIOJ, (u32)GPIOJ, (u32)RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOJ, 0, 0, 0 },
//{ ucGPIOK, (u32)GPIOK, (u32)RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_GPIOK, 0, 0, 0 },

{ ucEXTI0, (u32)EXTI, 0, 0, EXTI0_IRQn, (u32)&fnEXTIs[0], (u32)&ctEXTIs[0]},
{ ucEXTI1, (u32)EXTI, 0, 0, EXTI1_IRQn, (u32)&fnEXTIs[1], (u32)&ctEXTIs[1]},
{ ucEXTI2, (u32)EXTI, 0, 0, EXTI2_IRQn, (u32)&fnEXTIs[2], (u32)&ctEXTIs[2]},
{ ucEXTI3, (u32)EXTI, 0, 0, EXTI3_IRQn, (u32)&fnEXTIs[3], (u32)&ctEXTIs[3]},
{ ucEXTI4, (u32)EXTI, 0, 0, EXTI4_IRQn, (u32)&fnEXTIs[4], (u32)&ctEXTIs[4]},
{ ucEXTI5, (u32)EXTI, 0, 0, EXTI9_5_IRQn, (u32)&fnEXTIs[5], (u32)&ctEXTIs[5]},
{ ucEXTI6, (u32)EXTI, 0, 0, EXTI9_5_IRQn, (u32)&fnEXTIs[6], (u32)&ctEXTIs[6]},
{ ucEXTI7, (u32)EXTI, 0, 0, EXTI9_5_IRQn, (u32)&fnEXTIs[7], (u32)&ctEXTIs[7]},
{ ucEXTI8, (u32)EXTI, 0, 0, EXTI9_5_IRQn, (u32)&fnEXTIs[8], (u32)&ctEXTIs[8]},
{ ucEXTI9, (u32)EXTI, 0, 0, EXTI9_5_IRQn, (u32)&fnEXTIs[9], (u32)&ctEXTIs[9]},
{ ucEXTI10, (u32)EXTI, 0, 0, EXTI15_10_IRQn, (u32)&fnEXTIs[10], (u32)&ctEXTIs[10]},
{ ucEXTI11, (u32)EXTI, 0, 0, EXTI15_10_IRQn, (u32)&fnEXTIs[11], (u32)&ctEXTIs[11]},
{ ucEXTI12, (u32)EXTI, 0, 0, EXTI15_10_IRQn, (u32)&fnEXTIs[12], (u32)&ctEXTIs[12]},
{ ucEXTI13, (u32)EXTI, 0, 0, EXTI15_10_IRQn, (u32)&fnEXTIs[13], (u32)&ctEXTIs[13]},
{ ucEXTI14, (u32)EXTI, 0, 0, EXTI15_10_IRQn, (u32)&fnEXTIs[14], (u32)&ctEXTIs[14]},
{ ucEXTI15, (u32)EXTI, 0, 0, EXTI15_10_IRQn, (u32)&fnEXTIs[15], (u32)&ctEXTIs[15]},

{ ucDMA1, (u32)DMA1, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA1, 0, 0, 0 }, // no interrupt for the DMA itself?
{ ucDMA2, (u32)DMA2, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA2, 0, 0, 0 }, // no interrupt for the DMA itself?
{ ucDMA1S0, (u32)DMA1_Stream0, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA1, DMA1_Stream0_IRQn, (u32)&fnDMA1s[0], (u32)&ctDMA1s[0] },
{ ucDMA1S1, (u32)DMA1_Stream1, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA1, DMA1_Stream1_IRQn, (u32)&fnDMA1s[1], (u32)&ctDMA1s[1] },
{ ucDMA1S2, (u32)DMA1_Stream2, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA1, DMA1_Stream2_IRQn, (u32)&fnDMA1s[2], (u32)&ctDMA1s[2] },
{ ucDMA1S3, (u32)DMA1_Stream3, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA1, DMA1_Stream3_IRQn, (u32)&fnDMA1s[3], (u32)&ctDMA1s[3] },
{ ucDMA1S4, (u32)DMA1_Stream4, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA1, DMA1_Stream4_IRQn, (u32)&fnDMA1s[4], (u32)&ctDMA1s[4] },
{ ucDMA1S5, (u32)DMA1_Stream5, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA1, DMA1_Stream5_IRQn, (u32)&fnDMA1s[5], (u32)&ctDMA1s[5] },
{ ucDMA1S6, (u32)DMA1_Stream6, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA1, DMA1_Stream6_IRQn, (u32)&fnDMA1s[6], (u32)&ctDMA1s[6] },
{ ucDMA1S7, (u32)DMA1_Stream7, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA1, DMA1_Stream7_IRQn, (u32)&fnDMA1s[7], (u32)&ctDMA1s[7] },
{ ucDMA2S0, (u32)DMA2_Stream0, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA2, DMA1_Stream0_IRQn, (u32)&fnDMA2s[0], (u32)&ctDMA2s[0] },
{ ucDMA2S1, (u32)DMA2_Stream1, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA2, DMA1_Stream1_IRQn, (u32)&fnDMA2s[1], (u32)&ctDMA2s[1] },
{ ucDMA2S2, (u32)DMA2_Stream2, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA2, DMA1_Stream2_IRQn, (u32)&fnDMA2s[2], (u32)&ctDMA2s[2] },
{ ucDMA2S3, (u32)DMA2_Stream3, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA2, DMA1_Stream3_IRQn, (u32)&fnDMA2s[3], (u32)&ctDMA2s[3] },
{ ucDMA2S4, (u32)DMA2_Stream4, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA2, DMA1_Stream4_IRQn, (u32)&fnDMA2s[4], (u32)&ctDMA2s[4] },
{ ucDMA2S5, (u32)DMA2_Stream5, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA2, DMA1_Stream5_IRQn, (u32)&fnDMA2s[5], (u32)&ctDMA2s[5] },
{ ucDMA2S6, (u32)DMA2_Stream6, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA2, DMA1_Stream6_IRQn, (u32)&fnDMA2s[6], (u32)&ctDMA2s[6] },
{ ucDMA2S7, (u32)DMA2_Stream7, RCC_AHB1PeriphClockCmd, RCC_AHB1Periph_DMA2, DMA1_Stream7_IRQn, (u32)&fnDMA2s[7], (u32)&ctDMA2s[7] },

{ ucSPI1, (u32)SPI1, RCC_APB2PeriphClockCmd, RCC_APB2Periph_SPI1, SPI1_IRQn, (u32)&fnSPI1, (u32)&ctSPI1 },
{ ucSPI2, (u32)SPI2, RCC_APB1PeriphClockCmd, RCC_APB1Periph_SPI2, SPI2_IRQn, (u32)&fnSPI2, (u32)&ctSPI2 },
{ ucSPI3, (u32)SPI3, RCC_APB1PeriphClockCmd, RCC_APB1Periph_SPI3, SPI3_IRQn, (u32)&fnSPI3, (u32)&ctSPI3 },
{ ucSPI4, (u32)SPI4, RCC_APB2PeriphClockCmd, RCC_APB2Periph_SPI4, SPI4_IRQn, (u32)&fnSPI4, (u32)&ctSPI4 },
{ ucSPI5, (u32)SPI5, RCC_APB2PeriphClockCmd, RCC_APB2Periph_SPI5, SPI5_IRQn, (u32)&fnSPI5, (u32)&ctSPI5 },
{ ucSPI6, (u32)SPI6, RCC_APB2PeriphClockCmd, RCC_APB2Periph_SPI6, SPI6_IRQn, (u32)&fnSPI6, (u32)&ctSPI6 },

{ ucUSART1, (u32)USART1, RCC_APB2PeriphClockCmd, RCC_APB2Periph_USART1, USART1_IRQn, (u32)&fnUSART1, (u32)&ctUSART1 },
{ ucUSART2, (u32)USART2, RCC_APB1PeriphClockCmd, RCC_APB1Periph_USART2, USART2_IRQn, (u32)&fnUSART2, (u32)&ctUSART2 },
{ ucUSART3, (u32)USART3, RCC_APB1PeriphClockCmd, RCC_APB1Periph_USART3, USART3_IRQn, (u32)&fnUSART3, (u32)&ctUSART3 },
{ ucUART4, (u32)UART4, RCC_APB1PeriphClockCmd, RCC_APB1Periph_UART4, UART4_IRQn, (u32)&fnUART4, (u32)&ctUART4 },
{ ucUART5, (u32)UART5, RCC_APB1PeriphClockCmd, RCC_APB1Periph_UART5, UART5_IRQn, (u32)&fnUART5, (u32)&ctUART5 },
{ ucUSART6, (u32)USART6, RCC_APB2PeriphClockCmd, RCC_APB2Periph_USART6, USART6_IRQn, (u32)&fnUSART6, (u32)&ctUSART6 },
//{ ucUSART7, (u32)USART7, RCC_APB1PeriphClockCmd, , , (u32)&fn, (u32)&ct },
//{ ucUSART8, (u32)USART8, RCC_APB1PeriphClockCmd, , , (u32)&fn, (u32)&ct },

{ ucTIM6, (u32)TIM6, RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM6, TIM6_DAC_IRQn, (u32)&fnTIM6_DAC, (u32)&ctTIM6_DAC },
{ ucTIM7, (u32)TIM7, RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM7, TIM7_IRQn, (u32)&fnTIM7, (u32)&ctTIM7 },

};

//======================================================================
// First step, clock enable/disable per peripheral type
u32 ClockGateEnable_PPP(u32 PPP_Adr, FunctionalState Enable) {

  u32 n;
  for(n=0;n<countof(Signal2Info);n++) {
    if(Signal2Info[n].PPP_Adr == PPP_Adr) {
      Signal2Info[n].fnClk(Signal2Info[n].ctClk, Enable);
      return n;
    }
  }
  
  while(1);
}

//======================================================================
// Second step is to hookup the corresponding IRQs
u32 HookIRQ_PPP(u32 PPP_Adr, u32 fn, u32 ct) {
  
  u32 n;
  u32* f;
  u32* c;
  for(n=0;n<countof(Signal2Info);n++) {
    if(Signal2Info[n].PPP_Adr == PPP_Adr) {
      f = (u32*)Signal2Info[n].fnIRQ;
      c = (u32*)Signal2Info[n].ctIRQ;
      *f = fn;
      *c = ct;
      return n;
    }
  }
  
  
  while(1);
}


//======================================================================
// Here this will be called by Main Loop
// Any process which needs servicing at non critical time can be called back from main loop
// Using a FIFO of Jobs to run. This FIFO will have to be replenished regularly

static u32 MainLoopList[100]; // list of pointers
StuffsArtery myMainLoopSequence; // this can be accessed by anyone, through a global function callback...

u32 MainLoopServicingSA(u32 u) {
  
  u32 CallbackArmed = 0;
  // one task will be called everytime this function being polled by main loop.
  StuffsArtery* SA = (StuffsArtery*) u;

  if(SA->bCount==0) return 0; // nothing to do
  
  // one job to handle now
  ClipSA_Down(SA);
  OneJobType* Job = (OneJobType*)SA->Out;
  if(Job->fnJob) {
    CallbackArmed = Job->fnJob((u32)Job->ctJobs);//, Job->Param1, Job->Param2, Job->Param3);
  }else{
    while(1); // error, no function to call; Did you initialize the SA before your periph_Init() function? Otherwise, it is still 00000 inside...
  };
  
  if(CallbackArmed) while(1); // here is the main loop! no interrupt will come back here.
  
  return CallbackArmed; // if non zero, then it is a problem because we won't come back here
}

void NewMainLoopServicing(void) {
  
  StuffsArtery* P = &myMainLoopSequence; // program
  NewSA(P, (u32)&MainLoopList[0], countof(MainLoopList));  
}
