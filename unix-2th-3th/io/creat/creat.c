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
 * 在早期的UNIX版本中,open的第二个参数只能是0,1或2。没有办法打开一
 * 个尚未存在的文件，因此需要另一个系统调用creat以创建新文件。现在,
 * open函数提供了选择项O_CREAT和O_TRUNC，于是也就不再需要creat函数了。
 *
 * creat的一个不足之处是它以只写方式打开所创建的文件。在提供open的新版本之前
 * 如果要创建一个临时文件，并要先写该文件，然后又读该文件,
 * 则必须先调用 creat，close，然后再调用open
 * 它等价于 open(pathname, O_RDWR|O_CREAT|O_TRUNC, mode);
 *
 * close函数用来关闭一个已经打开的文件，它有如下特点：
 * 1）关闭一个文件时也释放该进程加在该文件上的所有记录锁
 * 2）当一个进程终止时，它所有的打开文件都由内核自动关闭。
 * 很多程序都使用这一功能而不显式地用close闭打开的文件
 *
 *
 *****************************************/
