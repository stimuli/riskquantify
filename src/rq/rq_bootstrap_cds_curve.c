/*
** rq_bootstrap_cds_curve.c
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
#include "rq_yield_curve.h"
#include "rq_bootstrap_cds_curve.h"
#include "rq_bootstrap_config.h"
#include "rq_asset_mgr.h"
#include "rq_asset_recovery_rate.h"
#include "rq_asset_cds_spread.h"
#include "rq_interpolate.h"

#include "rq_defs.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef TINY
#define TINY 1.0e-20;
#endif

/* Terms for which to pre-cacluate survival probabilities
 * Array is zero (0.0) terminated to indicate array end.
 */
static const double s_curveTerms[] =
{
	0.0136986301369863,
	0.0273972602739726,
	0.0821917808219178,
	0.1643835616438360,
	0.25,
	0.5,
	0.75,
	1.0,
	1.25,
	1.5,
	1.75,
	2.0,
	2.5,
	3.0,
	3.5,
	4.0,
	4.5,
	5.0,
	7.0,
	10.0,
	15.0,
	20.0,
	0.0
};

rq_cds_curve_t
load_cds_spread_rates(
    rq_date mktDate, 
    const rq_rate_mgr_t rate_mgr, 
    const rq_bootstrap_config_t config, 
    const rq_asset_mgr_t asset_mgr,
    double *rate_adjust_factors,
    rq_date last_date_to_bootstrap,
	short *isCdsSpreadSupplied
    )
{
	/* TODO replace with proper defn of curve builder */
    unsigned int i;
	double term;
    rq_date last_date_bootstrapped = 0;

    /* construct an empty CDS curve */
    rq_cds_curve_t cdsCrv = rq_cds_curve_build(
        rq_bootstrap_config_get_curve_id(config)
        );

	const char *underlying_asset_id = rq_bootstrap_config_get_asset_id(config);
    if (underlying_asset_id)
        rq_cds_curve_set_underlying_asset_id(cdsCrv, underlying_asset_id);

	*isCdsSpreadSupplied = 1;	/* default assume CDS spreads supplied */

    /* iterate through the bootstrap_config getting asset ids */
    for (i = 0; i < rq_bootstrap_config_get_num_rate_class_ids(config); i++)
    {
        const rq_rate_t rate = rq_rate_mgr_find(
            rate_mgr, 
            rq_bootstrap_config_get_rate_class_id_at(config, i)
            );
		
        if (rate)
        {
			/* find and process the asset */
			const char *asset_id = rq_rate_get_asset_id(rate);
            const rq_asset_t asset = rq_asset_mgr_get(
                asset_mgr, 
                asset_id
                );

			/* get adjusted rate */
            double r = rq_rate_get_value(rate);
			if (rate_adjust_factors && rate_adjust_factors[i] != 0.0)
                r *= rate_adjust_factors[i];

            if (asset)
            {
                if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_recovery_rate))
                {                    
                    rq_cds_curve_set_recovery_rate(cdsCrv, r);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_cds_spread))
                {
					const char *quoteType = rq_asset_cds_spread_get_quote_type(asset);
					enum rq_cds_quote_convention quoteConv = rq_cds_quote_convention_from_string(quoteType);

					switch (quoteConv) {
						case RQ_CDS_QUOTE_CONVENTION_DEFAULT:
							r = 1.0 - r;	/* convert to survival rate */
							/* fall thru to next option */
						case RQ_CDS_QUOTE_CONVENTION_SURVIVAL:
							*isCdsSpreadSupplied = 0;
							break;
						default:
							;/* do nothing */
					}

                    last_date_bootstrapped = rq_rate_get_value_date(rate);
					term = (double)(rq_date_diff(last_date_bootstrapped, mktDate)) / 365.0;
                    rq_cds_curve_set_survival_rate(cdsCrv, term, r);
                }
			}

            /*rq_cds_curve_set_spread(cdsCrv, last_date_bootstrapped, rq_rate_get_value(rate));*/

            if (last_date_to_bootstrap > 0 && last_date_bootstrapped > last_date_to_bootstrap)
                break; /* break out of for loop, we've bootstrapped enough */
        }
    }

    if (rq_cds_curve_size(cdsCrv))
	{
		/* ensure the zero point is set */
        if (*isCdsSpreadSupplied)
		    rq_cds_curve_set_survival_rate(cdsCrv, 0.0, 0.0);
        else
            rq_cds_curve_set_survival_rate(cdsCrv, 0.0, 1.0);
        return cdsCrv;
	}

    rq_cds_curve_free(cdsCrv);
    return NULL;
}

