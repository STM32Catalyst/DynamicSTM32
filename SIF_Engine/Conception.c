
#include "SebEngine.h"
#include "Conception.h"

// Draft of new concept that may be absorbed by this project if it shapes and functionality is appealing and elegant.

// Fist, we list the different IP styles

typedef enum {
  TBDx,
//  EVENT,
//  SWDx,
//  JTAGx,
//  HSEx,
//  LSEx,
//  RCCx,
//  SYSx,
  MCOx,   TIMx,   GPIOx,
  SPIx,   I2Cx,   I2Sx,   USARTx,   UARTx,   CANx,   ETHx,
  SDIOx,   LCDx,  FMCx,   DCMIx,   SAIx,   
  ADCx,  DACx,
  IP_COUNTOF
} IP_Race_t;


// for each of these IP groups, there a few spawned (by PPP)

typedef struct {
 IP_Race_t  Race;
 u32* PPPs;
 u8 CountOf;
} IP_Breeds_t;

const u32 GPIOx_PPPs[] = { (u32)GPIOA, (u32)GPIOB, (u32)GPIOC, (u32)GPIOD, (u32)GPIOE, (u32)GPIOF, (u32)GPIOG, (u32)GPIOH, (u32)GPIOI };
const u32 SPIx_PPPs[] = { (u32)SPI1, (u32)SPI2, (u32)SPI3, (u32)SPI4, (u32)SPI5, (u32)SPI6 };
const u32 TIMx_PPPs[] = { (u32)TIM1, (u32)TIM2, (u32)TIM3, (u32)TIM4, (u32)TIM5, (u32)TIM6, (u32)TIM7, (u32)TIM8, (u32)TIM9, (u32)TIM10, (u32)TIM11, (u32)TIM12, (u32)TIM13, (u32)TIM14 };
const u32 UARTx_PPPs[] = { (u32)UART4, (u32)UART5/*, (u32)UART7, (u32)UART8*/ };
const u32 USARTx_PPPs[] = { (u32)USART1, (u32)USART2, (u32)USART3, (u32)USART6 };
const u32 DACx_PPPs[] = { (u32)DAC_DHR12L1_ADDRESS, (u32)DAC_DHR12L2_ADDRESS}; // special 
const u32 ADCx_PPPs[] = { (u32)ADC1, (u32)ADC2, (u32)ADC3 };
const u32 ETHx_PPPs[] = { (u32)ETH };
const u32 SDIOx_PPPs[] = { (u32)SDIO };
const u32 DCMIx_PPPs[] = { (u32)DCMI };

IP_Breeds_t IP_Breeds_PPP[] = {
  
  {GPIOx, (u32*)GPIOx_PPPs, countof(GPIOx_PPPs) },
  {SPIx, (u32*)SPIx_PPPs, countof(SPIx_PPPs) },
  {TIMx, (u32*)TIMx_PPPs, countof(TIMx_PPPs) },
  {UARTx, (u32*)UARTx_PPPs, countof(UARTx_PPPs) },
  {USARTx, (u32*)USARTx_PPPs, countof(USARTx_PPPs) },
  {DACx, (u32*) DACx_PPPs, countof(DACx_PPPs) },
  {ADCx, (u32*) ADCx_PPPs, countof(ADCx_PPPs) },
  {ETHx, (u32*) ETHx_PPPs, countof(ETHx_PPPs) },
  {SDIOx, (u32*) SDIOx_PPPs, countof(SDIOx_PPPs) },
  {DCMIx, (u32*) DCMIx_PPPs, countof(DCMIx_PPPs) },

};

// now we can pass the list 
// Bijectivity of the function is important to get ready for anything. 
// Passing back the entry is good because NULL pointer tells failure to find answer.
IP_Breeds_t* GetIP_BreedByRace(IP_Race_t Race) {
  
  for(u8 n=0; n<countof(IP_Breeds_PPP);n++)
    if(IP_Breeds_PPP[n].Race == Race)
      return &IP_Breeds_PPP[n];
  
  return 0; // nothing found (non blocking)
}

IP_Breeds_t* GetIP_BreedByPPP(u32 PPP) {
  
  for(u8 n=0; n<countof(IP_Breeds_PPP);n++)
    for(u8 i=0; i<IP_Breeds_PPP[n].CountOf; i++)
      if(IP_Breeds_PPP[n].PPPs[i] == PPP)
        return &IP_Breeds_PPP[n];
  
  return 0; // nothing found (non blocking)
}


//===========================================================================================

// how to describe in a string the constrains?

// Label = Signal Type
// RX0,TX0 = USART#(RX,TX) = UART#(RX,TX) = TIM#(CH#, CH#)
// TX0 = USART#.TX

// This is to find which pin, which alternate function (PPP) would meed the choices.