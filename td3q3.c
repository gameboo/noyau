#include <stdio.h>
#include <unistd.h>

FILE * fd;

void flock(FILE * f)
{
}

void frelease(FILE * f)
{
}

int main()
{
    fd = fopen();
    flock(fd);
    // use file
    frelease(fd);
    return 0;
}
