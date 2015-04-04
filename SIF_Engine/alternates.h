
#ifndef _ALTERNATES_H_
#define _ALTERNATES_H_

// First, we list all the signals which can be assigned to pins
typedef enum {
  
NO_ALTERNATE, // 0
//---- clock outputs
MCO1,
MCO2,
//---- TIM1
TIM1_CH1,
TIM1_CH2,
TIM1_CH3,
TIM1_CH4,
TIM1_CH1N,
TIM1_CH2N,
TIM1_CH3N,
TIM1_BKIN,
TIM1_ETR,
//---- TIM2
TIM2_CH1,
TIM2_CH2,
TIM2_CH3,
TIM2_CH4,
TIM2_ETR,
//---- TIM3
TIM3_CH1,
TIM3_CH2,
TIM3_CH3,
TIM3_CH4,
TIM3_ETR,
//---- TIM4
TIM4_CH1,
TIM4_CH2,
TIM4_CH3,
TIM4_CH4,
TIM4_ETR,
//---- TIM5
TIM5_CH1,
TIM5_CH2,
TIM5_CH3,
TIM5_CH4,
TIM5_ETR,
//---- TIM6
TIM6_ETR,
//---- TIM7
TIM7_ETR,
//---- TIM8
TIM8_CH1,
TIM8_CH2,
TIM8_CH3,
TIM8_CH4,
TIM8_CH1N,
TIM8_CH2N,
TIM8_CH3N,
TIM8_BKIN,
TIM8_ETR,
//---- TIM9
TIM9_CH1,
TIM9_CH2,
TIM9_ETR,
//---- TIM10
TIM10_CH1,
TIM10_ETR,
//---- TIM11
TIM11_CH1,
TIM11_ETR,
//---- TIM12
TIM12_CH1,
TIM12_CH2,
TIM12_ETR,
//---- TIM13
TIM13_CH1,
TIM13_ETR,
//---- TIM14
TIM14_CH1,
TIM14_ETR,
//--- I2C1
I2C1_SDA,
I2C1_SCL,
I2C1_SMBA,
//--- I2C2
I2C2_SDA,
I2C2_SCL,
I2C2_SMBA,
//--- I2C3
I2C3_SDA,
I2C3_SCL,
I2C3_SMBA,
//--- SPI1
SPI1_NSS,
SPI1_SCK,
SPI1_MISO,
SPI1_MOSI,
//--- SPI2
SPI2_NSS,
SPI2_SCK,
SPI2_MISO,
SPI2_MOSI,
//--- SPI3
SPI3_NSS,
SPI3_SCK,
SPI3_MISO,
SPI3_MOSI,
//--- SPI4
SPI4_NSS,
SPI4_SCK,
SPI4_MISO,
SPI4_MOSI,
//--- SPI5
SPI5_NSS,
SPI5_SCK,
SPI5_MISO,
SPI5_MOSI,
//--- SPI6
SPI6_NSS,
SPI6_SCK,
SPI6_MISO,
SPI6_MOSI,
//--- USART1
USART1_RX,
USART1_TX,
USART1_CK,
USART1_RTS,
USART1_CTS,
//--- USART2
USART2_RX,
USART2_TX,
USART2_CK,
USART2_RTS,
USART2_CTS,
//--- USART3
USART3_RX,
USART3_TX,
USART3_CK,
USART3_RTS,
USART3_CTS,
//--- UART4
UART4_RX,
UART4_TX,
//--- UART5
UART5_RX,
UART5_TX,
//--- USART6
USART6_RX,
USART6_TX,
USART6_CK,
USART6_RTS,
USART6_CTS,
//--- UART7
UART7_RX,
UART7_TX,
//--- UART8
UART8_RX,
UART8_TX,
//--- CAN1
CAN1_RX,
CAN1_TX,
//--- CAN2
CAN2_RX,
CAN2_TX,
//--- LCD
LCD_R0,
LCD_R1,
LCD_R2,
LCD_R3,
LCD_R4,
LCD_R5,
LCD_R6,
LCD_R7,
LCD_G0,
LCD_G1,
LCD_G2,
LCD_G3,
LCD_G4,
LCD_G5,
LCD_G6,
LCD_G7,
LCD_B0,
LCD_B1,
LCD_B2,
LCD_B3,
LCD_B4,
LCD_B5,
LCD_B6,
LCD_B7,
LCD_HSYNC,
LCD_VSYNC,
LCD_DE,
LCD_CLK,
//--- DCMI
DCMI_D0,
DCMI_D1,
DCMI_D2,
DCMI_D3,
DCMI_D4,
DCMI_D5,
DCMI_D6,
DCMI_D7,
DCMI_D8,
DCMI_D9,
DCMI_D10,
DCMI_D11,
DCMI_D12,
DCMI_D13,
DCMI_HSYNC,
DCMI_VSYNC,
DCMI_PIXCLK,
//--- SDIO
SDIO_D0,
SDIO_D1,
SDIO_D2,
SDIO_D3,
SDIO_D4,
SDIO_D5,
SDIO_D6,
SDIO_D7,
SDIO_CK,
SDIO_CMD,
//--- OTG2_HS
//
//--- OTG1_FS
//
//--- OTG2_FS
//
//--- ETH
ETH_MII_CRS,
ETH_MII_RX_CLK,
ETH_RMII_REF_CLK,
ETH_MII_TX_CLK,

ETH_MDIO,
ETH_MII_COL,
ETH_MII_RX_DV,
ETH_RMII_CRS_DV,
ETH_PPS_OUT,

ETH_MII_TXD0,
ETH_MII_TXD1,
ETH_MII_TXD2,
ETH_MII_TXD3,

ETH_MII_RXD0,
ETH_MII_RXD1,
ETH_MII_RXD2,
ETH_MII_RXD3,

ETH_MII_RX_ER,
ETH_MII_TX_EN,
ETH_RMII_TX_EN,

ETH_RMII_RXD0,
ETH_RMII_RXD1,

ETH_RMII_TXD0,
ETH_RMII_TXD1,

ETH_MDC,
ETH_RMII_RX_EN,

//--- FMC
//
//--- I2S3

I2S_CKIN,

I2S2_WS,
I2S2_SD,
I2S2_EXT_SD,
I2S2_MCK,
I2S2_CK,


I2S3_WS,
I2S3_CK,
I2S3_EXT_SD,
I2S3_SD,
I2C3_SMBAA,

//---- RTC
RTC_REFIN,

//--- EVENTOUT
EVENTOUT,

//--- Internal only signals for DMA stream
SPI1_TX,
SPI1_RX,
SPI2_TX,
SPI2_RX,
SPI3_TX,
SPI3_RX,
SPI4_TX,
SPI4_RX,
SPI5_TX,
SPI5_RX,
SPI6_TX,
SPI6_RX,
CRYP_IN,
CRYP_OUT,
HASH_IN,
I2C1_RX,
I2C1_TX,
I2C2_RX,
I2C2_TX,
I2C3_RX,
I2C3_TX,
I2S2_EXT_RX,
I2S2_EXT_TX,
I2S3_EXT_RX,
I2S3_EXT_TX,
TIM1_UP,
TIM2_UP,
TIM3_UP,
TIM4_UP,
TIM5_UP,
TIM6_UP,
TIM7_UP,
TIM8_UP,
TIM9_UP,
TIM10_UP,
TIM11_UP,
TIM12_UP,
TIM13_UP,
TIM14_UP,
TIM3_TRIG,
DAC1_ANALOG,
DAC2_ANALOG,
ADC1_ANALOG,
ADC2_ANALOG,
ADC3_ANALOG,
TIM1_TRIG,
SAI1_A,
DCMI_BUS,
SDIO_BUS,
SAI1_B,
TIM1_COM,
TIM8_COM,
TIM8_TRIG,
TIM5_TRIG,
//=========> Analog input pins
//ADC123_IN0
ADC1_IN0,
ADC2_IN0,
ADC3_IN0,
//ADC123_IN1,
ADC1_IN1,
ADC2_IN1,
ADC3_IN1,
//ADC123_IN2,
ADC1_IN2,
ADC2_IN2,
ADC3_IN2,
//ADC123_IN3,
ADC1_IN3,
ADC2_IN3,
ADC3_IN3,
//ADC123_IN4,
ADC1_IN4,
ADC2_IN4,
ADC3_IN4,
//ADC123_IN5,
ADC1_IN5,
ADC2_IN5,
ADC3_IN5,
//ADC123_IN6,
ADC1_IN6,
ADC2_IN6,
ADC3_IN6,
//ADC123_IN7,
ADC1_IN7,
ADC2_IN7,
ADC3_IN7,
//ADC12_IN8,
ADC1_IN8,
ADC2_IN8,
ADC3_IN8,
//ADC12_IN9,
ADC1_IN9,
ADC2_IN9,
ADC3_IN9,
//ADC123_IN10,
ADC1_IN10,
ADC2_IN10,
ADC3_IN10,
//ADC123_IN11,
ADC1_IN11,
ADC2_IN11,
ADC3_IN11,
//ADC123_IN12,
ADC1_IN12,
ADC2_IN12,
ADC3_IN12,
//ADC123_IN13,
ADC1_IN13,
ADC2_IN13,
ADC3_IN13,
//ADC123_IN14,
ADC1_IN14,
ADC2_IN14,
ADC3_IN14,
//ADC123_IN15,
ADC1_IN15,
ADC2_IN15,
ADC3_IN15,

//ADC123_VTEMP,
ADC1_VTEMP,
ADC2_VTEMP,
ADC3_VTEMP,
//ADC123_VREF,
ADC1_VREF,
ADC2_VREF,
ADC3_VREF,
//ADC123_VBAT,
ADC1_VBAT,
ADC2_VBAT,
ADC3_VBAT,

} SignalName_t;

#endif
