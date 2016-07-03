#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

/*
 * 漏桶实现
 */

#define CPS 20

#define BUFSZIE CPS
static volatile loop = 0;

static void alrm_hander(int s)
{
	
	loop = 1;
	//alarm(1);
}

int main(int argc ,char **argv)
{
	int sfd,dfd = 1;
	char buf[BUFSZIE];
	int len,ret,pos;
	struct itimerval itv;
	
	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	
	signal(SIGALRM,alrm_hander);
	//alarm(1);
	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;
	
	setitimer(ITIMER_REAL,&itv ,NULL);
	
	do{
		
		sfd = open(argv[1],O_RDONLY);
		if(sfd < 0)
		{
			if(errno != EINTR)
			{
				perror("open()");
				exit(1);
			}
		}
	}while(sfd < 0);
	
	while(1){
		
		while(!loop){
			pause();
		}
		loop = 0;
		
			
		while((len = read(sfd,buf,BUFSZIE)) < 0)
		{
			if(errno == EINTR)
			{
				continue;
			}
			
			perror("read()");
			break;
			
		}
		
		if(len == 0)
			break;
		
		pos = 0;
		
		while(len > 0)
		{
			ret = write(dfd,buf+pos,len);
			if(ret < 0)
			{
				if(errno == EINTR)
					continue;
				perror("write()");
				exit(1);
			}
			
			
			
			pos += ret;
			len -= ret;
			
		}
		
	}
	
	close(dfd);
	close(sfd);
	
	exit(0);
}

