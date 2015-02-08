
#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

typedef struct {
  
  u32 (*fnJob)(u32); // may not be needed? The SPI Master HW structure assigned for this job (dispatch)
  u32 ctJobs[4]; // an array of parameters in u32 form, matching the need of the corresponding function
 
} OneJobType; // for schedulers?

u32 StartJobToDo(u32 u); // when starting a sequence
u32 JobToDo(u32 u);

#endif

