function codegen_sel_method (opname, func, atype)
%CODEGEN_SEL_METHOD create a selection function, C = select (A,thunk)

% SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2022, All Rights Reserved.
% SPDX-License-Identifier: Apache-2.0

if (nargin < 4)
    kind = [ ] ;
end

is_nonzombie_selector = isequal (opname, 'nonzombie') ;
C_iso = isequal (opname, 'eq_thunk') ;

f = fopen ('control.m4', 'w') ;
fprintf (f, 'm4_divert(-1)\n') ;

[aname, ~, ~] = codegen_type (atype) ;

name = sprintf ('%s_%s', opname, aname) ;

if (test_contains (opname, 'col'))
    % only bitmap selector is used
    enable_phase1 = false ;
    enable_phase2 = false ;
else
    enable_phase1 = (~is_nonzombie_selector) ;
    enable_phase2 = true ;
end

% function names
if (enable_phase1)
    fprintf (f, 'm4_define(`_sel_phase1'', `_sel_phase1__%s'')\n', name) ;
else
    fprintf (f, 'm4_define(`_sel_phase1'', `_sel_phase1__(none)'')\n') ;
end
if (enable_phase2)
    fprintf (f, 'm4_define(`_sel_phase2'', `_sel_phase2__%s'')\n', name) ;
else
    fprintf (f, 'm4_define(`_sel_phase2'', `_sel_phase2__(none)'')\n', name) ;
end

if isequal (opname, 'nonzombie')
    % no bitmap selectors for nonzombie selectors
    fprintf (f, 'm4_define(`_sel_bitmap'', `_sel_bitmap__(none)'')\n') ;
    fprintf (f, 'm4_define(`if_bitmap'', `-1'')\n') ;
else
    fprintf (f, 'm4_define(`_sel_bitmap'', `_sel_bitmap__%s'')\n', name) ;
    fprintf (f, 'm4_define(`if_bitmap'', `0'')\n') ;
end

% the type of A (no typecasting)
fprintf (f, 'm4_define(`GB_atype'', `%s'')\n', atype) ;

% create the operator to test the numerical values of the entries
fprintf (f, 'm4_define(`GB_test_value_of_entry'', `%s'')\n', func) ;

% enable phase1
if (enable_phase1)
    % enable phase1
    fprintf (f, 'm4_define(`if_phase1'', `0'')\n') ;
else
    % disable phase1
    fprintf (f, 'm4_define(`if_phase1'', `-1'')\n') ;
end

% enable phase2
if (enable_phase2)
    % enable phase2
    fprintf (f, 'm4_define(`if_phase2'', `0'')\n') ;
else
    % disable phase2
    fprintf (f, 'm4_define(`if_phase2'', `-1'')\n') ;
end

% for phase2: copy the numerical value of the entry
if (C_iso)
    % create C as iso for all EQ_THUNK ops even when A is not iso, with iso value athunk
    fprintf (f, 'm4_define(`GB_select_entry'', `/* assignment skipped, C is iso with all entries equal to thunk */'')\n') ;
else
    fprintf (f, 'm4_define(`GB_select_entry'', `Cx [pC] = Ax [pA]'')\n') ;
end

fprintf (f, 'm4_divert(0)\n') ;
fclose (f) ;

% construct the *.c file
cmd = sprintf ('cat control.m4 Generator/GB_sel.c | m4 -P | awk -f codegen_blank.awk > Generated2/GB_sel__%s.c', name) ;
fprintf ('.') ;
system (cmd) ;

% append to the *.h file
system ('cat control.m4 Generator/GB_sel.h | m4 -P | awk -f codegen_blank.awk | grep -v SPDX >> Generated2/GB_sel__include.h') ;

delete ('control.m4') ;


