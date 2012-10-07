#include <setjmp.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct ctx
{
    char stack [1024];
    unsigned int stack_size;
};

struct ctx f_ctx;
struct ctx g_ctx;

jmp_buf buff, bufg;

char * top_stack;

void f()
{
    int n = 0;

    while(1)
    {
        printf("Execute f : %d\n", n++);
        sleep(1);
        if(setjmp(buff) == 0)
        {
            // sauvegarde ctx
            f_ctx.stack_size = (unsigned int) top_stack - (unsigned int) &n;
            memcpy(f_ctx.stack,&n,f_ctx.stack_size);
            // jmp
            longjmp(bufg, 1);
        }
        else
        {
            // restaure ctx
            memcpy(&n,f_ctx.stack,f_ctx.stack_size);
        }
    }    
}

void g()
{
    int m = 1000;

    while(1)
    {
        printf("Execute g : %d\n", m++);
        sleep(1);
        if(setjmp(bufg) == 0)
        {
            // sauvegarde ctx
            g_ctx.stack_size = (unsigned int) top_stack - (unsigned int) &m;
            memcpy(g_ctx.stack,&m,g_ctx.stack_size);
            // jmp
            longjmp(buff, 1);
        }
        else
        {
            // restaure ctx
            memcpy(&m,g_ctx.stack,g_ctx.stack_size);
        }
    }    
}

void main()
{
    // utilisation d'une variable locale a main pour reperer le sp_main
    char sp_main_marker;
    top_stack = &sp_main_marker;

    if(setjmp(bufg) == 0)
        f();
    else
        g();
}
