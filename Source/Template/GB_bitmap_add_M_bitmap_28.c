//------------------------------------------------------------------------------
// GB_bitmap_add_M_bitmap_28: C<#M>=A+B, C bitmap; M,A bitmap/full, B sp./hyper
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2023, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

// C is bitmap.
// M is bitmap or full, complemented or not, and either value or structural.
// A is bitmap/full, B is sparse/hyper.

{

    //------------------------------------------------------------------
    // Method28: C bitmap; M and A bitmap or full; B sparse or hyper
    //------------------------------------------------------------------

    int tid ;
    #pragma omp parallel for num_threads(C_nthreads) schedule(static) \
        reduction(+:cnvals)
    for (tid = 0 ; tid < C_nthreads ; tid++)
    {
        int64_t pstart, pend, task_cnvals = 0 ;
        GB_PARTITION (pstart, pend, cnz, tid, C_nthreads) ;
        for (int64_t p = pstart ; p < pend ; p++)
        {
            GB_GET_MIJ (p) ;
            if (mij)
            { 
                int8_t a = GBB_A (Ab, p) ;
                #ifndef GB_ISO_ADD
                if (a)
                {
                    #if GB_IS_EWISEUNION
                    { 
                        // C (i,j) = A(i,j) + beta
                        GB_LOAD_A (aij, Ax, p, A_iso) ;
                        GB_BINOP (GB_CX (p), aij, beta_scalar,
                            p % vlen, p / vlen) ;
                    }
                    #else
                    { 
                        // C (i,j) = A (i,j)
                        GB_COPY_A_TO_C (Cx, p, Ax, p, A_iso) ;
                    }
                    #endif
                }
                #endif
                Cb [p] = a ;
                task_cnvals += a ;
            }
            else
            { 
                Cb [p] = 0 ;
            }
        }
        cnvals += task_cnvals ;
    }

    GB_SLICE_MATRIX (B, 8, chunk) ;

    #pragma omp parallel for num_threads(B_nthreads) \
        schedule(dynamic,1) reduction(+:cnvals)
    for (taskid = 0 ; taskid < B_ntasks ; taskid++)
    {
        int64_t kfirst = kfirst_Bslice [taskid] ;
        int64_t klast  = klast_Bslice  [taskid] ;
        int64_t task_cnvals = 0 ;
        for (int64_t k = kfirst ; k <= klast ; k++)
        {
            // find the part of B(:,k) for this task
            int64_t j = GBH_B (Bh, k) ;
            int64_t pB_start, pB_end ;
            GB_get_pA (&pB_start, &pB_end, taskid, k, kfirst,
                klast, pstart_Bslice, Bp, vlen) ;
            int64_t pC_start = j * vlen ;
            // traverse over B(:,j), the kth vector of B
            for (int64_t pB = pB_start ; pB < pB_end ; pB++)
            {
                int64_t i = Bi [pB] ;
                int64_t p = pC_start + i ;
                GB_GET_MIJ (p) ;
                if (mij)
                {
                    int8_t c = Cb [p] ;
                    if (c == 1)
                    { 
                        // C (i,j) = A (i,j) + B (i,j)
                        #ifndef GB_ISO_ADD
                        GB_LOAD_A (aij, Ax, p , A_iso) ;
                        GB_LOAD_B (bij, Bx, pB, B_iso) ;
                        GB_BINOP (GB_CX (p), aij, bij, i, j) ;
                        #endif
                    }
                    else
                    { 
                        #ifndef GB_ISO_ADD
                        #if GB_IS_EWISEUNION
                        {
                            // C (i,j) = alpha + B(i,j)
                            GB_LOAD_B (bij, Bx, pB, B_iso) ;
                            GB_BINOP (GB_CX (p), alpha_scalar, bij,
                                i, j) ;
                        }
                        #else
                        { 
                            // C (i,j) = B (i,j)
                            GB_COPY_B_TO_C (Cx, p, Bx, pB, B_iso) ;
                        }
                        #endif
                        #endif
                        Cb [p] = 1 ;
                        task_cnvals++ ;
                    }
                }
            }
        }
        cnvals += task_cnvals ;
    }

}
