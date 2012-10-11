#include <stdio.h>
#include <unistd.h>

void flock(FILE * fd)
{
    struct inode * ino = u.u_ofile[fd]->f_inode;
    while(ino->i_flag & ILOCK)
    {
        ino->i_flag |= IWANT;
        sleep(ino, PINOD);
    }
    ino->i_flag |= ILOCK;
}

void frelease(FILE * fd)
{
    struct inode * ino = u.u_ofile[fd]->f_inode;
    ino->i_flag &= ~ILOCK;
    if(ino->i_flag & IWANT)
    {
        ino->i_flag &= ~IWANT;
        wakeup(ino);
    }
}

int main()
{
    FILE * fd;
    fd = fopen();
    flock(fd);
    // use file
    frelease(fd);
    return 0;
}
