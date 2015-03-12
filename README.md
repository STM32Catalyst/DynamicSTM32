# DynamicSTM32
Taking a step back at existing ways to build a solution on STM32, and use of experience in the field to try a fun alternative and complementary scheme.

A custom demoboard based on STM32F437/BGA176 allow testing of many peripherals. Both USB are connected, Crystal is 24MHz, SYSCLK is 96MHz (to remain <100mA USB supply limit).

This pet project came up after reading from ChiBios, Clojure, Espruino, Blockly, Arduino,...

When a new MCU based concept needs to be developped quickly, there are few strategies:

1. The Interpreter Way: MCUs now support Javascript, Python, eLua languages. So just feed the source code to execute to the MCU as ASCII text (from serial port or from memory) and it will be executed right away: No need to recompile every bug fix or changes. The drawback is HW slowness.

2. The Compiler Way: MCUs can have their C or C++ source code project with libraries and assisting tool to create the skeleton of the code you need to test your solution with the MCU. The resulting performance is high, however, every compile itteration will be slower and slower as the codesize increases.

3. There would be possibly other approaches, such as using JVM virtual machines, JIT embedded compilers... all this being quite complex and heavy.

What if we could have nearly the best of (1) and (2)? Let's try to mimic the successful business that is living organism...

For this, we need a mean to run HW very fast with a special SW library of elements (cells)
Each of these elements are "floating". They need to be configured, and belong to a bigger structure. The configuration or the blueprints of what we architect does not need to be speedy, only its execution needs to.
The configuration and link of these cells in RAM  could be described to an interpreter (like Espruino's Javascript Interpreter)

We could compare this to high speed trains running on tracks. On the dashboard, we can change the train tracks slowly, without impairing the speediness of the trains themselves...

Before reaching the level of embedding an interpreter, lots of basic blocks (cells) have to be built and refined.
The more a code structure is simple, the easier it is to maintain and evolve. Also the functionality in the MCU should be translated into simple C tables (database), for example for the MCU to find out which DMA/Stream/Channel match a particular signal functionality.

Here are some of the thoughts guiding me in the development (functionality paramount before formalizing the source code to coding standard)

- 32 bit allows to store either a data or an address, universal
- Clojure uses weak type variables which also can be "not defined" "not available"
- A NULL pointer does similar job...
- Java is passing function parameters as hidden pointers (or in C any complex parameter is assumed pointer)
- Interrupt handlers are the time critical processes, their latency and duration should be kept to minimum
- A Job is a function pointer and its passing parameter (pointer to a structure which contains the function needed data)
- ARM instructions can be conditional, so, for free we can call a function only if its pointer is non zero...
- The code should give enough flexibility in a dynamic fashion, not precompiled.
- The code should be scalable and re-entrant (the same function can be used for any type of timers)
- All resource is a structure in RAM. Need more timers? Create more of them in RAM anytime needed!

As we have a 100kb project, the goal is to transform the code from optimized hardcoded to dynamically reconfigurable in the debugger or by terminal-style applications. This code is only the portion which can be added to any project (RTOS or not), and selectively "hook" itself from the NVIC interrupt handlers. (Cube uses __weak link statement, so it should be easy)

Each code release is like a new born code, each time its DNA is altered a bit to become a better future-proof fit :-)

We improve our code with customer's or colleague's feedback. What if we could C code feedbacks and let the MCU converge to an optimized setup? What if the MCU has enough feedbacks like remaining power consumption, interrupt overrun, missed deadlines, so he can modulate its clock frequency itself? That's like Chappie movie :-)

To create a system, it's like creating a soup. Create the ingredients you need (in RAM), prepare these ingredients (initialize them), link them together (blending), and cook them... by running the fireplace.

Some simple examples are present.

Have a look at the code example to understand the mecanics of the software.

Hope will be fun. The baseline works, and higher level smart features are still missing to make it very versatile.

The road is long, at least the vision exist and the path is clear, let's run!


