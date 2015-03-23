
#ifndef _MY_MCU_H_
#define _MY_MCU_H_

// this file is MCU salestype dependent

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
  
  ucTIM1,
  ucTIM2,
  ucTIM3,
  ucTIM4,
  ucTIM5,
  ucTIM6,
  ucTIM7,
  ucTIM8,
  ucTIM9,
  ucTIM10,
  ucTIM11,
  ucTIM12,
  ucTIM13,
  ucTIM14,
//===---->  
  ucDAC,
  ucADC1,
  ucADC2,
  ucADC3,
} MCU_NodeList;

#define SHARED 0x12345678


typedef struct {

  MCU_NodeList       Node;
  u32                PPP_Adr;
  void                  (*fnClk) (u32,FunctionalState);
  u32                 ctClk;
  u32                  IRQn;
  u32                  fnIRQ;
  u32                  ctIRQ;
  
} MCU_NodeDependency_t; // this points to a const data // this is one entry

extern const MCU_NodeDependency_t Signal2Info[];
MCU_NodeDependency_t* GetSignal2InfoBy_PPP(u32 PPP_Adr);  
  
typedef struct {

  MCU_NodeList       Node;
  u32                PPP_Adr;
  u32                Max;
  u32                SR_ValidFlags;
  u32                SignalCh1;
  u32                SignalCh2;
  u32                SignalCh3;
  u32                SignalCh4;
  
} MCU_TimerCapabilities_t; // this points to a const data // this is one entry


//extern const MCU_SignalDependencyType Signal2PPPnIRQ[];

u32 ClockGateEnable_PPP(u32 PPP_Adr, FunctionalState Enable);

u32 HookIRQ_PPP(u32 PPP_Adr, u32 fn, u32 ct);

void NewMainLoopServicing(void);
u32 MainLoopServicingSA(u32 u);

u8* Get_myMCU_SerialID(void);

MCU_TimerCapabilities_t* MCU_GetMCU_TimerCapabilitiesByPPP(u32 PPP_Adr);


typedef struct {
  SignalName_t Signal;
  PinNameDef PinName;
  ADC_TypeDef* ADCx;
  u16       Channel;
} ADC_Info_t;

ADC_Info_t* ADC_GetByPPP_PinName(ADC_TypeDef* ADCx, PinNameDef PinName);



#endif
