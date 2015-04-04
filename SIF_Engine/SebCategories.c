
#include "SebEngine.h"
#include "SebCategories.h"

// This file contains things that helps find a HW configuration that would work
// It does not solve the constrains, it offers the possibility to put links between general to details dependencies, to go from wide to narrow choice.

// This will list the different ways to name signals very generally

// Here we list the general category of signals an MCU deal with the external world
// It does not include the system pins (BOOT, OSCILLATOR, RESET, SUPPLY)

typedef enum {
  
  NO_CELL_TYPE,
  SPIx,
  TIMx,
  USARTx,
  UARTx,
  ETHx,
  SYSx,
  RCCx,
  JTAGx,
} Race_t;

typedef enum {
  NO_PROFESSION,
  
  CH1,
  CH2,
  CH3,
  CH4,
  CH1N,
  CH2N,
  CH3N,
  CH4N,
  BKIN,
  ETR,
  
  CTS,
  RTS,
  TX,
  RX,

MII_CRS,
MII_RX_CLK,
RMII_REF_CLK,
MII_TX_CLK,
MDIO,
MII_COL,
MII_RX_DV,
RMII_CRS_DV,
PPS_OUT,
MII_TXD0,
MII_TXD1,
MII_TXD2,
MII_TXD3,
MII_RXD0,
MII_RXD1,
MII_RXD2,
MII_RXD3,
MII_RX_ER,
MII_TX_EN,
RMII_TX_EN,
RMII_RXD0,
RMII_RXD1,
RMII_TXD0,
RMII_TXD1,
MDC,
RMII_TX_EN,
  
  EVENT_OUT,
  MDIO,
  R0,
  R1,
  R2,
  R3,
  R4,
  R5,
  R6,
  R7,
  G0,
  G1,
  G2,
  G3,
  G4,
  G5,
  G6,
  G7,
  B0,
  B1,
  B2,
  B3,
  B4,
  B5,
  B6,
  B7,
  CLK,
  HSYNC,
  VSYNC,
  DE,
  CLK1,
  CLK2,
} Profession_t;

