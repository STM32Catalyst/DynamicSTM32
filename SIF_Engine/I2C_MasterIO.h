
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
  
  IO_Pin_t* SDA; // we need the pointer to the pin
  IO_Pin_t* SCL; // we need the pointer to the pin
  
  Timer_t* Timer; // this will be to control time ticks
  u8 Cn; //  a number between 0 and 3

  u32 (*fnWaitMethod)(u32);
  u32 ctWaitMethod;
  u32 WaitParam;
  
  u32 MaxBps; // input
  
  StuffsArtery_t* SA; // this points to Job feeding  
  
  u8 SlaveAdr; // the 8 bit slave address which we are using from the start command. (tells if read or write operation on going)
  u8 AckFail : 1;
  u8 JobDone : 1;
  
} I2C_MasterIO_t;


u32 NewI2C_MasterIO_SDA_SCL(I2C_MasterIO_t* M, IO_Pin_t* SDA, IO_Pin_t* SCL);
u32 SetI2C_MasterIO_Timings( I2C_MasterIO_t* M, u32 MaxBps, MCUClocks_t* T);
u32 ConfigureI2C_MasterIO(I2C_MasterIO_t* M);
u32 EnableI2C_MasterIO(I2C_MasterIO_t* M);

u32 sq_I2C_MIO_StartJob(u32 u);
u32 sq_I2C_MIO_StopJob(u32 u);
u32 sq_I2C_MIO_MoveJob(u32 u);




#endif
