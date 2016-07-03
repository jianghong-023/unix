#include <stdio.h>
#include <stdlib.h>
#include <glob.h>


#define PAT	"/etc/*.conf"




int main(int argc,char **argv)
{	int i;
	glob_t glb;
#if 0	
	printf("argc = %d \n",argc);
	
	for(i = 0 ; argv[i] !=NULL ; i++)
		printf("%s \n",argv[i]);
#endif

	
	int ret = glob(PAT,0,NULL,&glb);
	if(ret != 0)
		printf("glob() %s\n",ret);

	for(i = 0; i< glb.gl_pathc ; i++)
		printf("%s \n",glb.gl_pathv[i]);

	globfree(&glb);	
	exit(0);
}















