
#ifndef _STMA_H_
#define _STMA_H_

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

u32 STMA_Run(u32 u);

#endif

/*
typedef void transition_func_t( instance_data_t *data );

void do_initial_to_foo( instance_data_t *data );
void do_foo_to_bar( instance_data_t *data );
void do_bar_to_initial( instance_data_t *data );
void do_bar_to_foo( instance_data_t *data );
void do_bar_to_bar( instance_data_t *data );

transition_func_t * const transition_table[ NUM_STATES ][ NUM_STATES ] = {
    { NULL,              do_initial_to_foo, NULL },
    { NULL,              NULL,              do_foo_to_bar },
    { do_bar_to_initial, do_bar_to_foo,     do_bar_to_bar }
};

state_t run_state( state_t cur_state, instance_data_t *data ) {
    state_t new_state = state_table[ cur_state ]( data );
    transition_func_t *transition =
               transition_table[ cur_state ][ new_state ];

    if ( transition ) {
        transition( data );
    }

    return new_state;
};
*/


/*
FSM {
  STATE(x) {
    ...
    NEXTSTATE(y);
  }

  STATE(y) {
    ...
    if (x == 0) 
      NEXTSTATE(y);
    else 
      NEXTSTATE(x);
  }
}

With the following macros defined
#define FSM
#define STATE(x)      s_##x :
#define NEXTSTATE(x)  goto s_##x

This can be modified to suit the specific case. For example, you may have a file FSMFILE that you want to drive your FSM, so you could incorporate the action of reading next char into the the macro itself:
#define FSM
#define STATE(x)         s_##x : FSMCHR = fgetc(FSMFILE); sn_##x :
#define NEXTSTATE(x)     goto s_##x
#define NEXTSTATE_NR(x)  goto sn_##x

now you have two types of transitions: one goes to a state and read a new character, the other goes to a state without consuming any input.

You can also automate the handling of EOF with something like:
#define STATE(x)  s_##x  : if ((FSMCHR = fgetc(FSMFILE) == EOF)\
                             goto sx_endfsm;\
                  sn_##x :

#define ENDFSM    sx_endfsm:
*/
