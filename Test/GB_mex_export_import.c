//------------------------------------------------------------------------------
// GB_mex_export_import: export and then reimport a matrix
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// format:
//  0: standard CSR
//  1: standard CSC
//  3: hyper CSR
//  4: hyper CSC

#include "GB_mex.h"

#define USAGE "C = GB_mex_export_import (A, format_matrix, format_export)"

#define FREE_WORK                       \
{                                       \
    GB_FREE (Cp) ;                      \
    GB_FREE (Ch) ;                      \
    GB_FREE (Ci) ;                      \
    GB_FREE (Cx) ;                      \
    GB_FREE (Cb) ;                      \
    GrB_Matrix_free_(&C) ;              \
}

#define FREE_ALL                        \
{                                       \
    FREE_WORK ;                         \
    GrB_Matrix_free_(&A) ;              \
    GB_mx_put_global (true) ;           \
}

#define OK(method)                              \
{                                               \
    info = method ;                             \
    if (info != GrB_SUCCESS)                    \
    {                                           \
        FREE_WORK ;                             \
        return (info) ;                         \
    }                                           \
}

GrB_Matrix A = NULL ;
GrB_Matrix C = NULL ;
GrB_Index *Cp = NULL, *Ch = NULL, *Ci = NULL ;
GB_void *Cx = NULL ;
int8_t *Cb = NULL ;
GB_Context Context = NULL ;
size_t csize = 0 ;
GrB_Index nvec = 0, nvals = 0, nrows = 0, ncols = 0, nzmax = 0 ;
int64_t nonempty = -1 ;
bool jumbled = false ;
GrB_Type type = NULL, atype = NULL;
GrB_Info info = GrB_SUCCESS ;

GrB_Info export_import ( int format_matrix, int format_export) ;

//------------------------------------------------------------------------------

