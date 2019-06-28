//------------------------------------------------------------------------------
// GB_unaryop:  hard-coded functions for each built-in unary operator
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2019, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// If this file is in the Generated/ folder, do not edit it (auto-generated)

#include "GB.h"
#ifndef GBCOMPACT
#include "GB_unaryop__include.h"
#include "GB_control.h"

// C=unop(A) is defined by the following types and operators:

// op(A)  function:  GB_unop__minv_int32_int32
// op(A') function:  GB_tran__minv_int32_int32

// C type:   int32_t
// A type:   int32_t
// cast:     int32_t cij = aij
// unaryop:  cij = GB_IMINV_SIGNED (aij, 32)

#define GB_ATYPE \
    int32_t

#define GB_CTYPE \
    int32_t

// aij = Ax [pA]
#define GB_GETA(aij,Ax,pA)  \
    int32_t aij = Ax [pA]

#define GB_CX(p) Cx [p]

// unary operator
#define GB_OP(z, x)   \
    z = GB_IMINV_SIGNED (x, 32) ;

// casting
#define GB_CASTING(z, x)   \
    int32_t z = x ;

// cij = op (cast (aij))
#define GB_CAST_OP(pC,pA)           \
{                                   \
    /* aij = Ax [pA] */             \
    GB_GETA (aij, Ax, pA) ;         \
    /* Cx [pC] = op (cast (aij)) */ \
    GB_CASTING (x, aij) ;           \
    GB_OP (GB_CX (pC), x) ;         \
}

// disable this operator and use the generic case if these conditions hold
#define GB_DISABLE \
    (GxB_NO_MINV || GxB_NO_INT32)

//------------------------------------------------------------------------------
// Cx = op (cast (Ax)): apply a unary operator
//------------------------------------------------------------------------------

GrB_Info GB_unop__minv_int32_int32
(
    int32_t *restrict Cx,
    int32_t *restrict Ax,
    int64_t anz,
    int nthreads
)
{ 
    #if GB_DISABLE
    return (GrB_NO_VALUE) ;
    #else
    #pragma omp parallel for num_threads(nthreads)
    for (int64_t p = 0 ; p < anz ; p++)
    {
        GB_CAST_OP (p, p) ;
    }
    return (GrB_SUCCESS) ;
    #endif
}

//------------------------------------------------------------------------------
// C = op (cast (A')): transpose, typecast, and apply a unary operator
//------------------------------------------------------------------------------

GrB_Info GB_tran__minv_int32_int32
(
    GrB_Matrix C,
    const GrB_Matrix A,
    int64_t **Rowcounts,
    GBI_single_iterator Iter,
    const int64_t *restrict A_slice,
    int naslice
)
{ 
    #if GB_DISABLE
    return (GrB_NO_VALUE) ;
    #else
    #define GB_PHASE_2_OF_2
    #include "GB_unaryop_transpose.c"
    return (GrB_SUCCESS) ;
    #endif
}

#endif

