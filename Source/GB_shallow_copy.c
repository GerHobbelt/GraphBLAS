//------------------------------------------------------------------------------
// GB_shallow_copy: create a shallow copy of a matrix
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// Create a purely shallow copy of a matrix.  No typecasting is done.

// The CSR/CSC format of C and A can differ, but they have they same vlen and
// vdim.  This function is CSR/CSC agnostic, except that C_is_csc is used to
// set the C->is_csc state in C.

// No errors are checked except for out-of-memory conditions.  This function is
// not user-callable.  Shallow matrices are never passed back to the user.

// Compare this function with GB_shallow_op.c

#include "GB_transpose.h"

GrB_Info GB_shallow_copy    // create a purely shallow matrix
(
    GrB_Matrix *Chandle,    // output matrix C
    const bool C_is_csc,    // desired CSR/CSC format of C
    const GrB_Matrix A,     // input matrix
    GB_Context Context
)
{

    //--------------------------------------------------------------------------
    // check inputs
    //--------------------------------------------------------------------------

    ASSERT (Chandle != NULL) ;
    ASSERT_MATRIX_OK (A, "A for shallow copy", GB0) ;
    ASSERT (!GB_ZOMBIES (A)) ;
    ASSERT (!GB_JUMBLED (A)) ;
    ASSERT (!GB_PENDING (A)) ;

    ASSERT (!GB_IS_BITMAP (A)) ;        // TODO:BITMAP

    (*Chandle) = NULL ;

    //--------------------------------------------------------------------------
    // construct a shallow copy of A for the pattern of C
    //--------------------------------------------------------------------------

    // allocate the struct for C, but do not allocate C->h, C->p, C->i, or C->x.
    // C has the exact same hypersparsity as A.
    GrB_Info info ;
    GrB_Matrix C = NULL ;
    int sparsity = (A->h != NULL) ? GxB_HYPERSPARSE : GxB_SPARSE ;
    info = GB_new (&C, // sparse or hyper, new header
        A->type, A->vlen, A->vdim, GB_Ap_null, C_is_csc,
        sparsity, A->hyper_switch, 0, Context) ;
    if (info != GrB_SUCCESS)
    { 
        // out of memory
        return (info) ;
    }

    //--------------------------------------------------------------------------
    // make a shallow copy of the vector pointers
    //--------------------------------------------------------------------------

    ASSERT (C->magic == GB_MAGIC2) ;   // [ be careful; C not yet initialized
    C->p_shallow = true ;           // C->p not freed when freeing C
    C->h_shallow = true ;           // C->h not freed when freeing C
    C->p = A->p ;                   // C->p is of size A->plen + 1
    C->h = A->h ;                   // C->h is of size A->plen
    C->plen = A->plen ;             // C and A have the same hyperlist sizes
    C->nvec = A->nvec ;
    ASSERT (A->nvec_nonempty == -1 ||   // can be postponed
            A->nvec_nonempty == GB_nvec_nonempty (A, Context)) ;
    C->nvec_nonempty = A->nvec_nonempty ;
    C->magic = GB_MAGIC ;           // C is now initialized ]

    //--------------------------------------------------------------------------
    // check for empty matrix
    //--------------------------------------------------------------------------

    if (A->nzmax == 0)
    { 
        // C->p and C->h are shallow but the rest is empty
        C->nzmax = 0 ;
        C->i = NULL ;
        C->x = NULL ;
        C->i_shallow = false ;
        C->x_shallow = false ;
        ASSERT_MATRIX_OK (C, "C = quick copy of empty A", GB0) ;
        (*Chandle) = C ;
        return (GrB_SUCCESS) ;
    }

    //--------------------------------------------------------------------------
    // make a shallow copy of the pattern
    //--------------------------------------------------------------------------

    C->i = A->i ;               // of size A->nzmax
    C->i_shallow = true ;       // C->i will not be freed when freeing C

    //--------------------------------------------------------------------------
    // make a shallow copy of the values
    //--------------------------------------------------------------------------

    C->nzmax = A->nzmax ;
    C->x = A->x ;
    C->x_shallow = true ;       // C->x will not be freed when freeing C
    ASSERT_MATRIX_OK (C, "C = pure shallow (A)", GB0) ;
    (*Chandle) = C ;
    return (GrB_SUCCESS) ;
}

