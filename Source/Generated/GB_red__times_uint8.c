//------------------------------------------------------------------------------
// GB_red:  hard-coded functions for reductions
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2019, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

#include "GB.h"
#ifndef GBCOMPACT
#include "GB_red__include.h"

// The reduction is defined by the following types and operators:

// Assemble tuples:    GB_red_build__times_uint8
// Reduce to scalar:   GB_red_scalar__times_uint8
// Reduce each vector: GB_red_eachvec__times_uint8
// Reduce each index:  GB_red_eachindex__times_uint8

// A type:   uint8_t
// C type:   uint8_t

// Reduce:   s *= aij
// Identity: 1
// Terminal: if (s == 0) break ;

#define GB_ATYPE \
    uint8_t

#define GB_CTYPE \
    uint8_t

// Array to array

    // W [k] = (ztype) S [i], with typecast
    #define GB_CAST_ARRAY_TO_ARRAY(W,k,S,i)         \
        W [k] = S [i]

    // W [k] += (ztype) S [i], with typecast
    #define GB_ADD_CAST_ARRAY_TO_ARRAY(W,k,S,i)     \
        W [k] *= S [i]

    // W [k] = S [i], no typecast
    #define GB_COPY_ARRAY_TO_ARRAY(W,k,S,i)         \
        W [k] = S [i]

    // W [k] += S [i], no typecast
    #define GB_ADD_ARRAY_TO_ARRAY(W,k,S,i)          \
        W [k] *= S [i]

// Array to scalar

    // ztype s = (ztype) Ax [p], with typecast
    #define GB_CAST_ARRAY_TO_SCALAR(s,Ax,p)         \
        uint8_t s = Ax [p]

    // s += (ztype) Ax [p], with typecast
    #define GB_ADD_CAST_ARRAY_TO_SCALAR(s,Ax,p)     \
        s *= Ax [p]

    // s += S [i], no typecast
    #define GB_ADD_ARRAY_TO_SCALAR(s,S,i)           \
        s *= S [i]

// Scalar to array

    // W [k] = s, no typecast
    #define GB_COPY_SCALAR_TO_ARRAY(W,k,s)          \
        W [k] = s

    // W [k] += s, no typecast
    #define GB_ADD_SCALAR_TO_ARRAY(W,k,s)           \
        W [k] *= s

// set scalar to identity

    // s = identity
    #define GB_SCALAR_IDENTITY(s)                   \
        uint8_t s = 1

// workspace

    // declare a ztype array of size ntasks
    #define GB_REDUCTION_WORKSPACE(W,ntasks)        \
        uint8_t W [ntasks]   

// break the loop if terminal condition reached

    #define GB_BREAK_IF_TERMINAL(s)                 \
        if (s == 0) break ;

    #define GB_IF_NOT_EARLY_EXIT                    \
        bool my_exit ; \
        GB_PRAGMA (omp atomic read) \
        my_exit = early_exit ; \
        if (!my_exit)

    #define GB_PARALLEL_BREAK_IF_TERMINAL(s)        \
        if (s == 0) \
        { \
            GB_PRAGMA (omp atomic write) \
            early_exit = true ; \
            break ; \
        }
 

//------------------------------------------------------------------------------
// reduce to a scalar, for monoids only
//------------------------------------------------------------------------------



void GB_red_scalar__times_uint8
(
    uint8_t *result,
    const GrB_Matrix A,
    int ntasks,
    int nthreads
)
{ 
    uint8_t s = (*result) ;
    #include "GB_reduce_to_scalar_template.c"
    (*result) = s ;
}

//------------------------------------------------------------------------------
// reduce to each vector: each vector A(:,k) reduces to a scalar Tx (k)
//------------------------------------------------------------------------------

void GB_red_eachvec__times_uint8
(
    uint8_t *restrict Tx,
    GrB_Matrix A,
    const int64_t *restrict kfirst_slice,
    const int64_t *restrict klast_slice,
    const int64_t *restrict pstart_slice,
    int ntasks,
    int nthreads
)
{
    #include "GB_reduce_each_vector.c"
}

//------------------------------------------------------------------------------
// reduce to each index: each A(i,:) reduces to a scalar T (i)
//------------------------------------------------------------------------------

GrB_Info GB_red_eachindex__times_uint8
(
    GrB_Matrix *Thandle,
    GrB_Type ttype,
    GrB_Matrix A,
    const int64_t *restrict pstart_slice,
    int nth,
    int nthreads,
    GB_Context Context
)
{
    GrB_Info info = GrB_SUCCESS ;
    GrB_Matrix T = NULL ;
    (*Thandle) = NULL ;
    #define GB_FREE_ALL ;
    #include "GB_reduce_each_index.c"
    (*Thandle) = T ;
    return (info) ;
}



//------------------------------------------------------------------------------
// build matrix
//------------------------------------------------------------------------------

void GB_red_build__times_uint8
(
    uint8_t *restrict Tx,
    int64_t  *restrict Ti,
    const uint8_t *restrict S,
    int64_t nvals,
    int64_t ndupl,
    const int64_t *restrict I_work,
    const int64_t *restrict K_work,
    const int64_t *restrict tstart_slice,
    const int64_t *restrict tnz_slice,
    int nthreads
)
{
    #include "GB_reduce_build_template.c"
}

#endif

