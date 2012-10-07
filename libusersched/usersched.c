#include "usersched.h"

///////////////////////////////////////////////////////////////////////////////
////////////////////// utils //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>

int mysetjmp(int idx)
{
    char current_top;

    if(setjmp(Tproc_table[idx].buf) == 0)
    {
        // sauvegarde ctx 
        Tproc_table[idx].stack_size = (unsigned int) top_stack - (unsigned int) &current_top;
        memcpy(Tproc_table[idx].stack, &current_top, Tproc_table[idx].stack_size);
        return 0;
    }
    else
    {
        // restaure ctx
        memcpy( &current_top, Tproc_table[elu].stack, Tproc_table[elu].stack_size);
        return 1;
    }
}

void mylongjmp(int idx)
{
    // change 
    elu = idx;
    longjmp(Tproc_table[elu].buf, 1);
}

int election()
{
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
    printf("\ncommut called with arg no = %d\n", no);
    alarm(TIC_SEC);
    int elected = election();
    if(mysetjmp(elu) == 0)
    {
        Tproc_table[elu].state = READY;
        elu = elected;
        printf("\nelu = %d, elected = %d\n", elu, elected);
        mylongjmp(elected);
    }
    else
    {
        Tproc_table[elu].state = RUNNING;
        printf("keep running process %d\n", elu);
    }
}
///////////////////////////////////////////////////////////////////////////////

void new_proc(void (*f)(int), int arg)
{
    int tmp_counter;
    for (tmp_counter = 0; tmp_counter < NB_PROCESS ; tmp_counter++)
    {
        if(Tproc_table[tmp_counter].state == FREE)
        {
            Tproc_table[tmp_counter].state = READY;
            if(mysetjmp(tmp_counter) == 0)
                return;
            else
                f(arg);
        }
    }
    // no entry available
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
