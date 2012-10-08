#ifndef USERSCHED
#define USERSCHED

#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

//////////
// ENUM //
//////////

// enum proc_state lists the different possible states of a Tproc process in the
// Tproc_table
enum proc_state
{
    FREE,   // the entry is available
    READY,  // the entry is ready to be elected
    RUNNING // the entry is now running
};

////////////
// STRUCT //
////////////

// Tproc struct holds information about a process
struct Tproc
{
    char stack [64536];			// the process'stack
    unsigned int stack_size;	// the process'stack size
    jmp_buf buf;				// the jmp_buf to save the process'context
    enum proc_state state;      // the process'state
};

///////////////////////
// GLOBALS & DEFINES //
///////////////////////

// NB_PROCESS
// the maximum number of processes
#define NB_PROCESS 100
// TIC_SEC
// the time in seconds between to calls to the commut function
#define TIC_SEC 1
// Tproc_table
// dhis table has NB_PROCESS struct Tproc entries. Each entry can describe a
// process
struct Tproc Tproc_table[NB_PROCESS];
// elu
// designates the elected process
unsigned int elu;
// top_stack
// Points at the last stack top
char * top_stack;

///////////////////////
// MACRO & FUNCTIONS //
///////////////////////

// internals :
// int mysetjmp(int idx);
// void mylongjmp(int idx);
// int election();
// void commut(int no);

// void init_sched();
// initializes misc stuff for the scheduling to work
#define init_sched()\
int tmp_counter;\
for (tmp_counter = 0; tmp_counter < NB_PROCESS ; tmp_counter++)\
    Tproc_table[tmp_counter].state = FREE;\
char dummy;\
top_stack = &dummy

// void new_proc(void (*f)(int), int arg);
// add a function to the scheduler
void new_proc(void (*f)(int), int arg);

// void start_sched();
// starts the scheduling
void start_sched();

#endif