const PinAlternateDescription_t PAD[] = {

  
NO_ALTERNATE, // 0
//---- clock outputs
  { MCO1, RCCx, CLK1 },
  { MCO2, RCCx, CLK2 },
//---- TIM1
  { TIM1_CH1, TIMx, CH1 },
  { TIM1_CH2, TIMx, CH2 },
  { TIM1_CH3, TIMx, CH3 },
  { TIM1_CH4, TIMx, CH4 };
  { TIM1_CH1N, TIMx, CH1N },
  { TIM1_CH2N, TIMx, CH2N },
  { TIM1_CH3N, TIMx, CH3N },
  { TIM1_BKIN, TIMx, BKIN },
  { TIM1_ETR, TIMx, ETR },
//---- TIM2
  { TIM2_CH1, TIMx, CH1 },
  { TIM2_CH2, TIMx, CH2 },
  { TIM2_CH3, TIMx, CH3 },
  { TIM2_CH4, TIMx, CH4 },
  { TIM2_ETR, TIMx, ETR },
//---- TIM3
  { TIM3_CH1, TIMx, CH1 },
  { TIM3_CH2, TIMx, CH2 },
  { TIM3_CH3, TIMx, CH3 },
  { TIM3_CH4, TIMx, CH4 },
  { TIM3_ETR, TIMx, ETR },
//---- TIM4
  { TIM4_CH1, TIMx, CH1 },
  { TIM4_CH2, TIMx, CH2 },
  { TIM4_CH3, TIMx, CH3 },
  { TIM4_CH4, TIMx, CH4 },
  { TIM4_ETR, TIMx, ETR },
//---- TIM5
  { TIM5_CH1, TIMx, CH1 },
  { TIM5_CH2, TIMx, CH2 },
  { TIM5_CH3, TIMx, CH3 },
  { TIM5_CH4, TIMx, CH4 },
  { TIM5_ETR, TIMx, ETR },
//---- TIM6
  { TIM6_ETR, TIMx, ETR },
//---- TIM7
  { TIM7_ETR, TIMx, ETR },
//---- TIM8
  { TIM8_CH1, TIMx, CH1 },
  { TIM8_CH2, TIMx, CH2 },
  { TIM8_CH3, TIMx, CH3 },
  { TIM8_CH4, TIMx, CH4 },
  { TIM8_CH1N, TIMx, CH1N },
  { TIM8_CH2N, TIMx, CH2N },
  { TIM8_CH3N, TIMx, CH3N },
  { TIM8_BKIN, TIMx, BKIN },
  { TIM8_ETR, TIMx, ETR },
//---- TIM9
  { TIM9_CH1, TIMx, CH1 },
  { TIM9_CH2, TIMx, CH2 },
  { TIM9_ETR, TIMx, ETR },
//---- TIM10
  { TIM10_CH1, TIMx, CH1 },
  { TIM10_ETR, TIMx, ETR },
//---- TIM11
  { TIM11_CH1, TIMx, CH1 },
  { TIM11_ETR, TIMx, ETR },
//---- TIM12
  { TIM12_CH1, TIMx, CH1 },
  { TIM12_CH2, TIMx, CH2 },
  { TIM12_ETR, TIMx, ETR },
//---- TIM13
  { TIM13_CH1, TIMx, CH1 },
  { TIM13_ETR, TIMx, ETR },
//---- TIM14
  { TIM14_CH1, TIMx, CH1 },
  { TIM14_ETR, TIMx, ETR },
//--- I2C1
  { I2C1_SDA, I2Cx, SDA },
  { I2C1_SCL, I2Cx, SCL },
  { I2C1_SMBA, I2Cx, SMBA },
//--- I2C2
  { I2C2_SDA, I2Cx, SDA },
  { I2C2_SCL, I2Cx, SCL },
  { I2C2_SMBA, I2Cx, SMBA },
//--- I2C3
  { I2C3_SDA, I2Cx, SDA },
  { I2C3_SCL, I2Cx, SCL },
  { I2C3_SMBA, I2Cx, SMBA },
//--- SPI1
  { SPI1_MISO, SPIx, MISO },
  { SPI1_MOSI, SPIx, MOSI },
  { SPI1_SCK,  SPIx, SCK },
  { SPI1_NSS,  SPIx, NSS },
//--- SPI2
  { SPI2_MISO, SPIx, MISO },
  { SPI2_MOSI, SPIx, MOSI },
  { SPI2_SCK,  SPIx, SCK },
  { SPI2_NSS,  SPIx, NSS },
//--- SPI3
  { SPI3_MISO, SPIx, MISO },
  { SPI3_MOSI, SPIx, MOSI },
  { SPI3_SCK,  SPIx, SCK },
  { SPI3_NSS,  SPIx, NSS },
//--- SPI4
  { SPI4_MISO, SPIx, MISO },
  { SPI4_MOSI, SPIx, MOSI },
  { SPI4_SCK,  SPIx, SCK },
  { SPI4_NSS,  SPIx, NSS },
//--- SPI5
  { SPI5_MISO, SPIx, MISO },
  { SPI5_MOSI, SPIx, MOSI },
  { SPI5_SCK,  SPIx, SCK },
  { SPI5_NSS,  SPIx, NSS },
//--- SPI6
  { SPI6_MISO, SPIx, MISO },
  { SPI6_MOSI, SPIx, MOSI },
  { SPI6_SCK,  SPIx, SCK },
  { SPI6_NSS,  SPIx, NSS },
//--- USART1
  { USART1_RX, USARTx, RX },
  { USART1_TX, USARTx, TX },
  { USART1_CK, USARTx, CK },
  { USART1_RTS, USARTx, RTS },
  { USART1_CTS, USARTx, CTS },
//--- USART2
  { USART2_RX, USARTx, RX },
  { USART2_TX, USARTx, TX },
  { USART2_CK, USARTx, CK },
  { USART2_RTS, USARTx, RTS },
  { USART2_CTS, USARTx, CTS },
//--- USART3
  { USART3_RX, USARTx, RX },
  { USART3_TX, USARTx, TX },
  { USART3_CK, USARTx, CK },
  { USART3_RTS, USARTx, RTS },
  { USART3_CTS, USARTx, CTS },
//--- UART4
  { UART4_RX, UARTx, RX },
  { UART4_TX, UARTx, TX },
//--- UART5
  { UART5_RX, UARTx, RX },
  { UART5_TX, UARTx, TX },
//--- USART6
  { USART6_RX, USARTx, RX },
  { USART6_TX, USARTx, TX },
  { USART6_CK, USARTx, CK },
  { USART6_RTS, USARTx, RTS },
  { USART6_CTS, USARTx, CTS },
//--- UART7
  { UART7_RX, USARTx, RX },
  { UART7_TX, USARTx, TX },
//--- UART8
  { UART8_RX, USARTx, RX },
  { UART8_TX, USARTx, TX },
//--- CAN1
  { CAN1_RX, CANx, RX },
  { CAN1_TX, CANx, TX },
//--- CAN2
  { CAN2_RX, CANx, RX },
  { CAN2_TX, CANx, TX },
//--- LCD
  { LCD_R0, LCDx, R0 },
  { LCD_R1, LCDx, R1 },
  { LCD_R2, LCDx, R2 },
  { LCD_R3, LCDx, R3 },
  { LCD_R4, LCDx, R4 },
  { LCD_R5, LCDx, R5 },
  { LCD_R6, LCDx, R6 },
  { LCD_R7, LCDx, R7 },
  
  { LCD_G0, LCDx, G0 },
  { LCD_G1, LCDx, G1 },
  { LCD_G2, LCDx, G2 },
  { LCD_G3, LCDx, G3 },
  { LCD_G4, LCDx, G4 },
  { LCD_G5, LCDx, G5 },
  { LCD_G6, LCDx, G6 },
  { LCD_G7, LCDx, G7 },
  
  { LCD_B0, LCDx, B0 },
  { LCD_B1, LCDx, B1 },
  { LCD_B2, LCDx, B2 },
  { LCD_B3, LCDx, B3 },
  { LCD_B4, LCDx, B4 },
  { LCD_B5, LCDx, B5 },
  { LCD_B6, LCDx, B6 },
  { LCD_B7, LCDx, B7 },
  
  { LCD_CLK, LCDx, CLK },
  { LCD_DE, LCDx, DE },
  { LCD_HSYNC, LCDx, HSYNC },
  { LCD_VSYNC, LCDx, VSYNC },

//--- DCMI
  { DCMI_D0, DCMIx, D0 },
  { DCMI_D1, DCMIx, D1 },
{ DCMI_D2, DCMIx, D2 },
{ DCMI_D3, DCMIx, D3 },
{ DCMI_D4, DCMIx, D4 },
{ DCMI_D5, DCMIx, D5 },
{ DCMI_D6, DCMIx, D6 },
{ DCMI_D7, DCMIx, D7 },
{ DCMI_D8, DCMIx, D8 },
{ DCMI_D9, DCMIx, D9 },
{ DCMI_D10, DCMIx, D10 },
{ DCMI_D11, DCMIx, D11 },
{ DCMI_D12, DCMIx, D12 },
{ DCMI_D13, DCMIx, D13 },
{ DCMI_HSYNC, DCMIx, HSYNC },
{ DCMI_VSYNC, DCMIx, VSYNC },
{ DCMI_PIXCLK, DCMIx, CLK },
//--- SDIO
{ SDIO_D0, SDIOx, D0 },
{ SDIO_D1, SDIOx, D1 },
{ SDIO_D2, SDIOx, D2 },
{ SDIO_D3, SDIOx, D3 },
{ SDIO_D4, SDIOx, D4 },
{ SDIO_D5, SDIOx, D5 },
{ SDIO_D6, SDIOx, D6 },
{ SDIO_D7, SDIOx, D7 },
{ SDIO_CK, SDIOx, CK},
{ SDIO_CMD, SDIOx, CMD },
//--- OTG2_HS
//
//--- OTG1_FS
//
//--- OTG2_FS
//
//--- ETH
{ ETH_MII_CRS, ETHx, MII_CRS },
{ ETH_MII_RX_CLK, ETHx, MII_RX_CLK },
{ ETH_RMII_REF_CLK, ETHx, RMII_REF_CLK },
{ ETH_MII_TX_CLK, ETHx, MII_TX_CLK },

{ ETH_MDIO, ETHx, MDIO },
{ ETH_MII_COL, ETHx, MII_COL },
{ ETH_MII_RX_DV, ETHx, MII_RX_DV },
{ ETH_RMII_CRS_DV, ETHx, RMII_CRS_DV },
{ ETH_PPS_OUT, ETHx, PPS_OUT },

{ ETH_MII_TXD0, ETHx, TXD0 },
{ ETH_MII_TXD1, ETHx, TXD1 },
{ ETH_MII_TXD2, ETHx, TXD2 },
{ ETH_MII_TXD3, ETHx, TXD3 },

{ ETH_MII_RXD0, ETHx, RXD0 },
{ ETH_MII_RXD1, ETHx, RXD1 },
{ ETH_MII_RXD2, ETHx, RXD2 },
{ ETH_MII_RXD3, ETHx, RXD3 },

{ ETH_MII_RX_ER, ETHx, MII_RX_ER },
{ ETH_MII_TX_EN, ETHx, MII_TX_EN },
{ ETH_RMII_TX_EN, ETHx, RMII_TX_EN },

{ ETH_RMII_RXD0, ETHx, RMII_RXD0 },
{ ETH_RMII_RXD1, ETHx, RMII_RXD1 },

{ ETH_RMII_TXD0, ETHx, RMII_TXD0 },
{ ETH_RMII_TXD1, ETHx, RMII_TXD1 },

{ ETH_MDC, ETHx, MDC },
{ ETH_RMII_TX_EN, ETHx, RMII_TX_EN },

//--- FMC
//
//--- I2S3

{ I2S_CKIN,

{ I2S2_WS,
{ I2S2_SD,
{ I2S2_EXT_SD,
{ I2S2_MCK,
{ I2S2_CK,


{ I2S3_WS,
{ I2S3_CK,
{ I2S3_EXT_SD,
{ I2S3_SD,
{ I2C3_SMBAA,

//---- RTC
{ RTC_REFIN,

//--- EVENTOUT
  { EVENTOUT,  SYSx, EVENT_OUT },


//--- Internal only signals for DMA stream
{ SPI1_TX,
{ SPI1_RX,
{ SPI2_TX,
{ SPI2_RX,
{ SPI3_TX,
{ SPI3_RX,
{ SPI4_TX,
{ SPI4_RX,
{ SPI5_TX,
{ SPI5_RX,
{ SPI6_TX,
{ SPI6_RX,
{ CRYP_IN,
{ CRYP_OUT,
{ HASH_IN,
{ I2C1_RX,
{ I2C1_TX,
{ I2C2_RX,
{ I2C2_TX,
{ I2C3_RX,
{ I2C3_TX,
{ I2S2_EXT_RX,
{ I2S2_EXT_TX,
{ I2S3_EXT_RX,
{ I2S3_EXT_TX,
{ TIM1_UP,
{ TIM2_UP,
{ TIM3_UP,
{ TIM4_UP,
{ TIM5_UP,
{ TIM6_UP,
{ TIM7_UP,
{ TIM8_UP,
{ TIM9_UP,
{ TIM10_UP,
{ TIM11_UP,
{ TIM12_UP,
{ TIM13_UP,
{ TIM14_UP,
{ TIM3_TRIG,
{ DAC1_ANALOG,
{ DAC2_ANALOG,
{ ADC1_ANALOG,
{ ADC2_ANALOG,
{ ADC3_ANALOG,
{ TIM1_TRIG,
{ SAI1_A,
{ DCMI_BUS,
{ SDIO_BUS,
{ SAI1_B,
{ TIM1_COM,
{ TIM8_COM,
{ TIM8_TRIG,
{ TIM5_TRIG,
//=========> Analog input pins
//ADC123_IN0
{ ADC1_IN0,
{ ADC2_IN0,
{ ADC3_IN0,
//ADC123_IN1,
{ ADC1_IN1,
{ ADC2_IN1,
{ ADC3_IN1,
//ADC123_IN2,
{ ADC1_IN2,
{ ADC2_IN2,
{ ADC3_IN2,
//ADC123_IN3,
{ ADC1_IN3,
{ ADC2_IN3,
{ ADC3_IN3,
//ADC123_IN4,
{ ADC1_IN4,
{ ADC2_IN4,
{ ADC3_IN4,
//ADC123_IN5,
{ ADC1_IN5,
{ ADC2_IN5,
{ ADC3_IN5,
//ADC123_IN6,
{ ADC1_IN6,
{ ADC2_IN6,
{ ADC3_IN6,
//ADC123_IN7,
{ ADC1_IN7,
{ ADC2_IN7,
{ ADC3_IN7,
//ADC12_IN8,
{ ADC1_IN8,
{ ADC2_IN8,
{ ADC3_IN8,
//ADC12_IN9,
{ ADC1_IN9,
{ ADC2_IN9,
{ ADC3_IN9,
//ADC123_IN10,
{ ADC1_IN10,
{ ADC2_IN10,
{ ADC3_IN10,
//ADC123_IN11,
{ ADC1_IN11,
{ ADC2_IN11,
{ ADC3_IN11,
//ADC123_IN12,
{ ADC1_IN12,
{ ADC2_IN12,
{ ADC3_IN12,
//ADC123_IN13,
{ ADC1_IN13,
{ ADC2_IN13,
{ ADC3_IN13,
//ADC123_IN14,
{ ADC1_IN14,
{ ADC2_IN14,
{ ADC3_IN14,
//ADC123_IN15,
{ ADC1_IN15,
{ ADC2_IN15,
{ ADC3_IN15,

//ADC123_VTEMP,
{ ADC1_VTEMP,
{ ADC2_VTEMP,
{ ADC3_VTEMP,
//ADC123_VREF,
{ ADC1_VREF,
{ ADC2_VREF,
{ ADC3_VREF,
//ADC123_VBAT,
{ ADC1_VBAT,
{ ADC2_VBAT,
{ ADC3_VBAT,
  
  
  
  
  
  
  
{ TIM2_CH1, TIMx, CH1 },
{ TIM2_ETR, TIMx, ETR },
{ TIM5_CH1, TIMx, CH1 },
{ TIM8_ETR, TIMx, ETR },
{ USART2_CTS, USARTx, CTS },
{ UART4_TX, UARTx, TX},
{ ETH_MII_CRS, ETHx, MII_CRS },

{ TIM2_CH2, TIMx, CH2 },
{ TIM5_CH2, 2, "TIM5_CH2", CH2 },
{ USART2_RTS, 7, "USART2_RTS", RTS },
{ UART4_RX, 8, "UART4_RX", RX },
{ ETH_MII_RX_CLK, 11, "ETH_MII_RX_CLK", ETHx, MII_RX_CLK },
{ ETH_RMII_REF_CLK, 11, "ETH_RMII_REF_CLK", ETHx, RMII_REF_CLK },
{ EVENTOUT, 15, "EVENTOUT", SYSx, EVENT_OUT },

{ TIM2_CH3, 1, "TIM2_CH3", TIMx, CH3 },
{ TIM5_CH3, 2, "TIM5_CH2", TIMx, CH2 },
{ TIM9_CH1, 3, "TIM9_CH1", TIMx, CH1 },
{ USART2_TX, 7, "USART2_TX", USARTx, TX },
{ ETH_MDIO, 11, "ETH_MDIO", ETHx, MDIO },

{ TIM2_CH4, 1, "TIM2_CH4", TIMx, CH4 },
{ TIM5_CH4, 2, "TIM5_CH4", TIMx, CH4 },
{ TIM9_CH2, 3, "TIM9_CH2",  },
{ USART2_RX, 7, "USART2_RX" },
//{ OTG_HS_ULPI_D0, 10, "OTG_HS_ULPI_D0" },
{ ETH_MII_COL, 11, "ETH_MII_COL" },

{ I2S3_WS, 6, "I2S3_WR" },
{ USART2_CK, 7, "USART2_CK" },
//{ OTG_HS_SOF, 12 , "OTG_HS_SOF" },
{ DCMI_HSYNC, 13, "DCMI_HSYNC" },

{ TIM2_CH1, 1, "TIM2_CH1" },
{ TIM2_ETR, 1, "TIM2_ETR" },
{ TIM8_CH1N, 3, "TIM8_CH1N" },
// ULPI_CLK

{ TIM1_BKIN, 1, "TIM1_BKIN" },
{ TIM3_CH1, 2, "TIM3_CH1" },
{ TIM8_BKIN, 3, "TIM8_BKIN" },
{ TIM13_CH1, 9, "TIM13_CH1" },
{ DCMI_PIXCLK, 13, "DCMI_PIXCLK" },

{ TIM1_CH1N, 1, "TIM1_CH1N" },
{ TIM3_CH2, 2, "TIM3_CH2" },
{ TIM8_CH1N, 3, "TIM8_CH1N" },
{ TIM14_CH1, 9, "TIM14_CH1" },
{ ETH_MII_RX_DV, 11, "ETH_MII_RX_DV" },
{ ETH_RMII_CRS_DV, 11, "ETH_RMII_CRS_DV" },

{ MCO1, 0, "MCO1" },
{ TIM1_CH1, 1, "TIM1_CH1" },
{ I2C3_SCL, 4, "I2C3_SCL" },
{ USART1_CK, 7, "USART1_CK" },
//{ OTG_FS_SOF, 10, "OTG_FS_SOF" },

{ TIM1_CH2, 1, "TIM1_CH2" },
{ I2C3_SMBA, 4, "I2C3_SMBA" },
{ USART1_TX, 7, "USART1_TX" },
{ DCMI_D0, 13, "DCMI_D0" },

{ TIM1_CH3, 1, "TIM1_CH3" },
{ USART1_RX, 7, "USART1_RX" },
//{ OTG_FS_ID, 10, "OTG_FS_ID" },
{ DCMI_D1, 13, "DCMI_D1" },

{ TIM1_CH4, 1, "TIM1_CH4" },
{ USART1_CTS, 7, "USART1_CTS" },
{ CAN1_RX, 9, "CAN1_RX" },
//{ OTG_FS_DM, 10, "OTG_FS_DM" },

{ TIM1_ETR, 1, "TIM1_ETR" },
{ USART1_RTS, 7, "USART1_RTS" },
{ CAN1_TX, 9, "CAN1_TX" },
//{ OTG_FS_DP, 10, "OTG_FS_DP" },

//{ JTMS_SWDIO, 0, "JTMS_SWDIO" },

//{ JTCK_SWCLK, 0, "JTCK_SWDCLK" },

//{ JTDI, SYSx, "JTDI" },
{ TIM2_CH1, 1, "TIM2_CH1" },
{ TIM2_ETR, 1, "TIM2_ETR" },
{ I2S3_WS, 6, "I2S3_WS" },

{ TIM1_CH2N, 1, "TIM1_CH2N" },
{ TIM3_CH3, 2, "TIM2_CH3" },
{ TIM8_CH2N, 3, "TIM8_CH2N" },
{ LCD_R3, 9, "LCD_R3" },
//{ OTG_HS_ULPI_D1, 10, "OTG_HS_ULPI_D1" },
{ ETH_MII_RXD2, 11, "ETH_MII_RXD2" },

{ TIM1_CH3N, 1, "TIM1_CH3N" },
{ TIM3_CH4, 2, "TIM3_CH4" },
{ TIM8_CH3N, 3, "TIM8_CH3N" },
//{ OTG_HS_ULPI_D2, 10, "OTG_HS_ULPI_D2" },
{ ETH_MII_RXD3, 11, "ETH_MII_RXD3" },

//{ JTD0, SYSx, "JTDO" },
//{ TRACSWO, SYSx, "TRACSWO" },
{ TIM2_CH2, 1, "TIM2_CH2" },
{ I2S3_CK, 5, "I2S3_CK" },

//{ NJTRST, SYSx, "NJTRST" },
{ TIM3_CH1, 2, "TIM3_CH1" },
{ I2S3_EXT_SD, 7, "I2S3_EXT_SD" },

{ TIM3_CH2, 2, "TIM3_CH2" },
{ I2C1_SMBA, 4, "I2C1_SMBA" },
{ I2S3_SD, 6, "I2S3_SD" },
{ CAN2_RX, 9, "CAN2_RX" },
//{ OTG_HS_ULPI_D7, 10, "OTG_HS_ULPI_D7" },
{ ETH_PPS_OUT, 11, "ETH_PPS_OUT" },
//{ FMC_SDCKE1, 12, "FMC_SDCKE1" },
{ DCMI_D10, 13, "DCMI_D10" },

{ TIM4_CH1, 2, "TIM4_CH1" },
{ I2C1_SCL, 4, "I2C1_SCL" },
{ USART1_TX, 7, "USART1_TX" },
{ CAN2_TX, 9, "CAN2_TX" },
//{ FMC_SDNE1, 12, "FMC_SDNE1" },
{ DCMI_D5, 13, "DCMI_D5" },

{ TIM4_CH2, 2, "TIM4_CH2" },
{ I2C1_SDA, 4, "I2C1_SDA" },
{ USART1_RX, 7, "USART1_RX" },
//{ FMC_NL, 12, "FMC_NL" },
{ DCMI_VSYNC, 13, "DCMI_VSYNC" },

{ TIM4_CH3, 2, "TIM4_CH3" },
{ TIM10_CH1, 3, "TIM10_CH1" },
{ I2C1_SCL, 4, "I2C1_SCL" },
{ CAN1_RX, 9, "CAN1_RX" },
{ ETH_MII_TXD3, 11, "ETH_MII_TXD3" },
{ SDIO_D4, 12, "SDIO_D4" },
{ DCMI_D6, 13, "DCMI_D6" },

{ TIM4_CH4, 2, "TIM4_CH4" },
{ TIM11_CH1, 3, "TIM11_CH1" },
{ I2C1_SDA, 4, "I2C1_SDA" },
{ I2S2_WS, 5, "I2S2_WS" },
{ CAN1_TX, 9, "CAN1_TX" },
{ SDIO_D5, 12, "SDIO_D5" },
//{ PB9, (u32) , , , "" },
{ DCMI_D7, 13, "DCMI_D7" },

{ TIM2_CH3, 1, "TIM2_CH3" },
{ I2C2_SCL, 4, "I2C2_SCL" },
{ SPI2_SCK, 5, "SPI2_SCK" },
{ USART3_TX, 7, "USART3_TX" },
//{ OTG_HS_ULPI_D3, 10, "OTG_ULPI_D3" },
{ ETH_MII_RX_ER, 11, "ETH_MII_RX_ER" },
{ LCD_G4, 14, "LCD_G4" },

{ TIM2_CH4, 1, "TIM2_CH4" },
{ I2C2_SDA, 4, "I2C2_SDA" },
{ USART3_RX, 7, "USART3_RX" },
//{ OTG_HS_ULPI_D4, 10, "OTG_HS_ULPI_D4" },
{ ETH_MII_TX_EN, 11, "ETH_MII_TX_EN" },
{ ETH_RMMII_TX_EN, 11, "ETH_RMMII_TX_EN" },
{ LCD_G5, 14, "LCD_G5" },

{ TIM1_BKIN, 1, "TIM1_BKIN" },
{ I2C2_SMBA, 4, "I2C2_SMBA" },
{ I2S2_WS, 5, "I2S2_WS" },
{ USART3_CK, 7, "USART3_CK" },
{ CAN2_RX, 9, "CAN2_RX" },
//{ OTG_HS_ULPI_D6, 10, "OTG_HS_ULPI_D6" },
{ ETH_MII_TXD0, 11, "ETH_MII_TXD0" },
{ ETH_RMII_TXD0, 11, "ETH_RMII_TXD0" },
//{ OTG_HS_ID, 12, "OTG_HS_ID" },

{ TIM1_CH1N, 1, "TIM1_CH1N" },
{ USART3_CTS, 7, "USART3_CTS" },
{ CAN2_TX, 9, "CAN2_TX" },
//{ OTG_HS_ULPI_D6, 10, "OTG_HS_ULPI_D6" },
{ ETH_MII_TXD1, 11, "ETH_MII_TXD1" },
{ ETH_RMII_TXD1, 11, "ETH_RMII_TX_D1" },

{ TIM1_CH2N, 1, "TIM1_CH2N" },
{ TIM8_CH2N, 3, "TIM8_CH2N" },
{ I2S2_EXT_SD, 6, "I2S2_EXT_SD" },
{ USART3_RTS, 7, "USART3_RTS" },
{ TIM12_CH1, 9, "TIM12_CH1" },
//{ OTG_HS_DM, 12, "OTG_HS_DM" },

{ RTC_REFIN, 0, "RTC_REFIN" },
{ TIM1_CH3N, 1, "TIM1_CH3N" },
{ TIM8_CH3N, 3, "TIM8_CH3N" },
{ I2S2_SD, 5, "I2S2_SD" },
{ TIM12_CH2, 9, "TIM12_CH2" },
//{ OTG_HS_DP, 12, "OTG_HS_DP" },


//{ OTG_HS_ULPI_STP, 10, "OTG_HS_ULPI_STP" },
//{ FMC_SDN_WE, 12, "FMC_SDN_WE" },

{ ETH_MDC, 11, "ETH_MDC" },

{ I2S2_EXT_SD, 6, "I2S2_EXT_SD" },
//{ OTG_HS_ULPI_DIR, 10, "OTG_HS_ULPI_DIR" },
{ ETH_MII_TXD2, 11, "ETH_MII_TXD2" },
//{ FMC_SDNE0, 12, "FMC_SDNE0" },

{ I2S2_SD, 5, "I2S2_SD" },
//{ OTG_HS_ULPI_NXT, 10, "OTG_ULPI_NXT" },
{ ETH_MII_TX_CLK, 11, "ETH_MII_TX_CLK" },
//{ FMC_SDCKE0, 12, "FMC_SDCKE0" },

{ ETH_MII_RXD0, 11, "ETH_MII_RXD0" },
{ ETH_RMII_RXD0, 11, "ETH_RMII_RXD0" },

{ ETH_MII_RXD1, 11, "ETH_MII_RXD1" },
{ ETH_RMII_RXD1, 11, "ETH_RMII_RXD1" },

{ TIM3_CH1, 2, "TIM3_CH1" },
{ TIM8_CH1, 3, "TIM8_CH1" },
{ I2S2_MCK, 5, "I2S2_MCK" },
{ USART6_TX, 8, "USART6_TX" },
{ SDIO_D6, 13, "SDIO_D6" },
{ DCMI_D0, 13, "DCMI_D0" },
{ LCD_HSYNC, 14, "LCD_HSYNC" },

{ TIM3_CH2, 2, "TIM3_CH2" },
{ TIM8_CH2, 3, "TIM8_CH2" },
{ USART6_RX, 8, "USART6_RX" },
{ SDIO_D7, 12, "SDIO_D7" },
{ DCMI_D1, 13, "DCMI_D1" },
{ LCD_G6, 14, "LCD_G6" },

{ TIM3_CH3, 2, "TIM3_CH3" },
{ TIM8_CH3, 3, "TIM8_CH3" },
{ USART6_CK, 8, "USART6_CK" },
{ SDIO_D0, 12, "SDIO_D0" },
{ DCMI_D2, 13, "DCMI_D2" },

{ MCO2, RCCx, "MCO2" },
{ TIM3_CH4, 2, "TIM3_CH4" },
{ TIM8_CH4, 3, "TIM8_CH4" },
{ I2C3_SDA, 4, "I2C3_SDA" },
//{ I2S_CKIN, 5, "I2S_CKIN" },
{ SDIO_D1, 12, "SDIO_D1" },
{ DCMI_D3, 13, "DCMI_D3" },

{ I2S3_CK, 6, "I2S3_CK" },
{ USART3_TX, 7, "USART3_TX" },
{ UART4_TX, 8, "UART4_TX" },
{ SDIO_D2, 12, "SDIO_D2" },
{ DCMI_D8, 13, "DCMI_D8" },
{ LCD_R2, 14, "LCD_R2" },

{ I2S3_EXT_SD, 5, "I2S3_EXT_SD" },
{ SPI3_MISO, 6, "SPI_MISO" },
{ USART3_RX, 7, "USART3_RX" },
{ UART4_RX, 8, "UART4_RX" },
{ SDIO_D3, 12, "SDIO_D3" },
{ DCMI_D4, 13, "DCMI_D4" },

{ I2S3_SD, 6, "I2S3_SD" },
{ USART3_CK, 7, "USART3_CK" },
{ UART5_TX, 8, "UART5_TX" },
{ SDIO_CK, 12, "SDIO_CK" },
{ DCMI_D9, 13, "DCMI_D9" },

{ CAN1_RX, 9, "CAN1_RX" },
//{ FMC_D2, 12, "FMC_D2" },

{ CAN1_TX, 9, "CAN1_TX" },
//{ FMC_D3, 12, "FMC_D3" },

{ TIM3_ETR, 2, "TIM3_ETR" },
{ UART5, UART5_RX, 8, "UART5_RX" },
{ SDIO_CMD, 12, "SDIO_CMD" },
{ DCMI, DCMI_D11, 13, "DCMI_D11" },

{ I2S2_CK, 5, "I2S2_CK" },
{ USART2_CTS, 7, "USART2_CTS" },
//{ FMC_CLK, 12, "FMC_CLK" },
{ DCMI_D5, 13, "DCMI_D5" },

{ USART2_RTS, 7, "USART2_RTS" },
//{ FMC_NOE, 12, "FMC_NOE" },

{ USART2_TX, 7, "USART2_TX" },
//{ FMC_NWE, 12, "FMC_NWE" },

{ I2S3_SD, 5, "I2S3_SD" },
//{ SAI1_SD_A, 6, "SAI1_SD_A" },
{ USART2_RX, 7, "USART2_RX" },
//{ FMC_NWAIT, 12, "FMC_NWAIT" },
{ DCMI_D10, 13, "DCMI_D10" },
{ LCD_B2, 14, "LCD_B2" },

{ PD7, (u32) USART2, USART2_CK, 7, "USART2_CK" },
//{ PD7, (u32) FMC, FMC_NE1, 12, "FMC_NE1" },
//{ PD7, (u32) FMC, FMC_NCE2, 12, "FMC_NCE2" },

{ PD8, (u32) USART3, USART3_TX, 7, "USART3_TX" },
//{ PD8, (u32) FMC, FMC_D13, 12, "FMC_D13" },

{ PD9, (u32) USART3, USART3_RX, 7, "USART3_RX" },
//{ PD9, (u32) FMC, FMC_D14, 12, "FMC_D14" },

{ PD10, (u32) USART3, USART3_CK, 7, "USART3_CK" },
//{ PD10, (u32) FMC, FMC_D14, 12, "FMC_D14" },
{ PD10, (u32) LCD, LCD_B3, 14, "LCD_B3" },

{ PD11, (u32) USART3, USART3_CTS, 7, "USART3_CTS" },
//{ PD11, (u32) FMC, FMC_A16, 12, "FMC_A16" },

{ PD12, (u32) TIM4, TIM4_CH1, 2, "TIM4_CH1" },
{ PD12, (u32) USART3, USART3_RTS, 7, "USART3_RTS" },
//{ PD12, (u32) FMC, FMC_A17, 12, "FMC_A17" },

{ PD13, (u32) TIM4, TIM4_CH2, 2, "TIM4_CH2" },
//{ PD13, (u32) FMC, FMC_A18, 12, "FMC_A18" },

{ PD14, (u32) TIM4, TIM4_CH3, 3, "TIM4_CH3" },
//{ PD14, (u32) FMC, FMC_D0, 12, "FMC_D0" },

{ PD15, (u32) TIM4, TIM4_CH4, 2, "TIM4_CH4" },
//{ PD15, (u32) FMC, FMC_D1, 12, "FMC_D1" },

{ PE0, (u32) TIM4, TIM4_ETR, 2, "TIM4_ETR" },
//??{ PE0, (u32) UART8, UART8_RX, 8, "UART8_RX" },
//{ PE0, (u32) FMC, FMC_NBL0, 12, "FMC_NBL0" },
{ PE0, (u32) DCMI, DCMI_D2, 13, "DCMI_D2" },

//?{ PE1, (u32) UART8, UART8_TX, 8, "UART8_TX" },
//{ PE1, (u32) FMC, FMC_NBL1, 12, "FMC_NBL1" },
{ PE1, (u32) DCMI, DCMI_D3, 13, "DCMI_D3" },

//{ PE2, (u32) SYS, TRACECLK, 0, "TRACECLK" },
//{ PE2, (u32) SAI1, SAI1_MCLK_A, 6, "SAI1_MCLK_A" },
{ PE2, (u32) ETH, ETH_MII_TXD3, 11, "ETH_MII_TXD3" },
//{ PE2, (u32) FMC, FMC_A23, 12, "FMC_A23" },

//{ PE3, (u32) SYS, TRACED0, 0, "TRACED0" },
//{ PE3, (u32) SAI1, SAI1_SD_B, 6, "SAI1_SD_B" },
//{ PE3, (u32) FMC, FMC_A19, 12, "FMC_A19" },

//{ PE4, (u32) SAI1, SAI1_FS_A, 6, "SAI1_FS_A" },
//{ PE4, (u32) FMC, FMC_A20, 12, "FMC_A20" },
{ PE4, (u32) DCMI, DCMI_D4, 13, "DCMI_D4" },
{ PE4, (u32) LCD, LCD_B0, 14, "LCD_B0" },

//{ PE5, (u32) SYS, TRACED2, 0, "TRACED2" },
{ PE5, (u32) TIM9, TIM9_CH1, 3, "TIM9_CH1" },
//{ PE5, (u32) SAI1, SAI1_SCK_A, 6, "SAI1_SCK_A" },
//{ PE5, (u32) FMC, FMC_A21, 12, "FMC_A21" },
{ PE5, (u32) DCMI, DCMI_D6, 13, "DCMI_D6" },
{ PE5, (u32) LCD, LCD_G0, 14, "LCD_G0" },

//{ PE6, (u32) SYS, TRACED3, 0, "TRACED3" },
{ PE6, (u32) TIM9, TIM9_CH2, 3, "TIM9_CH2" },
//{ PE6, (u32) SAI1, SAI1_SD_A, 6, "SAI1_SD_A" },
//{ PE6, (u32) FMC, FMC_A22, 12, "FMC_A22" },
{ PE6, (u32) DCMI, DCMI_D7, 13, "DCMI_D7" },
{ PE6, (u32) LCD, LCD_G1, 14, "LCD_G1" },

{ PE7, (u32) TIM1, TIM1_ETR, 1, "TIM1_ETR" },
//{ PE7, (u32) UART7, UART7_RX, 8, "UART7_RX" },
//{ PE7, (u32) FMC, FMC_D4, 12, "FMC_D4" },

{ PE8, (u32) TIM1, TIM1_CH1N, 1, "TIM1_CH1N" },
//{ PE8, (u32) UART7, UART7_TX, 8, "UART7_TX" },
//{ PE8, (u32) FMC, FMC_D5, 12, "FMC_D5" },

{ PE9, (u32) TIM1, TIM1_CH1, 1, "TIM1_CH1" },
//{ PE9, (u32) FMC, FMC_D6, 12, "FMC_D6" },

{ PE10, (u32) TIM1, TIM1_CH2N, 1, "TIM1_CH2N" },
//{ PE10, (u32) FMC, FMC_D7, 12, "FMC_D7" },

{ PE11, (u32) TIM1, TIM1_CH2, 1, "TIM1_CH2" },
//{ PE11, (u32) FMC, FMC_D8, 12, "FMC_D8" },
{ PE11, (u32) LCD, LCD_G3, 14, "LCD_G3" },

{ PE12, (u32) TIM1, TIM1_CH3N, 1, "TIM1_CH3N" },
//{ PE12, (u32) FMC, FMC_D9, 12, "FMC_D9" },
{ PE12, (u32) LCD, LCD_B4, 14, "LCD_B4" },

{ PE13, (u32) TIM1, TIM1_CH3, 1, "TIM1_CH3" },
//{ PE13, (u32) FMC, FMC_D10, 12, "FMC_D10" },
{ PE13, (u32) LCD, LCD_DE, 14, "LCD_DE" },

{ PE14, (u32) TIM1, TIM1_CH4, 1, "TIM1_CH4" },
//{ PE14, (u32) FMC, FMC_D11, 12, "FMC_D11" },
{ PE14, (u32) LCD, LCD_CLK, 14, "LCD_CLK" },

{ PE15, (u32) TIM1, TIM1_BKIN, 1, "TIM1_BKIN" },
//{ PE15, (u32) FMC, FMC_D12, 12, "FMC_D12" },
{ PE15, (u32) LCD, LCD_R7, 14, "LCD_R7" },

{ PF0, (u32) I2C2, I2C2_SDA, 4, "I2S2_SDA" },
//{ PF0, (u32) FMC, FMC_A0, 12, "FMC_A0" },

{ PF1, (u32) I2C2, I2C2_SCL, 4, "I2C2_SCL" },
//{ PF1, (u32) FMC, FMC_A1, 12, "FMC_A1" },

{ PF2, (u32) I2C2, I2C2_SMBA, 4, "I2C2_SMBA" },
//{ PF2, (u32) FMC, FMC_A2, 12, "FMC_A2" },

//{ PF3, (u32) FMC, FMC_A3, 12, "FMC_A3" },

//{ PF4, (u32) FMC, FMC_A4, 12, "FMC_A4" },

//{ PF5, (u32) FMC, FMC_A5, 12, "FMC_A5" },

{ PF6, (u32) TIM10, TIM10_CH1, 3, "TIM10_CH1" },
//{ PF6, (u32) SAI1, SAI1_SD_B, 6, "SAI1_SD_B" },
//?{ PF6, (u32) UART7, UART7_TX, 8, "UART7_TX" },
//{ PF6, (u32) FMC, FMC_NIORD, 12, "FMC_NIORD" },

{ PF7, (u32) TIM11, TIM11_CH1, 3, "TIM11_CH1" },
//{ PF7, (u32) SAI1, SAI1_MCLK_B, 6, "SAI1_MCLK_B" },
//{ PF7, (u32) UART7, UART7_RX, 8, "UART7_RX" },
//{ PF7, (u32) FMC, FMC_NREG, 12, "FMC_NREG" },

//{ PF8, (u32) SAI1, SAI1_SCK_B, 6, "SAI1_SCK_B" },
{ PF8, (u32) TIM13, TIM13_CH1, 9, "TIM13_CH1" },
//{ PF8, (u32) FMC, FMC_NIOWR, 12, "FMC_NIOWR" },

//{ PF9, (u32) SAI1, SAI1_FS_B, 6, "SAI1_FS_B" },
{ PF9, (u32) TIM14, TIM14_CH1, 9, "TIM14_CH1" },
//{ PF9, (u32) FMC, FMC_CD, 12, "FMC_CD" },

//{ PF10, (u32) FMC, FMC_INTR, 12, "FMC_INTR" },
{ PF10, (u32) DCMI, DCMI_D11, 13, "DCMI_D11" },
{ PF10, (u32) LCD, LCD_DE, 14, "LCD_DE" },

{ PF11, (u32) SPI5, SPI5_MOSI, 5, "SPI5_MOSI" },
//{ PF11, (u32) FMC, FMC_SDNRAS, 12, "FMC_SDNRAS" },
{ PF11, (u32) DCMI, DCMI_D12, 13, "DCMI_D12" },

//{ PF12, (u32) FMC, FMC_A6, 12, "FMC_A6" },

//{ PF13, (u32) FMC, FMC_A7, 12, "FMC_A7" },

//{ PF14, (u32) FMC, FMC_A8, 12, "FMC_A8" },

//{ PF15, (u32) FMC, FMC_A9, 12, "FMC_A9" },

//{ PG0, (u32) FMC, FMC_A10, 12, "FMC_A10" },

//{ PG1, (u32) FMC, FMC_A11, 12, "FMC_A11" },

//{ PG2, (u32) FMC, FMC_A12, 12, "FMC_A12" },

//{ PG3, (u32) FMC, FMC_A13, 12, "FMC_A13" },

//{ PG4, (u32) FMC, FMC_A14, 12, "FMC_A14" },
//{ PG4, (u32) FMC, FMC_BA0, 12, "FMC_BA0" },

//{ PG5, (u32) FMC, FMC_A15, 12, "FMC_A15" },
//{ PG5, (u32) FMC, FMC_BA1, 12, "FMC_BA1" },

//{ PG6, (u32) FMC, FMC_INT2, 12, "FMC_INT2" },
{ PG6, (u32) DCMI, DCMI_D12, 13, "DCMI_D12" },
{ PG6, (u32) LCD, LCD_R7, 14, "LCD_R7" },

{ PG7, (u32) USART6, USART6_CK, 8, "USART6_CK" },
//{ PG7, (u32) FMC, FMC_INT3, 12, "FMC_INT3" },
{ PG7, (u32) DCMI, DCMI_D13, 13, "DCMI_D13" },
{ PG7, (u32) LCD, LCD_CLK, 14, "LCD_CLK" },

{ PG8, (u32) USART6, USART6_RTS, 8, "USART6_RTS" },
{ PG8, (u32) ETH, ETH_PPS_OUT, 11, "ETH_PPS_OUT" },
//{ PG8, (u32) FMC, FMC_SDC_LK, 12, "FMC_SDC_LK" },

{ PG9, (u32) USART6, USART6_RX, 8, "USART6_RX" },
//{ PG9, (u32) FMC, FMC_NE2, 12, "FMC_NE2" },
//{ PG9, (u32) FMC, FMC_NCE3, 12, "FMC_NCE3" },
{ PG9, (u32) DCMI, DCMI_VSYNC, 13, "DCMI_VSYNC" },

{ PG10, (u32) LCD, LCD_G3, 9, "LCD_G3" },
//{ PG10, (u32) FMC, FMC_NCE4_1, 12, "FMC_NCE4_1" },
//{ PG10, (u32) FMC, FMC_NE3, 12, "FMC_NE3" },
{ PG10, (u32) DCMI, DCMI_D2, 13, "DCMI_D2" },
{ PG10, (u32) LCD, LCD_B2, 14, "LCD_B2" },

{ PG11, (u32) ETH, ETH_MII_TX_EN, 11, "ETH_MII_TX_EN" },
{ PG11, (u32) ETH, ETH_RMII_TX_EN, 11, "ETH_RMII_TX_EN" },
//{ PG11, (u32) FMC, FMC_NCE4_2, 12, "FMC_NCE4_2" },
{ PG11, (u32) DCMI, DCMI_D3, 13, "DCMI_D3" },
{ PG11, (u32) LCD, LCD_B3, 14, "LCD_B3" },

{ PG12, (u32) USART6, USART6_RTS, 8, "USART6_RTS" },
{ PG12, (u32) LCD, LCD_B4, 9, "LCD_B4" },
//{ PG12, (u32) FMC, FMC_NE4, 12, "FMC_NE4" },
{ PG12, (u32) LCD, LCD_B1, 14, "LCD_B1" },

{ PG13, (u32) USART6, USART6_CTS, 8, "USART6_CTS" },
{ PG13, (u32) ETH, ETH_MII_TXD0, 11, "ETH_MII_TCD0" },
{ PG13, (u32) ETH, ETH_RMII_TXD0, 11, "ETH_RMII_TXD0" },
//{ PG13, (u32) FMC, FMC_A24, 12, "FMC_A24" },

{ PG14, (u32) USART6, USART6_TX, 8, "USART6_TX" },
{ PG14, (u32) ETH, ETH_MII_TXD1, 11, "ETH_MII_TXD1" },
{ PG14, (u32) ETH, ETH_RMII_TXD1, 11, "ETH_RMII_TXD1" },
//{ PG14, (u32) FMC, FMC_A25, 12, "FMC_A25" },

{ PG15, (u32) USART6, USART6_CTS, 8, "USART6_CTS" },
//{ PG15, (u32) FMC, FMC_SDNCAS, 12, "FMC_SDNCAS" },
{ PG15, (u32) DCMI, DCMI_D13, 13, "DCMI_D13" },

{ PH2, (u32) ETH, ETH_MII_CRS, 11, "ETH_MII_CRS" },
//{ PH2, (u32) FMC, FMC_SDCKE0, 12, "FMC_SDCKE0" },
{ PH2, (u32) LCD, LCD_R0, 14, "LCD_R0" },

{ PH3, (u32) ETH, ETH_MII_COL, 11, "ETH_MII_COL" },
//{ PH3, (u32) FMC, FMC_SDN_E0, 12, "FMC_SDN_E0" },
{ PH3, (u32) LCD, LCD_R1, 14, "LCD_R1" },

{ PH4, (u32) I2C2, I2C2_SCL, 4, "I2C2_SCL" },
//{ PH4, (u32) , OTG_HS_ULPI_NXT, 10, "OTG_HS_ULPI_NXT" },

{ PH5, (u32) I2C2, I2C2_SDA, 4, "I2C2_SDA" },
//{ PH5, (u32) FMC, FMC_SDNWE, 12, "FMC_SDNWE" },

{ PH6, (u32) I2C2, I2C2_SMBA, 4, "I2C2_SMBA" },
{ PH6, (u32) TIM12, TIM12_CH1, 9, "TIM12_CH1" },
//{ PH6, (u32) FMC, FMC_SDNE1, 12, "FMC_SDNE1" },
{ PH6, (u32) DCMI, DCMI_D8, 13, "DCMI_D8" },

{ PH7, (u32) I2C3, I2C3_SCL, 4, "I2C3_SCL" },
{ PH7, (u32) ETH, ETH_MII_RXD3, 11, "ETH_MII_RXD3" },
//{ PH7, (u32) FMC, FMC_SDCKE1, 12, "FMC_SDCKE1" },
{ PH7, (u32) DCMI, DCMI_D9, 13, "DCMI_D9" },

{ PH8, (u32) I2C3, I2C3_SDA, 4, "I2C3_SDA" },
//{ PH8, (u32) FMC, FMC_D16 , 12, "FMC_D16" },
{ PH8, (u32) DCMI, DCMI_HSYNC, 13, "DCMI_HSYNC" },
{ PH8, (u32) LCD, LCD_R2, 14, "LCD_R2" },

{ PH9, (u32) I2S3ext, I2C3_SMBAA, 4, "I2C3_SMBA" },
{ PH9, (u32) TIM12, TIM12_CH2, 9, "TIM12_CH2" },
//{ PH9, (u32) FMC, FMC_D17, 12, "FMC_D17" },
{ PH9, (u32) DCMI, DCMI_D0, 13, "DCMI_D0" },
{ PH9, (u32) LCD, LCD_R3, 14, "LCD_R3" },

{ PH10, (u32) TIM5, TIM5_CH1, 2, "TIM5_CH1" },
//{ PH10, (u32) FMC, FMC_D18, 12, "FMC_D18" },
{ PH10, (u32) DCMI, DCMI_D1, 13, "DCMI_D1" },
{ PH10, (u32) LCD, LCD_R4, 14, "LCD_R4" },

{ PH11, (u32) TIM5, TIM5_CH2, 2, "TIM5_CH2" },
//{ PH11, (u32) FMC, FMC_D19, 12, "FMC_D19" },
{ PH11, (u32) DCMI, DCMI_D2, 13, "DCMI_D2" },
{ PH11, (u32) LCD, LCD_R5, 14, "LCD_R5" },

{ PH12, (u32) TIM5, TIM5_CH3, 2, "TIM5_CH3" },
//{ PH12, (u32) FMC, FMC_D20, 12, "FMC_D20" },
{ PH12, (u32) DCMI, DCMI_D3, 13, "DCMI_D3" },
{ PH12, (u32) LCD, LCD_R6, 14, "LCD_R6" },

{ PH13, (u32) TIM8, TIM8_CH1N, 3, "TIM8_CH1N" },
{ PH13, (u32) CAN1, CAN1_TX, 9, "CAN1_TX" },
//{ PH13, (u32) FMC, FMC_D21, 12, "FMC_D21" },
{ PH13, (u32) LCD, LCD_G2, 14, "LCD_G2" },

{ PH14, (u32) TIM8, TIM8_CH2N, 3, "TIM8_CH2N" },
//{ PH14, (u32) FMC, FMC_D22, 12, "FMC_D22" },
{ PH14, (u32) DCMI, DCMI_D4, 13, "DCMI_D4" },
{ PH14, (u32) LCD, LCD_G3, 14, "LCD_G3" },

{ PH15, (u32) TIM8, TIM8_CH3N, 3, "TIM8_CH3N" },
//{ PH15, (u32) FMC, FMC_D23, 12, "FMC_D23" },
{ PH15, (u32) DCMI, DCMI_D11, 13, "DCMI_D11" },
{ PH15, (u32) LCD, LCD_G4, 14, "LCD_G4" },

{ PI0, (u32) TIM5, TIM5_CH4, 2, "TIM5_CH4" },
{ PI0, (u32) I2S2ext, I2S2_WS, 5, "I2S2_WS" },
//{ PI0, (u32) FMC, FMC_D24, 12, "FMC_D24" },
{ PI0, (u32) DCMI, DCMI_D13, 13, "DCMI_D13" },
{ PI0, (u32) LCD, LCD_G5, 14, "LCD_G5" },

{ PI1, (u32) I2S2ext, I2S2_CK, 5, "I2S2_CK" },
//{ PI1, (u32) FMC, FMC_D25, 12, "FMC_D25" },
{ PI1, (u32) DCMI, DCMI_D8, 13, "DCMI_D8" },
{ PI1, (u32) LCD, LCD_G6, 14, "LCD_G6" },

{ PI2, (u32) TIM8, TIM8_CH4, 3, "TIM8_CH4" },
{ PI2, (u32) I2S2ext, I2S2_EXT_SD, 6, "I2S2_EXT_SD" },
//{ PI2, (u32) FMC, FMC_D26, 12, "FMC_D26" },
{ PI2, (u32) DCMI, DCMI_D9, 13, "DCMI_D9" },
{ PI2, (u32) LCD, LCD_G7, 14, "LCD_G7" },

{ PI3, (u32) TIM8, TIM8_ETR, 3, "TIM8_ETR" },
{ PI3, (u32) I2S2ext, I2S2_SD, 5, "I2S2_SD" },
//{ PI3, (u32) FMC, FMC_D27, 12, "FMC_D27" },
{ PI3, (u32) DCMI, DCMI_D10, 13, "DCMI_D10" },

{ PI4, (u32) TIM8, TIM8_BKIN, 3, "TIM8_BKIN" },
//{ PI4, (u32) FMC, FMC_NBL2, 12, "FMC_NBL2" },
{ PI4, (u32) DCMI, DCMI_D5, 13, "DCMI_D5" },
{ PI4, (u32) LCD, LCD_B4, 14, "LCD_B4" },

{ PI5, (u32) TIM8, TIM8_CH1, 3, "TIM8_CH1" },
//{ PI5, (u32) FMC, FMC_NBL3, 12, "FMC_NBL3" },
{ PI5, (u32) DCMI, DCMI_VSYNC, 13, "DCMI_VSYNC" },
{ PI5, (u32) LCD, LCD_B5, 14, "LCD_B5" },

{ PI6, (u32) TIM8, TIM8_CH2, 3, "TIM8_CH2" },
//{ PI6, (u32) FMC, FMC_D28, 12, "FMC_D28" },
{ PI6, (u32) DCMI, DCMI_D6, 13, "DCMI_D6" },
{ PI6, (u32) LCD, LCD_B6, 14, "LCD_B6" },

{ PI7, (u32) TIM8, TIM8_CH3, 3, "TIM8_CH3" },
//{ PI7, (u32) FMC, FMC_D29, 12, "FMC_D29" },
{ PI7, (u32) DCMI, DCMI_D7, 13, "DCMI_D7" },
{ PI7, (u32) LCD, LCD_B7, 14, "LCD_B7" },


{ PI9, (u32) CAN1, CAN1_RX, 9, "CAN1_RX" },
//{ PI9, (u32) FMC, FMC_D30, 11, "FMC_D30" },
{ PI9, (u32) LCD, LCD_VSYNC, 14, "LCD_VSYNC" },

{ PI10, (u32) ETH, ETH_MII_RX_ER, 11, "ETH_MII_RX_ER" },
//{ PI10, (u32) FMC, FMC_D31, 12, "FMC_D31" },
{ PI10, (u32) LCD, LCD_HSYNC, 14, "LCD_HSYNC" },

//{ PI11, (u32) , OTG_HS_ULPI_DIR, 10, "OTG_HS_ULPI_DIR" },

{ PI12, (u32) LCD, LCD_HSYNC, 14, "LCD_HSYNC" },

{ PI13, (u32) LCD, LCD_VSYNC, 14, "LCD_VSYNC" },

{ PI14, (u32) LCD, LCD_CLK, 14, "LCD_CLK" },

{ PI15, (u32) LCD, LCD_R0, 14, "LCD_R0" },

{ PJ0, (u32) LCD, LCD_R1, 14, "LCD_R1" },

{ PJ1, (u32) LCD, LCD_R2, 14, "LCD_R2" },

{ PJ2, (u32) LCD, LCD_R3, 14, "LCD_R3" },

{ PJ3, (u32) LCD, LCD_R4, 14, "LCD_R4" },

{ PJ4, (u32) LCD, LCD_R5, 14, "LCD_R5" },

{ PJ5, (u32) LCD, LCD_R6, 14, "LCD_R6" },

{ PJ6, (u32) LCD, LCD_R7, 14, "LCD_R7" },

{ PJ7, (u32) LCD, LCD_G0, 14, "LCD_G0" },

{ PJ8, (u32) LCD, LCD_G1, 14, "LCD_G1" },

{ PJ9, (u32) LCD, LCD_G2, 14, "LCD_G2" },

{ PJ10, (u32) LCD, LCD_G3, 14, "LCD_G3" },

{ PJ11, (u32) LCD, LCD_G4, 14, "LCD_G4" },

{ PJ12, (u32) LCD, LCD_B0, 14, "LCD_B0" },

{ PJ13, (u32) LCD, LCD_B1, 14, "LCD_B1" },

{ PJ14, (u32) LCD, LCD_B2, 14, "LCD_B2" },

{ PJ15, (u32) LCD, LCD_B3, 14, "LCD_B3" },

{ PK0, (u32) LCD, LCD_G5, 14, "LCD_G5" },

{ PK1, (u32) LCD, LCD_G6, 14, "LCD_G6" },

{ PK2, (u32) LCD, LCD_G7, 14, "LCD_G7" },

{ PK3, (u32) LCD, LCD_B4, 14, "LCD_B4" },

{ PK4, (u32) LCD, LCD_B5, 14, "LCD_B5" },

{ PK5, (u32) LCD, LCD_B6, 14, "LCD_B6" },

{ PK6, (u32) LCD, LCD_B7, 14, "LCD_B7" },

{ PK7, (u32) LCD, LCD_DE, 14, "LCD_DE" },

};

//=============================
// const PinAlternateDescription_t PAD[]
// search functions from this const global structure (depends on dice and package)
/*
typedef struct {
  
  u8 PinName;
  u32 PPP_Base;
  SignalName_t SignalName;
  u8 AF;
  char* SignalNameString;
  
} PinAlternateDescription_t;
*/

// give the pin info
u32 GetPinAF(PinNameDef PinName, u32 PPP_Adr) {
  
  u32 i;
  u32 result;
  
  for(i=0;i<countof(PAD);i++) {
    
    if(PAD[i].PinName==PinName)
      if(PAD[i].PPP_Base==PPP_Adr) {
        // we found the pin!
        result = PAD[i].AF;
        return result;
      };
    
  };
  
  while(1); // nothing found...
}


PinAlternateDescription_t* GetSignalDescription(PinNameDef PinName, u32 PPP_Adr) {

  u32 i;
  PinAlternateDescription_t* result;
  
  for(i=0;i<countof(PAD);i++) {
    
    if(PAD[i].PinName==PinName)
      if(PAD[i].PPP_Base==PPP_Adr) {
        // we found the pin!
        result = (PinAlternateDescription_t*) &PAD[i];
        return result;
      };
    
  };
  
  while(1); // nothing found...
}
