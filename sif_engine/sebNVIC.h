
#ifndef _SEB_NVIC_H_
#define _SEB_NVIC_H_

// These are all the NVIC hooks (positive IRQs)
// When a single IRQ channel is shared by several interrupt sources, the hooks should be split. (for now done only for EXTI+FastIO)
extern u32 fnWWDG,ctWWDG;
extern u32 fnPVD,ctPVD;
extern u32 fnTAMP_STAMP,ctTAMP_STAMP;
extern u32 fnRTC_WKUP,ctRTC_WKUP;
extern u32 fnFLASH,ctFLASH;
extern u32 fnRCC,ctRCC;
extern u32 fnEXTIs[16], ctEXTIs[16];
extern u32 fnDMA1s[8],ctDMA1s[8];
extern u32 fnADC,ctADC;
extern u32 fnCAN1_TX,ctCAN1_RX;
extern u32 fnCAN1_RX0,ctCAN1_RX0;
extern u32 fnCAN1_RX1,ctCAN1_RX1;
extern u32 fnCAN1_SCE,ctCAN1_SCE;
extern u32 fnTIM1_BRK_TIM9,ctTIM1_BRK_TIM9; // this should be split in 2 different hooks
extern u32 fnTIM1_UP_TIM10,ctTIM1_UP_TIM10; // this should be split in 2 different hooks
extern u32 fnTIM1_TRG_COM_TIM11,ctTIM1_TRG_COM_TIM11; // this should be split in 2 different hooks
extern u32 fnTIM1_CC,ctTIM1_CC;
extern u32 fnTIM2,ctTIM2;
extern u32 fnTIM3,ctTIM3;
extern u32 fnTIM4,ctTIM4;
extern u32 fnI2C1_EV,ctI2C1_EV;
extern u32 fnI2C1_ER,ctI2C1_ER;
extern u32 fnI2C2_EV,ctI2C2_EV;
extern u32 fnI2C2_ER,ctI2C2_ER;
extern u32 fnSPI1,ctSPI1;
extern u32 fnSPI2,ctSPI2;
extern u32 fnUSART1,ctUSART1;
extern u32 fnUSART2,ctUSART2;
extern u32 fnUSART3,ctUSART3;
extern u32 fnRTC_Alarm,ctRTC_Alarm;
extern u32 fnOTG_FS_WKUP,ctOTG_FS_WKUP;
extern u32 fnTIM8_BRK_TIM12,ctTIM8_BRK_TIM12; // this should be flipped to 2 different hooks
extern u32 fnTIM8_UP_TIM13,ctTIM8_UP_TIM13; // this should be flipped to 2 different hooks
extern u32 fnTIM8_TRG_COM_TIM14,ctTIM8_TRG_COM_TIM14; // this should be flipped to 2 different hooks
extern u32 fnTIM8_CC,ctTIM8_CC;
extern u32 fnFSMC,ctFSMC;
extern u32 fnSDIO,ctSDIO;
extern u32 fnTIM5,ctTIM5;
extern u32 fnSPI3,ctSPI3;
extern u32 fnUART4,ctUART4;
extern u32 fnUART5,ctUART5;
extern u32 fnTIM6_DAC,ctTIM6_DAC; // to be split in 2 hooks?
extern u32 fnTIM7,ctTIM7;
extern u32 fnDMA2s[8],ctDMA2s[8];
extern u32 fnETH,ctETH;
extern u32 fnETH_WKUP,ctETH_WKUP;
extern u32 fnCAN2_TX1,ctCAN2_TX;
extern u32 fnCAN2_RX0,ctCAN2_RX0;
extern u32 fnCAN2_RX1,ctCAN2_RX1;
extern u32 fnCAN2_SCE,ctCAN2_SCE;
extern u32 fnOTG_FS,ctOTG_FS;
extern u32 fnUSART6,ctUSART6;
extern u32 fnI2C3_EV,ctI2C3_EV;
extern u32 fnI2C3_ER,ctI2C3_ER;
extern u32 fnOTG_HS_EP1_OUT,ctOTG_HS_EP1_OUT;
extern u32 fnOTG_HS_EP1_IN,ctOTG_HS_EP1_IN;
extern u32 fnOTG_HS_WKUP,ctOTG_HS_WKUP;
extern u32 fnOTG_HS,ctOTG_HS;
extern u32 fnDCMI,ctDCMI;
extern u32 fnCRYP,ctCRYP;
extern u32 fnHASH_RNG,ctHASH_RNG;
extern u32 fnFPU,ctFPU;
extern u32 fnSPI4,ctSPI4;
extern u32 fnSPI5,ctSPI5;
extern u32 fnSPI6,ctSPI6;


#endif
