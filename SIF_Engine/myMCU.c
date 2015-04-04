
#include "SebEngine.h"
#include "myMCU.h"

// this file is MCU salestype dependent

/*
typedef struct {

  MCU_NodeList        KeySignalName;
  u32                   PPP_Adr;
  u32                   PPP_fnClk;
  u32                   PPP_ctClk;
  u32                   IRQn;
  u32                   fnIRQ;
  u32                   ctIRQ;
  
} MCU_SignalDepedencyType; // this points to a const data // this is one entry
*/
const MCU_NodeDependency_t Signal2Info[] = {

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
{ ucUART4,  (u32)UART4,  RCC_APB1PeriphClockCmd, RCC_APB1Periph_UART4,  UART4_IRQn,  (u32)&fnUART4,  (u32)&ctUART4 },
{ ucUART5,  (u32)UART5,  RCC_APB1PeriphClockCmd, RCC_APB1Periph_UART5,  UART5_IRQn,  (u32)&fnUART5,  (u32)&ctUART5 },
{ ucUSART6, (u32)USART6, RCC_APB2PeriphClockCmd, RCC_APB2Periph_USART6, USART6_IRQn, (u32)&fnUSART6, (u32)&ctUSART6 },
//{ ucUSART7, (u32)USART7, RCC_APB1PeriphClockCmd, , , (u32)&fn, (u32)&ct },
//{ ucUSART8, (u32)USART8, RCC_APB1PeriphClockCmd, , , (u32)&fn, (u32)&ct },
{ ucTIM1,  (u32)TIM1,  RCC_APB2PeriphClockCmd, RCC_APB2Periph_TIM1, TIM1_UP_TIM10_IRQn, (u32)&fnTIM1_UP, (u32)&ctTIM1_UP },
{ ucTIM2,  (u32)TIM2,  RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM2, TIM2_IRQn, (u32)&fnTIM2, (u32)&ctTIM2 },
{ ucTIM3,  (u32)TIM3,  RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM3, TIM3_IRQn, (u32)&fnTIM3, (u32)&ctTIM3 },
{ ucTIM4,  (u32)TIM4,  RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM4, TIM4_IRQn, (u32)&fnTIM4, (u32)&ctTIM4 },
{ ucTIM5,  (u32)TIM5,  RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM5, TIM5_IRQn, (u32)&fnTIM5, (u32)&ctTIM5 },
{ ucTIM6,  (u32)TIM6,  RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM6, TIM6_DAC_IRQn, (u32)&fnTIM6_DAC, (u32)&ctTIM6_DAC },
{ ucTIM7,  (u32)TIM7,  RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM7, TIM7_IRQn, (u32)&fnTIM7, (u32)&ctTIM7 },
{ ucTIM8,  (u32)TIM8,  RCC_APB2PeriphClockCmd, RCC_APB2Periph_TIM8, TIM8_UP_TIM13_IRQn, (u32)&fnTIM8_UP, (u32)&ctTIM8_UP },
{ ucTIM9,  (u32)TIM9,  RCC_APB2PeriphClockCmd, RCC_APB2Periph_TIM9, TIM1_BRK_TIM9_IRQn, (u32)&fnTIM9, (u32)&ctTIM9 },
{ ucTIM10, (u32)TIM10, RCC_APB2PeriphClockCmd, RCC_APB2Periph_TIM10, TIM1_UP_TIM10_IRQn, (u32)&fnTIM10, (u32)&ctTIM10 },
{ ucTIM11, (u32)TIM11, RCC_APB2PeriphClockCmd, RCC_APB2Periph_TIM11, TIM1_TRG_COM_TIM11_IRQn, (u32)&fnTIM11, (u32)&ctTIM11 },
{ ucTIM12, (u32)TIM12, RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM12, TIM8_BRK_TIM12_IRQn, (u32)&fnTIM12, (u32)&ctTIM12 },
{ ucTIM13, (u32)TIM13, RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM13, TIM8_UP_TIM13_IRQn, (u32)&fnTIM13, (u32)&ctTIM13 },
{ ucTIM14, (u32)TIM14, RCC_APB1PeriphClockCmd, RCC_APB1Periph_TIM14, TIM8_TRG_COM_TIM14_IRQn, (u32)&fnTIM14, (u32)&ctTIM14 },

{ ucDAC, (u32)DAC, RCC_APB1PeriphClockCmd, RCC_APB1Periph_DAC, 0, (u32)0, (u32)0 },
{ ucADC1, (u32)ADC1, RCC_APB2PeriphClockCmd, RCC_APB2Periph_ADC1, ADC_IRQn, (u32)&fnADC1, (u32)&ctADC1 },
{ ucADC2, (u32)ADC2, RCC_APB2PeriphClockCmd, RCC_APB2Periph_ADC2, ADC_IRQn, (u32)&fnADC2, (u32)&ctADC2 },
{ ucADC3, (u32)ADC3, RCC_APB2PeriphClockCmd, RCC_APB2Periph_ADC3, ADC_IRQn, (u32)&fnADC3, (u32)&ctADC3 },
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

MCU_NodeDependency_t* GetSignal2InfoBy_PPP(u32 PPP_Adr) {

  u32 n;
  for(n=0;n<countof(Signal2Info);n++) {
    if(Signal2Info[n].PPP_Adr == PPP_Adr) {
      return (MCU_NodeDependency_t*) &Signal2Info[n];
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
      c = (u32*)Signal2Info[n].ctIRQ;  
      f = (u32*)Signal2Info[n].fnIRQ;

      *c = ct;
      *f = fn;
      return n;
    }
  }
  
  
  while(1);
}


//==================================


//======================================================================
// Here this will be called by Main Loop
// Any process which needs servicing at non critical time can be called back from main loop
// Using a FIFO of Jobs to run. This FIFO will have to be replenished regularly

static u32 MainLoopList[100]; // list of pointers
StuffsArtery_t myMainLoopSequence; // this can be accessed by anyone, through a global function callback...

u32 MainLoopServicingSA(u32 u) {
  
  u32 CallbackArmed = 0;
  // one task will be called everytime this function being polled by main loop.
  StuffsArtery_t* SA = (StuffsArtery_t*) u;

  if(SA->bCount==0) return 0; // nothing to do
  
  // one job to handle now
  ClipSA_Down(SA);
  OneJob_t* Job = (OneJob_t*)SA->Out;
  if(Job->fnJob) {
    CallbackArmed = Job->fnJob((u32)Job->ctJobs);//, Job->Param1, Job->Param2, Job->Param3);
  }else{
    while(1); // error, no function to call; Did you initialize the SA before your periph_Init() function? Otherwise, it is still 00000 inside...
  };
  
  if(CallbackArmed) while(1); // here is the main loop! no interrupt will come back here.
  
  return CallbackArmed; // if non zero, then it is a problem because we won't come back here
}

void NewMainLoopServicing(void) {
  
  StuffsArtery_t* P = &myMainLoopSequence; // program
  NewSA(P, (u32)&MainLoopList[0], countof(MainLoopList));  
}

u8* Get_myMCU_SerialID(void) { return (u8*) 0x1FFF7A10;	} // the end of the 96 serial ID is changing chip by chip (LSB of the unique number)

//=============================================
// Timer capabilities

static const MCU_TimerCapabilities_t MCU_TimerCapabilities[] = {

{ ucNoSignal, 0, 0, 0, 0, 0},
// signal, PPP, Max, SR Valid Flags, <SignalCh1>, <SignalCh2>, <SignalCh3>, <SignalCh4>
{ ucTIM1, (u32)TIM1,   0x0000FFFF, TIM_FLAG_Update | TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4, TIM1_CH1, TIM1_CH2, TIM1_CH3, TIM1_CH4 },
{ ucTIM2, (u32)TIM2,   0xFFFFFFFF, TIM_FLAG_Update | TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4, TIM2_CH1, TIM2_CH2, TIM2_CH3, TIM2_CH4 },
{ ucTIM3, (u32)TIM3,   0x0000FFFF, TIM_FLAG_Update | TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4, TIM3_CH1, TIM3_CH2, TIM3_CH3, TIM3_CH4 },
{ ucTIM4, (u32)TIM4,   0x0000FFFF, TIM_FLAG_Update | TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4, TIM4_CH1, TIM4_CH2, TIM4_CH3, TIM4_CH4 },
{ ucTIM5, (u32)TIM5,   0xFFFFFFFF, TIM_FLAG_Update | TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4, TIM5_CH1, TIM5_CH2, TIM5_CH3, TIM5_CH4 },
{ ucTIM6, (u32)TIM6,   0x0000FFFF, TIM_FLAG_Update, 0, 0, 0, 0 },
{ ucTIM7, (u32)TIM7,   0x0000FFFF, TIM_FLAG_Update, 0, 0, 0, 0 },
{ ucTIM8, (u32)TIM8,   0x0000FFFF, TIM_FLAG_Update | TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4, TIM8_CH1, TIM8_CH2, TIM8_CH3, TIM8_CH4 },
{ ucTIM9, (u32)TIM9,   0x0000FFFF, TIM_FLAG_Update | TIM_FLAG_CC1 | TIM_FLAG_CC2,                               TIM9_CH1, TIM9_CH2, 0, 0 },
{ ucTIM10, (u32)TIM10, 0x0000FFFF, TIM_FLAG_Update | TIM_FLAG_CC1,                                              TIM10_CH1, 0, 0, 0 },
{ ucTIM11, (u32)TIM11, 0x0000FFFF, TIM_FLAG_Update | TIM_FLAG_CC1,                                              TIM11_CH1, 0, 0, 0 },
{ ucTIM12, (u32)TIM12, 0x0000FFFF, TIM_FLAG_Update | TIM_FLAG_CC1 | TIM_FLAG_CC2,                               TIM12_CH1, TIM12_CH2, 0, 0 },
{ ucTIM13, (u32)TIM13, 0x0000FFFF, TIM_FLAG_Update | TIM_FLAG_CC1, TIM13_CH1, 0, 0, 0 },
{ ucTIM14, (u32)TIM14, 0x0000FFFF, TIM_FLAG_Update | TIM_FLAG_CC1, TIM14_CH1, 0, 0, 0 },

};

//======================================================================
// First step, clock enable/disable per peripheral type

MCU_TimerCapabilities_t* MCU_GetMCU_TimerCapabilitiesByPPP(u32 PPP_Adr) {
  
  u32 n;
  for(n=0;n<countof(MCU_TimerCapabilities);n++) {
    if(MCU_TimerCapabilities[n].PPP_Adr == PPP_Adr) {
      return (MCU_TimerCapabilities_t*) &MCU_TimerCapabilities[n];
    }
  }
  
  while(1);
  
}


//=======================================================================

ADC_Info_t ADCI[] = {
  
  {ADC1_IN0, PA0, ADC1, ADC_Channel_0},
  {ADC2_IN0, PA0, ADC2, ADC_Channel_0},
  {ADC3_IN0, PA0, ADC3, ADC_Channel_0},
  
  {ADC1_IN1, PA1, ADC1, ADC_Channel_1},
  {ADC2_IN1, PA1, ADC2, ADC_Channel_1},
  {ADC3_IN1, PA1, ADC3, ADC_Channel_1},
  
  {ADC1_IN2, PA2, ADC1, ADC_Channel_2},
  {ADC2_IN2, PA2, ADC2, ADC_Channel_2},
  {ADC3_IN2, PA2, ADC3, ADC_Channel_2},
  
  {ADC1_IN3, PA3, ADC1, ADC_Channel_3},
  {ADC2_IN3, PA3, ADC2, ADC_Channel_3},
  {ADC3_IN3, PA3, ADC3, ADC_Channel_3},
  
  {ADC1_IN4, PA4, ADC1, ADC_Channel_4},
  {ADC2_IN4, PA4, ADC2, ADC_Channel_4},
  {ADC3_IN4, PF6, ADC3, ADC_Channel_4},
  
  {ADC1_IN5, PA5, ADC1, ADC_Channel_5},
  {ADC2_IN5, PA5, ADC2, ADC_Channel_5},
  {ADC3_IN5, PF7, ADC3, ADC_Channel_5},
  
  {ADC1_IN6, PA6, ADC1, ADC_Channel_6},
  {ADC2_IN6, PA6, ADC2, ADC_Channel_6},
  {ADC3_IN6, PF8, ADC3, ADC_Channel_6},
  
  {ADC1_IN7, PA7, ADC1, ADC_Channel_7},
  {ADC2_IN7, PA7, ADC2, ADC_Channel_7},
  {ADC3_IN7, PF9, ADC3, ADC_Channel_7},
  
  {ADC1_IN8, PB0, ADC1, ADC_Channel_8},
  {ADC2_IN8, PB0, ADC2, ADC_Channel_8},
  {ADC3_IN8, PF10, ADC3, ADC_Channel_8},
  
  {ADC1_IN9, PB1, ADC1, ADC_Channel_9},
  {ADC2_IN9, PB1, ADC2, ADC_Channel_9},
  {ADC3_IN9, PF3, ADC3, ADC_Channel_9},
  
  {ADC1_IN10, PC0, ADC1, ADC_Channel_10},
  {ADC2_IN10, PC0, ADC2, ADC_Channel_10},
  {ADC3_IN10, PC0, ADC3, ADC_Channel_10},
  
  {ADC1_IN11, PC1, ADC1, ADC_Channel_11},
  {ADC2_IN11, PC1, ADC2, ADC_Channel_11},
  {ADC3_IN11, PC1, ADC3, ADC_Channel_11},
  
  {ADC1_IN12, PC2, ADC1, ADC_Channel_12},
  {ADC2_IN12, PC2, ADC2, ADC_Channel_12},
  {ADC3_IN12, PC2, ADC3, ADC_Channel_12},
  
  {ADC1_IN13, PC3, ADC1, ADC_Channel_13},
  {ADC2_IN13, PC3, ADC2, ADC_Channel_13},
  {ADC3_IN13, PC3, ADC3, ADC_Channel_13},
  
  {ADC1_IN14, PC4, ADC1, ADC_Channel_14},
  {ADC2_IN14, PC4, ADC2, ADC_Channel_14},
  {ADC3_IN14, PF4, ADC3, ADC_Channel_14},
  
  {ADC1_IN15, PC5, ADC1, ADC_Channel_15},
  {ADC2_IN15, PC5, ADC2, ADC_Channel_15},
  {ADC3_IN15, PF5, ADC3, ADC_Channel_15},

// internal signals connected to ADCs  
  {ADC1_VTEMP, NO_ALTERNATE, ADC1, ADC_Channel_TempSensor},//16
  {ADC2_VTEMP, NO_ALTERNATE, ADC2, ADC_Channel_TempSensor},
  {ADC3_VTEMP, NO_ALTERNATE, ADC3, ADC_Channel_TempSensor},
  
  {ADC1_VREF, NO_ALTERNATE, ADC1, ADC_Channel_Vrefint},//17
  {ADC2_VREF, NO_ALTERNATE, ADC2, ADC_Channel_Vrefint},
  {ADC3_VREF, NO_ALTERNATE, ADC3, ADC_Channel_Vrefint},
  
  {ADC1_VBAT, NO_ALTERNATE, ADC1, ADC_Channel_Vbat},//18
  {ADC2_VBAT, NO_ALTERNATE, ADC2, ADC_Channel_Vbat},
  {ADC3_VBAT, NO_ALTERNATE, ADC3, ADC_Channel_Vbat},
  
};

ADC_Info_t* ADC_GetByPPP_PinName(ADC_TypeDef* ADCx, PinNameDef PinName) { // if there are multiple choice
  // if ADCx is null, we find the first match
  // if ADCx is non null, we use it for match check
  u32 n;
  for(n=0;n<countof(ADCI);n++)    
    if(ADCI[n].PinName == PinName)
      if((ADCx==0)||(ADCI[n].ADCx == ADCx))
        return &ADCI[n];
  while(1); // error, nothing found!
}
