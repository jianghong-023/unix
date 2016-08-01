#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc,char **argv)
{
    int i;
    struct stat buf;
    char *prt;
    
    for(i= 0 ; i < argc ; i++){
        
        printf("%s: ",argv[i]);

        if(lstat(argv[i],&buf) < 0){
            
            perror("-");

            continue;

        }

        if(S_ISREG(buf.st_mode))
            prt = "is it a regular file";
        else if(S_ISDIR(buf.st_mode))
            prt = "is it a directory";
        else if(S_ISCHR(buf.st_mode))
            prt = "charactor device";
        else if(S_ISBLK(buf.st_mode))
            prt = "block device";
        else if(S_ISFIFO(buf.st_mode))
            prt = "FIFO (name pipe)";
        else if(S_ISLNK(buf.st_mode))
            prt = "symbolic link";
        else if(S_ISSOCK(buf.st_mode))
            prt = "sockt";
        else
            prt = "** unknown mode **";

        printf("%s\n",prt);
    }



    exit(0);
}

