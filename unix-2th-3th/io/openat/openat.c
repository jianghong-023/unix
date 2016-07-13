#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>


/*
 * 
 *  为了使用openat 先使用一下线程
 *
 *
 * */


static void* start_pthr1(void *pthr)
{
    int ret = -1,i;
    char buf[50];
    DIR *dir;
    int fd,fd2;

    dir = opendir("./testdir/");  
    if(NULL == dir)  
    {  
        perror("open dir error");  
        return NULL;  
    }  

     fd2 = dirfd(dir);  
    if(-1 == fd2)  
    {  
        perror("fd2 error");  
        return NULL;  
    }  

   fd =  openat(fd2,"test.txt",O_CREAT|O_RDWR,0777);

    if(-1 == fd)  
    {  
        perror("opeat error");  
        close(fd2);
        return NULL;  
    }  

    for(i = 2048;i < 2048 * 2; i++){
        
        sprintf(buf,"this is first pthread %d\n",i);
        write(fd,buf,strlen(buf));
        usleep(1);
    }

    close(fd);
    closedir(dir);

    return NULL;
}

static void * start_pthr2(void *pthr2)
{
    int ret = -1,i;
    DIR *dir;
    char buf[50];

    int fd,fd2,fd3;

    dir = opendir("./testdir/");
    if(dir == NULL)
    {
        perror("opendir()");
        exit(1);
    }

   fd2 = dirfd(dir);

    fd3 = openat(fd2,"test.txt",O_CREAT|O_RDWR,0777);
    if( -1 == fd ){
        
        perror("openat()");
        return NULL;
    }


    for(i = 0; i < 2048; i++){
        
        sprintf(buf,"this is scend pthread %d \n",i);
        write(fd3,buf,strlen(buf));

        usleep(1);
    }


    closedir(dir);
    close(fd);
    return NULL;
}



static void thread_manager()
{
    pthread_t pthr1,pthr2;

    int result;
    
   result = pthread_create(&pthr1,NULL,start_pthr1,NULL);
    if(result < 0){
        fprintf(stderr,"pthread_create() : %s",strerror(errno));
        exit(1);
    }


    result += pthread_create(&pthr2,NULL,start_pthr2,NULL);
    if(result < 0){
        
        fprintf(stderr,"pthread_create(): %s",strerror(errno));
        exit(1);

    }

    pthread_join(pthr1,NULL);

    pthread_join(pthr2,NULL);


}

int main(void)
{
    

    thread_manager();
    
    exit(0);
}
