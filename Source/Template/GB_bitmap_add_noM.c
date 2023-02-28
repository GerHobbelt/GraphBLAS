//------------------------------------------------------------------------------
// GB_bitmap_add_noM: C=A+B, C bitmap, A or B bitmap
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2023, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

// C is bitmap.
// A or B is bitmap (or both).  Neither A nor B are full.

{

    //----------------------------------------------------------------------
    // M is not present
    //----------------------------------------------------------------------

    //      ------------------------------------------
    //      C       =           A       +       B
    //      ------------------------------------------
    //      bitmap  .           sparse          bitmap
    //      bitmap  .           bitmap          sparse
    //      bitmap  .           bitmap          bitmap

    ASSERT (A_is_bitmap || B_is_bitmap) ;
    ASSERT (!A_is_full) ;
    ASSERT (!B_is_full) ;

    #ifdef GB_JIT_KERNEL
    {
        #if (GB_A_IS_BITMAP && GB_B_IS_BITMAP)
        {
            #include "GB_bitmap_add_noM_21.c"
        }
        #elif (GB_A_IS_BITMAP)
        {
            #include "GB_bitmap_add_noM_22.c"
        }
        #else
        {
            #include "GB_bitmap_add_noM_23.c"
        }
        #endif
    }
    #else
    {
        if (A_is_bitmap && B_is_bitmap)
        {
            #include "GB_bitmap_add_noM_21.c"
        }
        else if (A_is_bitmap)
        {
            #include "GB_bitmap_add_noM_22.c"
        }
        else
        {
            #include "GB_bitmap_add_noM_23.c"
        }
    }
    #endif
}

