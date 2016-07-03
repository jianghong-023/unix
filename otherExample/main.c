#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include "mytpf.h"

/*
 * 漏桶实现
 */

#define CPS 20
#define BUFSZIE 1024
#define BURST	100


int main(int argc ,char **argv)
{
	int sfd,dfd = 1,size;
	char buf[BUFSZIE];
	int len,ret,pos;
	mytpf_t *tpf;
	
	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	
	tpf = mytpf_init(CPS,BURST);
	if(!tpf){
		fprintf(stderr,"mytpf_init()\n");
		exit(1);
	}
		
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
		
		size = mytpf_fetchtoken(tpf,BUFSZIE );
		if(size < 0)
		{
			fprintf(stderr,"mytpf_fetchtoken():%s\n",strerror(-size));
			exit(1);
		}
			
		while((len = read(sfd,buf,size)) < 0)
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
		if(size - len > 0)
			mytpf_returntoken(tpf,size - len);
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
	mytpf_destroy(tpf);
	exit(0);
}

