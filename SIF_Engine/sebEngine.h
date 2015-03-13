
#ifndef _SEB_ENGINE_H_
#define _SEB_ENGINE_H_

// This file is the only one to include to use the code from this repo.

#include "upgrade.h" // This will include all the things your main project include files has. Can be anything outside this repo

#define ADD_EXAMPLES_TO_PROJECT // Comment this line to remove all examples from the project

// this tells which NVIC Interrupt handlers to activate
#define SebEXTI1
#define SebEXTI2
#define SebEXTI9_5
#define SebEXTI15_10
#define SebTIM6_DAC
#define SebTIM7
#define SebUSART1
#define SebDMA2_Stream0
#define SebDMA2_Stream1
#define SebDMA2_Stream2
#define SebDMA2_Stream3
#define SebDMA2_Stream4
#define SebDMA2_Stream5
#define SebDMA2_Stream6
#define SebDMA2_Stream7

#define SebTIM2
#define SebTIM3
#define SebTIM4
#define SebTIM5
#define SebTIM6
#define SebTIM7

#define SebTIM1_BRK_TIM9
#define SebTIM1_UP_TIM10
#define SebTIM1_TRG_COM_TIM11
#define SebTIM8_BRK_TIM12
#define SebTIM8_UP_TIM13
#define SebTIM8_TRG_COM_TIM14

#define SebADC

// These are types we use and might not be available for other compilers or libraries
/*!< STM32F10x Standard Peripheral Library old types (maintained for legacy purpose) */
#if 0 // activate these if the types don't exist in your project, we use compact type names because eyesight is narrow area, so we make it compact for source code.
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

//typedef const int32_t sc32;  /*!< Read Only */
//typedef const int16_t sc16;  /*!< Read Only */
//typedef const int8_t sc8;   /*!< Read Only */

//typedef __IO int32_t  vs32;
//typedef __IO int16_t  vs16;
//typedef __IO int8_t   vs8;

//typedef __I int32_t vsc32;  /*!< Read Only */
//typedef __I int16_t vsc16;  /*!< Read Only */
//typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

//typedef const uint32_t uc32;  /*!< Read Only */
//typedef const uint16_t uc16;  /*!< Read Only */
//typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

//typedef __I uint32_t vuc32;  /*!< Read Only */
//typedef __I uint16_t vuc16;  /*!< Read Only */
//typedef __I uint8_t vuc8;   /*!< Read Only */

//typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
//#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
#endif


// There is a specific include order to follow here
#include "alternates.h"
#include "myMCU.h"
#include "SebClocks.h"
#include "SebNVIC.h"
#include "SebDMA.h"
#include "SebEXTI.h"
#include "IO_Pin.h"
//#include "sebBasicTimer.h"
#include "SebTimer.h"
#include "SebByteVein.h"
#include "SebStuffsArtery.h"
#include "SebPrintf.h"
#include "SebDac.h"

#include "I2C_SlaveIO.h"
#include "I2C_MasterIO.h"

#include "SPI_MasterHW.h"
#include "SPI_MasterIO.h"
#include "SebSequencer.h"
#include "RFFE_MasterIO.h"    

#include "NHD_C0216CZ_LCD16x2.h" // needs SPI, Timer
#include "RS232_HW.h"

#include "SebButtons.h"

//=== Demos
#ifdef ADD_EXAMPLES_TO_PROJECT
#include "I2C_MasterIO_Demos.h"
#include "I2C_SlaveIO_Demos.h"
#include "TimerDemos.h"
#include "NHD_C0216CZ_LCD16x2_Demos.h"
#include "RFFE_MasterIO_Demos.h"
#include "RS232_HW_Demos.h"
#include "StuffsArteryDemos.h"
#include "ADC_Demos.h"

#endif



#endif
