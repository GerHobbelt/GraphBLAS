//------------------------------------------------------------------------------
// GB_assoc_template.c: switch factory for associative operators
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2019, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

// TODO: write Generator/GB_assoc.c, and use it to create 66 files:
// Generator/GB_assoc__[operator]_[type].  Each file has 4 functions for the
// kernels for GB_reduce_to_scalar, GB_build_factory, and GB_reduce_to_column
// (2 kernels).  (but 22 files with FIRST and SECOND just have one kernel for
// GB_build_factory).

// This is a generic body of code for creating hard-coded versions of code for
// 44 combinations of associative operators and built-in types: 10 types (all
// but boolean) with min, max, plus, and times, and one type (boolean) with
// "or", "and" "xor" and "eq"

// If GB_INCLUDE_SECOND_OPERATOR is defined then an additional 11 built-in
// workers for the SECOND operator are also created, and 11 for FIRST, for
// GB_build_factory.

// parallel: not here

if (typecode != GB_BOOL_code)
{

    //--------------------------------------------------------------------------
    // non-boolean case
    //--------------------------------------------------------------------------

    switch (opcode)
    {
        case GB_MIN_opcode   :

            switch (typecode)
            {
                #define GB_DUP(w,t) w = GB_IMIN (w,t)
                case GB_INT8_code   : GB_WORKER (int8_t)   ; break ;
                case GB_UINT8_code  : GB_WORKER (uint8_t)  ; break ;
                case GB_INT16_code  : GB_WORKER (int16_t)  ; break ;
                case GB_UINT16_code : GB_WORKER (uint16_t) ; break ;
                case GB_INT32_code  : GB_WORKER (int32_t)  ; break ;
                case GB_UINT32_code : GB_WORKER (uint32_t) ; break ;
                case GB_INT64_code  : GB_WORKER (int64_t)  ; break ;
                case GB_UINT64_code : GB_WORKER (uint64_t) ; break ;
                #undef  GB_DUP
                #define GB_DUP(w,t) w = GB_FMIN (w,t)
                case GB_FP32_code   : GB_WORKER (float)    ; break ;
                case GB_FP64_code   : GB_WORKER (double)   ; break ;
                #undef  GB_DUP
                default: ;
            }
            break ;

        case GB_MAX_opcode   :

            switch (typecode)
            {
                #define GB_DUP(w,t) w = GB_IMAX (w,t)
                case GB_INT8_code   : GB_WORKER (int8_t)   ; break ;
                case GB_UINT8_code  : GB_WORKER (uint8_t)  ; break ;
                case GB_INT16_code  : GB_WORKER (int16_t)  ; break ;
                case GB_UINT16_code : GB_WORKER (uint16_t) ; break ;
                case GB_INT32_code  : GB_WORKER (int32_t)  ; break ;
                case GB_UINT32_code : GB_WORKER (uint32_t) ; break ;
                case GB_INT64_code  : GB_WORKER (int64_t)  ; break ;
                case GB_UINT64_code : GB_WORKER (uint64_t) ; break ;
                #undef  GB_DUP
                #define GB_DUP(w,t) w = GB_FMAX (w,t)
                case GB_FP32_code   : GB_WORKER (float)    ; break ;
                case GB_FP64_code   : GB_WORKER (double)   ; break ;
                #undef  GB_DUP
                default: ;
            }
            break ;

        case GB_PLUS_opcode  :

            #define GB_DUP(w,t) w += t
            switch (typecode)
            {
                case GB_INT8_code   : GB_WORKER (int8_t)   ; break ;
                case GB_UINT8_code  : GB_WORKER (uint8_t)  ; break ;
                case GB_INT16_code  : GB_WORKER (int16_t)  ; break ;
                case GB_UINT16_code : GB_WORKER (uint16_t) ; break ;
                case GB_INT32_code  : GB_WORKER (int32_t)  ; break ;
                case GB_UINT32_code : GB_WORKER (uint32_t) ; break ;
                case GB_INT64_code  : GB_WORKER (int64_t)  ; break ;
                case GB_UINT64_code : GB_WORKER (uint64_t) ; break ;
                case GB_FP32_code   : GB_WORKER (float)    ; break ;
                case GB_FP64_code   : GB_WORKER (double)   ; break ;
                default: ;
            }
            break ;
            #undef  GB_DUP

        case GB_TIMES_opcode :

            #define GB_DUP(w,t) w *= t
            switch (typecode)
            {
                case GB_INT8_code   : GB_WORKER (int8_t)   ; break ;
                case GB_UINT8_code  : GB_WORKER (uint8_t)  ; break ;
                case GB_INT16_code  : GB_WORKER (int16_t)  ; break ;
                case GB_UINT16_code : GB_WORKER (uint16_t) ; break ;
                case GB_INT32_code  : GB_WORKER (int32_t)  ; break ;
                case GB_UINT32_code : GB_WORKER (uint32_t) ; break ;
                case GB_INT64_code  : GB_WORKER (int64_t)  ; break ;
                case GB_UINT64_code : GB_WORKER (uint64_t) ; break ;
                case GB_FP32_code   : GB_WORKER (float)    ; break ;
                case GB_FP64_code   : GB_WORKER (double)   ; break ;
                default: ;
            }
            break ;
            #undef  GB_DUP

        //----------------------------------------------------------------------
        // FIRST and SECOND for GB_build_factory
        //----------------------------------------------------------------------

        #ifdef GB_INCLUDE_SECOND_OPERATOR

        case GB_FIRST_opcode :

            #define GB_DUP(w,t) ;      // do nothing; keep the first tuple
            switch (typecode)
            {
                case GB_INT8_code   : GB_WORKER (int8_t)   ; break ;
                case GB_UINT8_code  : GB_WORKER (uint8_t)  ; break ;
                case GB_INT16_code  : GB_WORKER (int16_t)  ; break ;
                case GB_UINT16_code : GB_WORKER (uint16_t) ; break ;
                case GB_INT32_code  : GB_WORKER (int32_t)  ; break ;
                case GB_UINT32_code : GB_WORKER (uint32_t) ; break ;
                case GB_INT64_code  : GB_WORKER (int64_t)  ; break ;
                case GB_UINT64_code : GB_WORKER (uint64_t) ; break ;
                case GB_FP32_code   : GB_WORKER (float)    ; break ;
                case GB_FP64_code   : GB_WORKER (double)   ; break ;
                default: ;
            }
            break ;
            #undef  GB_DUP

        case GB_SECOND_opcode :

            #define GB_DUP(w,t) w = t  // replace with the 2nd tuple
            switch (typecode)
            {
                case GB_INT8_code   : GB_WORKER (int8_t)   ; break ;
                case GB_UINT8_code  : GB_WORKER (uint8_t)  ; break ;
                case GB_INT16_code  : GB_WORKER (int16_t)  ; break ;
                case GB_UINT16_code : GB_WORKER (uint16_t) ; break ;
                case GB_INT32_code  : GB_WORKER (int32_t)  ; break ;
                case GB_UINT32_code : GB_WORKER (uint32_t) ; break ;
                case GB_INT64_code  : GB_WORKER (int64_t)  ; break ;
                case GB_UINT64_code : GB_WORKER (uint64_t) ; break ;
                case GB_FP32_code   : GB_WORKER (float)    ; break ;
                case GB_FP64_code   : GB_WORKER (double)   ; break ;
                default: ;
            }
            break ;
            #undef  GB_DUP

        #endif

        default: ;
    }

}
else
{

    //--------------------------------------------------------------------------
    // boolean case: rename the opcode as needed
    //--------------------------------------------------------------------------

    // non-associatve ops not handled here: *GT, *LT, *GE, *LE.

    // The FIRST and SECOND operators are not associative, but are added for
    // GB_build_factory.  Since FIRST == DIV, the operator is handled too.

    switch (GB_boolean_rename (opcode))
    {

        case GB_LOR_opcode : 

            // OR == MAX == PLUS
            #define GB_DUP(w,t) w = (w || t)
            GB_WORKER (bool) ;
            break ;
            #undef  GB_DUP

        case GB_LAND_opcode : 

            // AND == MIN == TIMES
            #define GB_DUP(w,t) w = (w && t)
            GB_WORKER (bool) ;
            break ;
            #undef  GB_DUP

        case GB_LXOR_opcode : 

            // XOR == NE == MINUS == ISNE
            #define GB_DUP(w,t) w = (w != t)
            GB_WORKER (bool) ;
            break ;
            #undef  GB_DUP

        case GB_EQ_opcode : 

            // EQ == ISEQ
            #define GB_DUP(w,t) w = (w == t)
            GB_WORKER (bool) ;
            break ;
            #undef  GB_DUP

        //----------------------------------------------------------------------
        // FIRST and SECOND for GB_build_factory
        //----------------------------------------------------------------------

        #ifdef GB_INCLUDE_SECOND_OPERATOR

        case GB_FIRST_opcode : 

            // FIRST == DIV
            #define GB_DUP(w,t) ;      // do nothing; keep the first tuple
            GB_WORKER (bool) ;
            break ;
            #undef  GB_DUP

        case GB_SECOND_opcode : 

            // SECOND
            #define GB_DUP(w,t) w = t  // replace with the 2nd tuple
            GB_WORKER (bool) ;
            break ;
            #undef  GB_DUP

        #endif

        default: ;
    }
}

