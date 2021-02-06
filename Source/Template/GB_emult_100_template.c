//------------------------------------------------------------------------------
// GB_emult_100_template: C<M>= A.*B, M sparse/hyper, A and B bitmap/full
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2021, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

// C is sparse, with the same sparsity structure as M.

{

    //--------------------------------------------------------------------------
    // get M, A, B, and C
    //--------------------------------------------------------------------------

    const int8_t *GB_RESTRICT Ab = A->b ;
    const int8_t *GB_RESTRICT Bb = B->b ;

    const GB_ATYPE *GB_RESTRICT Ax = (GB_ATYPE *) A->x ;
    const GB_BTYPE *GB_RESTRICT Bx = (GB_BTYPE *) B->x ;

    const int64_t *GB_RESTRICT Mp = M->p ;
    const int64_t *GB_RESTRICT Mh = M->h ;
    const int64_t *GB_RESTRICT Mi = M->i ;
    const GB_void *GB_RESTRICT Mx = (GB_void *) ((Mask_struct) ? NULL : M->x) ;
    const int64_t vlen = M->vlen ;

    const int64_t  *GB_RESTRICT Cp = C->p ;
          int64_t  *GB_RESTRICT Ci = C->i ;
          GB_CTYPE *GB_RESTRICT Cx = (GB_CTYPE *) C->x ;

    //--------------------------------------------------------------------------
    // C<M>=A.*B where M is sparse/hyper, A and B are bitmap/full
    //--------------------------------------------------------------------------

    int tid ;
    #pragma omp parallel for num_threads(M_nthreads) schedule(dynamic,1)
    for (tid = 0 ; tid < M_ntasks ; tid++)
    {
        int64_t kfirst = kfirst_Mslice [tid] ;
        int64_t klast  = klast_Mslice  [tid] ;
        for (int64_t k = kfirst ; k <= klast ; k++)
        {
            int64_t j = GBH (Mh, k) ;
            int64_t pstart = j * vlen ;
            int64_t pM, pM_end, pC ;
            GB_get_pA_and_pC (&pM, &pM_end, &pC, tid, k, kfirst, klast,
                pstart_Mslice, Cp_kfirst, Cp, vlen, Mp, vlen) ;
            for ( ; pM < pM_end ; pM++)
            {
                int64_t i = Mi [pM] ;
                if (GB_mcast (Mx, pM, vlen) &&
                    (GBB (Ab, pstart + i)
                    &&  // TODO: for GB_add, use || instead
                    GBB (Bb, pstart + i)))
                {
                    int64_t p = pstart + i ;
                    // C (i,j) = A (i,j) .* B (i,j)
                    Ci [pC] = i ;
                    GB_GETA (aij, Ax, p) ;
                    GB_GETB (bij, Bx, p) ;
                    GB_BINOP (GB_CX (pC), aij, bij, i, j) ;
                    pC++ ;
                }
            }
        }
    }
}

