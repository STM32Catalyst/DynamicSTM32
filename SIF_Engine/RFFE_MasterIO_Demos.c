
#include "SebEngine.h"


//===================================================
// Now it is time to check if the communication works...
// We use the HVDAC from STMicro as test.

// MIPI RFFE Serial Bus, up to 26MHz (here more like 1 MHz as it is implemented with GPIOs, blocking mode)
// In the example, we use ST's HVDAC code STHVDAC-253M which provides triple 7 bit 20V DAC from 3-5V supply.
// This DAC is primarily used to drive a voltaged driven bias variable capactitor (PARASCAN)

#define REG_WRITE               (2<<5)
#define REG_READ                (3<<5)
#define EXT_REG_WRITE(bc)       (bc)

#define USID                    (7<<8)

#define HVDAC_WRITE_REG(a)      ( USID | REG_WRITE | (a) )
#define HVDAC_READ_REG(a)       ( USID | REG_READ  | (a) )
#define HVDAC_EXT_WRITE(bc)     ( USID | EXT_REG_WRITE(bc) )

#define DAC_C           0
#define DAC_B           1
#define DAC_A           2
#define PWR_TRIG        28
#define PROD_ID         29
#define MANU_ID         30
#define MANU_ID2        31

u8 HVDAC_Regs[32];

static RFFE_MasterIO_t myRFFE;
static IO_Pin_t RFFE_SDATA, RFFE_SCLK;
static u32 RFFE_List[100];
static StuffsArtery_t mySequenceRFFE;
//static Timer_t Timer;

const OneJob_t RFFE_StartRegWriteDACA = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_WRITE_REG(DAC_A)} };
const OneJob_t RFFE_StartRegWriteDACB = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_WRITE_REG(DAC_B)} };
const OneJob_t RFFE_StartRegWriteDACC = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_WRITE_REG(DAC_C)} };
const OneJob_t RFFE_StartRegWritePWR_TRIG = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_WRITE_REG(PWR_TRIG)} };

const OneJob_t RFFE_StartRegReadDACA = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(DAC_A)} };
const OneJob_t RFFE_StartRegReadDACB = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(DAC_B)} };
const OneJob_t RFFE_StartRegReadDACC = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(DAC_C)} };
const OneJob_t RFFE_StartRegReadPWR_TRIG = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(PWR_TRIG)} };
const OneJob_t RFFE_StartRegReadProdID = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(PROD_ID)} };
const OneJob_t RFFE_StartRegReadManuID = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(MANU_ID)} };
const OneJob_t RFFE_StartRegReadManuID2 = { sq_RFFE_MIO_StartJob, {(u32)&myRFFE, HVDAC_READ_REG(MANU_ID2)} };

const OneJob_t RFFE_WriteDACA = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, (u32)&HVDAC_Regs[DAC_A], 0, 1} };
const OneJob_t RFFE_WriteDACB = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, (u32)&HVDAC_Regs[DAC_B], 0, 1} };
const OneJob_t RFFE_WriteDACC = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, (u32)&HVDAC_Regs[DAC_C], 0, 1} };
const OneJob_t RFFE_WritePWR_TRIG = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, (u32)&HVDAC_Regs[PWR_TRIG], 0, 1} };

const OneJob_t RFFE_ReadDACA = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[DAC_A], 1} };
const OneJob_t RFFE_ReadDACB = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[DAC_B], 1} };
const OneJob_t RFFE_ReadDACC = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[DAC_C], 1} };
const OneJob_t RFFE_ReadPWR_TRIG = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[PWR_TRIG], 1} };
const OneJob_t RFFE_ReadProdID = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[PROD_ID], 1} };
const OneJob_t RFFE_ReadManuID = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[MANU_ID], 1} };
const OneJob_t RFFE_ReadManuID2 = { sq_RFFE_MIO_MoveJob,  {(u32)&myRFFE, 0, (u32)&HVDAC_Regs[MANU_ID2], 1} };

