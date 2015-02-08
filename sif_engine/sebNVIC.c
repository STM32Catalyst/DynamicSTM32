
#include "sebEngine.h"

#if 0
typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
/******  STM32 specific Interrupt Numbers **********************************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt                         */
  TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
  RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
  DMA1_Stream0_IRQn           = 11,     /*!< DMA1 Stream 0 global Interrupt                                    */
  DMA1_Stream1_IRQn           = 12,     /*!< DMA1 Stream 1 global Interrupt                                    */
  DMA1_Stream2_IRQn           = 13,     /*!< DMA1 Stream 2 global Interrupt                                    */
  DMA1_Stream3_IRQn           = 14,     /*!< DMA1 Stream 3 global Interrupt                                    */
  DMA1_Stream4_IRQn           = 15,     /*!< DMA1 Stream 4 global Interrupt                                    */
  DMA1_Stream5_IRQn           = 16,     /*!< DMA1 Stream 5 global Interrupt                                    */
  DMA1_Stream6_IRQn           = 17,     /*!< DMA1 Stream 6 global Interrupt                                    */
  ADC_IRQn                    = 18,     /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
  CAN1_TX_IRQn                = 19,     /*!< CAN1 TX Interrupt                                                 */
  CAN1_RX0_IRQn               = 20,     /*!< CAN1 RX0 Interrupt                                                */
  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                                */
  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                                */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
  TIM1_BRK_TIM9_IRQn          = 24,     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
  TIM1_UP_TIM10_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
  TIM1_TRG_COM_TIM11_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                           */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
  RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
  OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */
  TIM8_BRK_TIM12_IRQn         = 43,     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
  TIM8_UP_TIM13_IRQn          = 44,     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
  TIM8_TRG_COM_TIM14_IRQn     = 45,     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
  TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                                    */
  DMA1_Stream7_IRQn           = 47,     /*!< DMA1 Stream7 Interrupt                                            */
  FSMC_IRQn                   = 48,     /*!< FSMC global Interrupt                                             */
  SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                             */
  TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
  SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
  UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                                            */
  UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                                            */
  TIM6_DAC_IRQn               = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
  TIM7_IRQn                   = 55,     /*!< TIM7 global interrupt                                             */
  DMA2_Stream0_IRQn           = 56,     /*!< DMA2 Stream 0 global Interrupt                                    */
  DMA2_Stream1_IRQn           = 57,     /*!< DMA2 Stream 1 global Interrupt                                    */
  DMA2_Stream2_IRQn           = 58,     /*!< DMA2 Stream 2 global Interrupt                                    */
  DMA2_Stream3_IRQn           = 59,     /*!< DMA2 Stream 3 global Interrupt                                    */
  DMA2_Stream4_IRQn           = 60,     /*!< DMA2 Stream 4 global Interrupt                                    */
  ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                                         */
  ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt                       */
  CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                                 */
  CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                                */
  CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                                */
  CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                                */
  OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                                       */
  DMA2_Stream5_IRQn           = 68,     /*!< DMA2 Stream 5 global interrupt                                    */
  DMA2_Stream6_IRQn           = 69,     /*!< DMA2 Stream 6 global interrupt                                    */
  DMA2_Stream7_IRQn           = 70,     /*!< DMA2 Stream 7 global interrupt                                    */
  USART6_IRQn                 = 71,     /*!< USART6 global interrupt                                           */
  I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
  I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
  OTG_HS_EP1_OUT_IRQn         = 74,     /*!< USB OTG HS End Point 1 Out global interrupt                       */
  OTG_HS_EP1_IN_IRQn          = 75,     /*!< USB OTG HS End Point 1 In global interrupt                        */
  OTG_HS_WKUP_IRQn            = 76,     /*!< USB OTG HS Wakeup through EXTI interrupt                          */
  OTG_HS_IRQn                 = 77,     /*!< USB OTG HS global interrupt                                       */
  DCMI_IRQn                   = 78,     /*!< DCMI global interrupt                                             */
  CRYP_IRQn                   = 79,     /*!< CRYP crypto global interrupt                                      */
  HASH_RNG_IRQn               = 80,      /*!< Hash and Rng global interrupt                                     */
  FPU_IRQn                    = 81,      /*!< FPU global interrupt                                              */
  SPI4_IRQn                   = 84,     /*!< SPI4 global Interrupt                                             */
  SPI5_IRQn                   = 85,     /*!< SPI5 global Interrupt                                             */
  SPI6_IRQn                   = 86,     /*!< SPI6 global Interrupt                                             */
} IRQn_Type;
#endif

// we are going to put ALL the interrupt vectors and related stuff here (NVIC specific)

#define NVIC_IRQn_Count 90
#define NVIC_ReleasePendingIRQ(IRQn) NVIC->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)) // Clear pending interrupt

// ~120 hooks = 500 RAM bytes
// ~90x20 = 180 bytes  stats structure for NVIC
// ~90x4 = 360 bytes for pre-post hooks
// ~1kb SRAM for NVIC handlers and statistics. Statistics could be unioned SRAM area depending on needs.

//============== 8>< ~~~~~~~~~~~~~~~~~~~~~~~~~
// for statistics. By default, they are disabled. Can be enabled manually, no function to hook/unhook yet by SW
typedef struct {
  u16 Start_cy;
  u16 End_cy;
  u16 Max_cy;
  u16 Count;
  u16 TooLong_cy;
  u32 fnTooLong; // hook
  u32 ctTooLong; // hook
} NVIC_StatsTypeDef;

NVIC_StatsTypeDef NVIC_Stats[NVIC_IRQn_Count]; // 0 1 2 3 4 5..9 10..15
u32   fnPreNVICs[NVIC_IRQn_Count];
u32   fnPostNVICs[NVIC_IRQn_Count];
// these function can be called to check the duration of interrupt stay within needs
u32 PreNVICs(u32 u) {
//-  NVIC_StatsTypeDef* NVIC_Stat = (NVIC_StatsTypeDef*) u;
  Set_LED_7(0);
//-  NVIC_Stat->Start_cy = TIME_GetTimeNow_us(); // pre
  return u;
}

u32 PostNVICs(u32 u) {
  NVIC_StatsTypeDef* NVIC_Stat = (NVIC_StatsTypeDef*) u;
//-  NVIC_Stat->End_cy = TIME_GetTimeStamp_cy(); // post
  NVIC_Stat->Max_cy = max2(NVIC_Stat->Max_cy,NVIC_Stat->End_cy-NVIC_Stat->Start_cy); // even if rollover comes, it should work (unless more than 1 rollover time elapsed which is too abnormal for interrupt handlers)
  if((NVIC_Stat->TooLong_cy!=0)&&(NVIC_Stat->Max_cy>NVIC_Stat->TooLong_cy))    while(1);
  NVIC_Stat->Count++;
  Set_LED_7(1);
  return u;
}
//============== 8>< ~~~~~~~~~~~~~~~~~~~~~~~~~

//==============================
// These are ALL the interrupt vector handlers

u32     u32_fn_u32(u32 u) {
  return u;
}

