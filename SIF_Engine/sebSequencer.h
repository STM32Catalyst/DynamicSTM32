
#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

typedef struct {
  
  u32 (*fnJob)(u32); // may not be needed? The SPI Master HW structure assigned for this job (dispatch)
  u32 ctJobs[4]; // an array of parameters in u32 form, matching the need of the corresponding function
 
} OneJob_t; // for schedulers?

u32 StartJobToDoInBackground(u32 u); // This creates a non blocking, interrupt based sequence run
u32 StartJobToDoInForeground(u32 u); // This creates a blocking sequence run. Returns when complete
u32 JobToDo(u32 u);

#endif