void linsolve2(double** a, int n, int *indx, double b[])
{
	int i,ii=0,ip,j;
	double sum;

	for (i=1;i<=n;i++) {
		ip=indx[i];
		sum=b[ip];
		b[ip]=b[i];
		if (ii)
			for (j=ii;j<=i-1;j++)
			{
				double aa = a[i][j];
				double ab = b[j];
				double ac = a[i][j]*b[j]; 
				sum -= a[i][j]*b[j];
			}
		else 
			if (sum) 
				ii=i;
		b[i]=sum;
	}
	for (i=n;i>=1;i--) {
		sum=b[i];
		for (j=i+1;j<=n;j++) sum -= a[i][j]*b[j];
		b[i]=sum/a[i][i];
	}
}

void linsolve1(double** a, int n, int *indx, double *d)
{
	int i,imax,j,k;
	double big,dum,sum,temp;
	double *vv = (double*)RQ_CALLOC(20, sizeof(double));
	double nrerror= 0.0;
	
	*d=1.0;
	for (i=1;i<=n;i++) {
		big=0.0;
		for (j=1;j<=n;j++)
			if ((temp=fabs(a[i][j])) > big) big=temp;
			
			if (big == 0.0) 
				nrerror = 1.0; /*("Singular matrix"); */
		
		vv[i]=1.0/big;
	}
	for (j=1;j<=n;j++) {
		for (i=1;i<j;i++) {
			sum=a[i][j];
			for (k=1;k<i;k++) sum -= a[i][k]*a[k][j];
			a[i][j]=sum;
		}
		big=0.0;
		for (i=j;i<=n;i++) {
			sum=a[i][j];
			for (k=1;k<j;k++)
				sum -= a[i][k]*a[k][j];
			a[i][j]=sum;
			if ( (dum=vv[i]*fabs(sum)) >= big) {
				big=dum;
				imax=i;
			}
		}
		if (j != imax) {
			for (k=1;k<=n;k++) {
				dum=a[imax][k];
				a[imax][k]=a[j][k];
				a[j][k]=dum;
			}
			*d = -(*d);
			vv[imax]=vv[j];
		}
		indx[j]=imax;
		if (a[j][j] == 0.0) a[j][j]=TINY;
		if (j != n) {
			dum=1.0/(a[j][j]);
			for (i=j+1;i<=n;i++) a[i][j] *= dum;
		}
	}
	
	RQ_FREE(vv);
}

double** allocateDoubleMatrix(long RowStartIndex, long RowEndIndex, long ColStartIndex, long ColEndIndex)
{
	long i;
	long NumRows = RowEndIndex - RowStartIndex + 1;
	long NumCols = ColEndIndex - ColStartIndex + 1;
	double **Matrix;

	/* allocate pointers to rows */
	Matrix = (double**)malloc((NumRows + 1) * sizeof(double*));
	Matrix += 1;
	Matrix -= RowStartIndex;

	/* allocate rows and set pointers to them */
	Matrix[RowStartIndex] = (double*)malloc((NumRows * NumCols + 1) * sizeof(double));
	Matrix[RowStartIndex] += 1;
	Matrix[RowStartIndex] -= ColStartIndex;
	for (i = RowStartIndex + 1; i <= RowEndIndex; i++)
		Matrix[i] = Matrix[i - 1] + NumCols;

	/* return pointer to an array of pointers to rows */
	return Matrix;
}

void deallocateDoubleMatrix(double **Matrix, long RowStartIndex, long RowEndIndex, long ColStartIndex, long ColEndIndex) 
{
	free(Matrix[RowStartIndex] + ColStartIndex - 1);
	free(Matrix + RowStartIndex - 1);
}

double
calcSurvialRate(const rq_cds_curve_t spreadCurve, double *alpha, double *psi, double term)
{
	int sourcesNumber = rq_cds_curve_size(spreadCurve);
	double S = alpha[sourcesNumber+1];
	int l = 1;
	int it = 0;
	for (; it < sourcesNumber; ++it, ++l)
	{
		struct rq_cds_curve_elem *elem = rq_cds_curve_element_at(spreadCurve, it);
		double spreadTerm = rq_cds_curve_elem_get_term(elem);

		S += alpha[l] / sqrt((term - spreadTerm) * (term - spreadTerm) + psi[l] * psi[l]);
	}
	if (S > 1.0)
		S = 1.0;
	if (S < 0.0)
		S = 0.0;

	return S;
}

