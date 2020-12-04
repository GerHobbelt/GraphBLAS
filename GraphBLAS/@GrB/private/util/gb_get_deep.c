//------------------------------------------------------------------------------
// gb_get_deep: create a deep GrB_Matrix copy of a MATLAB X
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

#include "gb_matlab.h"

GrB_Matrix gb_get_deep      // return a deep GrB_Matrix copy of a MATLAB X
(
    const mxArray *X        // input MATLAB matrix (sparse or struct)
)
{ 

    GrB_Matrix S = gb_get_shallow (X) ;
    GxB_Format_Value fmt ;
    OK (GxB_Matrix_Option_get (S, GxB_FORMAT, &fmt)) ;
    GrB_Matrix A = gb_typecast (NULL, S, fmt, 0) ;
    OK (GrB_Matrix_free (&S)) ;
    return (A) ;
}

