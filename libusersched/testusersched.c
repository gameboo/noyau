#include "usersched.h"
#include <stdio.h>

void f(int nb)
{
    int i = 0;
    for(i = 0; i < 10; i++)
    {
        printf("I'm f(%d), step #%d\n", nb, i);
        usleep(200000);
    }
}

void g(int nb)
{
    int i = 0;
    for(i = 0; i < 10; i++)
    {
        printf("I'm g(%d), step #%d\n", nb, i);
        usleep(500000);
    }
}

int main()
{
    init_sched();

    new_proc(f, 1);
    new_proc(f, 2);
    
    new_proc(g, 1);
    new_proc(g, 2);

    start_sched();

    return -1;
}
