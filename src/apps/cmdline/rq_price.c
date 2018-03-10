/*
** rq_price.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003 Brett Hutley
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
/*
  Workflow for adding a new pricing model
  =======================================

  1: Add the prototype to the prototypes list
  2: Add the adapter function
  3: Add an entry to the model_defs list
  4: Add any new parameters

  Workflow for adding a new model parameter
  =========================================

  The model parameter MUST be assigned it's own command line option
  letter.

  1: add an option to the optdefs array.
  2: add a static variable to the statics section.
  3: check for the option and assign to the static variable.  
  4: add a check in the validate_model_params() function.
*/
/* -- includes -- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rq.h>
#include "bh_getopt.h"
#include "bh_strcpy.h"

/* defines */
#define DEFAULT_PRECISION 6

/* Pricing Adapter Prototypes */
static short black_scholes(double *result);
static short digital(double *result);
static short barrier_single(double *result);
static short barrier_double(double *result);
static short average_rate_geometric(double *result);
static short average_rate_turnbullwakeman(double *result);
static short average_rate_levy(double *result);
static short barone_adesi_whaley(double *result);
static short bjerksund_stensland(double *result);

/*
  Add new options to here to avoid dups:
  ABDhHIJKLmMNOpSUvVXYZ
*/

/* Command line options */
struct bh_optdef optdefs[] = {
	{ "help", 'h', BH_GETOPT_ARGTYPE_NONE, NULL, "Display help" },
	{ "help-model", 'H', BH_GETOPT_ARGTYPE_OPTIONAL, "model", "Display help on a particular pricing model (with no arg list all models" },
	{ "model", 'm', BH_GETOPT_ARGTYPE_REQUIRED, "modelid", "Price using a particular pricing model" },
    { "option-type", 'O', BH_GETOPT_ARGTYPE_REQUIRED, "call|put|...", "The type of option (model dependant)" },
    { "underlying-price", 'S', BH_GETOPT_ARGTYPE_REQUIRED, "price", "The price of the underlying asset" },
    { "strike-price", 'K', BH_GETOPT_ARGTYPE_REQUIRED, "strike", "The strike price of the option" },
    { "barrier-price", 'B', BH_GETOPT_ARGTYPE_REQUIRED, "barrier", "The barrier price of the option" },
    { "upper-barrier", 'U', BH_GETOPT_ARGTYPE_REQUIRED, "barrier", "The upper barrier level of the option" },
    { "lower-barrier", 'L', BH_GETOPT_ARGTYPE_REQUIRED, "barrier", "The lower barrier level of the option" },
    { "average-price", 'A', BH_GETOPT_ARGTYPE_REQUIRED, "avg", "The average price of the underlying to date" },
    { "int-rate-dom", 'I', BH_GETOPT_ARGTYPE_REQUIRED, "intrate", "The domestic interest rate" },
    { "int-rate-for", 'J', BH_GETOPT_ARGTYPE_REQUIRED, "intrate", "The foreign interest rate" },
    { "volatility", 'V', BH_GETOPT_ARGTYPE_REQUIRED, "vol", "The volatility of the underlying (annualized)" },
    { "tau-opt-expiry", 'X', BH_GETOPT_ARGTYPE_REQUIRED, "tau", "The time remaining to expiry of the option (annualized)" },
    { "tau-opt-delivery", 'D', BH_GETOPT_ARGTYPE_REQUIRED, "tau", "The time remaining to delivery of the underlying (annualized)" },
    { "tau-opt-lifespan", 'Z', BH_GETOPT_ARGTYPE_REQUIRED, "tau", "The time from the start of observations to expiry (annualized)" },
    { "tau-period", 'Y', BH_GETOPT_ARGTYPE_REQUIRED, "tau", "The time of the period (annualized)" },
    { "num-timesteps", 'M', BH_GETOPT_ARGTYPE_REQUIRED, "steps", "The number of timesteps in the simulation" },
    { "num-iterations", 'N', BH_GETOPT_ARGTYPE_REQUIRED, "iters", "The number of iterations in the simulation" },
	{ "version", 'v', BH_GETOPT_ARGTYPE_NONE, NULL, "Display the version" },
	{ "precision", 'p', BH_GETOPT_ARGTYPE_REQUIRED, "number", "Display the result to this precision" }
};

