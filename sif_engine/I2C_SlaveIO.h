
#ifndef _I2C_SLAVE_IO_H_
#define _I2C_SLAVE_IO_H_

//****************************     I2C       **********************************
// this will implement I2C monitoring and/or memory slave function demonstration
// it could be used to implement custom serial interfaces like I3C or SPI slave, etc..
// I2C 100kHz @96MHz if compiled without any optimisation (debug)
// I2C 200kHz @96MHz if compiled with max optimisation (2us interrupt duration time, about 200 instructions)
// Make sure the interrupt priority is highest!
// If only using the spy mode, the bus speed can be much higher.

// The plumbing is to be improved (pin number and EXTI corresponding handler to be refined), this is the first functional release.
// We use S/W I2C Master feeding the EXTI SDA/SCL lines to debug, and there is a test program at the end of this source file.

// Key info to dig this code:
// We use 2 pin with any edge EXTI interrupt to catch ANY transition
// The EXTI interrupt entry is I2C_SLV_IO_EXTI_IRQHandler() for now
// The test program is I2C_SLV_IO_Test()
// We try to use the bit banding memory access for the GPIO output (low) or input (high), the EXTI PR bits.
// This runs with the standard library on the STM32F437


typedef struct {
  u32 State;    // this is the current state machine of the sequencer.
  u32 StatesCount; // this tells how many states exists
  // for each state, we have a function pointer table
  u32 *fnStates; // we limit state machines to 32 (otherwise, it becomes too unmanageable)
//  u8 *St2St; // index is the new state number, and output of the table is the effective (same) unretouted state number. Here is the option to hack a state machine.

  u32 ct;        // creator's pointer
//  u32 prefn;    // unused for now
//  u32 postfn;   // unused for now

//  u32 pMe; // points to this structure (the context)
//  u32 mySizeInByte; // to know the size of this structure
//  u32   (*fnIRQHandler)(u32); // debug only
} STateMAchine;

//============================================
// Let's think of I2C Slave function
// Imagine that the clock is stopped and we only work with events
// Then we need to first detect after an edge what happened

typedef enum {
  I2C_SDA0SCLRise, // 0 SCL went high and SDA is low  
  I2C_SDA1SCLRise, // 1 SCL went high and SDA is high
  I2C_SDA0SCLFall, // 2 SCL went low while SDA was high
  I2C_SDA1SCLFall, // 3 SCL just when low, so SDA can be modified by slave if needed (slave transmit a bit back to master)
  I2C_Start,    // 4 SDA goes low while SCL remains high
  I2C_Stop,     // 5 SDA goes high while SCL remains high
  I2C_Error,    // 6 wrong transition due to noise on bus, too fast bus or too slow MCU
  I2C_Timeout,  // 7 special event which is used to tell the SDA is maintained too long by the slave, SDA should revert as input
  I2C_Void,     // 8 events which do not cause any changes  
} I2C_Symbols;


typedef struct {
  // first, the slave state machine standard format
  STateMAchine STMA; // the state machine to handle events (unless linker is special, both I2C_Slave and STMA should have the same start address...

  IO_PinTypeDef* SDA; // we need the pointer to the pin
  IO_PinTypeDef* SCL; // we need the pointer to the pin

//===--- These are enablers for slave function to work or not, for I2C spy to work or not.
  u32 (*fnSlaveScheme)(u32); // within the interrupt handler, what slave scheme to do (if enabled) [1.5us @ 96MHz], non zero enabled.
  u32 (*fnSpyScheme)(u32); // any spy mode to run? [1us @ 96MHz] non zero is enabled 
  
  u8* Addresses;                // Slave addresses
  u8  AddressesCountof;         // The number of slave addresses. If 0, all the slave addresses will be acked
  u8  DecodedAddress;           // which of the slave address was last decoded
  
  u32 SubAddressInitial;        // the slave looks like a 256 SRAM (for hooks)
  u32 SubAddress;               // can be read when the bus is freed to check how many bytes were transferred

  u32 Bitmask;                  // This simulate a shift register token (8 bit for now)
  u32 Data;                     // This is the data to shift (8 bit for now)

  u8* Memory;                   // The memory of up to 256 bytes (we use 8 bit sub address)
  u16 MemoryCountof;            // Memory byte size
  
  I2C_Symbols I2C_Symbol; // updated from pin edge events (EXTI+GPIO source)
  u8 EventMask; // debug
  
//===--- spy members: (can be indirect later)
  ByteVein* BV; // this is where we will output the strings decoded by the spy.
  u8 I2C_Nibble;
  u8 I2C_BitCounter; 

//===---- Hooks to trigger higher level action ---===  
  // we will have to create jumping hooks from Slave to feed other with events.
  u32 (*fnBusFree)(u32); // the I2C bus is free (stop bit occured), this can be hooked to run some action (like turn off this slave, do something, turn it back on)
  u32 ctBusFree; // the context to return
  u32 (*fnReceived)(u32); // user data has been received (end of slave receive phase). This does not include any sub address
  u32 ctReceived; // the context to return
  u32 (*fnTransmitted)(u32); // data was transmitted out (end of slave transmitter phase), can be hooked to trigger higher level action
  u32 ctTransmitted; // the context to return
  
  u8 BusBusy : 1; // for monitoring live...     // status monitor bits
  u8 MemoryRead : 1; // set by interrupt, cleared by main loop  // status monitor bits
  u8 MemoryWritten : 1; // set by interrupt, cleared by main loop       // status monitor bits
  u8 TalkEnded : 1; // set by interrupt, cleared by main loop

  u8 FlagBusFree : 1;
  u8 FlagReceived : 1;
  u8 FlagTransmitted : 1;
} I2C_SlaveIO;


void TestI2C_SlaveIO(void);
void NewI2C_SlaveIO(I2C_SlaveIO* S, u8* SlaveAddresses, u8 SlaveAddressesCountof, u8* pMemory, u32 MemoryCountof);

u32 SpyI2C_SlaveIO(I2C_SlaveIO* u);
u32 UnSpyI2C_SlaveIO(I2C_SlaveIO* u);

#endif
