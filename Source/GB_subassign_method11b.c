//------------------------------------------------------------------------------
// GB_subassign_method11b: C(I,J)<!M> = scalar ; using S
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2019, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// Method 11b: C(I,J)<!M> = scalar ; using S

// M:           present
// Mask_comp:   true
// C_replace:   false
// accum:       NULL
// A:           scalar
// S:           constructed

#include "GB_subassign.h"

GrB_Info GB_subassign_method11b
(
    GrB_Matrix C,
    // input:
    const GrB_Index *I,
    const int64_t nI,
    const int Ikind,
    const int64_t Icolon [3],
    const GrB_Index *J,
    const int64_t nJ,
    const int Jkind,
    const int64_t Jcolon [3],
    const GrB_Matrix M,
    const void *scalar,
    const GrB_Type atype,
    const GrB_Matrix S,
    GB_Context Context
)
{

    //--------------------------------------------------------------------------
    // get inputs
    //--------------------------------------------------------------------------

    GB_GET_C ;
    GB_GET_MASK ;
    GB_GET_SCALAR ;
    GB_GET_S ;
    GrB_BinaryOp accum = NULL ;

    //--------------------------------------------------------------------------
    // Method 11b: C(I,J)<!M> = scalar ; using S
    //--------------------------------------------------------------------------

    // Time: Close to optimal; must visit all IxJ, so Omega(|I|*|J|) is
    // required.  The sparsity of !M cannot be exploited.

    // Method 11b and Method 12b are very similar.
    // Method 11a and Method 11b are very similar.

    //--------------------------------------------------------------------------
    // Parallel: all IxJ (Methods 3, 4, 7, 8, 11a, 11b, 12a, 12b)
    //--------------------------------------------------------------------------

    // The only difference with Method 3 is that the pre-analysis needed not
    // slice the vectors of C for fine tasks.  Each task looks up its own part
    // of S and M, but this does not affect the construction of parallel tasks.

    GB_SUBASSIGN_IXJ_SLICE (NULL) ;

    // Each task must also look up its part of S and M, but this does not
    // affect the parallel tasks.  Total work is about the same as Method 3.

    #pragma omp parallel for num_threads(nthreads) schedule(dynamic,1) \
        reduction(+:nzombies) reduction(&&:ok)
    for (int taskid = 0 ; taskid < ntasks ; taskid++)
    {

        //----------------------------------------------------------------------
        // get the task descriptor
        //----------------------------------------------------------------------

        GB_GET_IXJ_TASK_DESCRIPTOR ;

        //----------------------------------------------------------------------
        // compute all vectors in this task
        //----------------------------------------------------------------------

        for (int64_t j = kfirst ; task_ok && j <= klast ; j++)
        {

            //------------------------------------------------------------------
            // get jC, the corresponding vector of C
            //------------------------------------------------------------------

            GB_GET_jC ;

            //------------------------------------------------------------------
            // get S(iA_start:end,j) and M(iA_start:end,j)
            //------------------------------------------------------------------

            GB_GET_VECTOR_FOR_IXJ (S) ;
            GB_GET_VECTOR_FOR_IXJ (M) ;

            //------------------------------------------------------------------
            // C(I(iA_start,iA_end-1),jC)<!M,repl> = scalar
            //------------------------------------------------------------------

            for (int64_t iA = iA_start ; iA < iA_end ; iA++)
            {

                //--------------------------------------------------------------
                // Get the indices at the top of each list.
                //--------------------------------------------------------------

                int64_t iS = (pS < pS_end) ? Si [pS] : INT64_MAX ;
                int64_t iM = (pM < pM_end) ? Mi [pM] : INT64_MAX ;

                //--------------------------------------------------------------
                // find the smallest index of [iS iA iM] (always iA)
                //--------------------------------------------------------------

                int64_t i = iA ;

                //--------------------------------------------------------------
                // get M(i,j)
                //--------------------------------------------------------------

                bool mij ;
                if (i == iM)
                { 
                    // mij = (bool) M [pM]
                    cast_M (&mij, Mx +(pM*msize), 0) ;
                    GB_NEXT (M) ;
                }
                else
                { 
                    // mij not present, implicitly false
                    ASSERT (i < iM) ;
                    mij = false ;
                }

                // complement the mask entry mij since Mask_comp is true
                mij = !mij ;

                //--------------------------------------------------------------
                // assign the entry
                //--------------------------------------------------------------

                if (i == iS)
                {
                    ASSERT (i == iA) ;
                    {
                        // both S (i,j) and A (i,j) present
                        if (mij)
                        { 
                            // ----[C A 1] or [X A 1]---------------------------
                            // [C A 1]: action: ( =A ): copy A, no accum
                            // [X A 1]: action: ( undelete ): zombie lives
                            GB_C_S_LOOKUP ;
                            GB_noaccum_C_A_1_scalar ;
                        }
                        GB_NEXT (S) ;
                    }
                }
                else
                {
                    ASSERT (i == iA) ;
                    {
                        // S (i,j) is not present, A (i,j) is present
                        if (mij)
                        { 
                            // ----[. A 1]--------------------------------------
                            // [. A 1]: action: ( insert )
                            // iC = I [iA] ; or I is a colon expression
                            int64_t iC = GB_ijlist (I, iA, Ikind, Icolon) ;
                            GB_D_A_1_scalar ;
                        }
                    }
                }
            }
        }

        //----------------------------------------------------------------------
        // log the result of this task
        //----------------------------------------------------------------------

        ok = ok && task_ok ;
    }

    //--------------------------------------------------------------------------
    // finalize the matrix and return result
    //--------------------------------------------------------------------------

    GB_SUBASSIGN_WRAPUP ;
}