struct model_def {
	const char *name;
	short (*fn)(double *);
	const char *desc_short;
	const char *desc_long;
    const char *params_required;
    const char *params_optional;
} model_defs [] = {
	{ "black_scholes", black_scholes, "the Black-Scholes model", 
      "The Black-Scholes model is a closed-form solution for valuing european options.\nThe option type parameter is either 'C' or 'P' (Call/Put).",
      "OSKIJVXD", ""
    },
	{ "digital", digital, "the digital model", 
      "The digital model is a closed-form solution for valuing european digital options.\nThe option type parameter is a two character string.\nThe first character is either 'C' or 'P' (Call/Put).\nThe second character is either 'C' or 'A' (for Cash/Asset).",
      "OSKIJVXD", ""
    },
	{ "barrier_single", barrier_single, "the single barrier pricing model", 
      "The single barrier model is a closed-form solution for valuing european single barrier options.",
      "OSKBIJVXD", ""
    },
	{ "barrier_double", barrier_double, "the double barrier pricing model", 
      "The double barrier model is a closed-form solution for valuing european double barrier options.",
      "OSKULIJVXD", ""
    },
	{ "average_rate_geometric", average_rate_geometric, "the geometric average rate model", 
      "A geometric average rate option pricing model.",
      "OSAXIJVXZ", ""
    },
	{ "average_rate_turnbullwakeman", average_rate_turnbullwakeman, "the turnbull-wakeman average rate model", 
      "A turnbull-wakeman average rate option pricing model.",
      "OSAXIJVXZY", ""
    },
	{ "average_rate_levy", average_rate_levy, "the levy average rate model", 
      "A levy average rate option pricing model.",
      "OSAXIJVXZ", ""
    },
	{ "barone_adesi_whaley", barone_adesi_whaley, "the Barone-Adesi and Whaley model", 
      "The Barone-Adesi and Whaley model is an approximation for valuing american options.",
      "OSKIJVX", ""
    },
	{ "bjerksund_stensland", bjerksund_stensland, "the Bjerksund-Stensland model", 
      "The Bjerksund-Stensland model is an approximation for valuing american options.",
      "OSKIJVX", ""
    }
};

/* -- statics -- */
static char model[256];
static char option_type[256];
static double underlying_price = 0.0;
static double strike_price = 0.0;
static double barrier_price = 0.0;
static double upper_barrier = 0.0;
static double lower_barrier = 0.0;
static double underlying_avg = 0.0;
static double int_rate_dom = 0.0;
static double int_rate_for = 0.0;
static double volatility = 0.0;
static double tau_opt_expiry = 0.0;
static double tau_opt_delivery = 0.0;
static double tau_opt_lifespan = 0.0;
static double tau_period = 0.0;
static long num_timesteps = 0;
static long num_iterations = 0;

static double int_rate_dom_df = 0.0;
static short do_int_rate_dom_df_conversion = 0;
static double int_rate_for_df = 0.0;
static short do_int_rate_for_df_conversion = 0;

/* -- code ---- */

/*
	The pricing adapter callbacks
*/
static short 
black_scholes(double *result)
{
    short is_call;
    if (*option_type == 'C' || *option_type == 'c')
        is_call = 1;
    else if (*option_type == 'P' || *option_type == 'p')
        is_call = 0;
    else
    {
        printf("Error: Unknown option-type parameter - must be call or put\n");
        return 0;
    }

	*result = rq_pricing_blackscholes(
        is_call, 
        underlying_price,
        strike_price,
        int_rate_dom,
        int_rate_for,
        volatility,
        tau_opt_expiry,
        tau_opt_delivery
        );

	return 1;
}

