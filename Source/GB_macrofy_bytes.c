//------------------------------------------------------------------------------
// GB_macrofy_bytes: create a single scalar from an array of bytes
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2023, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

// When the macro is used, sizeof (ztype) must be the same as nbytes

#include "GB.h"
#include "GB_stringify.h"

void GB_macrofy_bytes
(
    FILE *fp,               // file to write macros, assumed open already
    // input:
    const char *Name,       // all-upper-case name
    const char *variable,   // variable to declaer
    const char *type_name,  // name of the type
    const uint8_t *value,   // array of size nbytes
    size_t nbytes,
    bool is_identity        // true for the identity value
)
{

    // FIXME: figure out a way to get rid of memcpy

    fprintf (fp,
        "#define GB_DECLARE_%s(modifier,%s) \\\n"
        "    modifier %s %s ;               \\\n"
        "    {                                                       \\\n"
        "        const uint8_t bytes [%d] =                          \\\n"
        "        {                                                   \\\n"
        "            ",
        Name, variable, type_name, variable, (int) nbytes) ;

    bool same = (nbytes > 0) ;
    for (int k = 0 ; k < nbytes ; k++)
    {
        fprintf (fp, "0x%02x", (int) (value [k])) ;
        same = same && (value [0] == value [k]) ;
        if (k < nbytes-1)
        {
            fprintf (fp, ", ") ;
            if (k > 0 && k % 8 == 7) fprintf (fp, "\\\n            ") ;
        }
    }
    // finalize the array and use memcpy to initialize the scalar
    fprintf (fp,
        "  \\\n"
        "        } ;                                                 \\\n"
        "        memcpy (&%s, bytes, %d) ;                           \\\n"
        "    }\n",
        variable, (int) nbytes) ;

    if (same && is_identity)
    {
        // all the bytes of the identity value are the same
        fprintf (fp, "#define GB_HAS_IDENTITY_BYTE 1\n") ;
        fprintf (fp, "#define GB_IDENTITY_BYTE 0x%02x\n", (int) value [0]) ;
    }
}

