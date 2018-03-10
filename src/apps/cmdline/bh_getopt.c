/*
** bh_getopt.c
**
** A portable getopt() package. Support gnu-style long options
** like "--option=test", as well as short options of the "-o"
** variety. Unlike getopt(3) - no global variables are used.
** 
** Written by Brett Hutley - brett@hutley.net
** 
** Copyright (C) 2001 Brett Hutley
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
#include "bh_getopt.h"
#include "bh_strcpy.h"

int
bh_getopt(int argc, char **argv, unsigned int *curpos, struct bh_optdef *optdefs, unsigned int num_optdefs, char *name, unsigned int maxnamelen, char *value, unsigned int maxvaluelen)
{
	int optdefs_offset = BH_GETOPT_RESULT_UNKNOWN;

	if (*curpos >= argc)
		return BH_GETOPT_RESULT_DONE;

	*name = *value = '\0';

	if (!memcmp(argv[*curpos], "--", 2)) 
	{
		/* found a possible option */
		short gotopt = 0;

		/* check to see if it has a parameter (ie an "equals") */
		char *equals = strchr(argv[*curpos], '=');
		if (equals)
		{
			const char *name_start = argv[*curpos] + 2; /* point to start of name */
			unsigned name_length = equals - name_start;

			if (name_length > 0)
			{
				unsigned value_length = strlen(equals+1);

				bh_memzcpy(name, name_start, name_length, maxnamelen);
				bh_memzcpy(value, equals+1, value_length, maxvaluelen);

				gotopt = 1;
			}
			else
			{
				/* Option argument "--" */
				bh_strcpy(name, "--", maxnamelen);
			}

		}
		else
		{
			bh_strcpy(name, argv[*curpos]+2, maxnamelen);
			gotopt = 1;
		}

		if (gotopt)
		{
			unsigned i;

			for (i = 0; i < num_optdefs && optdefs[i].optname; i++)
			{
				if (!strcmp(name, optdefs[i].optname))
				{
					optdefs_offset = i;
					break;
				}
			}
		}

		(*curpos)++;
	}
	else if (*argv[*curpos] == '-')
	{
		unsigned i;
		enum bh_getopt_argtype argtype = BH_GETOPT_ARGTYPE_NONE;

		if (strlen(argv[*curpos]) == 2)
		{
			for (i = 0; i < num_optdefs && optdefs[i].optname; i++)
			{
				if (optdefs[i].optshort != '\0' && 
					*(argv[*curpos]+1) == optdefs[i].optshort)
				{
					optdefs_offset = i;
					bh_strcpy(name, optdefs[i].optname, maxnamelen);
					argtype = optdefs[i].argtype;
					break;
				}
			}
		}

		if (optdefs_offset < 0)
		{
			/* we didn't recognise the option - copy it into the
			   name buffer to return to the caller
			*/
			bh_strcpy(name, argv[*curpos], maxnamelen);
		}

		(*curpos)++;

		if (argtype != BH_GETOPT_ARGTYPE_NONE)
		{
			/* is there another parameter? */
			if ((*curpos) < argc)
			{
				if (argtype != BH_GETOPT_ARGTYPE_OPTIONAL ||
					*argv[*curpos] != '-')
				{
					bh_strcpy(value, argv[*curpos], maxvaluelen);
					(*curpos)++;
				}
			}
		}
	}
	else
	{
		bh_strcpy(name, argv[*curpos], maxnamelen);
		(*curpos)++;
	}

	return optdefs_offset;
}

void
bh_getopt_help_option(struct bh_optdef *optdef)
{
    if (optdef->optshort != 0)
        printf("-%c ", optdef->optshort);
    else
        printf("   ");
    if (optdef->argtype != BH_GETOPT_ARGTYPE_NONE)
    {
        char optopen = '<';
        char optclose = '>';
        const char *argdesc = "option";

        if (optdef->argtype == BH_GETOPT_ARGTYPE_OPTIONAL)
        {
            optopen = '[';
            optclose = ']';
        }

        if (optdef->argdesc)
            argdesc = optdef->argdesc;

        printf("%c%s%c", optopen, argdesc, optclose);
    }

    if (optdef->optshort != 0)
        printf(" | ");
    printf("--%s", optdef->optname);

    if (optdef->argtype != BH_GETOPT_ARGTYPE_NONE)
    {
        const char *argdesc = "option";
        const char *argstart = "";
        const char *argend = argstart;

        if (optdef->argdesc)
            argdesc = optdef->argdesc;
        if (optdef->argtype == BH_GETOPT_ARGTYPE_OPTIONAL)
        {
            argstart = "[";
            argend = "]";
        }

        printf("%s=%s%s", argstart, argdesc, argend);
    }

    if (optdef->desc)
        printf(" : %s", optdef->desc);
    printf("\n");
}

void
bh_getopt_help(struct bh_optdef *optdefs, unsigned int num_optdefs)
{
	unsigned int i;
	for (i = 0; i < num_optdefs; i++, ++optdefs)
        bh_getopt_help_option(optdefs);
}

#ifdef TEST_BH_GETOPT

struct optdef optdefs[] = {
	{ "help", 'h', BH_GETOPT_ARGTYPE_NONE, NULL, "Display help" },
	{ "help-topic", 't', BH_GETOPT_ARGTYPE_OPTIONAL, "topic", "Display help on a particular topic" },
	{ "version", 'v', BH_GETOPT_ARGTYPE_NONE, NULL, "Display the version" }
};

int
main(int argc, char **argv)
{
	unsigned curpos = 0;
	char name[256];
	char value[256];
	int ret;

	while ((ret = bh_getopt(argc, argv, &curpos, optdefs, sizeof(optdefs)/sizeof(struct optdef), name, 256, value, 256)) != BH_GETOPT_RESULT_DONE)
	{
		printf("%d\t%s", ret, name);
		if (strlen(value))
			printf("\t%s", value);
		printf("\n");

		if (ret >= 0)
		{
			if (!strcmp(name, "help"))
				bh_getopt_help(optdefs, sizeof(optdefs)/sizeof(struct optdef));
		}
	}

	return 0;
}

#endif

