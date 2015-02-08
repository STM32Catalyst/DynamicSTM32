
// Should the hook change the interrupt level back to original?
// Will the stack survive?
// Should the interrupt level change after the callback is done to avoid the stack to be too deep?
// if there is no hook, it means main loop + flag is going to be used?

// Right now, we assume the I2C is done from main loop, not under interrupt handler.
// If need reactivity, then set a flag which will trigger action for I2C to proceed.
// Much later, we can try to have Interupt handler and queue for pending job from any source with callback (and its interrupt level)

// already, with this scheme, the I2C master can be duplicated on as many pins as possible, which is not bad already.
// low power, clock speed tuning are possible from this base.
#include "sebEngine.h"
//~~~~~~~~~~~~~~~~~~~~~ global variables to use differently~~~~~~~~~~~

#define SetSDAOutput()          { *(MIO->SDA->bbMode0) = 1;}
#define SetSDAInput()           { *(MIO->SDA->bbMode0) = 0;}

#define SetSCLOutput()          { *(MIO->SCL->bbMode0) = 1;}
#define SetSCLInput()           { *(MIO->SCL->bbMode0) = 0;}

#define SetSDALow SetSDAOutput
#define SetSDAHigh SetSDAInput
#define SetSCLLow SetSCLOutput
#define SetSCLHigh SetSCLInput

#define bit_SDA_Read()      (*(MIO->SDA->bbIn))

u32 Receive (u32 u, u32 DoAck);
u32 Transmit (u32 u, u8 bValue);
u32 GenerateStart (u32 u, u8 SlaveAdr);
u32 ErrorRecovery (u32 u);
u32 GenerateStop (u32 u);

//u32 I2C_MasterIO_EXTI_IRQHandler(u32 u); // the event handler function for this I2Cslave, passing the context to it
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
u32 TimerCountdownWait(u32 u);
u32 NopsWait(u32 u);
//--------- this will become later global resources --------

u32 NewI2C_MasterIO(I2C_MasterIO* M) {

// we have to initialize the state machine first
  // configure the GPIOs
  // configure SDA pin
  IO_PinClockEnable(M->SDA);
  IO_PinConfiguredAs(M->SDA,GPIO_AF16_DIGITAL_INPUT);  
  IO_PinSetLow(M->SDA);
  IO_PinSetSpeedMHz(M->SDA, 1);
  IO_PinEnablePullUp(M->SDA, ENABLE);
  IO_PinEnablePullDown(M->SDA, DISABLE);
  IO_PinEnableHighDrive(M->SDA, DISABLE);

  // configure SCL pin
  IO_PinClockEnable(M->SCL);
  IO_PinConfiguredAs(M->SCL,GPIO_AF16_DIGITAL_INPUT);  
  IO_PinSetLow(M->SCL);
  IO_PinSetSpeedMHz(M->SCL, 1);
  IO_PinEnablePullUp(M->SCL, ENABLE);
  IO_PinEnablePullDown(M->SCL, DISABLE);
  IO_PinEnableHighDrive(M->SCL, DISABLE);
 
  return 0;
}


u32 SetI2C_MasterIO_Timings(I2C_MasterIO* M, u32 MaxBps ) { 

// In theory, knowing the clocks information, we should be able to find a strategy for I2C.
// Or we do by tuning the delay... we generate I2C traffic and we check how long and the bit rate... (takes more time, adaptive, could be dynamic too)
  
  M->MaxBps = MaxBps; // 400khz
  // The MIO has its own hooks to setup first
  //MIO->fnMasterScheme = I2C_MasterIO_STMA_Run; // run the state machine instead of a hardcoded stuff
  M->WaitParam = 1;
  NopsWait((u32) M);
  M->fnWaitMethod = TimerCountdownWait; // here we should take care of the timings, and choose the best scheme based on CPU MHz and bps of bus...    
  
  return 0;
}


//=============================================

#define SetSDAOutput()          { *(MIO->SDA->bbMode0) = 1;}
#define SetSDAInput()           { *(MIO->SDA->bbMode0) = 0;}