static short 
digital(double *result)
{
    short is_call;
    short is_cash;
    if (option_type[0] == 'C' || option_type[0] == 'c')
        is_call = 1;
    else if (option_type[0] == 'P' || option_type[0] == 'p')
        is_call = 0;
    else
    {
        printf("Error: Unknown option-type parameter - first character must be C or P for (call or put)\n");
        return 0;
    }
    if (option_type[1] == 'C' || option_type[1] == 'c')
        is_cash = 1;
    else if (option_type[1] == 'A' || option_type[1] == 'a')
        is_cash = 0;
    else
    {
        printf("Error: Unknown option-type parameter - second character must be C or A for cash or asset\n");
        return 0;
    }

	*result = rq_pricing_digital(
        is_cash,
        is_call, 
        underlying_price,
        strike_price,
        int_rate_dom,
        int_rate_for,
        volatility,
        tau_opt_expiry,
        tau_opt_delivery
        );

	return 1;
}

static short 
barrier_single(double *result)
{
    short in;
    short is_call;
    if (option_type[0] == 'I' || option_type[0] == 'i')
        in = 1;
    else if (option_type[0] == 'O' || option_type[1] == 'o')
        in = 0;
    else
    {
        printf("Error: Unknown option-type parameter - must be one of:\n");
        printf("       'IC' - In Call\n");
        printf("       'IP' - In Put\n");
        printf("       'OC' - Out Call\n");
        printf("       'OP' - Out Put\n");
        return 0;
    }

    if (option_type[1] == 'C' || option_type[1] == 'c')
        is_call = 1;
    else if (option_type[1] == 'P' || option_type[1] == 'p')
        is_call = 0;
    else
    {
        printf("Error: Unknown option-type parameter - must be one of:\n");
        printf("       'IC' - In Call\n");
        printf("       'IP' - In Put\n");
        printf("       'OC' - Out Call\n");
        printf("       'OP' - Out Put\n");
        return 0;
    }

	*result = rq_pricing_single_barrier(
        in,
        is_call, 
        underlying_price,
        strike_price,
        barrier_price,
        int_rate_dom,
        int_rate_for,
        volatility,
        tau_opt_expiry,
        tau_opt_delivery
        );

	return 1;
}

static short 
barrier_double(double *result)
{
    short in;
    short is_call;
    if (option_type[0] == 'I' || option_type[0] == 'i')
        in = 1;
    else if (option_type[0] == 'O' || option_type[1] == 'o')
        in = 0;
    else
    {
        printf("Error: Unknown option-type parameter - must be one of:\n");
        printf("       'IC' - In Call\n");
        printf("       'IP' - In Put\n");
        printf("       'OC' - Out Call\n");
        printf("       'OP' - Out Put\n");
        return 0;
    }

    if (option_type[1] == 'C' || option_type[1] == 'c')
        is_call = 1;
    else if (option_type[1] == 'P' || option_type[1] == 'p')
        is_call = 0;
    else
    {
        printf("Error: Unknown option-type parameter - must be one of:\n");
        printf("       'IC' - In Call\n");
        printf("       'IP' - In Put\n");
        printf("       'OC' - Out Call\n");
        printf("       'OP' - Out Put\n");
        return 0;
    }

    if (in)
        *result = rq_pricing_double_barrier_knockin(
            is_call, 
            underlying_price,
            strike_price,
            lower_barrier,
            upper_barrier,
            int_rate_dom,
            int_rate_for,
            volatility,
            tau_opt_expiry,
            tau_opt_delivery
            );
    else
        *result = rq_pricing_double_barrier_knockout(
            is_call, 
            underlying_price,
            strike_price,
            lower_barrier,
            upper_barrier,
            int_rate_dom,
            int_rate_for,
            volatility,
            tau_opt_expiry,
            tau_opt_delivery
            );

	return 1;
}

