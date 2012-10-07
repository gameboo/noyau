#ifndef USERSCHED
#define USERSCHED

#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

enum proc_state
{
    FREE,
    READY,
    RUNNING
};

// Tproc struct holds information about a process
struct Tproc
{
    char stack [64536];			// the process'stack
    unsigned int stack_size;	// the process'stack size
    jmp_buf buf;				// the jmp_buf to save the process'context
    enum proc_state state;
};

// Tproc_table
// This table has NB_PROCESS struct Tproc entries. Each entry can describe a
// process.
#define NB_PROCESS 100
struct Tproc Tproc_table[NB_PROCESS];

// elu global variable
// Designates the elected process
unsigned int elu;

// top_stack global variable
// Points at the last stack top
char * top_stack;

//int mysetjmp(int idx);
//void mylongjmp(int idx);

// void init_sched();
#define init_sched()\
int tmp_counter;\
for (tmp_counter = 0; tmp_counter < NB_PROCESS ; tmp_counter++)\
    Tproc_table[tmp_counter].state = FREE;\
char dummy;\
top_stack = &dummy

void new_proc(void (*f)(int), int arg);

#define TIC_SEC 1
void start_sched();

#endif
