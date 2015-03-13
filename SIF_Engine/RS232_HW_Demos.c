
#include "SebEngine.h"


// Here is an example of using RS232 cell
static u8 Rs232TXBuf[512]; // should be declared by the other party (if any)
static u8 Rs232RXBuf[512]; // the one who receive should declare its buffer, not the transmitting one, which flow control must be done by receiver.
static ByteVein_t BV_TX,BV_RX;
static Rs232_t Rs232;
static IO_Pin_t Rs232TX, Rs232RX;

const char* Hello1 = "Test from Main Loop with Interrupt Enabled.\n";
const char* Hello2 = "Test from Main Loop with Interrupt Disabled.\n";
// third test is to plug I2C Spy to the BV_TX.

void TestSebUART(void) {
  
  char* pu8;
  u32 tmp = 0;
  Rs232.RX = &Rs232RX;
  Rs232.TX = &Rs232TX;
  NewRs232HW(&Rs232, USART1); // The HW connectivity, handle, register base, RX pin, TX pin, CTS pin, RTS pin, if non null.
  SetRs232Timings(&Rs232, 115200, 2, 1); // Things depending on time and internal clocks
  // we have also to initialize the BV_TX and BV_RX at higher level...
  // initialize the BV first!
  NewBV(&BV_TX, (u32)Rs232TXBuf, sizeof(Rs232TXBuf));
  NewBV(&BV_RX, (u32)Rs232RXBuf, sizeof(Rs232RXBuf));

  SetRs232BVs( &Rs232, &BV_TX, &BV_RX);
  
  // now we need to activate everything after all the architecture is done (the other end of the BV_TX, BV_RX)
  NVIC_Rs232sEnable(ENABLE);

  // first test is to fill this FIFO from main loop.
  pu8 = (char*)Hello1;
  while(*pu8) {// empty string check first
    Wait_us(500);
    AddToBV(Rs232.BV_TX, *pu8); // add to head (new)
    pu8++;
  };

  __set_PRIMASK(tmp | 1);  
  pu8 = (char*)Hello2;
  while(*pu8) {// empty string check first
    AddToBV(Rs232.BV_TX, *pu8); // add to head (new)
    pu8++;
  };
  __set_PRIMASK(tmp);
  
  while(1);
}

