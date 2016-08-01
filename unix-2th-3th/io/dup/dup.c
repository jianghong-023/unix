#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


#define mutex_flag 
/********************************
* dup 依据以打开的文件描述进行
* 复制，返回一定是一个当前未用
* 的最小描述符，但并不close旧的
* 描述符，
* dup 等价于fcntl(fd,F_DUPFD,0) 
*
**********************************/


int main(void)
{
    int fd,fd2,newfd;
#ifndef mutex_flag
    fd = dup(1);
#else
    fd = fcntl(1,F_DUPFD,0);
#endif


    printf("file dicription [%d] \n",fd);

#ifndef mutex_flag
   newfd =  dup2(fd,fd2);
#else

    close(fd2);
    newfd = fcntl(fd,F_DUPFD,fd2);
    if(newfd == -1)
        perror("fcntl()");
#endif
    printf("file dicription [dup2 : %d]\n",newfd);
    exit(0);
}