//=========================
//#if defined(SebEXTI0)||defined(SebEXTI1)||defined(SebEXTI2)||defined(SebEXTI3)||defined(SebEXTI4)||defined(SebEXTI5_9)||defined(SebEXTI15_10)
u32 fnEXTIs[16],ctEXTIs[16];
//#endif
//#if defined(SebDMA1_Stream0)||defined(SebDMA1_Stream1)||defined(SebDMA1_Stream2)||defined(SebDMA1_Stream3)||defined(SebDMA1_Stream4)||defined(SebDMA1_Stream5)||defined(SebDMA1_Stream6)||defined(SebDMA1_Stream7)
u32 fnDMA1s[8],ctDMA1s[8];
//#endif
//#if defined(SebDMA2_Stream0)||defined(SebDMA2_Stream1)||defined(SebDMA2_Stream2)||defined(SebDMA2_Stream3)||defined(SebDMA2_Stream4)||defined(SebDMA2_Stream5)||defined(SebDMA2_Stream6)||defined(SebDMA2_Stream7)
u32 fnDMA2s[8],ctDMA2s[8];
//#endif
u32 fnWWDG,ctWWDG;
u32 fnPVD,ctPVD;
u32 fnTAMP_STAMP,ctTAMP_STAMP;
u32 fnRTC_WKUP,ctRTC_WKUP;
u32 fnFLASH,ctFLASH;
u32 fnRCC,ctRCC;
u32 fnADC,ctADC;
u32 fnCAN1_TX,ctCAN1_TX;
u32 fnCAN1_RX0,ctCAN1_RX0;
u32 fnCAN1_RX1,ctCAN1_RX1;
u32 fnCAN1_SCE,ctCAN1_SCE;
u32 fnTIM1_BRK_TIM9,ctTIM1_BRK_TIM9;
u32 fnTIM1_UP_TIM10,ctTIM1_UP_TIM10;
u32 fnTIM1_TRG_COM_TIM11,ctTIM1_TRG_COM_TIM11;
u32 fnTIM1_CC,ctTIM1_CC;
u32 fnTIM2,ctTIM2;
u32 fnTIM3,ctTIM3;
u32 fnTIM4,ctTIM4;
u32 fnI2C1_EV,ctI2C1_EV;
u32 fnI2C1_ER,ctI2C1_ER;
u32 fnI2C2_EV,ctI2C2_EV;
u32 fnI2C2_ER,ctI2C2_ER;
u32 fnSPI1,ctSPI1;
u32 fnSPI2,ctSPI2;
u32 fnUSART1,ctUSART1,lpUSART1;
u32 fnUSART2,ctUSART2;
u32 fnUSART3,ctUSART3;
u32 fnRTC_Alarm,ctRTC_Alarm;
u32 fnOTG_FS_WKUP,ctOTG_FS_WKUP;
u32 fnTIM8_BRK_TIM12,ctTIM8_BRK_TIM12;
u32 fnTIM8_UP_TIM13,ctTIM8_UP_TIM13;
u32 fnTIM8_TRG_COM_TIM14,ctTIM8_TRG_COM_TIM14;
u32 fnTIM8_CC,ctTIM8_CC;
u32 fnFSMC,ctFSMC;
u32 fnSDIO,ctSDIO;
u32 fnTIM5,ctTIM5;
u32 fnSPI3,ctSPI3;
u32 fnUART4,ctUART4;
u32 fnUART5,ctUART5;
u32 fnTIM6_DAC,ctTIM6_DAC;
u32 fnTIM7,ctTIM7;
u32 fnETH,ctETH;
u32 fnETH_WKUP,ctETH_WKUP;
u32 fnCAN2_TX,ctCAN2_TX;
u32 fnCAN2_RX0,ctCAN2_RX0;
u32 fnCAN2_RX1,ctCAN2_RX1;
u32 fnCAN2_SCE,ctCAN2_SCE;
u32 fnOTG_FS,ctOTG_FS;
u32 fnUSART6,ctUSART6;
u32 fnI2C3_EV,ctI2C3_EV;
u32 fnI2C3_ER,ctI2C3_ER;
u32 fnOTG_HS_EP1_OUT,ctOTG_HS_EP1_OUT;
u32 fnOTG_HS_EP1_IN,ctOTG_HS_EP1_IN;
u32 fnOTG_HS_WKUP,ctOTG_HS_WKUP;
u32 fnOTG_HS,ctOTG_HS;
u32 fnDCMI,ctDCMI;
u32 fnCRYP,ctCRYP;
u32 fnHASH_RNG,ctHASH_RNG;
u32 fnFPU,ctFPU;
u32 fnSPI4,ctSPI4;
u32 fnSPI5,ctSPI5;
u32 fnSPI6,ctSPI6;
//=========================


