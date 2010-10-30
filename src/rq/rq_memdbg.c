/*
** rq_memdbg.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002-2008 Brett Hutley
**
** This file is part of the Risk Quantify Library
**
** Risk Quantify is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
**
** Risk Quantify is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public
** License along with Risk Quantify; if not, write to the Free
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include "rq_memdbg.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* -- defines ----------------------------------------------------- */

#define RQ_MEMDBG_RQ_MALLOC_ID		1
#define RQ_MEMDBG_RQ_CALLOC_ID		2
#define RQ_MEMDBG_RQ_REALLOC_ID	3
#define RQ_MEMDBG_RQ_STRDUP_ID		4

/* -- structs ----------------------------------------------------- */

struct memhdr {
    unsigned long alloc_id;
    short freed;
    short alloced_by;
};

/* -- statics ----------------------------------------------------- */

static unsigned long alloc_id = 0;

/* -- code -------------------------------------------------------- */

RQ_EXPORT void *
malloc_dbg(const char *file, unsigned int line, unsigned int size)
{
    struct memhdr *p = (struct memhdr *)malloc(size + sizeof(struct memhdr));
	if (p)
	{
		p->alloc_id = ++alloc_id;
		p->freed = 0;
		p->alloced_by = RQ_MEMDBG_RQ_MALLOC_ID;

		printf("malloc\t%s\t%d\t'%p'\n", file, line, p);
	}
	else
	{
		printf("memory error! Unable to malloc %d bytes\n", size);
		assert(0);
	}

    return ((char *)p) + sizeof(struct memhdr);
}

RQ_EXPORT void *
calloc_dbg(const char *file, unsigned int line, unsigned int n, unsigned int s)
{
    struct memhdr *p = (struct memhdr *)malloc((n * s) + sizeof(struct memhdr));
	if (p)
	{
		memset((void *)(((char *)p) + sizeof(struct memhdr)), '\0', n * s);

		p->alloc_id = ++alloc_id;
		p->freed = 0;
		p->alloced_by = RQ_MEMDBG_RQ_CALLOC_ID;

		printf("calloc\t%s\t%d\t'%p'\n", file, line, (void *)p);
	}
	else
	{
		printf("memory error! Unable to calloc %d x %d bytes\n", n, s);
		assert(0);
	}

    return (void *)(((char *)p) + sizeof(struct memhdr));
}

RQ_EXPORT void *
realloc_dbg(const char *file, unsigned int line, void *ptr, unsigned int s)
{
    struct memhdr *mh = (struct memhdr *)(((char *)ptr) - sizeof(struct memhdr));
    struct memhdr *p;

    /** \todo Print out original header here */

    p = (struct memhdr *)realloc(mh, s + sizeof(struct memhdr));
	if (p)
	{
		p->alloc_id = ++alloc_id;
		p->freed = 0;
		p->alloced_by = RQ_MEMDBG_RQ_REALLOC_ID;

		printf("realloc\t%s\t%d\t'%p'\n", file, line, p);
	}
	else
	{
		printf("memory error! Unable to realloc %d bytes\n", s);
		assert(0);
	}

    return ((char *)p) + sizeof(struct memhdr);

}

RQ_EXPORT char *
strdup_dbg(const char *file, unsigned int line, const char *s)
{
    struct memhdr *p = (struct memhdr *)malloc(strlen(s) + 1 + sizeof(struct memhdr));
	if (p)
	{
		p->alloc_id = ++alloc_id;
		p->freed = 0;
		p->alloced_by = RQ_MEMDBG_RQ_STRDUP_ID;

		strcpy((char *)(((char *)p) + sizeof(struct memhdr)), s);
    
        printf("strdup\t%s\t%d\t'%p'\t%s\n", file, line, p, s); 
	}
	else
	{
		printf("memory error! Unable to strdup %d bytes\n", strlen(s)+1+sizeof(struct memhdr));
		assert(0);
	}

    return (char *)(((char *)p) + sizeof(struct memhdr));
}

RQ_EXPORT void 
free_dbg(const char *file, unsigned int line, void *p)
{
    struct memhdr *mh = (struct memhdr *)(((char *)p) - sizeof(struct memhdr));
    const char *alloced_by = "UNKNOWN";
    const char *memdmp = "";

    if (mh->freed)
        printf("*Block %p alloced by %d has already been freed!\n", 
               mh,
               mh->alloced_by
               );

    switch (mh->alloced_by)
    {
        case RQ_MEMDBG_RQ_MALLOC_ID:
            alloced_by = "malloc";
            break;

        case RQ_MEMDBG_RQ_CALLOC_ID:
            alloced_by = "calloc";
            break;

        case RQ_MEMDBG_RQ_REALLOC_ID:
            alloced_by = "realloc";
            break;

        case RQ_MEMDBG_RQ_STRDUP_ID:
            alloced_by = "strdup";
            memdmp = p;
            break;
    }

    printf("free called by\t%s\t%d\t'%p'\t%s\t%s\n", file, line, mh, alloced_by, memdmp);

	mh->freed = 1;

    free(mh);
}