rq_cds_curve_t
compute_survival_rates(
    rq_date mktDate, 
    const rq_rate_mgr_t rate_mgr, 
    const rq_bootstrap_config_t config, 
    const rq_asset_mgr_t asset_mgr,
    rq_date last_date_to_bootstrap,
	const rq_cds_curve_t spreadCurve,
	rq_yield_curve_t yldCurve
    )
{
    /* construct an empty CDS curve */
    rq_cds_curve_t cdsCrv = rq_cds_curve_build(
        rq_bootstrap_config_get_curve_id(config)
        );

	int sourcesNumber = rq_cds_curve_size(spreadCurve);
	double spreadTerm = rq_cds_curve_elem_get_term(rq_cds_curve_element_at(spreadCurve, sourcesNumber - 1));
	int maxLegs = (int)(ceil(spreadTerm / rq_cds_curve_get_frequency(spreadCurve))+1);		
	double* SM = (double*)RQ_CALLOC(maxLegs, sizeof(double));
	double* SR = (double*)RQ_CALLOC(maxLegs, sizeof(double));
	double** SLT = allocateDoubleMatrix(0,maxLegs,0,maxLegs);
	double** DSC = allocateDoubleMatrix(0,maxLegs,0,maxLegs);
	double* SLN = (double*)RQ_CALLOC(maxLegs, sizeof(double));
	double** K = allocateDoubleMatrix(0,maxLegs,0,maxLegs);
	double* tau = (double*)RQ_CALLOC(maxLegs, sizeof(double));
	double* b = (double*)RQ_CALLOC(maxLegs, sizeof(double));
	int* indx = (int*)RQ_CALLOC(maxLegs, sizeof(int));
	double* d = (double*)RQ_CALLOC(maxLegs, sizeof(double));
	int l = 1;
	int i = 1;
	int it = 0;
	/*curve params */
	double pi = rq_cds_curve_get_recovery_rate(spreadCurve);		
	double* psi = (double*)RQ_CALLOC(sourcesNumber + 1, sizeof(double));
	double* alpha = (double*)RQ_CALLOC(sourcesNumber + 2, sizeof(double));

	/* copy existing params */
    rq_cds_curve_set_underlying_asset_id(cdsCrv, rq_cds_curve_get_underlying_asset_id(spreadCurve));
    rq_cds_curve_set_recovery_rate(cdsCrv, rq_cds_curve_get_recovery_rate(spreadCurve));

	for (it = 0; it < sourcesNumber; ++it)
	{
		double legs;
		int j;
		struct rq_cds_curve_elem *elem = rq_cds_curve_element_at(spreadCurve, it);
		double spreadTerm = rq_cds_curve_elem_get_term(elem);
		double spreadValue = rq_cds_curve_elem_get_survival_rate(elem);

		/* TODO implement fix for negative #'s
		if(spreadValue <= 0.0)
		{
			TermStructure::iterator fix_it = it;
			++fix_it;
			if(fix_it == pColl->curve_.spreads_.end())
			{
				--fix_it;
				--fix_it;
			}
			spreadTerm += ((*fix_it).first - spreadTerm)/8.0;
			spreadValue = (*fix_it).second/8.0;
		}*/

		SM[i] = spreadTerm;				
		SR[i] = spreadValue;	 
		tau[i] = rq_cds_curve_get_frequency(spreadCurve);		

		/* THC forced this loop to proceed when j =1 so always init something */
		for(legs = tau[i], j=1; (j == 1) || legs <= 1.0*SM[i]; legs += tau[i], ++j)
		{
			SLT[i][j] = legs;			
			DSC[i][j] = rq_yield_curve_get_discount_factor(yldCurve, mktDate + (long)(365.0 * legs));
		}
		SLN[i]=--j;						

		++i;
	};
	
	psi[1] = 200.0;
	for(i=2; i<=sourcesNumber-1; ++i) 
		psi[i] = (SM[i+1]-SM[i-1]); 
	psi[sourcesNumber]=200.0;

	for(l = 1; l<=sourcesNumber+1; ++l) 
	{ 		
		int legsNumber = (int)(SLN[l]);
		int j = 1;

		for(; j<=sourcesNumber+1; ++j) 
		{		 
			double a_bar = tau[l]*SR[l]+(1.0-pi);
			double b_bar = 1.0 - pi;

			if(l <= sourcesNumber)
			{
				if(j > sourcesNumber)
				{
					double sum = 0.0;
					int k= 2;
					for(; k<= legsNumber; ++k)
						sum += DSC[l][k];

					K[l][j]= a_bar*DSC[l][1] + (a_bar-b_bar)*sum;
				}
				else 
				{	
					double sum1 = 0.0;
					double sum2 = 0.0;
					double K_k_j = 1.0;
					int k=1;

					for(; k<=legsNumber; ++k) 
					{	
						double K_km1_j = K_k_j;	
						K_k_j = 1.0 / sqrt((SLT[l][k]-SM[j])*(SLT[l][k]-SM[j])+ psi[j]* psi[j]);
						
						sum1 += DSC[l][k]*K_k_j;
						if(k >= 2)
							sum2 += DSC[l][k]*K_km1_j;
						
						K[l][j] = a_bar*sum1 - b_bar*sum2; 
					}			
				}				
			}
			else
			{
				if(j > sourcesNumber)
					K[l][j] = 1.0;
				else
					K[l][j] = 1.0 / sqrt(SM[j]*SM[j] + psi[j] * psi[j]);
			}
		}
	}	

	for(l=1; l<=sourcesNumber+1; ++l)
	{
		if(l<=sourcesNumber)
			b[l] = (1.0 - pi)*DSC[l][1];
		else
			b[l] = 1.0;
	}

	linsolve1(K, sourcesNumber+1, &indx[0], &d[0]);	
	linsolve2(K, sourcesNumber+1, &indx[0], &b[0]);

	for(l=1; l <= sourcesNumber + 1; ++l)
		alpha[l] = b[l];

	/* free up tmp mem */
	RQ_FREE(SM);
	RQ_FREE(SR);
	deallocateDoubleMatrix(SLT,0,maxLegs,0,maxLegs);
	deallocateDoubleMatrix(DSC,0,maxLegs,0,maxLegs);
	RQ_FREE(SLN);
	deallocateDoubleMatrix(K,0,maxLegs,0,maxLegs);
	RQ_FREE(tau);
	RQ_FREE(b);
	RQ_FREE(indx);
	RQ_FREE(d);

	/* use params to calc & store some survival prob's */
	rq_cds_curve_set_survival_rate(cdsCrv, 0.0, 1.0);

	/*
    for(i = 0; s_curveTerms[i] != 0.0; ++i)
	{
		double surv = calcSurvialRate(spreadCurve, alpha, psi, s_curveTerms[i]);
		rq_cds_curve_set_survival_rate(cdsCrv, s_curveTerms[i], surv);
	}
    */
	for (i = 0; i < sourcesNumber; ++i)
	{
		struct rq_cds_curve_elem *elem = rq_cds_curve_element_at(spreadCurve, i);
		double spreadTerm = rq_cds_curve_elem_get_term(elem);
        double spreadRate = calcSurvialRate(spreadCurve, alpha, psi, spreadTerm);
        rq_cds_curve_set_survival_rate(cdsCrv, spreadTerm, spreadRate);
    }
    
	/* free up curve params */
	RQ_FREE(alpha);
	RQ_FREE(psi);

	return cdsCrv;
}