short 
average_rate_geometric(double *result)
{
    short is_call;
    if (*option_type == 'C' || *option_type == 'c')
        is_call = 1;
    else if (*option_type == 'P' || *option_type == 'p')
        is_call = 0;
    else
    {
        printf("Error: Unknown option-type parameter - must be call or put\n");
        return 0;
    }

	*result = rq_pricing_average_rate_geometric(
        is_call, 
        underlying_price,
        underlying_avg,
        strike_price,
        int_rate_dom,
        int_rate_for,
        volatility,
        tau_opt_lifespan,
        tau_opt_expiry
        );

	return 1;
}

short 
average_rate_turnbullwakeman(double *result)
{
    short is_call;
    if (*option_type == 'C' || *option_type == 'c')
        is_call = 1;
    else if (*option_type == 'P' || *option_type == 'p')
        is_call = 0;
    else
    {
        printf("Error: Unknown option-type parameter - must be call or put\n");
        return 0;
    }

	*result = rq_pricing_average_rate_turnbullwakeman(
        is_call, 
        underlying_price,
        underlying_avg,
        strike_price,
        int_rate_dom,
        int_rate_for,
        volatility,
        tau_opt_lifespan,
        tau_opt_expiry,
        tau_period
        );

	return 1;
}

short 
average_rate_levy(double *result)
{
    short is_call;
    if (*option_type == 'C' || *option_type == 'c')
        is_call = 1;
    else if (*option_type == 'P' || *option_type == 'p')
        is_call = 0;
    else
    {
        printf("Error: Unknown option-type parameter - must be call or put\n");
        return 0;
    }

	*result = rq_pricing_average_rate_levy(
        is_call, 
        underlying_price,
        underlying_avg,
        strike_price,
        int_rate_dom,
        int_rate_for,
        volatility,
        tau_opt_lifespan,
        tau_opt_expiry
        );

	return 1;
}

static short 
barone_adesi_whaley(double *result)
{
    short is_call;
    if (*option_type == 'C' || *option_type == 'c')
        is_call = 1;
    else if (*option_type == 'P' || *option_type == 'p')
        is_call = 0;
    else
    {
        printf("Error: Unknown option-type parameter - must be call or put\n");
        return 0;
    }

	*result = rq_pricing_barone_adesi_whaley(
        is_call, 
        underlying_price,
        strike_price,
        int_rate_dom,
        int_rate_for,
        volatility,
        tau_opt_expiry
        );

	return 1;
}

static short 
bjerksund_stensland(double *result)
{
    short is_call;
    if (*option_type == 'C' || *option_type == 'c')
        is_call = 1;
    else if (*option_type == 'P' || *option_type == 'p')
        is_call = 0;
    else
    {
        printf("Error: Unknown option-type parameter - must be call or put\n");
        return 0;
    }

	*result = rq_pricing_bjerksund_stensland(
        is_call, 
        underlying_price,
        strike_price,
        int_rate_dom,
        int_rate_for,
        volatility,
        tau_opt_expiry
        );

	return 1;
}

/*
	General code
*/
static short
validate_model(const char *modelid)
{
	int i;

	for (i = 0; i < sizeof(model_defs) / sizeof(struct model_def); i++)
		if (!strcmp(model_defs[i].name, modelid))
			return i;
	return -1;
}

