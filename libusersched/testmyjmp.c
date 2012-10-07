#include "usersched.h"
#include "stdio.h"
#include "unistd.h"

int f_idx, g_idx;

void f()
{
    int n = 0;

    while(1)
    {
        printf("Execute f : %d\n", n++);
        sleep(1);
        if(mysetjmp(f_idx) == 0)
            mylongjmp(g_idx);
    }    
}

void g()
{
    int m = 1000;

    while(1)
    {
        printf("Execute g : %d\n", m++);
        sleep(1);
        if(mysetjmp(g_idx) == 0)
            mylongjmp(f_idx);
    }    
}

int main()
{
    f_idx = 0;
    g_idx = 1;
    // utilisation d'une variable locale a main pour reperer le sp_main
    init_sched();

    if(mysetjmp(g_idx) == 0)
        f();
    else
        g();

    return -1;
}
