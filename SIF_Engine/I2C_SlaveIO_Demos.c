
#include "SebEngine.h"

//================= At the end we have the test function to check functionality
static u8 I2C_SlaveAdresses[] = { 0x40, 0xB0, 0xC0, 0xD0 }; // ram is faster
static u8 I2C_SlaveMemory[256];
static u8 RxBuf[10];

// to make the slave not responding, we need to change to add a "slave busy" flag which latches when bus is idle.
static I2C_SlaveIO_t gI2C_Slave; // can be multiple of them, as array (we pass by pointer)
static IO_Pin_t I2C_SlaveIO_SDA; // for fast pin access using bitbanding area (we will use a fake HW registers which will check if all pins are not colliding)
static IO_Pin_t I2C_SlaveIO_SCL; // for fast pin access using bitbanding area
// we have to cleanup the Architect function for the fPin



void TestI2C_SlaveIO(void) {

  NewI2C_SlaveIO_SDA_SCL(&gI2C_Slave, NewIO_Pin(&I2C_SlaveIO_SDA,PH10), NewIO_Pin(&I2C_SlaveIO_SCL,PH12));
  EmulateMemoryI2C_SlaveIO(&gI2C_Slave, (u8*)I2C_SlaveAdresses, countof(I2C_SlaveAdresses), I2C_SlaveMemory, countof(I2C_SlaveMemory));
  
  ConfigureI2C_SlaveIO(&gI2C_Slave);
  SpyI2C_SlaveIO(&gI2C_Slave); // this is to go to spy code as well
  EnableI2C_SlaveIO(&gI2C_Slave);
  
//  u32 (*fnBusFree)(u32); // the I2C bus is free (stop bit occured), this can be hooked to run some action (like turn off this slave, do something, turn it back on)
//  u32 (*fnReceived)(u32); // user data has been received (end of slave receive phase). This does not include any sub address
//  u32 (*fnTransmitted)(u32); // data was transmitted out (end of slave transmitter phase), can be hooked to trigger higher level action

  gI2C_Slave.fnBusFree = u32_fn_u32;
  gI2C_Slave.fnReceived = u32_fn_u32;
  gI2C_Slave.fnTransmitted = u32_fn_u32;
  
  
  NVIC_EXTIsEnable(ENABLE);
  //===============

  while(1) {

    // here we will use the I2C single master to generate events...
#if 1 // basic slave receive test
    // can put a breakpoint here
    I2CIO_Start(0x40);  // if return FALSE, chip is not present or bus stuck
    I2CIO_Transmit(0x01);
    I2CIO_Transmit(0x12);
    I2CIO_Transmit(0x23);
    I2CIO_Transmit(0x34);
    I2CIO_Stop();
#endif
#if 1
    // can put a breakpoint here
    I2C_SlaveMemory[0] = 0x55;
    I2C_SlaveMemory[1] = 0x66;
    I2C_SlaveMemory[2] = 0x77;
    I2C_SlaveMemory[3] = 0x88;
    I2C_SlaveMemory[4] = 0x99;

    I2CIO_Start(0x40);  // if return FALSE, chip is not present or bus stuck
    I2CIO_Transmit(0x01);
    I2CIO_Start(0x41);
    RxBuf[0] = I2CIO_Receive(1);
    RxBuf[1] = I2CIO_Receive(1);
    RxBuf[2] = I2CIO_Receive(0);
#endif
  }

};

