
#ifndef _NHD_C0216CZ_LCD16x2_H_
#define _NHD_C0216CZ_LCD16x2_H_

// NHD-C0216CZ-NSW-BBW-3V3 www.digikey.com (blue with backlight - FROG) Around 11 USD per piece, compact form, removable backlight for thinner display
// NHD-C0216CZ-NSW-FBW-3V3 www.digikey.com (tranflextive, optional backlight - Hybrid FROG) Like a calculator screen, no energy wasted in display for lightning...
#undef LCD_COG_NB_COL_X
#define LCD_COG_NB_COL_X	16
#define LCD_COG_NB_ROW_Y	2

typedef struct {
  
  IO_PinTypeDef* NRST; // Reset the module
  IO_PinTypeDef* RS; // Command/Data
//  IO_PinTypeDef* CS; // SPI->NSS
//  IO_PinTypeDef* SI; // SPI->MOSI
//  IO_PinTypeDef* SCL; // SPI->SCK
  
  u8 Cursor_X, Cursor_Y;

  u8 RAM[LCD_COG_NB_ROW_Y][LCD_COG_NB_COL_X]; // the command to address it will be inserted inside to avoid calculations every time, [cmd adr][data]...

  u8 FullFrameRefreshRate; // if non zero, we update the display only if there is a change from previous update (speed up), but due to ESD, we also need to full refresh once in a while. Here is the tuning number.
  u8 FrameCountdown;// full refresh when reach zero. (2=full update every 2 frames)
  u8 PreviousRAM[LCD_COG_NB_ROW_Y][LCD_COG_NB_COL_X]; // the command to address it will be inserted inside to avoid calculations every time, [cmd adr][data]...
  
  u8 Byte; // for SPI communication
  
  SPI_MasterIO* MIO; // IO Emulated SPI (no need speed to send few bytes over)
  
  StuffsArtery* SA; // Sequence
  
  BasicTimer* BT; // this will be to control time ticks
  u8 BTn; //  a number between 0 and 3

  u32 (*fnWaitMethod)(u32);
  u32 ctWaitMethod;
  u32 WaitParam;
  
  OneJobType Jobs[5]; // can do 5 jobs defined here (this will morph later)
  
  PrintfTools Print;
  
} NHD_LCD16x2;


u32 NewNHD_LCD16x2(NHD_LCD16x2* L);
u32 SetNHD_LCD16x2_Timings( NHD_LCD16x2* L);

u32 NHD_LCD16x2Test(void);

// LCD Display Specifics (Controls)
u32 NHD_LCD16x2_UpdateDisplay(NHD_LCD16x2* L);
u32 NHD_FillRAM_With(NHD_LCD16x2* L, u8 c);
u32 NHD_ClearRAM(NHD_LCD16x2* L);
u32 NHD_LCD16x2_FullUpdateDisplay(NHD_LCD16x2* L);
u32 NHD_LCD16x2_SmartUpdateDisplay(NHD_LCD16x2* L);

//================== LCD RAM Manipulation
u32 NHD_LCD_GOTO_X_Y(NHD_LCD16x2* L, u8 x, u8 y);
u32 NHD_LCD_GO_Home(NHD_LCD16x2* L);
u32 NHD_LCD16x2_UpdateDisplay(NHD_LCD16x2* L); // This will refresh the LCD with the current frame buffer (full refresh)
u32 NHD_LCD_GOTO_NextLine(NHD_LCD16x2* L);
u32 NHD_LCD_GOTO_HorizontalCenter(NHD_LCD16x2* L);
u32 NHD_LCD_putchar(NHD_LCD16x2* L, u32 c);

#endif