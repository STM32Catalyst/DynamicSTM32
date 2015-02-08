
#ifndef _SEB_ENGINE_H_
#define _SEB_ENGINE_H_

#include "upgrade.h"

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

#include "myMCU.h"
#include "alternates.h"
#include "sebNVIC.h"
#include "sebDMA.h"
#include "sebEXTI.h"
#include "IO_Pin.h"
#include "sebBasicTimer.h"
#include "sebByteVein.h"
#include "sebStuffsArtery.h"

#include "I2C_SlaveIO.h"
#include "I2C_MasterIO.h"
#include "RS232_HW.h"
#include "SPI_MasterHW.h"

#include "sebSequencer.h"

#endif
