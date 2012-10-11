#include "usersched.h"

///////////////////////////////////////////////////////////////////////////////
////////////////////// utils //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>

void print_Tproc_table()
{
    int i;
    for( i = 0; i < NB_PROCESS; i++)
    {
        printf("Tproc_table[%d].state = %d\n", i, Tproc_table[i].state);
        printf("Tproc_table[%d].buf = %d\n", i, Tproc_table[i].buf);
        printf("Tproc_table[%d].stack = %d\n", i, Tproc_table[i].stack);
        printf("Tproc_table[%d].stack_size = %d\n", i,Tproc_table[i].stack_size);
    }
}

int mysetjmp(int idx)
{
    // dummy variable
    char current_top;

    // first call to mysetjmp ?
    //if(setjmp(Tproc_table[idx].buf) == 0)
    if(sigsetjmp(Tproc_table[idx].buf, 42) == 0)
    {
        // save process stack
        Tproc_table[idx].stack_size = (unsigned int) top_stack - (unsigned int) &current_top;
        memcpy(Tproc_table[idx].stack, &current_top, Tproc_table[idx].stack_size);
        // returning 0
        return 0;
    }
    // landing from mylongjmp
    else
    {
        // restore process stack
        memcpy( &current_top, Tproc_table[elu].stack, Tproc_table[elu].stack_size);
        // returning 1
        return 1;
    }
}

void mylongjmp(int idx)
{
    // change elected process
    elu = idx;
    // do the jump
    //longjmp(Tproc_table[elu].buf, 1);
    siglongjmp(Tproc_table[elu].buf, 1);
}

int election()
{
    // simple Tproc_table parsing, selecting the next READY Tproc
    //TODO might turn into an endless loop if no Tproc is in READY state
    int tmp = elu;
    do
    {
        tmp++;
        tmp%=NB_PROCESS;
    }
    while(Tproc_table[tmp].state != READY);
    return tmp;
}

void commut(int no)
{
    printf("######## commut called with arg no = %d ########\n", no);
    //print_Tproc_table();
    alarm(TIC_SEC);
    int elected = election();
    if(mysetjmp(elu) == 0)
    {
        printf("######## Tproc %d -> READY state\n", elu);
        printf("######## Tproc %d -> RUNNING state\n", elected);
        //TODO mask signals
        Tproc_table[elu].state = READY;
        Tproc_table[elected].state = RUNNING;
        //TODO restore signals
        mylongjmp(elected);
    }
}
///////////////////////////////////////////////////////////////////////////////

void new_proc(void (*f)(int), int arg)
{
    int tmp_counter;
    for (tmp_counter = 0; tmp_counter < NB_PROCESS ; tmp_counter++)
    {
        //TODO mask signals
        if(Tproc_table[tmp_counter].state == FREE)
        {
            Tproc_table[tmp_counter].state = READY;
            //TODO restore signals
            if(mysetjmp(tmp_counter) == 0)
                return;
            else
            {
                f(arg);
                commut(0);
            }
        }
        //TODO restore signals
    }
    // no entry available
    printf("error : couldn't create process, no entry available in Tproc_table\n");
    exit(-1);
}

void start_sched()
{
    struct sigaction alarm_action;
    alarm_action.sa_handler = commut;
    alarm_action.sa_flags = 0;
    sigfillset(&alarm_action.sa_mask);
    sigaction(SIGALRM, &alarm_action, NULL);

    alarm(TIC_SEC);

    int elected = election();
    Tproc_table[elected].state = RUNNING;
    mylongjmp(elected);

    while(1);
}
