#ifndef __MYTPF__H

#define __MYTPF__H

#define MYTPF_MAX	1024

typedef void mytpf_t;

mytpf_t *mytpf_init(int cps,int burst);

int mytpf_fetchtoken(mytpf_t *,int );
int mytpf_returntoken(mytpf_t *,int);

int mytpf_destroy(mytpf_t *);

#endif
