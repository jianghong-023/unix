#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#include "mytpf.h"

typedef void (*sighandler_t)(int);



static volatile int initd = 0;
static sighandler_t alarm_handler_save;

struct mytpf_st{
	
	int cps;
	int burst;
	int token;
	int pos;
};

static struct mytpf_st *job[MYTPF_MAX];

static int  get_free_pos(void)
{
	
	int i;
	for(i = 0; i< MYTPF_MAX ;i++)
	{
		if(job[i] == NULL)
			return i;
	}
	
	return -1;
}

static void alam_hander(int s)
{
	int i;
	
	alarm(1);
	
	for(i = 0; i < MYTPF_MAX ;i++)
	{
		if(job[i] != NULL)
		{
			job[i]->token += job[i]->cps;
			
			if(job[i]->token > job[i]->burst)
				job[i]->token = job[i]->burst;
		}
	}
	
}



static void module_unload(void)
{	
	int i;
	
	signal(SIGALRM,alarm_handler_save);
	alarm(0);
	
	for(i = 0; i < MYTPF_MAX ;i++)
	{
		free(job[i]);
	}
}

static void module_load(void)
{
	
	alarm_handler_save = signal(SIGALRM,alam_hander);
	alarm(1);
	atexit(module_unload);
}

mytpf_t *mytpf_init(int cps,int burst)
{
	int pos;
	
	struct mytpf_st *me = NULL;
	
	if(initd == 0)
	{
		module_load();
		
		initd = 1;
	}
	
	pos = get_free_pos();
	if(pos < 0)
		return NULL;
	
	me = malloc(sizeof(*me));
	if(!me)
		return NULL;
	me->token = 0;
	me->cps = cps;
	me->burst = burst;
	me->pos = pos;
	job[pos] = me;
	
	return me;
}

static int min(int a,int b)
{
	if(a < b)
		return a;
	
	return b;
}

int mytpf_fetchtoken(mytpf_t *ptr,int size)
{
	int n;
	
	struct mytpf_st *me = ptr;
	
	if(size <= 0)
		return -EINVAL;
	
	while(me->token <= 0)
		pause();
	
	n = min(me->token,size);
	
	me->token -= n;
	
	return n;
}


int mytpf_returntoken(mytpf_t *ptr,int size)
{
	
	struct mytpf_st *me = ptr;
	
	if(size <= 0)
		return -EINVAL;
	
	me->token += size;
	
	if(me->token > me->burst)
		me->token = me->burst;
	
	return size;
}

int mytpf_destroy(mytpf_t *ptr)
{
	struct mytpf_st *me = ptr;
	
	job[me->pos] = NULL;
	free(me);
	
	return 0;
	
}

