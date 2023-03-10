//------------------------------------------------------------------------------
// GB_sel:  hard-coded functions for selection operators
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2023, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

// FIXME:
#define GB_DEBUG

#include "GB_select.h"
#include "GB_ek_slice.h"
#include "GB_sel__include.h"

// kind
#define GB_ENTRY_SELECTOR

#define GB_A_TYPE \
    GB_atype

// test value of Ax [p]
#define GB_TEST_VALUE_OF_ENTRY(keep,p)                  \
    GB_test_value_of_entry

// Cx [pC] = Ax [pA], no typecast
#define GB_SELECT_ENTRY(Cx,pC,Ax,pA)                    \
    GB_select_entry

#include "GB_kernel_shared_definitions.h"

m4_divert(if_phase1)
//------------------------------------------------------------------------------
// GB_sel_phase1
//------------------------------------------------------------------------------

void GB (_sel_phase1)
(
    int64_t *restrict Cp,
    int64_t *restrict Wfirst,
    int64_t *restrict Wlast,
    const GrB_Matrix A,
    const GB_void *restrict ythunk,
    const GB_IndexUnaryOp op,
    const int64_t *A_ek_slicing,
    const int A_ntasks,
    const int A_nthreads
)
{ 
    #include "GB_select_entry_phase1_template.c"
}

m4_divert(if_phase2)
//------------------------------------------------------------------------------
// GB_sel_phase2
//------------------------------------------------------------------------------

void GB (_sel_phase2)
(
    int64_t *restrict Ci,
    GB_void *restrict Cx_out,
    const int64_t *restrict Cp,
    const int64_t *restrict Cp_kfirst,
    const GrB_Matrix A,
    const GB_void *restrict ythunk,
    const GB_IndexUnaryOp op,
    const int64_t *A_ek_slicing,
    const int A_ntasks,
    const int A_nthreads
)
{ 
    GB_A_TYPE *restrict Cx = (GB_A_TYPE *) Cx_out ;
    #include "GB_select_phase2.c"
}
m4_divert(0)

m4_divert(if_bitmap)
//------------------------------------------------------------------------------
// GB_sel_bitmap
//------------------------------------------------------------------------------

void GB (_sel_bitmap)
(
    int8_t *Cb,
    int64_t *cnvals_handle,
    GrB_Matrix A,
    const GB_void *restrict ythunk,
    const GB_IndexUnaryOp op,
    const int nthreads
)
{ 
    #include "GB_bitmap_select_template.c"
}
m4_divert(0)