//==============================================
//=============== These are the optional I2C spy mode to see what is happening on a bus.
// the slave function is then disabled

static u32 TimerCountdownWait(u32 u) {
  
  I2C_MasterIO* MIO = (I2C_MasterIO*) u;
  ArmBasicTimerCountdown(MIO->BT,MIO->BTn, MIO->WaitParam);
  while(MIO->BT->CountDownDone[MIO->BTn]==0) ;
  return 0;
}

static u32 NopsWait(u32 u) {
  I2C_MasterIO* MIO = (I2C_MasterIO*) u;
  u32 n = MIO->WaitParam;
  while(n--) asm("nop\n");
  return 0;
}

static u32 WaitHere(u32 u, u32 delay) {
  I2C_MasterIO* MIO = (I2C_MasterIO*) u;
  MIO->WaitParam = delay;
  if(MIO->fnWaitMethod) MIO->fnWaitMethod(u);
  return 0;
}
 


// This function can be called upon any I2C bus error condition.
static u32 ErrorRecovery (u32 u) 
{
  // blindly generate 9 stop bits to flush any stuck situation
  // non-invasive, no side effects.
  GenerateStop(u);
  GenerateStop(u);
  GenerateStop(u);
  GenerateStop(u);
  GenerateStop(u);
  GenerateStop(u);
  GenerateStop(u);
  GenerateStop(u);	// flush the bus if it is stuck
  return u;
}

static u32 GenerateStart (u32 u, u8 SlaveAdr) 
{
  I2C_MasterIO* MIO = (I2C_MasterIO*) u;
  MIO->SlaveAdr = SlaveAdr;
  SetSDAInput();//dir_I2C_SDA_IN;	// to check if I2C is idle... or stuck
  WaitHere(u,1);
  if(bit_I2C_SDA_Read()==0)	// Samsung decoder has I2C compatibility problems, it does not detect NACK in Read Mode...
          ErrorRecovery(u);

  // Seb this is bugged, it's not a start bit if SCL is low...  it's too short.
  SetSCLHigh();//bit_I2C_SCL_HIGH;
  WaitHere(u,1);					

  // Fixed violation on Start hold time
  SetSDALow();//bit_I2C_SDA_LOW;
  WaitHere(u,1);

  SetSCLLow();//bit_I2C_SCL_LOW;
  WaitHere(u,1);

  return Transmit (u,SlaveAdr);				// Send the slave address
}


static u32 Transmit(u32 u, u8 bValue) 
{
  I2C_MasterIO* MIO = (I2C_MasterIO*) u;
  u8 loop;

  for (loop = 0; loop < 8; loop++) 
  {
      if (bValue & 0x80) {
        SetSDAHigh();//bit_I2C_SDA_HIGH;
      }else{ 
        SetSDALow();//bit_I2C_SDA_LOW;
      }

      WaitHere(u,1);// Sept 17
//		dir_I2C_SDA_OUT;				// make sure SDA is configured as output (once DR initialised)

      SetSCLHigh();//bit_I2C_SCL_HIGH;
      WaitHere(u,1);//1028
      bValue <<= 1;
      SetSCLLow();//bit_I2C_SCL_LOW;
      WaitHere(u,1);
  }

  // Acknowledge Write
  // bit_I2C_SCL = HIGH;
  // ack is READ to check if Slave is responding

  SetSDAInput();//dir_I2C_SDA_IN;
  WaitHere(u,1);
  SetSCLHigh();//bit_I2C_SCL_HIGH;	// SCL = 1
  WaitHere(u,1);					

  // Here we could sense NACK and manage error info to calling function
  // for debug to find ACK bit as long scl pulse...
  // Error = bit_I2C_SDA; // 1 = Error, 0 = Ok
  SetSCLHigh();//bit_I2C_SCL_HIGH;	// SCL = 1
  WaitHere(u,2);//	NOP;					
  MIO->AckFail |= bit_I2C_SDA_Read();	// Acknowledge bit

  SetSCLLow();//bit_I2C_SCL_LOW;	// SCL = 0

  WaitHere(u,1);// Sept 17

//-	SetSDAOutput();//dir_I2C_SDA_OUT;

//-	WaitHere(u,1);//	NOP;	add sept 17

  return MIO->AckFail;
}


