/**
 * @file
 *
 * A matrix object
 */
/*
** rq_matrix.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2004-2008 Brett Hutley
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
#ifndef rq_matrix_h
#define rq_matrix_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The matrix structure.
 */
typedef struct rq_matrix {
    unsigned long rows;
    unsigned long cols;
    double *vals;
} *rq_matrix_t;


/** Test whether the rq_matrix is NULL */
RQ_EXPORT int rq_matrix_is_null(rq_matrix_t obj);

/** Allocate a new matrix.
 */
RQ_EXPORT rq_matrix_t rq_matrix_alloc();

/** Clone a matrix.
 */
RQ_EXPORT rq_matrix_t rq_matrix_clone(rq_matrix_t mat);

/** Builds a new matrix of the specified dimensions, and initializes
 * it to zeros.
 */
RQ_EXPORT rq_matrix_t rq_matrix_build(unsigned long rows, unsigned long cols);

/** Builds a new matrix of the specified dimensions, and initializes
 * it with the specified values.
 */
RQ_EXPORT rq_matrix_t rq_matrix_build_with_values(unsigned long rows, unsigned long cols, double first_value, ...);

/** Free an object of type rq_matrix
 */
RQ_EXPORT void rq_matrix_free(rq_matrix_t matrix);

/** (Re)dimension a matrix.
 */
RQ_EXPORT void rq_matrix_dim(rq_matrix_t matrix, unsigned long rows, unsigned long cols);

/** Set a value in the matrix.
 */
RQ_EXPORT void rq_matrix_set(rq_matrix_t matrix, unsigned row, unsigned col, double val);

/** Set a number of values in the matrix.
 *
 * The values are set from row 0, column 0 for each column the first
 * row, and then down to the next row, etc until 'num_values' values
 * have been set.
 */
RQ_EXPORT void rq_matrix_set_values(rq_matrix_t matrix, unsigned long num_values, double first_value, ...);

/** Get a value in the matrix.
 */
RQ_EXPORT double rq_matrix_get(const rq_matrix_t matrix, unsigned row, unsigned col);

/** Get the number of rows in the matrix.
 */
RQ_EXPORT unsigned long rq_matrix_get_rows(const rq_matrix_t matrix);

/** Get the number of columns in the matrix.
 */
RQ_EXPORT unsigned long rq_matrix_get_columns(const rq_matrix_t matrix);

/** Zero a matrix.
 */
RQ_EXPORT void rq_matrix_zero(rq_matrix_t matrix);

/** Test for matrix equality.
 */
RQ_EXPORT short rq_matrix_is_equal(const rq_matrix_t m, const rq_matrix_t m2);

/** Print a matrix to stdout.
 */
RQ_EXPORT void rq_matrix_print(const rq_matrix_t m);

/** Subtract one matrix from another matrix.
 */
RQ_EXPORT short rq_matrix_subtract(const rq_matrix_t m1, const rq_matrix_t m2, rq_matrix_t out);

/** Add one matrix to another matrix.
 */
RQ_EXPORT short rq_matrix_add(const rq_matrix_t m1, const rq_matrix_t m2, rq_matrix_t out);

/** Multiply a matrix by a scalar value.
 */
RQ_EXPORT short rq_matrix_multiply_scalar(const rq_matrix_t m1, double scalar, rq_matrix_t out);

/** Transpose the in matrix, putting the transpose into the out matrix.
 */
RQ_EXPORT void rq_matrix_transpose(const rq_matrix_t in, rq_matrix_t out);

/** Perform a cholesky decomposition of a matrix.
 */
RQ_EXPORT short rq_matrix_cholesky(const rq_matrix_t in, rq_matrix_t out);

/** Perform the householder function on a matrix.
 */
RQ_EXPORT void rq_matrix_householder(rq_matrix_t m, double *d, double *e);

/** Perform the QL algorithm with implicit shifts to determine the
 * eigenvalues and eigenvectors of a real symmetric tridiagonal
 * matrix, or a real, symmetric matrix previously reduced using the
 * householder algorithm.
 */
RQ_EXPORT short rq_matrix_ql(rq_matrix_t m, double *d, double *e);

/** Calculate the cross product of 2 matrices.
 */
RQ_EXPORT short rq_matrix_multiply(const rq_matrix_t m1, rq_matrix_t m2, rq_matrix_t out);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