/* Take the current top_curve, and add the interpolated base curve beneath it */
void 
rq_layer_cds_spread_rates( rq_cds_curve_t top_curve, const rq_cds_curve_t base_curve )
{
	unsigned int i = 0, j = 0;
	if ( top_curve == NULL || top_curve->num_elements == 0 || base_curve == NULL || base_curve->num_elements == 0 ) return;
	for ( ; i < top_curve->num_elements ; ++i )
	{
		double currentTerm = top_curve->spreads[i].term;	

		/* Increment j until you find the first element of the
         * base_curve equal or after the current 
         */
		while ( j < base_curve->num_elements && base_curve->spreads[j].term < currentTerm ) 
			++j;

		if ( base_curve->spreads[j].term < currentTerm )
		{
			/* not enough data in base curve, top curve has a later time spam
               Don't manipulate value
            */
		}
		else if ( base_curve->spreads[j].term == currentTerm )
		{
			/* Dates line up, just add values */
			top_curve->spreads[i].survival_rate += base_curve->spreads[j].survival_rate;
		}
		else
		{
			/* Dates dont line up, need to interpolate value. */
			if ( j == 0 )
			{
				/* First top curve date is before first base curve date
                   Don't manipulate value
                */
			}
			else
			{
				/* interpolate the two base points either side for the
                 * addition value
                 */
				top_curve->spreads[i].survival_rate += rq_interpolate_linear(
                            (double)currentTerm,
							(double)base_curve->spreads[j-1].term,
							base_curve->spreads[j-1].survival_rate,
                            (double)base_curve->spreads[j].term,
							base_curve->spreads[j].survival_rate
                            );
			}
		}

	}
}

