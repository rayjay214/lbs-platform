#ifndef GREY_SCALE_H
#define GREY_SCALE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define GS_SHM_KEY 0x1234

#define GS_SHM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#ifndef GS_RULE_SIZE
#define GS_RULE_SIZE 50000
#endif

#ifndef GS_LIST_SIZE
#define GS_LIST_SIZE 500
#endif

typedef struct _GS_RULE
{
	unsigned int busid, method, factor, version, lst_cnt;
	unsigned long long up, low, list[GS_LIST_SIZE];
} GS_RULE;

typedef struct _GS_SHMSEG
{
    int idx, cnt[2];
    GS_RULE rule[2][GS_RULE_SIZE];
} GS_SHMSEG;

enum GS_STRATEGY
{
	GS_RANGE = 1,
	GS_WHITE = 2,
	GS_BLACK = 3,
	GS_MODULE = 4,
};

#define STRTOULL(PTR, END_PTR, BASE) ((PTR == NULL)? -1: strtoull(PTR, END_PTR, BASE))

extern "C"
{
int greyscale(unsigned long long uin, unsigned int busid);
}

#endif