static u32 Receive (u32 u, u32 DoAck) 
{ 
  I2C_MasterIO* MIO = (I2C_MasterIO*) u;
  u8 bValue, loop;

  bValue = 0;
  SetSDAInput();//dir_I2C_SDA_IN; // make SDA as input before reading pin level

  for (loop = 0; loop < 8; loop ++) 
  {
      WaitHere(u,1);// NOP; NOP;	// 1 us delay

      SetSCLHigh();//bit_I2C_SCL_HIGH;	// SCL = 1
      WaitHere(u,2);					
      bValue <<= 1;

      if(bit_I2C_SDA_Read()) bValue++;
      SetSCLLow();//bit_I2C_SCL_LOW;	// SCL = 0
      WaitHere(u,1);//1028
  }

// Manage the ackknowledge bit
  if(DoAck) {
    SetSDALow();//bit_I2C_SDA_LOW;
  }else{
    SetSDAHigh();//bit_I2C_SDA_HIGH;
  }

//  SetSDAOutput();//dir_I2C_SDA_OUT; // make sure SDA is configured as output (once DR initialised)
  WaitHere(u,1);	// enlarge the pulse to see it on the scope
  SetSCLHigh();//bit_I2C_SCL_HIGH;	// SCL = 1
  WaitHere(u,1);
  SetSCLLow();//bit_I2C_SCL_LOW;	// SCL = 0

  WaitHere(u,1);//	NOP;	add sept 17

  if(!DoAck)	// If NACK, STOP will automatically follows (I2C spec)
    GenerateStop(u);

  return bValue;
}


static u32 GenerateStop (u32 u) {
  
  I2C_MasterIO* MIO = (I2C_MasterIO*) u;
  SetSCLLow();//bit_I2C_SCL_LOW;
  WaitHere(u,1);
  SetSDALow();//bit_I2C_SDA_LOW;
  WaitHere(u,1);							// Extra to make sure delay is ok
  
  SetSCLHigh();//bit_I2C_SCL_HIGH;
  WaitHere(u,1);

  SetSDAHigh();//bit_I2C_SDA_HIGH;
//	WaitHere(u,1);
  return 0;
}


//================================---------------------------> 8>< <----------------===========================]
// Here we try to have a job scheduler which more or less will look like a sequence of I2C communication happening...
// This way we can prepare some job that could be done in background (when background I2C Master becomes possible with a BYTE HW cell

static u32 I2C_MIO_Start(u32 u, u32 SlaveAdr) {

  I2C_MasterIO* MIO = (I2C_MasterIO*) u;

  MIO->AckFail = GenerateStart (u,SlaveAdr);				// Send the slave address
  return 0; // no interrupt setup
}

// This is NEVER NEEDED unless error recovery is asked
static u32 I2C_MIO_Stop(u32 u, u32 BitMask) { // This is used only in transmit mode (Adr.b0=0)

//  I2C_MasterIO* S = (I2C_MasterIO*) u;
  GenerateStop(u);
  WaitHere(u,1);
  return 0; // no call back, next job right away
}


static u32 I2C_MIO_Move(u32 u, u32 Param1, u32 Param2, u32 Param3) { // Param1: Block adr, Param2: Block size byte, Param3: Ack when read or not
  
  I2C_MasterIO* S = (I2C_MasterIO*) u;
  u8* pu8 = (u8*) Param1;
  u16 bCount = (u16) Param2;
  u8 MoreComing = Param3;
  
  // if the TX adr is null, no bytes to transmit, disable MOSI, and transmit dummy things instead
  if(pu8==0) while(1); // not supported with no buffer pointer
//  if(bCount==0) while(1); // not supported

  if(S->SlaveAdr & 1) { // receiver mode

    if((bCount==0)||(S->AckFail)) {
      Receive(u,0);
    }else
    {
      do{ // at least read one byte if ACK failed on slave address, to generate the STOP bit at right opportunity. If 0 bytes requested, will still read one of them
          *pu8++ = Receive (u, MoreComing | bCount); // a STOP bit will be generated if no more packets and last byteCount :-)
        } while(bCount--);
    }

  }else{ // transmitter mode

    while((S->AckFail==0)&& (bCount--)) { // for all bytes... if ack fail, skip the transmit part
       S->AckFail |= Transmit (u, *pu8++);
    };

    // let's take care of the stop bit if last packet here for compactness
    if(MoreComing==FALSE)
      GenerateStop(u);
  };
  
  return 0; // no interrupt call back setup here
}


