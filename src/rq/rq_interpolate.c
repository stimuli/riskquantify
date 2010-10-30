/*
** rq_interpolate.c
**
** Interpolation functions.
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
#include "rq_interpolate.h"
#include "rq_config.h"
#include "rq_defs.h"
#include <math.h>
#include <stdlib.h>

/**
 * Linearly interpolate between two points.
 */
RQ_EXPORT double 
rq_interpolate_linear(double x, double x1, double y1, double x2, double y2)
{
	if (!RQ_DOUBLE_EQUAL(x1,x2))
	{
	    double lambda = (x2 - x) / (x2 - x1);
	    double y = y1 * lambda + y2 * (1.0-lambda); 
	    return y;
	}
	else
	{
		return y1;
	}
}

/** The log linear interpolation is alternatively known as the
 * geometric interpolation.
 *
 * This method linearly interpolates the logarithm of a point at
 * (x,y), given the two points (x1,y1) and (x2,y2)
 */
RQ_EXPORT double 
rq_interpolate_log_linear(double x, double x1, double y1, double x2, double y2)
{
	if (!RQ_DOUBLE_EQUAL(x1,x2))
	{
		double alpha = (x - x1) / (x2 - x1);
		double y = pow(y1,(1.0-alpha)) * pow(y2,(alpha));
	    return y;
	}
	else
	{
		return y1;
	}
}

/** This func is the first step of the two step cubic spline.  This
 * func calculates the time consuming calculations of retrieving the
 * second derivates.  Hence this func should only be called once due
 * to performance.
 *
 * ARGS
 * natural  - for a natural spline set to 1 otherwise 0
 * x[]  -  ordered array of x co-ordinate points
 * y[]  -  ordered array of y co-ordinate points
 * n    -  number of input points
 * yp1  -  first derivative at point 1
 * ypn  -  first derivative at point n
 *
 * RETURN
 * y2[]  -  ordered array of the second derivatives of points 1 to n
*/
RQ_EXPORT void rq_interpolate_spline(unsigned int natural, double x[], double y[], int n, double yp1, double ypn, double y2[])
{
	int i,k;
	double p,qn,sig,un, u[RQ_MAX_SPLINE_POINTS*4];

	if (natural)
	{
		y2[1] = 0.0;
		u[1] = 0.0;
	}
	else 
	{
		y2[1] = -0.5;
		u[1]=(3.0/(x[2]-x[1]))*((y[2]-y[1])/(x[2]-x[1])-yp1);
	}
	for (i=2;i<=n-1;i++)
	{
		sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
		p=sig*y2[i-1]+2.0;
		y2[i]=(sig-1.0)/p;
		u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
		u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
	}
	
	if (natural)
	{
		qn=un=0.0;
	}
	else 
	{
		qn=0.5;
		un=(3.0/(x[n]-x[n-1]))*(ypn-(y[n]-y[n-1])/(x[n]-x[n-1]));
	}

	y2[n]=(un-qn*u[n-1])/(qn*y2[n-1]+1.0);
	for (k=n-1;k>=1;k--)
	{
		y2[k]=y2[k]*y2[k+1]+u[k];
	}
}

// This func is the second step of the two step cubic spline.  It can calculate any interpolated value
// for x given the second derivative.
// ARGS
// xa[]  -  ordered array of x co-ordinate points
// ya[]  -  oredered array of y co-ordinate points
// y2a[]  -  ordered array of the second derivatives of points 1 to n
// n    -  number of input points
// x  -  x co-ordinate of an interpolated point
// y  -  interpolated y co-ordinate
RQ_EXPORT double rq_interpolate_splint(double x, double xa[], double ya[], double y2a[], int n)
{
	int klo,khi,k;
	double h,b,a;
	klo=1; 
	khi=n;
	while (khi-klo > 1) 
	{
		k=(khi+klo) >> 1;
		if (xa[k] > x) 
			khi=k;
		else 
			klo=k;
	}
	h=xa[khi]-xa[klo];
	if (h == 0.0)
	{
		return 0.0;
	}
	a=(xa[khi]-x)/h;
	b=(x-xa[klo])/h; 
	return a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
}
