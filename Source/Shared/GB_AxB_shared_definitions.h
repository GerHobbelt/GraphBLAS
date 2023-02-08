//------------------------------------------------------------------------------
// GB_AxB_shared_defintions.h: common macros for A*B kernels
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2023, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

// GB_AxB_shared_defintions.h provides default definitions for all semirings,
// if the special cases have not been #define'd prior to #include'ing this
// file.

//------------------------------------------------------------------------------
// special semirings
//------------------------------------------------------------------------------

// 1 for the symbolic ANY_PAIR semiring
#ifndef GB_IS_ANY_PAIR_SEMIRING
#define GB_IS_ANY_PAIR_SEMIRING 0
#endif

// 1 for PLUS_PAIR semirings (integer, float, and double; not bool or complex)
#ifndef GB_IS_PLUS_PAIR_REAL_SEMIRING
#define GB_IS_PLUS_PAIR_REAL_SEMIRING 0
#endif

// 1 for EQ_PAIR_BOOL
#ifndef GB_IS_EQ_PAIR_SEMIRING
#define GB_IS_EQ_PAIR_SEMIRING 0
#endif

// 1 for XOR_PAIR_BOOL
#ifndef GB_IS_XOR_PAIR_SEMIRING
#define GB_IS_XOR_PAIR_SEMIRING 0
#endif

// 1 for PLUS_PAIR_INT8 and PLUS_PAIR_UINT8
#ifndef GB_IS_PLUS_8_PAIR_SEMIRING
#define GB_IS_PLUS_8_PAIR_SEMIRING 0
#endif

// 1 for PLUS_PAIR_INT16 and PLUS_PAIR_UINT16
#ifndef GB_IS_PLUS_16_PAIR_SEMIRING
#define GB_IS_PLUS_16_PAIR_SEMIRING 0
#endif

// 1 for PLUS_PAIR_INT32 and PLUS_PAIR_UINT32
#ifndef GB_IS_PLUS_32_PAIR_SEMIRING
#define GB_IS_PLUS_32_PAIR_SEMIRING 0
#endif

// 1 for PLUS_PAIR_(INT64, UINT64, FP32, and FP64)
#ifndef GB_IS_PLUS_BIG_PAIR_SEMIRING
#define GB_IS_PLUS_BIG_PAIR_SEMIRING 0
#endif

// 1 for PLUS_PAIR_FC32
#ifndef GB_IS_PLUS_FC32_PAIR_SEMIRING
#define GB_IS_PLUS_FC32_PAIR_SEMIRING 0
#endif

// 1 for PLUS_PAIR_FC64
#ifndef GB_IS_PLUS_FC64_PAIR_SEMIRING
#define GB_IS_PLUS_FC64_PAIR_SEMIRING 0
#endif

//------------------------------------------------------------------------------
// special multiply operators
//------------------------------------------------------------------------------

// 1 if the multiply operator is PAIR
#ifndef GB_IS_PAIR_MULTIPLIER
#define GB_IS_PAIR_MULTIPLIER 0
#endif

// 1 for the FIRSTI1, FIRSTJ1, SECONDI1, or SECONDJ1 multiply operators
#ifndef GB_OFFSET
#define GB_OFFSET 0
#endif

// 1 for the FIRSTI or FIRSTI1 multiply operator
#ifndef GB_IS_FIRSTI_MULTIPLIER
#define GB_IS_FIRSTI_MULTIPLIER 0
#endif

// 1 for the FIRSTJ, FIRSTJ1, SECONDI, or SECONDI1 multiply operator
#ifndef GB_IS_FIRSTJ_MULTIPLIER
#define GB_IS_FIRSTJ_MULTIPLIER 0
#endif

// 1 for the SECONDJ or SECONDJ1 multiply operator
#ifndef GB_IS_SECONDJ_MULTIPLIER
#define GB_IS_SECONDJ_MULTIPLIER 0
#endif

//------------------------------------------------------------------------------
// numerical operations and assignments
//------------------------------------------------------------------------------

#if GB_IS_ANY_PAIR_SEMIRING

    //--------------------------------------------------------------------------
    // ANY_PAIR semiring: no values are accessed
    //--------------------------------------------------------------------------

    // declare a scalar of ctype
    #ifndef GB_DECLAREC
    #define GB_DECLAREC(c)
    #endif

    // Cx [p] = t
    #ifndef GB_CIJ_WRITE
    #define GB_CIJ_WRITE(p,t)
    #endif

    // Hx [i] = t
    #ifndef GB_HX_WRITE
    #define GB_HX_WRITE(i,t)
    #endif

    // Cx [p] = Hx [i]
    #ifndef GB_CIJ_GATHER
    #define GB_CIJ_GATHER(p,i)
    #endif

    // C(i,j) += t
    #ifndef GB_CIJ_UPDATE
    #define GB_CIJ_UPDATE(p,t)
    #endif

    // Cx [p] += Hx [i]
    #ifndef GB_CIJ_GATHER_UPDATE
    #define GB_CIJ_GATHER_UPDATE(p,i)
    #endif

    // Hx [i] += t
    #ifndef GB_HX_UPDATE
    #define GB_HX_UPDATE(i,t)
    #endif

    // Cx [p:p+len-1] = Hx [i:i+len-1]
    #ifndef GB_CIJ_MEMCPY
    #define GB_CIJ_MEMCPY(p,i,len)
    #endif

#else

    //--------------------------------------------------------------------------
    // all pre-generated and JIT kernels
    //--------------------------------------------------------------------------

    // These definitions require explicit types to be used, not GB_void.
    // Generic methods using GB_void for all types, memcpy, and function
    // pointers for all computations must #define these macros first.

    // declare a scalar of ctype
    #ifndef GB_DECLAREC
    #define GB_DECLAREC(c) GB_C_TYPE c
    #endif

    // Cx [p] = t
    #ifndef GB_CIJ_WRITE
    #define GB_CIJ_WRITE(p,t) Cx [p] = t
    #endif

    // Hx [i] = t
    #ifndef GB_HX_WRITE
    #define GB_HX_WRITE(i,t) Hx [i] = t
    #endif

    // Cx [p] = Hx [i]
    #ifndef GB_CIJ_GATHER
    #define GB_CIJ_GATHER(p,i) Cx [p] = Hx [i]
    #endif

    // C(i,j) += t
    #ifndef GB_CIJ_UPDATE
    #define GB_CIJ_UPDATE(p,t) GB_UPDATE (Cx [p], t)
    #endif

    // Hx [i] += t
    #ifndef GB_HX_UPDATE
    #define GB_HX_UPDATE(i,t) GB_UPDATE (Hx [i], t)
    #endif

    // Cx [p] += Hx [i]
    #ifndef GB_CIJ_GATHER_UPDATE
    #define GB_CIJ_GATHER_UPDATE(p,i) GB_UPDATE (Cx [p], Hx [i])
    #endif

    // Cx [p:p+len-1] = Hx [i:i+len-1]
    #ifndef GB_CIJ_MEMCPY
    #define GB_CIJ_MEMCPY(p,i,len) \
        memcpy (Cx +(p), Hx +(i), (len) * sizeof (GB_C_TYPE))
    #endif

#endif

//------------------------------------------------------------------------------
// monoid definitions
//------------------------------------------------------------------------------

#include "GB_monoid_shared_definitions.h"