//==============================================================
// SEQUENCER COMPATIBLE FUNCTION START

// This is the single instruction process
// These functions can be used in sequencers
// from 3 param down to 1
u32 sq_I2C_MIO_StartJob(u32 u) {
  u32* p = (u32*) u;
  return I2C_MIO_Start(p[0], p[1]);//Param1);//1 parameter
}

// from 3 param down to 1
u32 sq_I2C_MIO_StopJob(u32 u) {
  u32* p = (u32*) u;
  return I2C_MIO_Stop(p[0], p[1]);//Param1);
}

u32 sq_I2C_MIO_MoveJob(u32 u) {
  u32* p = (u32*) u;
  return I2C_MIO_Move(p[0], p[1], p[2], p[3]);
}

u32 sq_I2C_MIO_DMA_Interrupt(u32 u) {
//  u32* p = (u32*) u;
//  DMA_Interrupt(p[0], (FunctionalState)p[1]);
  return 0;
}
 
//===================================================================
// translated example with sequencer scheme
static I2C_MasterIO gMIO;
static IO_PinTypeDef MIO_SDA, MIO_SCL;
static BasicTimer BT;
// create the instructions
static u32 SubAdr = 0x80;
static u8 SensorRegs[64];

OneJobType I2C_StartWrite_LPS25H = { sq_I2C_MIO_StartJob, (u32)&gMIO, 0xBA };
OneJobType I2C_Write_LPS25H = { sq_I2C_MIO_MoveJob, (u32)&gMIO, (u32)&SubAdr, 1, 1 }; // more coming, don't stop bit
OneJobType I2C_StartRead_LPS25H = { sq_I2C_MIO_StartJob, (u32)&gMIO, 0xBB };
OneJobType I2C_Read_LPS25H = { sq_I2C_MIO_MoveJob, (u32)&gMIO,  (u32)&SensorRegs, 64, 0 };

static u32 List[100]; // list of pointers
static StuffsArtery mySequence;

void I2C_MasterIO_Test(void) {

  u32 u = (u32)&gMIO;
  
  IO_PinInit(&MIO_SDA, PH7 ); // Initialize some quick pointers
  IO_PinInit(&MIO_SCL, PH8 ); // Initialize some quick pointers
  gMIO.SDA = &MIO_SDA; // global shared resource, to be generic later
  gMIO.SCL = &MIO_SCL; // global shared resource, to be generic later

  BT.FeedClockMHz = 96/2; // 96MHz (this should later come from RAM global structure)  
  NewBasicTimer_us(&BT, TIM6, 1); // usec countdown
  gMIO.BT = &BT;
  gMIO.BTn = 0; // use Countdown[0]

  gMIO.FeedClockMHz = 96; // MCU clock as it is by S/W
  gMIO.MaxBps = 400000; // 400khz
  
  NewI2C_MasterIO(&gMIO);
  SetI2C_MasterIO_Timings(&gMIO, 400*1000 );
  
  // all zero: no action
  NVIC_BasicTimersEnable(ENABLE);

  //===============
  StuffsArtery* P = &mySequence; // program
  NewSA(P, (u32)&List[0], countof(List));  
  gMIO.SA = P;
  
  while(1) {  
    
  AddToSA(P, (u32) &I2C_StartWrite_LPS25H);
  AddToSA(P, (u32) &I2C_Write_LPS25H);
  AddToSA(P, (u32) &I2C_StartRead_LPS25H);
  AddToSA(P, (u32) &I2C_Read_LPS25H);
  JobToDo((u32)P);
  
  while(P->FlagEmptied==0);
  
  };

};