static short
validate_model_params(int model_offset)
{
    short err = 0;
    int i;
    for (i = 0; model_defs[model_offset].params_required[i] != '\0'; i++)
    {
        switch (model_defs[model_offset].params_required[i])
        {
            case 'O':
                if (strlen(option_type) == 0)
                {
                    printf("The option-type must be specified\n");
                    err++;
                }
                break;
            case 'S':
                if (underlying_price == 0.0)
                {
                    printf("The underlying-price option must be specified\n");
                    err++;
                }
                break;
            case 'K':
                if (strike_price == 0.0)
                {
                    printf("The strike-price option must be specified\n");
                    err++;
                }
                break;
            case 'B':
                if (barrier_price == 0.0)
                {
                    printf("The barrier-price option must be specified\n");
                    err++;
                }
                break;
            case 'U':
                if (upper_barrier == 0.0)
                {
                    printf("The upper-barrier option must be specified\n");
                    err++;
                }
                break;
            case 'L':
                if (lower_barrier == 0.0)
                {
                    printf("The lower-barrier option must be specified\n");
                    err++;
                }
                break;
            case 'A':
                if (underlying_avg == 0.0)
                {
                    printf("The average-price option must be specified\n");
                    err++;
                }
                break;
            case 'I':
                if (int_rate_dom == 0.0)
                {
                    printf("The int-rate-dom must be specified\n");
                    err++;
                }
                break;
            case 'J':
                if (int_rate_for == 0.0)
                {
                    printf("The int-rate-for must be specified\n");
                    err++;
                }
                break;
            case 'V':
                if (volatility == 0.0)
                {
                    printf("The volatility must be specified\n");
                    err++;
                }
                break;
            case 'X':
                if (tau_opt_expiry == 0.0)
                {
                    printf("The tau-opt-expiry option must be specified\n");
                    err++;
                }
                break;
            case 'D':
                if (tau_opt_delivery == 0.0)
                {
                    printf("The tau-opt-delivery option must be specified\n");
                    err++;
                }
                break;
            case 'Z':
                if (tau_opt_lifespan == 0.0)
                {
                    printf("The tau-opt-lifespan option must be specified\n");
                    err++;
                }
                break;
            case 'Y':
                if (tau_period == 0.0)
                {
                    printf("The tau-period option must be specified\n");
                    err++;
                }
                break;
            case 'M':
                if (num_timesteps == 0)
                {
                    printf("The num-timesteps option must be specified\n");
                    err++;
                }
                break;
            case 'N':
                if (num_iterations == 0)
                {
                    printf("The num-iterations option must be specified\n");
                    err++;
                }
                break;
        }
    }

    return err;
}

