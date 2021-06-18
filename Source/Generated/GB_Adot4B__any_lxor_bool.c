//------------------------------------------------------------------------------
// GB_Adot4B:  hard-coded dot4 method for a semiring
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2021, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

// If this file is in the Generated/ folder, do not edit it (auto-generated).

#include "GB_AxB_defs__any_lxor_bool.h"
#ifndef GBCOMPACT

//------------------------------------------------------------------------------
// C+=A'*B: dense dot product
//------------------------------------------------------------------------------

GrB_Info GB (_Adot4B__any_lxor_bool)
(
    GrB_Matrix C, const bool C_in_iso, const GB_void *cinput_void,
    const GrB_Matrix A, bool A_is_pattern,
    int64_t *restrict A_slice, int naslice,
    const GrB_Matrix B, bool B_is_pattern,
    int64_t *restrict B_slice, int nbslice,
    const int nthreads
)
{ 
    #if GB_DISABLE
    return (GrB_NO_VALUE) ;
    #else
    const bool cinput = (*((const bool *) cinput_void)) ;
    #include "GB_AxB_dot4_meta.c"
    return (GrB_SUCCESS) ;
    #endif
}

#endif

