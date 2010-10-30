/**
 * \file rq_tokenizer.h
 * \author Brett Hutley
 *
 * \brief The rq_tokenizer files provide tokenizing helper functions.
 */
/*
** rq_tokenizer.h
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
#ifndef rq_tokenizer_h
#define rq_tokenizer_h

#include "rq_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** Parse a token from a string.
 */
RQ_EXPORT const char *
rq_tokenizer_get_token(
    const char **pptr, /**< A pointer to a pointer into the string */
    char *tok, /**< The buffer to store the parsed token */
    int maxtoklen, /**< The maximum token length of the token */
    const char *delims, /**< A string that is the list of token delimiters */
    const char *white, /**< A string that contains the list of white space chars */
    const char *quotes /**< A string that contains the list of quote chars */
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
