//------------------------------------------------------------------------------
// GB_macrofy_subref: construct all macros for subref methods
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2024, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

#include "GB.h"
#include "jitifyer/GB_stringify.h"

void GB_macrofy_subref          // construct all macros for GrB_extract
(
    // output:
    FILE *fp,                   // target file to write, already open
    // input:
    uint64_t scode,
    GrB_Type ctype
)
{

    //--------------------------------------------------------------------------
    // extract the subref scode
    //--------------------------------------------------------------------------

    // need_qsort, I_has_duplicates (1 hex digit)
    int ihasdupl    = GB_RSHIFT (scode, 13, 1) ;
    int needqsort   = GB_RSHIFT (scode, 12, 1) ;

    // Ikind, Jkind (1 hex digit)
    int Ikind       = GB_RSHIFT (scode, 10, 2) ;
    int Jkind       = GB_RSHIFT (scode,  8, 2) ;

    // type of C and A (1 hex digit)
    int ccode       = GB_RSHIFT (scode,  4, 4) ;

    // sparsity structures of C and A (1 hex digit)
    int csparsity   = GB_RSHIFT (scode,  2, 2) ;
    int asparsity   = GB_RSHIFT (scode,  0, 2) ;

    //--------------------------------------------------------------------------
    // describe the subref
    //--------------------------------------------------------------------------

    fprintf (fp, "// subref: C=A(I,J) where C and A are %s\n",
        (asparsity <= 1) ? "sparse/hypersparse" : "bitmap/full") ;

    fprintf (fp, "#define GB_I_KIND ") ;
    switch (Ikind)
    {
        case GB_ALL    : fprintf (fp, "GB_ALL\n"    ) ; break ;
        case GB_RANGE  : fprintf (fp, "GB_RANGE\n"  ) ; break ;
        case GB_STRIDE : fprintf (fp, "GB_STRIDE\n" ) ; break ;
        case GB_LIST   : fprintf (fp, "GB_LIST\n"   ) ; break ;
        default:;
    }
    if (asparsity <= 1)
    { 
        // C and A are sparse/hypersparse
        // Jkind not needed for sparse subsref
        fprintf (fp, "#define GB_NEED_QSORT %d\n", needqsort) ;
        fprintf (fp, "#define GB_I_HAS_DUPLICATES %d\n", ihasdupl) ;
    }
    else
    { 
        // C and A are bitmap/full
        // need_qsort, I_has_duplicates not needed for bitmap subsref
        switch (Jkind)
        {
            case GB_ALL    : fprintf (fp, "GB_ALL\n"    ) ; break ;
            case GB_RANGE  : fprintf (fp, "GB_RANGE\n"  ) ; break ;
            case GB_STRIDE : fprintf (fp, "GB_STRIDE\n" ) ; break ;
            case GB_LIST   : fprintf (fp, "GB_LIST\n"   ) ; break ;
            default:;
        }
    }

    //--------------------------------------------------------------------------
    // construct the typedefs
    //--------------------------------------------------------------------------

    GB_macrofy_typedefs (fp, ctype, NULL, NULL, NULL, NULL, NULL) ;

    //--------------------------------------------------------------------------
    // construct the macros for C and A
    //--------------------------------------------------------------------------

    GB_macrofy_sparsity (fp, "C", csparsity) ;
    GB_macrofy_nvals (fp, "C", csparsity, false) ;
    GB_macrofy_type (fp, "C", "_", ctype->name) ;

    GB_macrofy_sparsity (fp, "A", asparsity) ;
    GB_macrofy_nvals (fp, "A", asparsity, false) ;

    //--------------------------------------------------------------------------
    // construct the qsort macros
    //--------------------------------------------------------------------------

    fprintf (fp, "\n// qsort:\n#define GB_QSORT_1B(Ci,Cx,pC,clen) \\\n    ") ;

    switch (ctype->size)
    {
        case GB_1BYTE : 
            fprintf (fp, "GB_qsort_1b_size1 (Ci+pC, Cx+pC, clen)\n"
                "#define GB_GET_QSORT_CALLBACK "
                "GB_GET_CALLBACK (GB_qsort_1b_size1)\n") ;
            break ;

        case GB_2BYTE : 
            fprintf (fp, "GB_qsort_1b_size2 (Ci+pC, Cx+pC, clen)\n"
                "#define GB_GET_QSORT_CALLBACK "
                "GB_GET_CALLBACK (GB_qsort_1b_size2)\n") ;
            break ;

        case GB_4BYTE : 
            fprintf (fp, "GB_qsort_1b_size4 (Ci+pC, Cx+pC, clen)\n"
                "#define GB_GET_QSORT_CALLBACK "
                "GB_GET_CALLBACK (GB_qsort_1b_size2)\n") ;
            break ;

        case GB_8BYTE : 
            fprintf (fp, "GB_qsort_1b_size8 (Ci+pC, Cx+pC, clen)\n"
                "#define GB_GET_QSORT_CALLBACK "
                "GB_GET_CALLBACK (GB_qsort_1b_size8)\n") ;
            break ;

        case GB_16BYTE  : 
            fprintf (fp, "GB_qsort_1b_size16 (Ci+pC, Cx+pC, clen)\n"
                "#define GB_GET_QSORT_CALLBACK "
                "GB_GET_CALLBACK (GB_qsort_1b_size16)\n") ;
            break ;

        default : 
            fprintf (fp,    
                "GB_qsort_1b (Ci+pC, Cx+pC, sizeof (GB_C_TYPE), clen)\n"
                "#define GB_GET_QSORT_CALLBACK "
                "GB_GET_CALLBACK (GB_qsort_1b)\n") ;
            break ;
    }

    //--------------------------------------------------------------------------
    // include the final default definitions
    //--------------------------------------------------------------------------

    fprintf (fp, "\n#include \"include/GB_kernel_shared_definitions.h\"\n") ;
}