const OneJob_t RFFE_Stop = { sq_RFFE_MIO_StopJob,  {(u32)&myRFFE, 0} };


// we play with HVDAC RFFE device : STHVDAC-253M 3 output 7 bit high voltage DAC with embedded charge pump.
// That device could become a higher abstract cell later on...
void RFFE_Test(void) {
  
  MCUInitClocks();
  
  myRFFE.SDATA = NewIO_Pin(&RFFE_SDATA, PI6);
  myRFFE.SCLK = NewIO_Pin(&RFFE_SCLK, PI5);

  StuffsArtery_t* P = &mySequenceRFFE; // program
  NewSA(P, (u32)&RFFE_List[0], countof(RFFE_List));
  myRFFE.SA = P;
  
//  NewTimer_us(&Timer, TIM6, 1, GetMCUClockTree()); // usec countdown
//  myRFFE.Timer = &Timer;
  
  NewRFFE_MasterIO_RX_TX(&myRFFE); // this will configure the IO pins too.
  SetRFFE_MasterIO_Timings(&myRFFE, 26000000, GetMCUClockTree() ); // RFFE max clock speed is 26Mhz
  
  // Now we create a sequence to try on RFFE...
  
  while(1) {

    AddToSA(P, (u32) &RFFE_StartRegReadDACA);
    AddToSA(P, (u32) &RFFE_ReadDACA);
    AddToSA(P, (u32) &RFFE_Stop);

    AddToSA(P, (u32) &RFFE_StartRegReadDACB);
    AddToSA(P, (u32) &RFFE_ReadDACB);
    AddToSA(P, (u32) &RFFE_Stop);
    
    AddToSA(P, (u32) &RFFE_StartRegReadDACC);
    AddToSA(P, (u32) &RFFE_ReadDACC);
    AddToSA(P, (u32) &RFFE_Stop);
    
    AddToSA(P, (u32) &RFFE_StartRegReadPWR_TRIG);
    AddToSA(P, (u32) &RFFE_ReadPWR_TRIG);
    AddToSA(P, (u32) &RFFE_Stop);

    AddToSA(P, (u32) &RFFE_StartRegReadProdID);
    AddToSA(P, (u32) &RFFE_ReadProdID);    
    AddToSA(P, (u32) &RFFE_Stop);
    
    AddToSA(P, (u32) &RFFE_StartRegReadManuID);
    AddToSA(P, (u32) &RFFE_ReadManuID);
    AddToSA(P, (u32) &RFFE_Stop);
    
    AddToSA(P, (u32) &RFFE_StartRegReadManuID2);
    AddToSA(P, (u32) &RFFE_ReadManuID2);
    AddToSA(P, (u32) &RFFE_Stop);

    StartJobToDoInForeground((u32)&mySequenceRFFE);
    while(mySequenceRFFE.FlagEmptied==0);
    NOPs(1);
    mySequenceRFFE.FlagEmptied = 0; // clear the flag here

    HVDAC_Regs[DAC_A] = 0x55;
    AddToSA(P, (u32) &RFFE_StartRegWriteDACA);
    AddToSA(P, (u32) &RFFE_WriteDACA);
    AddToSA(P, (u32) &RFFE_Stop);

    HVDAC_Regs[DAC_B] = 0xAA;
    AddToSA(P, (u32) &RFFE_StartRegWriteDACB);
    AddToSA(P, (u32) &RFFE_WriteDACB);
    AddToSA(P, (u32) &RFFE_Stop);

    HVDAC_Regs[DAC_C] = 0x5A;
    AddToSA(P, (u32) &RFFE_StartRegWriteDACC);
    AddToSA(P, (u32) &RFFE_WriteDACC);
    AddToSA(P, (u32) &RFFE_Stop);
    
  // add this to the head of the program 
/*    StartJobToDoInForeground((u32)&mySequenceRFFE);
    while(mySequenceRFFE.FlagEmptied==0);
    NOPs(1);
    mySequenceRFFE.FlagEmptied = 0; // clear the flag here
*/    
  };// loop
  
}


