
#ifndef _SEB_ENGINE_H_
#define _SEB_ENGINE_H_

#include "upgrade.h"

#define ADD_EXAMPLES_TO_PROJECT // Comment this line if no example is needed in the project

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



#include "myMCU.h"
#include "SebClocks.h"
#include "alternates.h"
#include "sebNVIC.h"
#include "sebDMA.h"
#include "sebEXTI.h"
#include "IO_Pin.h"
#include "sebBasicTimer.h"
#include "sebByteVein.h"
#include "sebStuffsArtery.h"
#include "SebPrintf.h"


#include "I2C_SlaveIO.h"
#include "I2C_MasterIO.h"
#include "RS232_HW.h"
#include "SPI_MasterHW.h"
#include "SPI_MasterIO.h"
#include "sebSequencer.h"
#include "RFFE_MasterIO.h"    

#include "NHD_C0216CZ_LCD16x2.h" // needs SPI, BT

#endif