int
main(int argc, char **argv)
{
	unsigned curpos = 1; /* start off from the option AFTER the program name */
	char name[256];
	char value[256];
	int ret;
    int model_offset;
    double optval = 0;
    int precision = DEFAULT_PRECISION;

    model[0] = option_type[0] = '\0';

	while ((ret = bh_getopt(argc, argv, &curpos, optdefs, sizeof(optdefs)/sizeof(struct bh_optdef), name, 256, value, 256)) != BH_GETOPT_RESULT_DONE)
	{
		if (ret < 0 || !strcmp(name, "help"))
		{
			bh_getopt_help(optdefs, sizeof(optdefs)/sizeof(struct bh_optdef));
			exit(0);
		}
		else if (!strcmp(name, "version"))
		{
			printf("This is rq_price based on Risk Quantify version %s\n", VERSION);
			exit(0);
		}
		else if (!strcmp(name, "help-model"))
		{
			int model_offset = -1;
			if (value[0] == '\0' || 
				(model_offset = validate_model(value)) < 0)
			{
				int i;
				printf("You can pass the following options to the model parameter\n");
				for (i = 0; i < sizeof(model_defs) / sizeof(struct model_def); i++)
				{
					printf(" %s - %s\n", 
						model_defs[i].name,
						model_defs[i].desc_short
						);
				}

				exit(0);
			}
			else
			{
                if (model_defs[model_offset].desc_long)
                {
                    int i;
                    printf("%s\n", model_defs[model_offset].desc_long);
                    printf("\nThis model requires the following parameters:\n");

                    for (i = 0; model_defs[model_offset].params_required[i] != '\0'; i++)
                    {
                        char optshort = model_defs[model_offset].params_required[i];
                        int j;
                        int max_j = sizeof(optdefs) / sizeof(struct bh_optdef);

                        for (j = 0; j < max_j; j++)
                        {
                            if (optdefs[j].optshort == optshort)
                            {
                                bh_getopt_help_option(&optdefs[j]);
                                break;
                            }
                        }

                        if (j == max_j)
                        {
                            printf("ERROR: option %c not found\n", optshort);
                        }
                    }
                }

                exit(0);
			}
		}
        else if (!strcmp(name, "model"))
            bh_strcpy(model, value, sizeof(model));
        else if (!strcmp(name, "option-type"))
            bh_strcpy(option_type, value, sizeof(option_type));
        else if (!strcmp(name, "underlying-price"))
            underlying_price = atof(value);
        else if (!strcmp(name, "strike-price"))
            strike_price = atof(value);
        else if (!strcmp(name, "barrier-price"))
            barrier_price = atof(value);
        else if (!strcmp(name, "upper-barrier"))
            upper_barrier = atof(value);
        else if (!strcmp(name, "lower-barrier"))
            lower_barrier = atof(value);
        else if (!strcmp(name, "average-price"))
            underlying_avg = atof(value);
        else if (!strcmp(name, "int-rate-dom"))
        {
            int_rate_dom = 0.0;
            if (tolower(*value) == 'd')
            {
                char *p = strchr(value, ':');
                if (p)
                {
                    int_rate_dom_df = atof(p+1);
                    if (int_rate_dom_df > 0.0)
                        do_int_rate_dom_df_conversion = 1;
                }
            }

            if (int_rate_dom == 0.0 && !do_int_rate_dom_df_conversion)
                int_rate_dom = atof(value);
            
        }
        else if (!strcmp(name, "int-rate-for"))
        {
            int_rate_for = 0.0;
            if (tolower(*value) == 'd')
            {
                char *p = strchr(value, ':');
                if (p)
                {
                    int_rate_for_df = atof(p+1);
                    if (int_rate_for_df > 0.0)
                        do_int_rate_for_df_conversion = 1;
                }
            }

            if (int_rate_for == 0.0 && !do_int_rate_for_df_conversion)
                int_rate_for = atof(value);
        }
        else if (!strcmp(name, "volatility"))
            volatility = atof(value);
        else if (!strcmp(name, "tau-opt-expiry"))
            tau_opt_expiry = atof(value);
        else if (!strcmp(name, "tau-opt-delivery"))
            tau_opt_delivery = atof(value);
        else if (!strcmp(name, "tau-opt-lifespan"))
            tau_opt_lifespan = atof(value);
        else if (!strcmp(name, "tau-period"))
            tau_period = atof(value);
        else if (!strcmp(name, "num-timesteps"))
            num_timesteps = atol(value);
        else if (!strcmp(name, "num-iterations"))
            num_iterations = atol(value);
        else if (!strcmp(name, "precision"))
            precision = atoi(value);
	}

    if (strlen(model) == 0)
    {
        printf("ERROR: A model identifier must be specified!\n");
        exit(-1);
    }

    model_offset = validate_model(model);
    if (model_offset < 0)
    {
        printf("ERROR: An invalid model was specified!\n");
        exit(-2);
    }

    if (do_int_rate_dom_df_conversion && tau_opt_delivery > 0.0)
        int_rate_dom = -log(int_rate_dom_df) / tau_opt_delivery;

    if (do_int_rate_for_df_conversion && tau_opt_delivery > 0.0)
        int_rate_for = -log(int_rate_for_df) / tau_opt_delivery;

    if (validate_model_params(model_offset) > 0)
    {
        printf("FATAL: Required parameters not specified\n");
        exit(-3);
    }

    if (!(*model_defs[model_offset].fn)(&optval))
    {
        printf("Error in valuation\n");
        exit(-4);
    }

    printf("%.*f\n", precision, optval);

	return 0;
}