GrB_Info export_import
(
    int format_matrix,
    int format_export
)
{

    GxB_Matrix_type (&atype, A) ;
    GxB_Type_size (&csize, atype) ;

    OK (GrB_Matrix_dup (&C, A)) ;

    //--------------------------------------------------------------------------
    // convert C to the requested format
    //--------------------------------------------------------------------------

    switch (format_matrix)
    {

        //----------------------------------------------------------------------
        case 0 :    // standard CSR
        //----------------------------------------------------------------------

            OK (GxB_Matrix_Option_set_(C, GxB_HYPER_SWITCH, GxB_NEVER_HYPER)) ;
            OK (GxB_Matrix_Option_set_(C, GxB_SPARSITY, GxB_SPARSE)) ;
            OK (GxB_Matrix_Option_set_(C, GxB_FORMAT, GxB_BY_ROW)) ;
            break ;

        //----------------------------------------------------------------------
        case 1 :    // standard CSC
        //----------------------------------------------------------------------

            OK (GxB_Matrix_Option_set_(C, GxB_HYPER_SWITCH, GxB_NEVER_HYPER)) ;
            OK (GxB_Matrix_Option_set_(C, GxB_SPARSITY, GxB_SPARSE)) ;
            OK (GxB_Matrix_Option_set_(C, GxB_FORMAT, GxB_BY_COL)) ;
            break ;

        //----------------------------------------------------------------------
        case 2 :    // hypersparse CSR
        //----------------------------------------------------------------------

            OK (GxB_Matrix_Option_set_(C, GxB_HYPER_SWITCH, GxB_ALWAYS_HYPER)) ;
            OK (GxB_Matrix_Option_set_(C, GxB_SPARSITY, GxB_HYPERSPARSE)) ;
            OK (GxB_Matrix_Option_set_(C, GxB_FORMAT, GxB_BY_ROW)) ;
            break ;

        //----------------------------------------------------------------------
        case 3 :    // hypersparse CSC
        //----------------------------------------------------------------------

            OK (GxB_Matrix_Option_set_(C, GxB_HYPER_SWITCH, GxB_ALWAYS_HYPER)) ;
            OK (GxB_Matrix_Option_set_(C, GxB_SPARSITY, GxB_HYPERSPARSE)) ;
            OK (GxB_Matrix_Option_set_(C, GxB_FORMAT, GxB_BY_COL)) ;
            break ;

        //----------------------------------------------------------------------
        case 4 :    // bitmapR
        //----------------------------------------------------------------------

            OK (GxB_Matrix_Option_set_(C, GxB_SPARSITY, GxB_BITMAP)) ;
            OK (GxB_Matrix_Option_set_(C, GxB_FORMAT, GxB_BY_ROW)) ;
            break ;

        //----------------------------------------------------------------------
        case 5 :    // bitmapC
        //----------------------------------------------------------------------

            OK (GxB_Matrix_Option_set_(C, GxB_SPARSITY, GxB_BITMAP)) ;
            OK (GxB_Matrix_Option_set_(C, GxB_FORMAT, GxB_BY_COL)) ;
            break ;

        default : mexErrMsgTxt ("invalid format") ;
    }

    //--------------------------------------------------------------------------
    // export then import
    //--------------------------------------------------------------------------

    switch (format_export)
    {

        //----------------------------------------------------------------------
        case 0 :    // standard CSR
        //----------------------------------------------------------------------

            OK (GxB_Matrix_export_CSR (&C, &type, &nrows, &ncols, &nzmax,
                &jumbled, &nonempty, &Cp, &Ci, &Cx, NULL)) ;
            nvec = nrows ;

            OK (GxB_Matrix_import_CSR (&C, type, nrows, ncols, nzmax,
                jumbled, nonempty, &Cp, &Ci, &Cx, NULL)) ;
            break ;

        //----------------------------------------------------------------------
        case 1 :    // standard CSC
        //----------------------------------------------------------------------

            OK (GxB_Matrix_export_CSC (&C, &type, &nrows, &ncols, &nzmax,
                &jumbled, &nonempty, &Cp, &Ci, &Cx, NULL)) ;
            nvec = ncols ;

            OK (GxB_Matrix_import_CSC (&C, type, nrows, ncols, nzmax,
                jumbled, nonempty, &Cp, &Ci, &Cx, NULL)) ;
            break ;

        //----------------------------------------------------------------------
        case 2 :    // hypersparse CSR
        //----------------------------------------------------------------------

            OK (GxB_Matrix_export_HyperCSR (&C, &type, &nrows, &ncols, &nzmax,
                &jumbled, &nonempty, &nvec, &Ch, &Cp, &Ci, &Cx, NULL)) ;

            OK (GxB_Matrix_import_HyperCSR (&C, type, nrows, ncols, nzmax,
                jumbled, nonempty, nvec, &Ch, &Cp, &Ci, &Cx, NULL)) ;
            break ;

        //----------------------------------------------------------------------
        case 3 :    // hypersparse CSC
        //----------------------------------------------------------------------

            OK (GxB_Matrix_export_HyperCSC (&C, &type, &nrows, &ncols, &nzmax,
                &jumbled, &nonempty, &nvec, &Ch, &Cp, &Ci, &Cx, NULL)) ;

            OK (GxB_Matrix_import_HyperCSC (&C, type, nrows, ncols, nzmax,
                jumbled, nonempty, nvec, &Ch, &Cp, &Ci, &Cx, NULL)) ;
            break ;

        //----------------------------------------------------------------------
        case 4 :    // bitmapR
        //----------------------------------------------------------------------

            OK (GxB_Matrix_export_BitmapR (&C, &type, &nrows, &ncols, &nvals,
                &Cb, &Cx, NULL)) ;

            OK (GxB_Matrix_import_BitmapR (&C, type, nrows, ncols, nvals,
                &Cb, &Cx, NULL)) ;
            break ;

        //----------------------------------------------------------------------
        case 5 :    // bitmapC
        //----------------------------------------------------------------------

            OK (GxB_Matrix_export_BitmapC (&C, &type, &nrows, &ncols, &nvals,
                &Cb, &Cx, NULL)) ;

            OK (GxB_Matrix_import_BitmapC (&C, type, nrows, ncols, nvals,
                &Cb, &Cx, NULL)) ;
            break ;

        //----------------------------------------------------------------------
        // TODO: full R,C
        //----------------------------------------------------------------------

        default : mexErrMsgTxt ("invalid format") ;
    }

    return (GrB_SUCCESS) ;
}

//------------------------------------------------------------------------------

void mexFunction
(
    int nargout,
    mxArray *pargout [ ],
    int nargin,
    const mxArray *pargin [ ]
)
{

    bool malloc_debug = GB_mx_get_global (true) ;

    // check inputs
    if (nargout > 1 || nargin != 3)
    {
        mexErrMsgTxt ("Usage: " USAGE) ;
    }

    // get A (shallow copy)
    {
        A = GB_mx_mxArray_to_Matrix (pargin [0], "A input", false, true) ;
        if (A == NULL)
        {
            FREE_ALL ;
            mexErrMsgTxt ("A failed") ;
        }
    }

    // get matrix format (0 to 5)
    int GET_SCALAR (1, int, format_matrix, 0) ;

    // get export/import format (0 to 5)
    int GET_SCALAR (2, int, format_export, 0) ;

    #define GET_DEEP_COPY ;
    #define FREE_DEEP_COPY ;

    // convert matrix, export, then import
    METHOD (export_import (format_matrix, format_export)) ;

    // return C to MATLAB as a struct and free the GraphBLAS C
    pargout [0] = GB_mx_Matrix_to_mxArray (&C, "C output", true) ;

    FREE_ALL ;
}

