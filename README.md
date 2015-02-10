# DynamicSTM32
A different way to build solutions on STM32 MCUs (STM32F437 as baseline)

When a new MCU based concept needs to be developped quickly, there are few strategies:

1. The Interpreter Way: MCUs now support Javascript, Python, eLua languages. So just feed the source code to execute to the MCU as ASCII text (from serial port or from memory) and it will be executed right away: No need to recompile every bug fix or changes. The drawback is HW slowness.

2. The Compiler Way: MCUs can have their C or C++ source code project with libraries and assisting tool to create the skeleton of the code you need to test your solution with the MCU. The resulting performance is high, however, every compile itteration will be slower and slower as the codesize increases.

3. There would be possibly other approaches, such as using JVM virtual machines, JIT embedded compilers... all this being quite complex and heavy.

What if we could have nearly the best of (1) and (2)? Let's try to mimic the successful business that is living organism...

For this, we need a mean to run HW very fast with a special SW library of elements (cells)
Each of these elements are "floating". They need to be configured, and belong to a bigger structure. The configuration or the blueprints of what we architect does not need to be speedy. It could be described to an interpreter.

We could compare this to high speed trains running on tracks. On the dashboard, we can change the train tracks slowly, without impairing the speediness of the trains themselves...


Before reaching the level of embedding an interpreter, lots of basic blocks (cells) have to be built and refined.
The more a code structure is simple, the easier it is to maintain and evolve.

Looking at Clojure language (LISP derivative) books, some interesting concepts could be tried.

Have heard of RTOS/FreeRTOS/Chibios/MainLoop? RTOS is a multiple task management at a cost: Need to manage multiple tasks in memory (RAM). Clojure makes multiple Core or multiple Task management simple by only using re-entrant functions: Functions don't use global variables, they only use the passing parameters.

What if the passing parameter is in fact all the funciton need to work?
What if the passing parameter is a pointer to a structure?

Heard of callback functions? What if the callback is defined by a "Hook" which is 2 things: A pointer of function AND a pointer to the data structure the function needs to operate? If the function pointer is NULL, then raise a flag instead.

Next, we need something that is scalable, so we can reuse the same code for multiple implementations of the same "functionality". We also need a scheme which exploits Interrupt Handlers to the best and in a simple way.

This is the goal of this fun project.

For now, we are using the STM32F437 MCU as it has lots of peripherals, exists in BGA 176 pins and has enough memory to be future proof....

Have a look at this code. It can "invade" a normal project (being simple main loop based or RTOS), as it only needs to grab some of the interrupt handlers for himself. This is the myNVIC.h file and SIF_Engine.h

To create a system, it's like creating a soup. Create the ingredients you need (in RAM), prepare these ingredients (initialize them), link them together (blending), and cook them... by running the fireplace.

Some simple examples are present.

Have a look at the code example to understand the mecanics of the software.

Hope will be fun. The baseline works, and higher level smart features are still missing to make it very versatile.

This folder is added to an IAR bigger standard application project which is main loop'ed base (no RTOS).
The MCU is STM32437 with SYSCLK at 96MHz, and this clock is currently manually entered in this code.

Next big add-ons will be:

1. Support of General and Advanced Timers (programmable pulses and triggers)
2. Support of Analog functions (including DMAs) (DAC and ADCs)
3. Redesign the Slave State machines to use the same sequencer used in Master mode
4. Add SPI Slave support
5. Add Main loop task FIFO for non speed critical jobs, fired by timer interrupt or other background tasks
6. Add support to launch in parralel multiple sequencers and have a mean for them to wait for each others in a milestone step
7. Add 16x2 simple LCD Char using SPI in main loop
8. Add Keyboard (5 way joystick)
9. Add I2C EEPROM support
10. Add Bluetooth protocol to have COM Port accessible from Phone or PC apps. (protocol handlers)
11. Add Sensors support
12. Clocks management (knowing the external crystal value, find the lowest clock that fits the required performance)
13. HW Management (MCU finds out itself which HW resource to use like DMA Streams and Channels)
14. How to pause a sequence (say waiting for interrupt from Sensor to run I2C read, pause it for main loop to use I2C bus to talk to other devices asap)

Once the base line grows with more complex structures, things will become interesting


