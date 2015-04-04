
#ifndef _SSD1306_H_
#define _SSD1306_H_

 
#define SSD1306_DEFAULT_ADDRESS 0x78 
#define SSD1306_SETCONTRAST 0x81 
#define SSD1306_DISPLAYALLON_RESUME 0xA4 
#define SSD1306_DISPLAYALLON 0xA5 
#define SSD1306_NORMALDISPLAY 0xA6 
#define SSD1306_INVERTDISPLAY 0xA7 
#define SSD1306_DISPLAYOFF 0xAE 
#define SSD1306_DISPLAYON 0xAF 
#define SSD1306_SETDISPLAYOFFSET 0xD3 
#define SSD1306_SETCOMPINS 0xDA 
#define SSD1306_SETVCOMDETECT 0xDB 
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5 
#define SSD1306_SETPRECHARGE 0xD9 
#define SSD1306_SETMULTIPLEX 0xA8 
#define SSD1306_SETLOWCOLUMN 0x00 
#define SSD1306_SETHIGHCOLUMN 0x10 
#define SSD1306_SETSTARTLINE 0x40 
#define SSD1306_MEMORYMODE 0x20 
#define SSD1306_COLUMNADDR 0x21 
#define SSD1306_PAGEADDR   0x22 
#define SSD1306_COMSCANINC 0xC0 
#define SSD1306_COMSCANDEC 0xC8 
#define SSD1306_SEGREMAP 0xA0 
#define SSD1306_CHARGEPUMP 0x8D 
#define SSD1306_SWITCHCAPVCC 0x2 
#define SSD1306_NOP 0xE3 
 
#define SSD1306_WIDTH 128 
#define SSD1306_HEIGHT 64 
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH*SSD1306_HEIGHT)/8  // we are using 32 bit arrays
 

typedef struct {
  
  I2C_MasterIO_t* I2C; // This is the blocking mode // this includes the sequencer
  u8 PreviousFrame[SSD1306_BUFFERSIZE];
  u8 Frame[SSD1306_BUFFERSIZE];
  u8 BytesTX[8];
  u8 BytesRX[8];
  // ASCII font to convert string to pixels.
  
} SSD1306_128x64x1_t;


// display level functions
void SSD1306(SSD1306_128x64x1_t* S); 
void SSD1306sendFramebuffer(SSD1306_128x64x1_t* S); 
void SSD1306invert(SSD1306_128x64x1_t* S, uint8_t inverted);
void SSD1306sendCommand(SSD1306_128x64x1_t* S, uint8_t command); 
void SSD1306sendData(SSD1306_128x64x1_t* S, uint8_t data); 

#endif
