
{

    //------------------------------------------------------------------
    // Method31: C and A are full; B is bitmap
    //------------------------------------------------------------------

    #pragma omp parallel for num_threads(C_nthreads) schedule(static)
    for (p = 0 ; p < cnz ; p++)
    {
        if (Bb [p])
        { 
            // C (i,j) = A (i,j) + B (i,j)
            GB_LOAD_A (aij, Ax, p, A_iso) ;
            GB_LOAD_B (bij, Bx, p, B_iso) ;
            GB_BINOP (GB_CX (p), aij, bij, p % vlen, p / vlen) ;
        }
        else
        { 
            #if GB_IS_EWISEUNION
            { 
                // C (i,j) = A(i,j) + beta
                GB_LOAD_A (aij, Ax, p, A_iso) ;
                GB_BINOP (GB_CX (p), aij, beta_scalar,
                    p % vlen, p / vlen);
            }
            #else
            { 
                // C (i,j) = A (i,j)
                GB_COPY_A_TO_C (Cx, p, Ax, p, A_iso) ;
            }
            #endif
        }
    }
}

