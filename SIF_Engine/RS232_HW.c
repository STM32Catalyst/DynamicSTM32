
#include "sebEngine.h"

u32 RS232_USARTs_BV_TX_ITE_Update(u32 u);

// the base functions
// The code should be in RAM to change easily the behaviour, but the linker will be lost...
static u32 RS232_UARTs_IRQHandler(u32 u) {

  Rs232_t* RS = (Rs232_t*) u;
// receive first, because we need to read the DR  
  if((RS->USART->CR1 & USART_FLAG_RXNE)&&(RS->USART->SR & USART_FLAG_RXNE)) {  // dummy read of the data to clear the pending interrupt, this will clear PE, RXNE and FE bits
    // WHAT IF BV IS NOT ASSIGNED?
    if(RS->BV_RX==0)      while(1);
    // byte received, prep to place in BV_RX
    AddToBV(RS->BV_RX, RS->USART->DR); // add this byte to the head of this data (fresh is head), and possibly trigger a use of it, we can't delay it!
  };
  
  // we should also add the error management here
  if((RS->USART->CR1 & USART_FLAG_TXE)&&(RS->USART->SR & USART_FLAG_TXE)) { // if ready to transmit by interrupt...
    // WHAT IF BV IS NOT ASSIGNED?
    if(RS->BV_TX==0)      while(1);
    // get a byte from BV_TX and send it out
    // WHAT IF BV IS EMPTY?
    //RS232_USARTs_BV_TX_ITE_Update(u); // update ITE based on BV_TX count. (0 disable, enable otherwise)      
    if(RS->BV_TX->bCount==0) // nothing to send now, kill the interrupt, nothing to send
      while(1);

    ClipBV_Down(RS->BV_TX);
    RS->USART->DR = RS->BV_TX->Out; // write the cut byte from tail to usart's DR (this will trigger possible reading event if hooked up)    // WHAT IF BV IS EMPTY?
  };

  return 0;
}

// In the RX Interrupt, if we have to provide a data byte to TX and the FIFO is empty, no choice: We have to disable the TX interrupt.
// We will tell the FIFO to reenable the interrupt as soon as the FIFO is no longer empty.

void NewRs232HW(Rs232_t* RS, USART_TypeDef* USART, IO_Pin_t* RX, IO_Pin_t* TX) {

  // first, we have to enable the GPIO and Clock of the peripheral (for now done already for all)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);// enable the peripheral clock
  
  RS->USART=USART;
  RS->RX = RX;
  RS->TX = TX;
  
//  Hook_Rs232_IRQ_fn_ct(RS, (u32)RS232_UARTs_IRQHandler, (u32)RS);// we assume handlers are in flash or already loaded
  HookIRQ_PPP((u32)USART, (u32)RS232_UARTs_IRQHandler, (u32)RS); // to check if it works well.
 
  RS->Print.fnPutChar = (u32) RS232_putchar;
  RS->Print.ctPutChar = (u32) RS;
}


