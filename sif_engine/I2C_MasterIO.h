
#ifndef _I2C_MASTER_IO_H_
#define _I2C_MASTER_IO_H_

//****************************     I2C       **********************************
// This master mode is not using interrupts because it does not have any critical timings inside
// We should reserve the interrupts only when justified.
// This Master polling method could use polling or be at less priority interrupt level, or timesliced by RTOS.

// When using a byte HW cell, then runnning a master job stack in the background would make sense, so that multiple I2C Master could run in parallel and have the multiple HW I2C Master cells 
// justified as parallel processing.
// If the CPU has multiple core, each one of them could run a single MasterIO as polling mecanism.

//=== This is the description of a job, so they can be queued... in a StuffArtery (chocking items)
// Please note we are manipulating pointers to the jobs which could be defined in different places, or even under interrupt....
// to be triggered by a job

typedef struct {
  
  IO_PinTypeDef* SDA; // we need the pointer to the pin
  IO_PinTypeDef* SCL; // we need the pointer to the pin
  
  BasicTimer* BT; // this will be to control time ticks
  u8 BTn; //  a number between 0 and 3

  u32 (*fnWaitMethod)(u32);
  u32 ctWaitMethod;
  u32 WaitParam;
  
  u32 MaxBps; // input
  
  StuffsArtery* SA; // this points to Job feeding  
  
  u8 SlaveAdr; // the 8 bit slave address which we are using from the start command. (tells if read or write operation on going)
  u8 AckFail : 1;
  u8 JobDone : 1;
  
} I2C_MasterIO;


u32 NewI2C_MasterIO(I2C_MasterIO* MIO);
u32 SetI2C_MasterIO_Timings( I2C_MasterIO* M, u32 MaxBps, MCUClockTree* T);

u32 sq_I2C_MIO_StartJob(u32 u);
u32 sq_I2C_MIO_StopJob(u32 u);
u32 sq_I2C_MIO_MoveJob(u32 u);




#endif