/* Recursive method for doing layered CDS spread curves */
void 
rq_build_net_cds_spread_rates(
	rq_date mktDate,
	const rq_system_t system,
	const rq_market_t market,
    const rq_bootstrap_config_t config, 
    rq_date last_date_to_bootstrap,
	short *isCdsSpreadSupplied,
	rq_cds_curve_t *net_cds_spread_rates,
	rq_yield_curve_t* base_yield_curve )
{
	/* get spread rates for specified curve */
	rq_cds_curve_t spread_rates = load_cds_spread_rates(mktDate, 
														rq_market_get_rate_mgr(market), 
														config, 
														rq_system_get_asset_mgr(system),
														NULL,
														last_date_to_bootstrap,
														isCdsSpreadSupplied
														);
		
	if ( *net_cds_spread_rates == NULL )
	{
		/* First level curve, thus this is the whole curve */
		*net_cds_spread_rates = spread_rates;
	}
	else
	{
		/* add rates to current curve being built if layered
         * (interpolate this curve to fit the arguments times) 
         */
		rq_layer_cds_spread_rates( *net_cds_spread_rates, spread_rates );
		if ( spread_rates )
			rq_cds_curve_free(spread_rates);

		if (rq_cds_curve_size(*net_cds_spread_rates))
		{
			/* ensure the zero point is set to 1 */
            /* should always be spread */
            if (*isCdsSpreadSupplied)
                rq_cds_curve_set_survival_rate(*net_cds_spread_rates, 0.0, 0.0);
            else
                rq_cds_curve_set_survival_rate(*net_cds_spread_rates, 0.0, 1.0);
		}
	}

	if (!(*isCdsSpreadSupplied)) return;

	/* references another curve */
	if (config && config->curve_id1)
    {
		/* Check if underlying curve is a yield curve */
        rq_yield_curve_mgr_t ycmgr = rq_market_get_yield_curve_mgr(market);
        *base_yield_curve = rq_yield_curve_mgr_get(
            ycmgr,
            config->curve_id1
            );
		if ( *base_yield_curve )	
		{
			/* Underlying curve is a yield curve */
		}
		else /* Check if underlying curve is a CDS curve, and therefore layered */
		{
			rq_bootstrap_config_t layered_bootstrap_config = rq_bootstrap_config_mgr_find(
																rq_system_get_bootstrap_config_mgr(system),
																config->curve_id1,
																RQ_TERMSTRUCT_TYPE_CDS_SPREAD_CURVE
																);
			if ( layered_bootstrap_config ) 
			{
				/* Recurse layered CDS curves */
				rq_build_net_cds_spread_rates(	mktDate,
												system,
												market,
												layered_bootstrap_config, 
												last_date_to_bootstrap,
												isCdsSpreadSupplied,
												net_cds_spread_rates,
												base_yield_curve );
			}
		}
	}
}

RQ_EXPORT rq_cds_curve_t
rq_bootstrap_cds_curve(
    const rq_system_t system,
	const rq_market_t market,
    const rq_bootstrap_config_t config, 
    rq_date last_date_to_bootstrap    
    )
{
	rq_date mktDate = rq_market_get_market_date(market);
	short isCdsSpreadSupplied;
	rq_yield_curve_t yldCurve = NULL;
	rq_cds_curve_t cdsCurve = NULL;
	rq_cds_curve_t net_cds_spread_rates = NULL;

	rq_build_net_cds_spread_rates(	mktDate,
									system,
									market,
									config, 
									last_date_to_bootstrap,
									&isCdsSpreadSupplied,
									&net_cds_spread_rates,
									&yldCurve );

	if (!isCdsSpreadSupplied)
		return net_cds_spread_rates;		/* actual probabilities supplied so no further bootstrapping required */

	/* now compute survival rates fdreom the spread rates */
	if (net_cds_spread_rates && yldCurve)
	{
		cdsCurve = compute_survival_rates(mktDate, 
											rq_market_get_rate_mgr(market), 
											config, 
											rq_system_get_asset_mgr(system),
											last_date_to_bootstrap,
											net_cds_spread_rates,
											yldCurve
											);		
	}

	if (net_cds_spread_rates)
	    rq_cds_curve_free(net_cds_spread_rates);

	return cdsCurve;
}

