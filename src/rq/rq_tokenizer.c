/*
** rq_tokenizer.c
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
#include "rq_tokenizer.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT const char *
rq_tokenizer_get_token(
    const char **pptr, 
    char *tok, 
    int maxtoklen, 
    const char *delims, 
    const char *white, 
    const char *quotes
    )
{
    int in_quote = 0;

    while (strchr(white, **pptr))
        if (**pptr != '\0')
            (*pptr)++;
        else
            break;
    if (**pptr == '\0')
        return NULL;
	
    if (quotes)
        if (strchr(quotes, **pptr))
        {
            (*pptr)++;
            in_quote = 1;
        }
	
    if (in_quote)
    {
        int i;
        for (i = 0; i < maxtoklen - 1; i++, (*pptr)++)
            if (**pptr == '\0')
                break;
            else if (strchr(quotes, **pptr))
            {
                (*pptr)++;
                break;
            }
            else
                *(tok+i) = **pptr;
        *(tok+i) = '\0';
    }
    else
    {
        int i;
        for (i = 0; i < maxtoklen - 1; i++, (*pptr)++)
            if (**pptr == '\0')
                break;
            else if (strchr(delims, **pptr))
            {
                (*pptr)++;
                break;
            }
            else
                *(tok+i) = **pptr;
        *(tok+i) = '\0';
        for (i--; i > 0; i--)
            if (strchr(white, *(tok+i)))
                *(tok+i) = '\0';
            else
                break;
    }

    return tok;
}
