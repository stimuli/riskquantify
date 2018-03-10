/*
** bh_strcpy.c
**
** safe strcpy(3)
** 
** Written by Brett Hutley - brett@hutley.net
** 
** Copyright (C) 2003 Brett Hutley
** 
** This file is part of the Risk Quantify Library
** 
** Risk Quantify is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public License
** as published by the Free Software Foundation; either version 2 of
** the License, or (at your option) any later version.
** 
** Risk Quantify is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
** 
** You should have received a copy of the GNU Library General Public
** License along with Risk Quantify; if not, write to the Free
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA
*/ 
/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bh_strcpy.h"

int
bh_memcpy(char *dest, const char *src, unsigned numtocopy, unsigned maxdestlen)
{
    int ret = 0;
	if (numtocopy > maxdestlen)
    {
        ret = numtocopy - maxdestlen;
		numtocopy = maxdestlen;
    }

	memcpy(dest, src, numtocopy);

    return ret;
}

int
bh_strcpy(char *dest, const char *src, unsigned maxdestlen)
{
    int ret = 0;
    unsigned numtocopy = strlen(src);
	if (numtocopy > maxdestlen-1)
    {
        ret = numtocopy - (maxdestlen - 1);
		numtocopy = maxdestlen - 1;
    }

	memcpy(dest, src, numtocopy);
	dest[numtocopy] = '\0';

    return ret;
}


int
bh_memzcpy(char *dest, const char *src, unsigned numtocopy, unsigned maxdestlen)
{
    int ret = 0;
	if (numtocopy > maxdestlen-1)
    {
        ret = numtocopy - (maxdestlen - 1);
		numtocopy = maxdestlen - 1;
    }

	memcpy(dest, src, numtocopy);
	dest[numtocopy] = '\0';

    return ret;
}

#ifdef TEST_BH_STRCPY

int
main(int argc, char **argv)
{
    char dest[10];
    const char *src = "123456789012";
	int ret = bh_strcpy(dest, src, 10);

    printf("dest = '%s'\n", dest);
    printf("strlen(dest) = %d (should be 9)\n", strlen(dest));
    printf("ret = %d (should be 3)\n", ret);

    memset(dest, '\0', 10);
    ret = bh_memcpy(dest, src, 8, 8);
    printf("dest = '%s' (should go to 8)\n", dest);
    printf("ret = %d (should be 0)\n", ret);

    memset(dest, '\0', 10);
    ret = bh_memzcpy(dest, src, 12, 10);
    printf("dest = '%s' (should go to 9)\n", dest);
    printf("ret = %d (should be 3)\n", ret);

    ret = bh_memzcpy(dest, src, 4, 10);
    printf("dest = '%s' (should go to 4)\n", dest);
    printf("ret = %d (should be 0)\n", ret);

	return 0;
}

#endif