//=====================================
//===== 8>< ~~~~~~~~~~~~~~~~~~~~~~
#ifdef SebWWDG
//-u32 fnWWDG,ctWWDG;
__irq void WWDG_IRQHandler(void) { //  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
  if(fnPreNVICs[WWDG_IRQn]) ((u32(*)(u32))fnPreNVICs[WWDG_IRQn])((u32)&NVIC_Stats[WWDG_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnWWDG) ((u32(*)(u32))fnWWDG)(ctWWDG); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(WWDG_IRQn);
  if(fnPostNVICs[WWDG_IRQn]) ((u32(*)(u32))fnPostNVICs[WWDG_IRQn])((u32)&NVIC_Stats[WWDG_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebPVD
//-u32 fnPVD,ctPVD;
__irq void PVD_IRQHandler(void) {//  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt                         */
  if(fnPreNVICs[PVD_IRQn]) ((u32(*)(u32))fnPreNVICs[PVD_IRQn])((u32)&NVIC_Stats[PVD_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnPVD) ((u32(*)(u32))fnPVD)(ctPVD); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(PVD_IRQn);
  if(fnPostNVICs[PVD_IRQn]) ((u32(*)(u32))fnPostNVICs[PVD_IRQn])((u32)&NVIC_Stats[PVD_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTAMP_STAMP
//-u32 fnTAMP_STAMP,ctTAMP_STAMP;
__irq void TAMP_STAMP_IRQHandler(void) {//  TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
  if(fnPreNVICs[TAMP_STAMP_IRQn]) ((u32(*)(u32))fnPreNVICs[TAMP_STAMP_IRQn])((u32)&NVIC_Stats[TAMP_STAMP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTAMP_STAMP) ((u32(*)(u32))fnTAMP_STAMP)(ctTAMP_STAMP); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TAMP_STAMP_IRQn);
  if(fnPostNVICs[TAMP_STAMP_IRQn]) ((u32(*)(u32))fnPostNVICs[TAMP_STAMP_IRQn])((u32)&NVIC_Stats[TAMP_STAMP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebRTC_WKUP
//-u32 fnRTC_WKUP,ctRTC_WKUP;
__irq void RTC_WKUP_IRQHandler(void) {//  RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
  if(fnPreNVICs[RTC_WKUP_IRQn]) ((u32(*)(u32))fnPreNVICs[RTC_WKUP_IRQn])((u32)&NVIC_Stats[RTC_WKUP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnRTC_WKUP) ((u32(*)(u32))fnRTC_WKUP)(ctRTC_WKUP); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(RTC_WKUP_IRQn);
  if(fnPostNVICs[RTC_WKUP_IRQn]) ((u32(*)(u32))fnPostNVICs[RTC_WKUP_IRQn])((u32)&NVIC_Stats[RTC_WKUP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebFLASH
//-u32 fnFLASH,ctFLASH;
__irq void FLASH_IRQHandler(void) {//  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
  if(fnPreNVICs[FLASH_IRQn]) ((u32(*)(u32))fnPreNVICs[FLASH_IRQn])((u32)&NVIC_Stats[FLASH_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnFLASH) ((u32(*)(u32))fnFLASH)(ctFLASH); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(FLASH_IRQn);
  if(fnPostNVICs[FLASH_IRQn]) ((u32(*)(u32))fnPostNVICs[FLASH_IRQn])((u32)&NVIC_Stats[FLASH_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebRCC
//-u32 fnRCC,ctRCC;
__irq void RCC_IRQHandler(void) {//  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
  if(fnPreNVICs[RCC_IRQn]) ((u32(*)(u32))fnPreNVICs[RCC_IRQn])((u32)&NVIC_Stats[RCC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnRCC) ((u32(*)(u32))fnRCC)(ctRCC); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(RCC_IRQn);
  if(fnPostNVICs[RCC_IRQn]) ((u32(*)(u32))fnPostNVICs[RCC_IRQn])((u32)&NVIC_Stats[RCC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebEXTI0
//u32 fnEXTIs[16], ctEXTIs[16];
__irq void EXTI0_IRQHandler(void) {//  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
  if(fnPreNVICs[EXTI0_IRQn]) ((u32(*)(u32))fnPreNVICs[EXTI0_IRQn])((u32)&NVIC_Stats[EXTI0_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnEXTIs[0]) ((u32(*)(u32))fnEXTIs[0])(ctEXTIs[0]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<0);  // clear the pending flag
    NVIC_ReleasePendingIRQ(EXTI0_IRQn);
  if(fnPostNVICs[EXTI0_IRQn]) ((u32(*)(u32))fnPostNVICs[EXTI0_IRQn])((u32)&NVIC_Stats[EXTI0_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebEXTI1
__irq void EXTI1_IRQHandler(void) {//  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
  if(fnPreNVICs[EXTI1_IRQn]) ((u32(*)(u32))fnPreNVICs[EXTI1_IRQn])((u32)&NVIC_Stats[EXTI1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnEXTIs[1]) ((u32(*)(u32))fnEXTIs[1])(ctEXTIs[1]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<1);  // clear the pending flag
    NVIC_ReleasePendingIRQ(EXTI1_IRQn);
  if(fnPostNVICs[EXTI1_IRQn]) ((u32(*)(u32))fnPostNVICs[EXTI1_IRQn])((u32)&NVIC_Stats[EXTI1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebEXTI2
__irq void EXTI2_IRQHandler(void) {//  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
  if(fnPreNVICs[EXTI2_IRQn]) ((u32(*)(u32))fnPreNVICs[EXTI2_IRQn])((u32)&NVIC_Stats[EXTI2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnEXTIs[2]) ((u32(*)(u32))fnEXTIs[2])(ctEXTIs[2]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<2);  // clear the pending flag
    NVIC_ReleasePendingIRQ(EXTI2_IRQn);
  if(fnPostNVICs[EXTI2_IRQn]) ((u32(*)(u32))fnPostNVICs[EXTI2_IRQn])((u32)&NVIC_Stats[EXTI2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebEXTI3
__irq void EXTI3_IRQHandler(void) {//  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
  if(fnPreNVICs[EXTI3_IRQn]) ((u32(*)(u32))fnPreNVICs[EXTI3_IRQn])((u32)&NVIC_Stats[EXTI3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnEXTIs[3]) ((u32(*)(u32))fnEXTIs[3])(ctEXTIs[3]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<3);  // clear the pending flag
    NVIC_ReleasePendingIRQ(EXTI3_IRQn);
  if(fnPostNVICs[EXTI3_IRQn]) ((u32(*)(u32))fnPostNVICs[EXTI3_IRQn])((u32)&NVIC_Stats[EXTI3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebEXTI4
__irq void EXTI4_IRQHandler(void) {//  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
  if(fnPreNVICs[EXTI4_IRQn]) ((u32(*)(u32))fnPreNVICs[EXTI4_IRQn])((u32)&NVIC_Stats[EXTI4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnEXTIs[4]) ((u32(*)(u32))fnEXTIs[4])(ctEXTIs[4]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<4);  // clear the pending flag
    NVIC_ReleasePendingIRQ(EXTI4_IRQn);
  if(fnPostNVICs[EXTI4_IRQn]) ((u32(*)(u32))fnPostNVICs[EXTI4_IRQn])((u32)&NVIC_Stats[EXTI4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA1_Stream0
//u32 fnDMA1s[8],ctDMA1s[8];
__irq void DMA1_Stream0_IRQHandler(void) {//  DMA1_Stream0_IRQn           = 11,     /*!< DMA1 Stream 0 global Interrupt                                    */
  if(fnPreNVICs[DMA1_Stream0_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA1_Stream0_IRQn])((u32)&NVIC_Stats[DMA1_Stream0_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA1s[0]) ((u32(*)(u32))fnDMA1s[0])(ctDMA1s[0]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA1_Stream0_IRQn);
  if(fnPostNVICs[DMA1_Stream0_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA1_Stream0_IRQn])((u32)&NVIC_Stats[DMA1_Stream0_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA1_Stream1
__irq void DMA1_Stream1_IRQHandler(void) {//  DMA1_Stream1_IRQn           = 12,     /*!< DMA1 Stream 1 global Interrupt                                    */
  if(fnPreNVICs[DMA1_Stream1_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA1_Stream1_IRQn])((u32)&NVIC_Stats[DMA1_Stream1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA1s[1]) ((u32(*)(u32))fnDMA1s[1])(ctDMA1s[1]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA1_Stream0_IRQn);
  if(fnPostNVICs[DMA1_Stream1_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA1_Stream1_IRQn])((u32)&NVIC_Stats[DMA1_Stream1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA1_Stream2
__irq void DMA1_Stream2_IRQHandler(void) {//  DMA1_Stream2_IRQn           = 13,     /*!< DMA1 Stream 2 global Interrupt                                    */
  if(fnPreNVICs[DMA1_Stream2_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA1_Stream2_IRQn])((u32)&NVIC_Stats[DMA1_Stream2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA1s[2]) ((u32(*)(u32))fnDMA1s[2])(ctDMA1s[2]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA1_Stream2_IRQn);
  if(fnPostNVICs[DMA1_Stream2_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA1_Stream2_IRQn])((u32)&NVIC_Stats[DMA1_Stream2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA1_Stream3
__irq void DMA1_Stream3_IRQHandler(void) {//  DMA1_Stream3_IRQn           = 14,     /*!< DMA1 Stream 3 global Interrupt                                    */
  if(fnPreNVICs[DMA1_Stream3_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA1_Stream3_IRQn])((u32)&NVIC_Stats[DMA1_Stream3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA1s[3]) ((u32(*)(u32))fnDMA1s[3])(ctDMA1s[3]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA1_Stream3_IRQn);
  if(fnPostNVICs[DMA1_Stream3_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA1_Stream3_IRQn])((u32)&NVIC_Stats[DMA1_Stream3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA1_Stream4
__irq void DMA1_Stream4_IRQHandler(void) {//  DMA1_Stream4_IRQn           = 15,     /*!< DMA1 Stream 4 global Interrupt                                    */
  if(fnPreNVICs[DMA1_Stream4_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA1_Stream4_IRQn])((u32)&NVIC_Stats[DMA1_Stream4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA1s[4]) ((u32(*)(u32))fnDMA1s[4])(ctDMA1s[4]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA1_Stream4_IRQn);
  if(fnPostNVICs[DMA1_Stream4_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA1_Stream4_IRQn])((u32)&NVIC_Stats[DMA1_Stream4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA1_Stream5
__irq void DMA1_Stream5_IRQHandler(void) {//  DMA1_Stream5_IRQn           = 16,     /*!< DMA1 Stream 5 global Interrupt                                    */
  if(fnPreNVICs[DMA1_Stream5_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA1_Stream5_IRQn])((u32)&NVIC_Stats[DMA1_Stream5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA1s[5]) ((u32(*)(u32))fnDMA1s[5])(ctDMA1s[5]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA1_Stream5_IRQn);
  if(fnPostNVICs[DMA1_Stream5_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA1_Stream5_IRQn])((u32)&NVIC_Stats[DMA1_Stream5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA1_Stream6
__irq void DMA1_Stream6_IRQHandler(void) {//  DMA1_Stream6_IRQn           = 17,     /*!< DMA1 Stream 6 global Interrupt                                    */
  if(fnPreNVICs[DMA1_Stream6_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA1_Stream6_IRQn])((u32)&NVIC_Stats[DMA1_Stream6_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA1s[6]) ((u32(*)(u32))fnDMA1s[6])(ctDMA1s[6]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA1_Stream6_IRQn);
  if(fnPostNVICs[DMA1_Stream6_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA1_Stream6_IRQn])((u32)&NVIC_Stats[DMA1_Stream6_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebADC
//-u32 fnADC,ctADC;
__irq void ADC_IRQHandler(void) {//  ADC_IRQn                    = 18,     /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
  if(fnPreNVICs[ADC_IRQn]) ((u32(*)(u32))fnPreNVICs[ADC_IRQn])((u32)&NVIC_Stats[ADC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnADC) ((u32(*)(u32))fnADC)(ctADC); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else;//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
    NVIC_ReleasePendingIRQ(ADC_IRQn);
  if(fnPostNVICs[ADC_IRQn]) ((u32(*)(u32))fnPostNVICs[ADC_IRQn])((u32)&NVIC_Stats[ADC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebCAN1_TX
//-u32 fnCAN1_TX,ctCAN1_TX;
__irq void CAN1_TX_IRQHandler(void) {//  CAN1_TX_IRQn                = 19,     /*!< CAN1 TX Interrupt                                                 */
  if(fnPreNVICs[CAN1_TX_IRQn]) ((u32(*)(u32))fnPreNVICs[CAN1_TX_IRQn])((u32)&NVIC_Stats[CAN1_TX_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnCAN1_TX) ((u32(*)(u32))fnCAN1_TX)(ctCAN1_TX); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(CAN1_TX_IRQn);
  if(fnPostNVICs[CAN1_TX_IRQn]) ((u32(*)(u32))fnPostNVICs[CAN1_TX_IRQn])((u32)&NVIC_Stats[CAN1_TX_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebCAN1_RX0
//-u32 fnCAN1_RX0,ctCAN1_RX0;
__irq void CAN1_RX0_IRQHandler(void) {//  CAN1_RX0_IRQn               = 20,     /*!< CAN1 RX0 Interrupt                                                */
  if(fnPreNVICs[CAN1_RX0_IRQn]) ((u32(*)(u32))fnPreNVICs[CAN1_RX0_IRQn])((u32)&NVIC_Stats[CAN1_RX0_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnCAN1_RX0) ((u32(*)(u32))fnCAN1_RX0)(ctCAN1_RX0); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(CAN1_RX0_IRQn);
  if(fnPostNVICs[CAN1_RX0_IRQn]) ((u32(*)(u32))fnPostNVICs[CAN1_RX0_IRQn])((u32)&NVIC_Stats[CAN1_RX0_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebCAN1_RX1
//-u32 fnCAN1_RX1,ctCAN1_RX1;
__irq void CAN1_RX1_IRQHandler(void) {//  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                                */
  if(fnPreNVICs[CAN1_RX1_IRQn]) ((u32(*)(u32))fnPreNVICs[CAN1_RX1_IRQn])((u32)&NVIC_Stats[CAN1_RX1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnCAN1_RX1) ((u32(*)(u32))fnCAN1_RX1)(ctCAN1_RX1); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(CAN1_RX1_IRQn);
  if(fnPostNVICs[CAN1_RX1_IRQn]) ((u32(*)(u32))fnPostNVICs[CAN1_RX1_IRQn])((u32)&NVIC_Stats[CAN1_RX1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebCAN1_SCE
//-u32 fnCAN1_SCE,ctCAN1_SCE;
__irq void CAN1_SCE_IRQHandler(void) {//  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                                */
  if(fnPreNVICs[CAN1_SCE_IRQn]) ((u32(*)(u32))fnPreNVICs[CAN1_SCE_IRQn])((u32)&NVIC_Stats[CAN1_SCE_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnCAN1_SCE) ((u32(*)(u32))fnCAN1_SCE)(ctCAN1_SCE); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(CAN1_SCE_IRQn);
  if(fnPostNVICs[CAN1_SCE_IRQn]) ((u32(*)(u32))fnPostNVICs[CAN1_SCE_IRQn])((u32)&NVIC_Stats[CAN1_SCE_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebEXTI9_5
__irq void EXTI9_5_IRQHandler(void) {//  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
  if(fnPreNVICs[EXTI9_5_IRQn]) ((u32(*)(u32))fnPreNVICs[EXTI9_5_IRQn])((u32)&NVIC_Stats[EXTI9_5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnEXTIs[5]) ((u32(*)(u32))fnEXTIs[5])(ctEXTIs[5]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<5); // clear the signal causing the interrupt when no handler
    if(fnEXTIs[6]) ((u32(*)(u32))fnEXTIs[6])(ctEXTIs[6]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<6); // clear the signal causing the interrupt when no handler
    if(fnEXTIs[7]) ((u32(*)(u32))fnEXTIs[7])(ctEXTIs[7]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<7); // clear the signal causing the interrupt when no handler
    if(fnEXTIs[8]) ((u32(*)(u32))fnEXTIs[8])(ctEXTIs[8]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<8); // clear the signal causing the interrupt when no handler
    if(fnEXTIs[9]) ((u32(*)(u32))fnEXTIs[9])(ctEXTIs[9]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<9); // clear the signal causing the interrupt when no handler
    NVIC_ReleasePendingIRQ(EXTI9_5_IRQn);
  if(fnPostNVICs[EXTI9_5_IRQn]) ((u32(*)(u32))fnPostNVICs[EXTI9_5_IRQn])((u32)&NVIC_Stats[EXTI9_5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM1_BRK_TIM9
//-u32 fnTIM1_BRK_TIM9,ctTIM1_BRK_TIM9; // this should be split in 2 different hooks
__irq void TIM1_BRK_TIM9_IRQHandler(void) {//  TIM1_BRK_TIM9_IRQn          = 24,     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
  if(fnPreNVICs[TIM1_BRK_TIM9_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM1_BRK_TIM9_IRQn])((u32)&NVIC_Stats[TIM1_BRK_TIM9_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM1_BRK_TIM9) ((u32(*)(u32))fnTIM1_BRK_TIM9)(ctTIM1_BRK_TIM9); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM1_BRK_TIM9_IRQn);
  if(fnPostNVICs[TIM1_BRK_TIM9_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM1_BRK_TIM9_IRQn])((u32)&NVIC_Stats[TIM1_BRK_TIM9_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM1_UP_TIM10
//-u32 fnTIM1_UP_TIM10,ctTIM1_UP_TIM10; // this should be split in 2 different hooks
__irq void TIM1_UP_TIM10_IRQHandler(void) {//  TIM1_UP_TIM10_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
  if(fnPreNVICs[TIM1_UP_TIM10_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM1_UP_TIM10_IRQn])((u32)&NVIC_Stats[TIM1_UP_TIM10_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM1_UP_TIM10) ((u32(*)(u32))fnTIM1_UP_TIM10)(ctTIM1_UP_TIM10); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM1_UP_TIM10_IRQn);
  if(fnPostNVICs[TIM1_UP_TIM10_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM1_UP_TIM10_IRQn])((u32)&NVIC_Stats[TIM1_UP_TIM10_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM1_TRG_COM_TIM11
//-u32 fnTIM1_TRG_COM_TIM11,ctTIM1_TRG_COM_TIM11; // this should be split in 2 different hooks
__irq void TIM1_TRG_COM_TIM11_IRQHandler(void) {//  TIM1_TRG_COM_TIM11_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
  if(fnPreNVICs[TIM1_TRG_COM_TIM11_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM1_TRG_COM_TIM11_IRQn])((u32)&NVIC_Stats[TIM1_TRG_COM_TIM11_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM1_TRG_COM_TIM11) ((u32(*)(u32))fnTIM1_TRG_COM_TIM11)(ctTIM1_TRG_COM_TIM11); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//    NVIC_ReleasePendingIRQ(TIM1_TRG_COM_TIM11_IRQn);
  if(fnPostNVICs[TIM1_TRG_COM_TIM11_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM1_TRG_COM_TIM11_IRQn])((u32)&NVIC_Stats[TIM1_TRG_COM_TIM11_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM1_CC
//-u32 fnTIM1_CC,ctTIM1_CC;
__irq void TIM1_CC_IRQHandler(void) {//  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
  if(fnPreNVICs[TIM1_CC_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM1_CC_IRQn])((u32)&NVIC_Stats[TIM1_CC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM1_CC) ((u32(*)(u32))fnTIM1_CC)(ctTIM1_CC); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM1_CC_IRQn);
  if(fnPostNVICs[TIM1_CC_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM1_CC_IRQn])((u32)&NVIC_Stats[TIM1_CC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM2
//-u32 fnTIM2,ctTIM2;
__irq void TIM2_IRQHandler(void) {//  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
  if(fnPreNVICs[TIM2_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM2_IRQn])((u32)&NVIC_Stats[TIM2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM2) ((u32(*)(u32))fnTIM2)(ctTIM2); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM2_IRQn);
  if(fnPostNVICs[TIM2_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM2_IRQn])((u32)&NVIC_Stats[TIM2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM3
//-u32 fnTIM3,ctTIM3;
__irq void TIM3_IRQHandler(void) {//  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
  if(fnPreNVICs[TIM3_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM3_IRQn])((u32)&NVIC_Stats[TIM3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM3) ((u32(*)(u32))fnTIM3)(ctTIM3); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM3_IRQn);
  if(fnPostNVICs[TIM3_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM3_IRQn])((u32)&NVIC_Stats[TIM3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM4
//-u32 fnTIM4,ctTIM4;
__irq void TIM4_IRQHandler(void) {//  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
  if(fnPreNVICs[TIM4_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM4_IRQn])((u32)&NVIC_Stats[TIM4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM4) ((u32(*)(u32))fnTIM4)(ctTIM4); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM4_IRQn);
  if(fnPostNVICs[TIM4_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM4_IRQn])((u32)&NVIC_Stats[TIM4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebI2C1_EV
//-u32 fnI2C1_EV,ctI2C1_EV;
__irq void I2C1_EV_IRQHandler(void) {//  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
  if(fnPreNVICs[I2C1_EV_IRQn]) ((u32(*)(u32))fnPreNVICs[I2C1_EV_IRQn])((u32)&NVIC_Stats[I2C1_EV_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnI2C1_EV) ((u32(*)(u32))fnI2C1_EV)(ctI2C1_EV); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(I2C1_EV_IRQn);
  if(fnPostNVICs[I2C1_EV_IRQn]) ((u32(*)(u32))fnPostNVICs[I2C1_EV_IRQn])((u32)&NVIC_Stats[I2C1_EV_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebI2C1_ER
//-u32 fnI2C1_ER,ctI2C1_ER;
__irq void I2C1_ER_IRQHandler(void) {//  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
  if(fnPreNVICs[I2C1_ER_IRQn]) ((u32(*)(u32))fnPreNVICs[I2C1_ER_IRQn])((u32)&NVIC_Stats[I2C1_ER_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnI2C1_ER) ((u32(*)(u32))fnI2C1_ER)(ctI2C1_ER); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(I2C1_ER_IRQn);
  if(fnPostNVICs[I2C1_ER_IRQn]) ((u32(*)(u32))fnPostNVICs[I2C1_ER_IRQn])((u32)&NVIC_Stats[I2C1_ER_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebI2C2_EV
//-u32 fnI2C2_EV,ctI2C2_EV;
__irq void I2C2_EV_IRQHandler(void) {//  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
  if(fnPreNVICs[I2C2_EV_IRQn]) ((u32(*)(u32))fnPreNVICs[I2C2_EV_IRQn])((u32)&NVIC_Stats[I2C2_EV_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnI2C2_EV) ((u32(*)(u32))fnI2C2_EV)(ctI2C2_EV); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(I2C2_EV_IRQn);
  if(fnPostNVICs[I2C2_EV_IRQn]) ((u32(*)(u32))fnPostNVICs[I2C2_EV_IRQn])((u32)&NVIC_Stats[I2C2_EV_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebI2C2_ER
//-u32 fnI2C2_ER,ctI2C2_ER;
__irq void I2C2_ER_IRQHandler(void) {//  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */
  if(fnPreNVICs[I2C2_ER_IRQn]) ((u32(*)(u32))fnPreNVICs[I2C2_ER_IRQn])((u32)&NVIC_Stats[I2C2_ER_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnI2C2_ER) ((u32(*)(u32))fnI2C2_ER)(ctI2C2_ER); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(I2C2_ER_IRQn);
  if(fnPostNVICs[I2C2_ER_IRQn]) ((u32(*)(u32))fnPostNVICs[I2C2_ER_IRQn])((u32)&NVIC_Stats[I2C2_ER_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebSPI1
//-u32 fnSPI1,ctSPI1;
__irq void SPI1_IRQHandler(void) {//  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
  if(fnPreNVICs[SPI1_IRQn]) ((u32(*)(u32))fnPreNVICs[SPI1_IRQn])((u32)&NVIC_Stats[SPI1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnSPI1) ((u32(*)(u32))fnSPI1)(ctSPI1); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(SPI1_IRQn);
  if(fnPostNVICs[SPI1_IRQn]) ((u32(*)(u32))fnPostNVICs[SPI1_IRQn])((u32)&NVIC_Stats[SPI1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebSPI2
//-u32 fnSPI2,ctSPI2;
__irq void SPI2_IRQHandler(void) {//  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
  if(fnPreNVICs[SPI2_IRQn]) ((u32(*)(u32))fnPreNVICs[SPI2_IRQn])((u32)&NVIC_Stats[SPI2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnSPI2) ((u32(*)(u32))fnSPI2)(ctSPI2); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(SPI2_IRQn);
  if(fnPostNVICs[SPI2_IRQn]) ((u32(*)(u32))fnPostNVICs[SPI2_IRQn])((u32)&NVIC_Stats[SPI2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebUSART1
//-u32 fnUSART1,ctUSART1,lpUSART1;
__irq void USART1_IRQHandler(void) {//  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
  if(fnPreNVICs[USART1_IRQn]) ((u32(*)(u32))fnPreNVICs[USART1_IRQn])((u32)&NVIC_Stats[USART1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnUSART1) ((u32(*)(u32))fnUSART1)(ctUSART1); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else { // clear the pending flag
      if((USART1->CR1 & USART_FLAG_TXE)&&(USART1->SR & USART_FLAG_TXE)) // if ready to transmit by interrupt... send a dummy! Write to DR
          USART1->DR = lpUSART1; // LF ASCII char, will clear the interrupt
      else lpUSART1 = USART1->DR;   // dummy read of the data to clear the pending interrupt, this will clear PE, RXNE and FE bits (this will route RX back to TX)
    }
//-    NVIC_ReleasePendingIRQ(USART1_IRQn);
  if(fnPostNVICs[USART1_IRQn]) ((u32(*)(u32))fnPostNVICs[USART1_IRQn])((u32)&NVIC_Stats[USART1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebUSART2
//-u32 fnUSART2,ctUSART2;
__irq void USART2_IRQHandler(void) {//  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
  if(fnPreNVICs[USART2_IRQn]) ((u32(*)(u32))fnPreNVICs[USART2_IRQn])((u32)&NVIC_Stats[USART2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnUSART2) ((u32(*)(u32))fnUSART2)(ctUSART2); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(USART2_IRQn);
  if(fnPostNVICs[USART2_IRQn]) ((u32(*)(u32))fnPostNVICs[USART2_IRQn])((u32)&NVIC_Stats[USART2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebUSART3
//-u32 fnUSART3,ctUSART3;
__irq void USART3_IRQHandler(void) {//  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                           */
  if(fnPreNVICs[USART3_IRQn]) ((u32(*)(u32))fnPreNVICs[USART3_IRQn])((u32)&NVIC_Stats[USART3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnUSART3) ((u32(*)(u32))fnUSART3)(ctUSART3); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//    NVIC_ReleasePendingIRQ(USART3_IRQn);
  if(fnPostNVICs[USART3_IRQn]) ((u32(*)(u32))fnPostNVICs[USART3_IRQn])((u32)&NVIC_Stats[USART3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebEXTI15_10
__irq void EXTI15_10_IRQHandler(void) {//  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
//  Set_LED_7(0);
  if(fnPreNVICs[EXTI15_10_IRQn]) ((u32(*)(u32))fnPreNVICs[EXTI15_10_IRQn])((u32)&NVIC_Stats[EXTI15_10_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnEXTIs[10]) ((u32(*)(u32))fnEXTIs[10])(ctEXTIs[10]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<10); // clear the signal causing the interrupt when no handler
    if(fnEXTIs[11]) ((u32(*)(u32))fnEXTIs[11])(ctEXTIs[11]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<11); // clear the signal causing the interrupt when no handler
    if(fnEXTIs[12]) ((u32(*)(u32))fnEXTIs[12])(ctEXTIs[12]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<12); // clear the signal causing the interrupt when no handler
    if(fnEXTIs[13]) ((u32(*)(u32))fnEXTIs[13])(ctEXTIs[13]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<13); // clear the signal causing the interrupt when no handler
    if(fnEXTIs[14]) ((u32(*)(u32))fnEXTIs[14])(ctEXTIs[14]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<14); // clear the signal causing the interrupt when no handler
    if(fnEXTIs[15]) ((u32(*)(u32))fnEXTIs[15])(ctEXTIs[15]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else EXTI->PR |= (1<<15); // clear the signal causing the interrupt when no handler
    NVIC_ReleasePendingIRQ(EXTI15_10_IRQn);
  if(fnPostNVICs[EXTI15_10_IRQn]) ((u32(*)(u32))fnPostNVICs[EXTI15_10_IRQn])((u32)&NVIC_Stats[EXTI15_10_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
//  Set_LED_7(1);
}
#endif
#ifdef SebRTC_Alarm
//-u32 fnRTC_Alarm,ctRTC_Alarm;
__irq void RTC_Alarm_IRQHandler(void) {//  RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
  if(fnPreNVICs[RTC_Alarm_IRQn]) ((u32(*)(u32))fnPreNVICs[RTC_Alarm_IRQn])((u32)&NVIC_Stats[RTC_Alarm_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnRTC_Alarm) ((u32(*)(u32))fnRTC_Alarm)(ctRTC_Alarm); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(RTC_Alarm_IRQn);
  if(fnPostNVICs[RTC_Alarm_IRQn]) ((u32(*)(u32))fnPostNVICs[RTC_Alarm_IRQn])((u32)&NVIC_Stats[RTC_Alarm_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebOTG_FS_WKUP
//-u32 fnOTG_FS_WKUP,ctOTG_FS_WKUP;
__irq void OTG_FS_WKUP_IRQHandler(void) {//  OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */
  if(fnPreNVICs[OTG_FS_WKUP_IRQn]) ((u32(*)(u32))fnPreNVICs[OTG_FS_WKUP_IRQn])((u32)&NVIC_Stats[OTG_FS_WKUP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnOTG_FS_WKUP) ((u32(*)(u32))fnOTG_FS_WKUP)(ctOTG_FS_WKUP); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(OTG_FS_WKUP_IRQn);
  if(fnPostNVICs[OTG_FS_WKUP_IRQn]) ((u32(*)(u32))fnPostNVICs[OTG_FS_WKUP_IRQn])((u32)&NVIC_Stats[OTG_FS_WKUP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM8_BRK_TIM12
//-u32 fnTIM8_BRK_TIM12,ctTIM8_BRK_TIM12; // this should be flipped to 2 different hooks
__irq void TIM8_BRK_TIM12_IRQHandler(void) {//  TIM8_BRK_TIM12_IRQn         = 43,     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
  if(fnPreNVICs[TIM8_BRK_TIM12_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM8_BRK_TIM12_IRQn])((u32)&NVIC_Stats[TIM8_BRK_TIM12_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM8_BRK_TIM12) ((u32(*)(u32))fnTIM8_BRK_TIM12)(ctTIM8_BRK_TIM12); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//    NVIC_ReleasePendingIRQ(TIM8_BRK_TIM12_IRQn);
  if(fnPostNVICs[TIM8_BRK_TIM12_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM8_BRK_TIM12_IRQn])((u32)&NVIC_Stats[TIM8_BRK_TIM12_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM8_UP_TIM13
//-u32 fnTIM8_UP_TIM13,ctTIM8_UP_TIM13; // this should be flipped to 2 different hooks
__irq void TIM8_UP_TIM13_IRQHandler(void) {//  TIM8_UP_TIM13_IRQn          = 44,     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
  if(fnPreNVICs[TIM8_UP_TIM13_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM8_UP_TIM13_IRQn])((u32)&NVIC_Stats[TIM8_UP_TIM13_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM8_UP_TIM13) ((u32(*)(u32))fnTIM8_UP_TIM13)(ctTIM8_UP_TIM13); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//    NVIC_ReleasePendingIRQ(TIM8_UP_TIM13_IRQn);
  if(fnPostNVICs[TIM8_UP_TIM13_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM8_UP_TIM13_IRQn])((u32)&NVIC_Stats[TIM8_UP_TIM13_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM8_TRG_COM_TIM14
//-u32 fnTIM8_TRG_COM_TIM14,ctTIM8_TRG_COM_TIM14; // this should be flipped to 2 different hooks
__irq void TIM8_TRG_COM_TIM14_IRQHandler(void) {//  TIM8_TRG_COM_TIM14_IRQn     = 45,     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
  if(fnPreNVICs[TIM8_TRG_COM_TIM14_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM8_TRG_COM_TIM14_IRQn])((u32)&NVIC_Stats[TIM8_TRG_COM_TIM14_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM8_TRG_COM_TIM14) ((u32(*)(u32))fnTIM8_TRG_COM_TIM14)(ctTIM8_TRG_COM_TIM14); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
   2 else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM8_TRG_COM_TIM14_IRQn);
  if(fnPostNVICs[TIM8_TRG_COM_TIM14_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM8_TRG_COM_TIM14_IRQn])((u32)&NVIC_Stats[TIM8_TRG_COM_TIM14_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM8_CC
//-u32 fnTIM8_CC,ctTIM8_CC;
__irq void TIM8_CC_IRQHandler(void) {//  TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                                    */
  if(fnPreNVICs[TIM8_CC_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM8_CC_IRQn])((u32)&NVIC_Stats[TIM8_CC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM8_CC) ((u32(*)(u32))fnTIM8_CC)(ctTIM8_CC); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM8_CC_IRQn);
  if(fnPostNVICs[TIM8_CC_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM8_CC_IRQn])((u32)&NVIC_Stats[TIM8_CC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA1_Stream7
__irq void DMA1_Stream7_IRQHandler(void) {//  DMA1_Stream7_IRQn           = 47,     /*!< DMA1 Stream7 Interrupt                                            */
  if(fnPreNVICs[DMA1_Stream7_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA1_Stream7_IRQn])((u32)&NVIC_Stats[DMA1_Stream7_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA1s[7]) ((u32(*)(u32))fnDMA1s[7])(ctDMA1s[7]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA1_Stream7_IRQn);
  if(fnPostNVICs[DMA1_Stream7_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA1_Stream7_IRQn])((u32)&NVIC_Stats[DMA1_Stream7_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebFSMC
//-u32 fnFSMC,ctFSMC;
__irq void FSMC_IRQHandler(void) {//  FSMC_IRQn                   = 48,     /*!< FSMC global Interrupt                                             */
  if(fnPreNVICs[FSMC_IRQn]) ((u32(*)(u32))fnPreNVICs[FSMC_IRQn])((u32)&NVIC_Stats[FSMC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnFSMC) ((u32(*)(u32))fnFSMC)(ctFSMC); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(FSMC_IRQn);
  if(fnPostNVICs[FSMC_IRQn]) ((u32(*)(u32))fnPostNVICs[FSMC_IRQn])((u32)&NVIC_Stats[FSMC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebSDIO
//-u32 fnSDIO,ctSDIO;
__irq void SDIO_IRQHandler(void) {//  SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                             */
  if(fnPreNVICs[SDIO_IRQn]) ((u32(*)(u32))fnPreNVICs[SDIO_IRQn])((u32)&NVIC_Stats[SDIO_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnSDIO) ((u32(*)(u32))fnSDIO)(ctSDIO); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(SDIO_IRQn);
  if(fnPostNVICs[SDIO_IRQn]) ((u32(*)(u32))fnPostNVICs[SDIO_IRQn])((u32)&NVIC_Stats[SDIO_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM5
//-u32 fnTIM5,ctTIM5;
__irq void TIM5_IRQHandler(void) {//  TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
  if(fnPreNVICs[TIM5_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM5_IRQn])((u32)&NVIC_Stats[TIM5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM5) ((u32(*)(u32))fnTIM5)(ctTIM5); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM5_IRQn);
  if(fnPostNVICs[TIM5_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM5_IRQn])((u32)&NVIC_Stats[TIM5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebSPI3
//-u32 fnSPI3,ctSPI3;
__irq void SPI3_IRQHandler(void) {//  SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
  if(fnPreNVICs[SPI3_IRQn]) ((u32(*)(u32))fnPreNVICs[SPI3_IRQn])((u32)&NVIC_Stats[SPI3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnSPI3) ((u32(*)(u32))fnSPI3)(ctSPI3); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(SPI3_IRQn);
  if(fnPostNVICs[SPI3_IRQn]) ((u32(*)(u32))fnPostNVICs[SPI3_IRQn])((u32)&NVIC_Stats[SPI3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebUART4
//-u32 fnUART4,ctUART4;
__irq void UART4_IRQHandler(void) {//  UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                                            */
  if(fnPreNVICs[UART4_IRQn]) ((u32(*)(u32))fnPreNVICs[UART4_IRQn])((u32)&NVIC_Stats[UART4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnUART4) ((u32(*)(u32))fnUART4)(ctUART4); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//    NVIC_ReleasePendingIRQ(UART4_IRQn);
  if(fnPostNVICs[UART4_IRQn]) ((u32(*)(u32))fnPostNVICs[UART4_IRQn])((u32)&NVIC_Stats[UART4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebUART5
//-u32 fnUART5,ctUART5;
__irq void UART5_IRQHandler(void) {//  UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                                            */
  if(fnPreNVICs[UART5_IRQn]) ((u32(*)(u32))fnPreNVICs[UART5_IRQn])((u32)&NVIC_Stats[UART5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnUART5) ((u32(*)(u32))fnUART5)(ctUART5); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(UART5_IRQn);
  if(fnPostNVICs[UART5_IRQn]) ((u32(*)(u32))fnPostNVICs[UART5_IRQn])((u32)&NVIC_Stats[UART5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM6_DAC
//-u32 fnTIM6_DAC,ctTIM6_DAC; // to be split in 2 hooks?
__irq void TIM6_DAC_IRQHandler(void) {//  TIM6_DAC_IRQn               = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
  if(fnPreNVICs[TIM6_DAC_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM6_DAC_IRQn])((u32)&NVIC_Stats[TIM6_DAC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM6_DAC) ((u32(*)(u32))fnTIM6_DAC)(ctTIM6_DAC); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else TIM6->SR &= ~1;//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM6_DAC_IRQn);
  if(fnPostNVICs[TIM6_DAC_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM6_DAC_IRQn])((u32)&NVIC_Stats[TIM6_DAC_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebTIM7
//-u32 fnTIM7,ctTIM7;
__irq void TIM7_IRQHandler(void) {//  TIM7_IRQn                   = 55,     /*!< TIM7 global interrupt                                             */
  if(fnPreNVICs[TIM7_IRQn]) ((u32(*)(u32))fnPreNVICs[TIM7_IRQn])((u32)&NVIC_Stats[TIM7_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnTIM7) ((u32(*)(u32))fnTIM7)(ctTIM7); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else TIM7->SR &= ~1;//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(TIM7_IRQn);
  if(fnPostNVICs[TIM7_IRQn]) ((u32(*)(u32))fnPostNVICs[TIM7_IRQn])((u32)&NVIC_Stats[TIM7_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA2_Stream0
//u32 fnDMA2s[8],ctDMA2s[8];
__irq void DMA2_Stream0_IRQHandler(void) {//  DMA2_Stream0_IRQn           = 56,     /*!< DMA2 Stream 0 global Interrupt                                    */
  if(fnPreNVICs[DMA2_Stream0_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA2_Stream0_IRQn])((u32)&NVIC_Stats[DMA2_Stream0_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA2s[0]) ((u32(*)(u32))fnDMA2s[0])(ctDMA2s[0]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA2_Stream0_IRQn);
  if(fnPostNVICs[DMA2_Stream0_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA2_Stream0_IRQn])((u32)&NVIC_Stats[DMA2_Stream0_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA2_Stream1
__irq void DMA2_Stream1_IRQHandler(void) {//  DMA2_Stream1_IRQn           = 57,     /*!< DMA2 Stream 1 global Interrupt                                    */
  if(fnPreNVICs[DMA2_Stream1_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA2_Stream1_IRQn])((u32)&NVIC_Stats[DMA2_Stream1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA2s[1]) ((u32(*)(u32))fnDMA2s[1])(ctDMA2s[1]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA2_Stream0_IRQn);
  if(fnPostNVICs[DMA2_Stream1_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA2_Stream1_IRQn])((u32)&NVIC_Stats[DMA2_Stream1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA2_Stream2
__irq void DMA2_Stream2_IRQHandler(void) {//  DMA2_Stream2_IRQn           = 58,     /*!< DMA2 Stream 2 global Interrupt                                    */
  if(fnPreNVICs[DMA2_Stream2_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA2_Stream2_IRQn])((u32)&NVIC_Stats[DMA2_Stream2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA2s[2]) ((u32(*)(u32))fnDMA2s[2])(ctDMA2s[2]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA2_Stream2_IRQn);
  if(fnPostNVICs[DMA2_Stream2_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA2_Stream2_IRQn])((u32)&NVIC_Stats[DMA2_Stream2_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA2_Stream3
__irq void DMA2_Stream3_IRQHandler(void) {//  DMA2_Stream3_IRQn           = 59,     /*!< DMA2 Stream 3 global Interrupt                                    */
  if(fnPreNVICs[DMA2_Stream3_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA2_Stream3_IRQn])((u32)&NVIC_Stats[DMA2_Stream3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA2s[3]) ((u32(*)(u32))fnDMA2s[3])(ctDMA2s[3]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA2_Stream3_IRQn);
  if(fnPostNVICs[DMA2_Stream3_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA2_Stream3_IRQn])((u32)&NVIC_Stats[DMA2_Stream3_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA2_Stream4
__irq void DMA2_Stream4_IRQHandler(void) {//  DMA2_Stream4_IRQn           = 60,     /*!< DMA2 Stream 4 global Interrupt                                    */
  if(fnPreNVICs[DMA2_Stream4_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA2_Stream4_IRQn])((u32)&NVIC_Stats[DMA2_Stream4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA2s[4]) ((u32(*)(u32))fnDMA2s[4])(ctDMA2s[4]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA2_Stream4_IRQn);
  if(fnPostNVICs[DMA2_Stream4_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA2_Stream4_IRQn])((u32)&NVIC_Stats[DMA2_Stream4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebETH
//-u32 fnETH,ctETH;
__irq void ETH_IRQHandler(void) {//  ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                                         */
  if(fnPreNVICs[ETH_IRQn]) ((u32(*)(u32))fnPreNVICs[ETH_IRQn])((u32)&NVIC_Stats[ETH_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnETH) ((u32(*)(u32))fnETH)(ctETH); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else;//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
    NVIC_ReleasePendingIRQ(ETH_IRQn);
  if(fnPostNVICs[ETH_IRQn]) ((u32(*)(u32))fnPostNVICs[ETH_IRQn])((u32)&NVIC_Stats[ETH_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebETH_WKUP
//-u32 fnETH_WKUP,ctETH_WKUP;
__irq void ETH_WKUP_IRQHandler(void) {//  ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt                       */
  if(fnPreNVICs[ETH_WKUP_IRQn]) ((u32(*)(u32))fnPreNVICs[ETH_WKUP_IRQn])((u32)&NVIC_Stats[ETH_WKUP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnETH_WKUP) ((u32(*)(u32))fnETH_WKUP)(ctETH_WKUP); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(ETH_WKUP_IRQn);
  if(fnPostNVICs[ETH_WKUP_IRQn]) ((u32(*)(u32))fnPostNVICs[ETH_WKUP_IRQn])((u32)&NVIC_Stats[ETH_WKUP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebCAN2_TX
//-u32 fnCAN2_TX,ctCAN2_TX;
__irq void CAN2_TX_IRQHandler(void) {//  CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                                 */
  if(fnPreNVICs[CAN2_TX_IRQn]) ((u32(*)(u32))fnPreNVICs[CAN2_TX_IRQn])((u32)&NVIC_Stats[CAN2_TX_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnCAN2_TX) ((u32(*)(u32))fnCAN2_TX)(ctCAN2_TX); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(CAN2_TX_IRQn);
  if(fnPostNVICs[CAN2_TX_IRQn]) ((u32(*)(u32))fnPostNVICs[CAN2_TX_IRQn])((u32)&NVIC_Stats[CAN2_TX_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebCAN2_RX0
//-u32 fnCAN2_RX0,ctCAN2_RX0;
__irq void CAN2_RX0_IRQHandler(void) {//  CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                                */
  if(fnPreNVICs[CAN2_RX0_IRQn]) ((u32(*)(u32))fnPreNVICs[CAN2_RX0_IRQn])((u32)&NVIC_Stats[CAN2_RX0_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnCAN2_RX0) ((u32(*)(u32))fnCAN2_RX0)(ctCAN2_RX0); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(CAN2_RX0_IRQn);
  if(fnPostNVICs[CAN2_RX0_IRQn]) ((u32(*)(u32))fnPostNVICs[CAN2_RX0_IRQn])((u32)&NVIC_Stats[CAN2_RX0_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebCAN2_RX1
//-u32 fnCAN2_RX1,ctCAN2_RX1;
__irq void CAN2_RX1_IRQHandler(void) {//  CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                                */
  if(fnPreNVICs[CAN2_RX1_IRQn]) ((u32(*)(u32))fnPreNVICs[CAN2_RX1_IRQn])((u32)&NVIC_Stats[CAN2_RX1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnCAN2_RX1) ((u32(*)(u32))fnCAN2_RX1)(ctCAN2_RX1); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(CAN2_RX1_IRQn);
  if(fnPostNVICs[CAN2_RX1_IRQn]) ((u32(*)(u32))fnPostNVICs[CAN2_RX1_IRQn])((u32)&NVIC_Stats[CAN2_RX1_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebCAN2_SCE
//-u32 fnCAN2_SCE,ctCAN2_SCE;
__irq void CAN2_SCE_IRQHandler(void) {//  CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                                */
  if(fnPreNVICs[CAN2_SCE_IRQn]) ((u32(*)(u32))fnPreNVICs[CAN2_SCE_IRQn])((u32)&NVIC_Stats[CAN2_SCE_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnCAN2_SCE) ((u32(*)(u32))fnCAN2_SCE)(ctCAN2_SCE); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(CAN2_SCE_IRQn);
  if(fnPostNVICs[CAN2_SCE_IRQn]) ((u32(*)(u32))fnPostNVICs[CAN2_SCE_IRQn])((u32)&NVIC_Stats[CAN2_SCE_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebOTG_FS
//-u32 fnOTG_FS,ctOTG_FS;
__irq void OTG_FS_IRQHandler(void) {//  OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                                       */
  if(fnPreNVICs[OTG_FS_IRQn]) ((u32(*)(u32))fnPreNVICs[OTG_FS_IRQn])((u32)&NVIC_Stats[OTG_FS_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnOTG_FS) ((u32(*)(u32))fnOTG_FS)(ctOTG_FS); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(OTG_FS_IRQn);
  if(fnPostNVICs[OTG_FS_IRQn]) ((u32(*)(u32))fnPostNVICs[OTG_FS_IRQn])((u32)&NVIC_Stats[OTG_FS_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA2_Stream5
__irq void DMA2_Stream5_IRQHandler(void) {//  DMA2_Stream5_IRQn           = 68,     /*!< DMA2 Stream 5 global interrupt                                    */
  if(fnPreNVICs[DMA2_Stream5_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA2_Stream5_IRQn])((u32)&NVIC_Stats[DMA2_Stream5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA2s[5]) ((u32(*)(u32))fnDMA2s[5])(ctDMA2s[5]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA2_Stream5_IRQn);
  if(fnPostNVICs[DMA2_Stream5_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA2_Stream5_IRQn])((u32)&NVIC_Stats[DMA2_Stream5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA2_Stream6
__irq void DMA2_Stream6_IRQHandler(void) {//  DMA2_Stream6_IRQn           = 69,     /*!< DMA2 Stream 6 global interrupt                                    */
  if(fnPreNVICs[DMA2_Stream6_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA2_Stream6_IRQn])((u32)&NVIC_Stats[DMA2_Stream6_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA2s[6]) ((u32(*)(u32))fnDMA2s[6])(ctDMA2s[6]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA2_Stream6_IRQn);
  if(fnPostNVICs[DMA2_Stream6_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA2_Stream6_IRQn])((u32)&NVIC_Stats[DMA2_Stream6_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDMA2_Stream7
__irq void DMA2_Stream7_IRQHandler(void) {//  DMA2_Stream7_IRQn           = 70,     /*!< DMA2 Stream 7 global interrupt                                    */
  if(fnPreNVICs[DMA2_Stream7_IRQn]) ((u32(*)(u32))fnPreNVICs[DMA2_Stream7_IRQn])((u32)&NVIC_Stats[DMA2_Stream7_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDMA2s[7]) ((u32(*)(u32))fnDMA2s[7])(ctDMA2s[7]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DMA2_Stream7_IRQn);
  if(fnPostNVICs[DMA2_Stream7_IRQn]) ((u32(*)(u32))fnPostNVICs[DMA2_Stream7_IRQn])((u32)&NVIC_Stats[DMA2_Stream7_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebUSART6
//-u32 fnUSART6,ctUSART6;
__irq void USART6_IRQHandler(void) {//  USART6_IRQn                 = 71,     /*!< USART6 global interrupt                                           */
  if(fnPreNVICs[USART6_IRQn]) ((u32(*)(u32))fnPreNVICs[USART6_IRQn])((u32)&NVIC_Stats[USART6_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnUSART6) ((u32(*)(u32))fnUSART6)(ctUSART6); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(USART6_IRQn);
  if(fnPostNVICs[USART6_IRQn]) ((u32(*)(u32))fnPostNVICs[USART6_IRQn])((u32)&NVIC_Stats[USART6_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebI2C_EV
//-u32 fnI2C3_EV,ctI2C3_EV;
__irq void I2C3_EV_IRQHandler(void) {//  I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
  if(fnPreNVICs[I2C3_EV_IRQn]) ((u32(*)(u32))fnPreNVICs[I2C3_EV_IRQn])((u32)&NVIC_Stats[I2C3_EV_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnI2C3_EV) ((u32(*)(u32))fnI2C3_EV)(ctI2C3_EV); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(I2C3_EV_IRQn);
  if(fnPostNVICs[I2C3_EV_IRQn]) ((u32(*)(u32))fnPostNVICs[I2C3_EV_IRQn])((u32)&NVIC_Stats[I2C3_EV_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebI2C3_ER
//-u32 fnI2C3_ER,ctI2C3_ER;
__irq void I2C3_ER_IRQHandler(void) {//  I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
  if(fnPreNVICs[I2C3_ER_IRQn]) ((u32(*)(u32))fnPreNVICs[I2C3_ER_IRQn])((u32)&NVIC_Stats[I2C3_ER_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnI2C3_ER) ((u32(*)(u32))fnI2C3_ER)(ctI2C3_ER); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(I2C3_ER_IRQn);
  if(fnPostNVICs[I2C3_ER_IRQn]) ((u32(*)(u32))fnPostNVICs[I2C3_ER_IRQn])((u32)&NVIC_Stats[I2C3_ER_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebOTG_EP1_OUT
//-u32 fnOTG_HS_EP1_OUT,ctOTG_HS_EP1_OUT;
__irq void OTG_HS_EP1_OUT_IRQHandler(void) {//  OTG_HS_EP1_OUT_IRQn         = 74,     /*!< USB OTG HS End Point 1 Out global interrupt                       */
  if(fnPreNVICs[OTG_HS_EP1_OUT_IRQn]) ((u32(*)(u32))fnPreNVICs[OTG_HS_EP1_OUT_IRQn])((u32)&NVIC_Stats[OTG_HS_EP1_OUT_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnOTG_HS_EP1_OUT) ((u32(*)(u32))fnOTG_HS_EP1_OUT)(ctOTG_HS_EP1_OUT); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(OTG_HS_EP1_OUT_IRQn);
  if(fnPostNVICs[OTG_HS_EP1_OUT_IRQn]) ((u32(*)(u32))fnPostNVICs[OTG_HS_EP1_OUT_IRQn])((u32)&NVIC_Stats[OTG_HS_EP1_OUT_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebOTG_HR_EP1_IN
//-u32 fnOTG_HS_EP1_IN,ctOTG_HS_EP1_IN;
__irq void OTG_HS_EP1_IN_IRQHandler(void) {//  OTG_HS_EP1_IN_IRQn          = 75,     /*!< USB OTG HS End Point 1 In global interrupt                        */
  if(fnPreNVICs[OTG_HS_EP1_IN_IRQn]) ((u32(*)(u32))fnPreNVICs[OTG_HS_EP1_IN_IRQn])((u32)&NVIC_Stats[OTG_HS_EP1_IN_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnOTG_HS_EP1_IN) ((u32(*)(u32))fnOTG_HS_EP1_IN)(ctOTG_HS_EP1_IN); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(OTG_HS_EP1_IN_IRQn);
  if(fnPostNVICs[OTG_HS_EP1_IN_IRQn]) ((u32(*)(u32))fnPostNVICs[OTG_HS_EP1_IN_IRQn])((u32)&NVIC_Stats[OTG_HS_EP1_IN_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebOTG_HS_WKUP
//-u32 fnOTG_HS_WKUP,ctOTG_HS_WKUP;
__irq void OTG_HS_WKUP_IRQHandler(void) {//  OTG_HS_WKUP_IRQn            = 76,     /*!< USB OTG HS Wakeup through EXTI interrupt                          */
  if(fnPreNVICs[OTG_HS_WKUP_IRQn]) ((u32(*)(u32))fnPreNVICs[OTG_HS_WKUP_IRQn])((u32)&NVIC_Stats[OTG_HS_WKUP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnOTG_HS_WKUP) ((u32(*)(u32))fnOTG_HS_WKUP)(ctOTG_HS_WKUP); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(OTG_HS_WKUP_IRQn);
  if(fnPostNVICs[OTG_HS_WKUP_IRQn]) ((u32(*)(u32))fnPostNVICs[OTG_HS_WKUP_IRQn])((u32)&NVIC_Stats[OTG_HS_WKUP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebOTG_HS
//-u32 fnOTG_HS,ctOTG_HS;
__irq void OTG_HS_IRQHandler(void) {//  OTG_HS_IRQn                 = 77,     /*!< USB OTG HS global interrupt                                       */
  if(fnPreNVICs[OTG_HS_IRQn]) ((u32(*)(u32))fnPreNVICs[OTG_HS_IRQn])((u32)&NVIC_Stats[OTG_HS_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnOTG_HS) ((u32(*)(u32))fnOTG_HS)(ctOTG_HS); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(OTG_HS_IRQn);
  if(fnPostNVICs[OTG_HS_IRQn]) ((u32(*)(u32))fnPostNVICs[OTG_HS_IRQn])((u32)&NVIC_Stats[OTG_HS_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebDCMI
//-u32 fnDCMI,ctDCMI;
__irq void DCMI_IRQHandler(void) {//  DCMI_IRQn                   = 78,     /*!< DCMI global interrupt                                             */
  if(fnPreNVICs[DCMI_IRQn]) ((u32(*)(u32))fnPreNVICs[DCMI_IRQn])((u32)&NVIC_Stats[DCMI_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnDCMI) ((u32(*)(u32))fnDCMI)(ctDCMI); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(DCMI_IRQn);
  if(fnPostNVICs[DCMI_IRQn]) ((u32(*)(u32))fnPostNVICs[DCMI_IRQn])((u32)&NVIC_Stats[DCMI_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebCRYP
//-u32 fnCRYP,ctCRYP;
__irq void CRYP_IRQHandler(void) {//  CRYP_IRQn                   = 79,     /*!< CRYP crypto global interrupt                                      */
  if(fnPreNVICs[CRYP_IRQn]) ((u32(*)(u32))fnPreNVICs[CRYP_IRQn])((u32)&NVIC_Stats[CRYP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnCRYP) ((u32(*)(u32))fnCRYP)(ctCRYP); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(CRYP_IRQn);
  if(fnPostNVICs[CRYP_IRQn]) ((u32(*)(u32))fnPostNVICs[CRYP_IRQn])((u32)&NVIC_Stats[CRYP_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebHASH_RNG
//-u32 fnHASH_RNG,ctHASH_RNG;
__irq void HASH_RNG_IRQHandler(void) {//  HASH_RNG_IRQn               = 80,      /*!< Hash and Rng global interrupt                                     */
  if(fnPreNVICs[HASH_RNG_IRQn]) ((u32(*)(u32))fnPreNVICs[HASH_RNG_IRQn])((u32)&NVIC_Stats[HASH_RNG_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnHASH_RNG) ((u32(*)(u32))fnHASH_RNG)(ctHASH_RNG); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(HASH_RNG_IRQn);
  if(fnPostNVICs[HASH_RNG_IRQn]) ((u32(*)(u32))fnPostNVICs[HASH_RNG_IRQn])((u32)&NVIC_Stats[HASH_RNG_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebFPU
//-u32 fnFPU,ctFPU;
__irq void FPU_IRQHandler(void) {//  FPU_IRQn                    = 81,      /*!< FPU global interrupt                                              */
  if(fnPreNVICs[FPU_IRQn]) ((u32(*)(u32))fnPreNVICs[FPU_IRQn])((u32)&NVIC_Stats[FPU_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnFPU) ((u32(*)(u32))fnFPU)(ctFPU); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(FPU_IRQn);
  if(fnPostNVICs[FPU_IRQn]) ((u32(*)(u32))fnPostNVICs[FPU_IRQn])((u32)&NVIC_Stats[FPU_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebSPI4
//-u32 fnSPI4,ctSPI4;
__irq void SPI4_IRQHandler(void) {//  SPI4_IRQn                   = 84,     /*!< SPI4 global Interrupt                                             */
  if(fnPreNVICs[SPI4_IRQn]) ((u32(*)(u32))fnPreNVICs[SPI4_IRQn])((u32)&NVIC_Stats[SPI4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnSPI4) ((u32(*)(u32))fnSPI4)(ctSPI4); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(SPI4_IRQn);
  if(fnPostNVICs[SPI4_IRQn]) ((u32(*)(u32))fnPostNVICs[SPI4_IRQn])((u32)&NVIC_Stats[SPI4_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebSPI5
//-u32 fnSPI5,ctSPI5;
__irq void SPI5_IRQHandler(void) {//  SPI5_IRQn                   = 85,     /*!< SPI5 global Interrupt                                             */
  if(fnPreNVICs[SPI5_IRQn]) ((u32(*)(u32))fnPreNVICs[SPI5_IRQn])((u32)&NVIC_Stats[SPI5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnSPI5) ((u32(*)(u32))fnSPI5)(ctSPI5); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(SPI5_IRQn);
  if(fnPostNVICs[SPI5_IRQn]) ((u32(*)(u32))fnPostNVICs[SPI5_IRQn])((u32)&NVIC_Stats[SPI5_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
#ifdef SebSPI6
//-u32 fnSPI6,ctSPI6;
__irq void SPI6_IRQHandler(void) {//  SPI6_IRQn                   = 86,     /*!< SPI6 global Interrupt                                             */
  if(fnPreNVICs[SPI6_IRQn]) ((u32(*)(u32))fnPreNVICs[SPI6_IRQn])((u32)&NVIC_Stats[SPI6_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    if(fnSPI6) ((u32(*)(u32))fnSPI6)(ctSPI6); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
    else while(1);//TODO clear the pending flag EXTI->PR |= (1<<10);  // clear the pending flag
//-    NVIC_ReleasePendingIRQ(SPI6_IRQn);
  if(fnPostNVICs[SPI6_IRQn]) ((u32(*)(u32))fnPostNVICs[SPI6_IRQn])((u32)&NVIC_Stats[SPI6_IRQn]); // call either an empty function or a hooked one with desired context predefined (if context missing... error)
}
#endif
