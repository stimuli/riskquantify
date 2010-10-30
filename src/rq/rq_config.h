/**
 * @file
 *
 * rq_config.h contains defines for platform-specific stuff.
 */
/*
** rq_config.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2001-2008 Brett Hutley
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
#ifndef rq_config_h
#define rq_config_h

/* -- general stuff ----------------------------------------------- */
/** Make sure VERSION is defined.  

    'configure' will define this for most platforms. It's not
    necessarily defined for MSVC builds.
*/
#ifndef VERSION
# define VERSION "0.8.1"
#endif

/** The number of objects in each object pool */
#define RQ_OBJECT_MGR_OBJECT_POOL_SIZE 1024


/** The maximum number of messages we can store */
#define RQ_PRICING_RESULTS_MAX_MESSAGES 50


/* -- platform defines -------------------------------------------- */

/* borland likes to use different pre-defined macros */
#ifdef __WIN32__
    # ifndef WIN32
    #  define WIN32
    # endif
#endif

#ifdef WIN32
# define snprintf _snprintf
# define isnan _isnan

# ifdef RQSO_EXPORTS
#  define RQ_EXPORT __declspec(dllexport)
# else
#  define RQ_EXPORT
# endif

# define NO_EXPM1

#else
/* Non-Win32 stuff */
# define RQ_EXPORT
#endif

#ifdef HAVE_STRCASECMP
# define stricmp strcasecmp
#endif

/* -- memory allocation ------------------------------------------- */
/* Define this in order to try and find memory leaks. */
#undef DEBUG_MEMORY

#ifdef DEBUG_MEMORY
#  define RQ_MALLOC(x) malloc_dbg(__FILE__, __LINE__, x)
#  define RQ_CALLOC(n, s) calloc_dbg(__FILE__, __LINE__, n, s)
#  define RQ_REALLOC(p, s) realloc_dbg(__FILE__, __LINE__, p, s)
#  define RQ_STRDUP(s) strdup_dbg(__FILE__, __LINE__, s)
#  define RQ_FREE(p) free_dbg(__FILE__, __LINE__, p)

#  include "rq_memdbg.h"

#else

# define RQ_MALLOC malloc
# define RQ_CALLOC calloc
# define RQ_REALLOC realloc

#ifdef MSVC
# define RQ_STRDUP _strdup
#else
# define RQ_STRDUP strdup
#endif

# define RQ_FREE free

# endif

#ifdef MSVC
	#pragma warning (disable: 4996)
#endif

#endif

