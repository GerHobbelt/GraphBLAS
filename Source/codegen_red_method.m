function codegen_red_method (opname, func, atype, identity, terminal)
%CODEGEN_RED_METHOD create a reductionfunction
%
% codegen_red_method (opname, func, atype, identity, terminal)

f = fopen ('control.m4', 'w') ;

[aname, unsigned, bits] = codegen_type (atype) ;

name = sprintf ('%s_%s', opname, aname) ;

% function names
fprintf (f, 'define(`GB_bild'', `GB_bild__%s'')\n', name) ;

% type (no typecasting)
fprintf (f, 'define(`GB_atype'', `%s'')\n', atype) ;

if (~isempty (identity))
    fprintf (f, 'define(`GB_red_scalar'', `GB_red_scalar__%s'')\n', name) ;
    % identity and terminal values for the monoid
    fprintf (f, 'define(`GB_identity'', `%s'')\n', identity) ;
    fprintf (f, 'define(`if_is_monoid'', `'')\n') ;
    fprintf (f, 'define(`endif_is_monoid'', `'')\n') ;
else
    fprintf (f, 'define(`GB_red_scalar'', `GB_red_scalar__(none)'')\n') ;
    % first and second operators are not monoids (GB_bild only)
    fprintf (f, 'define(`GB_identity'', `(none)'')\n') ;
    fprintf (f, 'define(`if_is_monoid'', `#if 0'')\n') ;
    fprintf (f, 'define(`endif_is_monoid'', `#endif'')\n') ;
end

if (~isempty (terminal))
    fprintf (f, 'define(`GB_terminal'', `if (s == %s) break ;'')\n', ...
        terminal) ;
else
    fprintf (f, 'define(`GB_terminal'', `;'')\n') ;
end

% create the operator
func = strrep (func, 'zarg', '`$1''') ;
func = strrep (func, 'yarg', '`$2''') ;
fprintf (f, 'define(`GB_REDUCE_OP'', `%s'')\n', func) ;

fclose (f) ;

% construct the *.c file
cmd = sprintf (...
'cat control.m4 Generator/GB_red.c | m4 | tail -n +9 > Generated/GB_red__%s.c', ...
name) ;
fprintf ('.') ;
system (cmd) ;

% append to the *.h file
cmd = sprintf (...
'cat control.m4 Generator/GB_red.h | m4 | tail -n +9 >> Generated/GB_red__include.h') ;
system (cmd) ;

delete ('control.m4') ;

