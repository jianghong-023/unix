#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>


//#define READ_FLAG
#define MAXSIZE   (1024)

/**
 * creat 函数测试
 * creat  
 * creat 的功能与open一样，也可以用于打开或者创建一个文件
 *
 *
 * ****************/


int main(void)
{

    int32_t fd;
    int8_t *wr_string =  "test write successfu";
    int8_t buf[MAXSIZE];

    fd = creat("./test.in",S_IRWXU|S_IRWXG|S_IRWXO);
    if(fd < 0){
        perror("creat()");
        exit(1);
    }
    
    write(fd,wr_string,strlen(wr_string));


#ifdef READ_FLAG


    close(fd);
    fd = open("./test.in",O_RDONLY,S_IRWXU|S_IRWXG|S_IRWXO);
    if(fd < 0){
        perror("open()");
        exit(1);
    }


    read(fd,buf,MAXSIZE);
#else
    
    read(fd,buf,MAXSIZE);
#endif

    printf("read :%s \n",buf);

    close(fd);

    exit(0);

}

/****************************************
 *
 * 
 *
 *
 *
 *****************************************/