void SetRs232Timings(Rs232_t* RS, u32 BaudRate, u32 Parity2, u32 StopBits){ // Things depending on time and internal clocks
// we don't do CTS and RTS for now
// hardcoded for first heartbeat test  
  /* USARTy and USARTz configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - Even parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  if(RS->Clocks==0) while(1); // define the clock tree first
    
  RS->UI.USART_BaudRate = 115200;
  RS->UI.USART_WordLength = USART_WordLength_9b;
  RS->UI.USART_StopBits = USART_StopBits_1;
  RS->UI.USART_Parity = USART_Parity_Even;
  RS->UI.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  RS->UI.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(RS->USART, &RS->UI); // this relies on std lib core frequency info from std lib. Should be modified to be free from it.
  
  MCU_NodeDependency_t* MND = GetSignal2InfoBy_PPP((u32)RS->USART);
  if(RS->Clocks->OutAPB1Clk_Hz.Value==0) { // set the min-max range of the corresponding APBx clock tree
    if(MND->fnClk==RCC_APB1PeriphClockCmd) {
        MakeItNoLessThan(RS->Clocks->OutAPB1Clk_Hz.Min, MHzToHz(8));
    }else{
        MakeItNoLessThan(RS->Clocks->OutAPB2Clk_Hz.Min, MHzToHz(8));
    }
  };
  
}

void SetRs232Format(Rs232_t* RS) {
  
}

void ConfigureRs232HW(Rs232_t* R) {

  // second, we initialize the IO pins when they exist
  // configure the GPIOs
  // configure RX pin
  if(R->RX) {
    ConfigurePinAsInputTrigger(R->RX);
    IO_PinConfiguredAs(R->RX, GetPinAF(R->RX->Name, (u32) R->USART)); // to check it works //IO_PinConfiguredAs(RS->RX,GPIO_AF_USART1); // to change later! based on pin name
  }

  if(R->TX) {
    ConfigurePinAsPushPullOutputPU(R->TX);
    IO_PinConfiguredAs(R->TX, GetPinAF(R->TX->Name, (u32) R->USART)); // to check it works//IO_PinConfiguredAs(RS->TX,GPIO_AF_USART1); // to change later! based on pin name
  }

}

void EnableRs232HW(Rs232_t* R) {

  USART_Cmd(R->USART, ENABLE); // The HW is ready, later we can enable interrupts to kick real transit of data
  
  // we can enable the RX interrupt assuming that the BV_RX is already setup
  R->USART->CR1 |= USART_IT_RXNE;
}



void NVIC_Rs232sEnable(FunctionalState Enable) {
  // all the interrupt handlers exist and clear flags by default
  // call this only when final activation of the EXTI interrupt generation mecanism
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = Enable;
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  /*
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  */
  // These IRQ can be always enabled as the timer has its own interrupt enable bit.
}

static u32 RS232_USARTs_BV_TX_EnableInterrupt(u32 u) {
  Rs232_t* RS = (Rs232_t*) u;
  RS->USART->CR1 |= USART_FLAG_TXE; // (re)activate interrupts  
  return 0;
}

static u32 RS232_USARTs_BV_TX_DisableInterrupt(u32 u) {
  Rs232_t* RS = (Rs232_t*) u;
  RS->USART->CR1 &= ~USART_FLAG_TXE; // deactivate interrupts
  return 0;
}

void SetRs232BVs(Rs232_t* RS, ByteVein_t* BV_TX, ByteVein_t*BV_RX) {
  
  RS->BV_TX = BV_TX;
  RS->BV_RX = BV_RX;
  // add the proper hook for incoming data flow (if BV_TX is empty and we have nothing to transmit out, we stop the interrupt and we remind BV_TX to reactivate the Interrupt as soon as the FIFO is not empty anymore)
//-  BV_TX->fnNoLongerEmpty = RS232_USARTs_BV_TX_EnableInterrupt;//RS232_USARTs_BV_TX_ITE_Update;
//-  BV_TX->ctNoLongerEmpty = (u32) RS; // this RS232 context
  HookBV_NoLongerEmpty(BV_TX, RS232_USARTs_BV_TX_EnableInterrupt, (u32) RS);
  
//-  BV_TX->fnEmptied = RS232_USARTs_BV_TX_DisableInterrupt;//u32_fn_u32;
//-  BV_TX->ctEmptied = (u32) RS;
  HookBV_Emptied(BV_TX, RS232_USARTs_BV_TX_DisableInterrupt, (u32) RS);
}




// SEND A DATA BYTE TO THE LCD
u32 RS232_putchar(Rs232_t* RS, u32 c) {

  AddToBV(RS->BV_TX, c); // add the character to the Transmit FIFO
  return 0;
}


//==================================
// Self tuning clock
u32 GetRs232HW_Timings( Rs232_t* RS) {
  
  // the minimum speed depends on the baudrate and this will impact a clock frequency.
  // to simplify, we will express the minimum clock frequency for both APB1 and APB2.
  // we will assume that the min clock rate is 16MHz fixed.
  
//  T->SysClk_Hz = 16000000;
  return 0; // prescaler have a fine granularity as they were used with low clock frequency 8 bit MCUs
}




