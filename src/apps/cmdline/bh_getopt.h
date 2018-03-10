/*
** bh_getopt.h
**
** A portable getopt() package. Support gnu-style long options
** like "--option=test", as well as short options of the "-o"
** variety. Unlike getopt(3) - no global variables are used.
** 
** Written by Brett Hutley - brett@hutley.net
** 
** Copyright (C) 2003 Brett Hutley
** 
** This file is part of the bh_getopt package
** 
** bh_getopt is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public License
** as published by the Free Software Foundation; either version 2 of
** the License, or (at your option) any later version.
** 
** bh_getopt is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
** 
** You should have received a copy of the GNU Library General Public
** License along with bh_getopt; if not, write to the Free
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA
*/ 
#ifndef bh_getopt_h
#define bh_getopt_h

enum bh_getopt_result {
	BH_GETOPT_RESULT_DONE = -1,
	BH_GETOPT_RESULT_UNKNOWN = -2
};

enum bh_getopt_argtype {
	BH_GETOPT_ARGTYPE_NONE,
	BH_GETOPT_ARGTYPE_REQUIRED,
	BH_GETOPT_ARGTYPE_OPTIONAL
};

/* -- structures -- */
struct bh_optdef {
	const char *optname; /**< The name of the option - will be used in the long-name form of the option processing - REQUIRED */
	char optshort; /**< The one character option - optional */
	enum bh_getopt_argtype argtype; /**< If this option requires an argument, what argument does it require? */
	const char *argdesc; /**< A short description of any command line option arguments. Will be used in help */
	const char *desc; /**< A description of the command line option itself */
};

/* -- prototypes -- */

int bh_getopt(int argc, char **argv, unsigned int *curpos, struct bh_optdef *optdefs, unsigned int num_optdefs, char *name, unsigned int maxnamelen, char *value, unsigned int maxvaluelen);

void bh_getopt_help_option(struct bh_optdef *optdef);

void bh_getopt_help(struct bh_optdef *optdefs, unsigned int num_optdefs);

#endif

