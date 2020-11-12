
//------------------------------------------------------------------------------
// GB_unop:  hard-coded functions for each built-in unary operator
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

// If this file is in the Generated/ folder, do not edit it (auto-generated).

#include "GB.h"
#ifndef GBCOMPACT
#include "GB_control.h"
#include "GB_unop__include.h"

// C=unop(A) is defined by the following types and operators:

// op(A)  function:  GB_unop_apply__tanh_fp32_fp32
// op(A') function:  GB_unop_tran__tanh_fp32_fp32

// C type:   float
// A type:   float
// cast:     float cij = aij
// unaryop:  cij = tanhf (aij)

#define GB_ATYPE \
    float

#define GB_CTYPE \
    float

// aij = Ax [pA]
#define GB_GETA(aij,Ax,pA) \
    float aij = Ax [pA]

#define GB_CX(p) Cx [p]

// unary operator
#define GB_OP(z, x) \
    z = tanhf (x) ;

// casting
#define GB_CAST(z, aij) \
    float z = aij ;

// cij = op (aij)
#define GB_CAST_OP(pC,pA)           \
{                                   \
    /* aij = Ax [pA] */             \
    float aij = Ax [pA] ;          \
    /* Cx [pC] = op (cast (aij)) */ \
    float z = aij ;               \
    Cx [pC] = tanhf (z) ;        \
}

// true if operator is the identity op with no typecasting
#define GB_OP_IS_IDENTITY_WITH_NO_TYPECAST \
    0

// disable this operator and use the generic case if these conditions hold
#define GB_DISABLE \
    (GxB_NO_TANH || GxB_NO_FP32)

//------------------------------------------------------------------------------
// Cx = op (cast (Ax)): apply a unary operator
//------------------------------------------------------------------------------

GrB_Info GB_unop_apply__tanh_fp32_fp32
(
    float *Cx,       // Cx and Ax may be aliased
    const float *Ax,
    const int8_t *GB_RESTRICT Ab,   // A->b if A is bitmap
    int64_t anz,
    int nthreads
)
{
    #if GB_DISABLE
    return (GrB_NO_VALUE) ;
    #else
    int64_t p ;
    if (Ab == NULL)
    { 
        #if ( GB_OP_IS_IDENTITY_WITH_NO_TYPECAST )
            GB_memcpy (Cx, Ax, anz * sizeof (float), nthreads) ;
        #else
            #pragma omp parallel for num_threads(nthreads) schedule(static)
            for (p = 0 ; p < anz ; p++)
            {
                float aij = Ax [p] ;
                float z = aij ;
                Cx [p] = tanhf (z) ;
            }
        #endif
    }
    else
    { 
        // bitmap case, no transpose; A->b already memcpy'd into C->b
        #pragma omp parallel for num_threads(nthreads) schedule(static)
        for (p = 0 ; p < anz ; p++)
        {
            if (!Ab [p]) continue ;
            float aij = Ax [p] ;
            float z = aij ;
            Cx [p] = tanhf (z) ;
        }
    }
    return (GrB_SUCCESS) ;
    #endif
}

//------------------------------------------------------------------------------
// C = op (cast (A')): transpose, typecast, and apply a unary operator
//------------------------------------------------------------------------------

GrB_Info GB_unop_tran__tanh_fp32_fp32
(
    GrB_Matrix C,
    const GrB_Matrix A,
    int64_t *GB_RESTRICT *Rowcounts,
    const int64_t *GB_RESTRICT A_slice,
    int naslice,
    int nthreads
)
{ 
    #if GB_DISABLE
    return (GrB_NO_VALUE) ;
    #else
    #define GB_PHASE_2_OF_2
    #include "GB_unop_transpose.c"
    return (GrB_SUCCESS) ;
    #endif
}

#endif

