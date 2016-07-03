#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
*
*  测试系统支持多少个连续打开的描述符
*
** ****/


int main(int argc ,char **argv)
{
    int fd,i = 0;

    if(argc < 2){
        printf("Usage ...\n");
        printf("./programbin <filedescription>\n");
        exit(1);
    }

    while(1){
        fd = open(argv[1],O_RDONLY);
        if(fd < 0){
          perror("open");
        break;
    
        }
        ++i;
    }
    printf("fd cout :%d\n" ,i);
    exit(0);
}

/*
 * make open
 * ./open  open.c
 *
 * 运行结果：
 * open: Too many open files
 * fd cout :1021
 * 为什么是1021?用命令看一下系统支持多少个文件系统描述符
 * unix/unix-2th-3th/io/open# ulimit -a
 * core file size          (blocks, -c) 0
 * data seg size           (kbytes, -d) unlimited
 * scheduling priority             (-e) 0
 * file size               (blocks, -f) unlimited
 * pending signals                 (-i) 7720
 * max locked memory       (kbytes, -l) 64
 * max memory size         (kbytes, -m) unlimited
 * open files                      (-n) 1024 「可以看到支持1024个」 
 * pipe size            (512 bytes, -p) 8
 * POSIX message queues     (bytes, -q) 819200
 * real-time priority              (-r) 0
 * stack size              (kbytes, -s) 8192
 * cpu time               (seconds, -t) unlimited
 * max user processes              (-u) 7720
 * virtual memory          (kbytes, -v) unlimited
 * file locks                      (-x) unlimited
 *
 * 
 * 为什么只有1021个呢，还有3个呢？
 *
 * 原因是按照惯例，UNIXshell使文件描述符0与进程的标准输入相结合，
 * 文件描述符1与标准输出相结合，文件描述符 2与标准出错输出相结合。
 * 这是UNIXshell以及很多应用程序使用的惯例，而与内核无关。
 * 尽管如此，如果不遵照这种惯例，那么很多UNIX应用程序就不能工作
 * 1021+3 =1024 
 *
 *  在POSIX.1应用程序中，幻数0,1,2应被代换成符号常数STDIN_FILENO
 *  ,STDOUT_FILENO和STDERR_FILENO,这些常数都定义在头文件<unistd.h>中
 *
 * 当然可以改变描述的大小,用命令 ulimit -n  2048
 * 在函数中也可用ulimit(int cmd, long newlimit) 去改变设置
 *
 * open 函数的功能：
 * 它可以创建文件或者打开一个文件
 *
 * 下面对open函数原型的参数进行罗列的说明一下
 * int open(const char *pathname, int flags);
 * int open(const char *pathname, int flags, mode_t mode);
 *
 * int openat(int dirfd, const char *pathname, int flags);
 * int openat(int dirfd, const char *pathname, int flags, mode_t mode);
 *
 * pathname是要打开或创建的文件的名字
 * oflag参数可用来说明此函数的多个选择项的行为
 * O_RDONLY 只读打开
 * O_WRONLY 只写打开
 * O_RDWR 读、写打开
 *
 * 注意;很多实现将 O_RDONLY定义为0，O_WRONLY定义为1，O_RDWR定义为2th
 *
 *
 * ***/
