function test106
%TEST106 GxB_subassign with alias

rng 'default'
fprintf ('\ntest106: GxB_subassign with alias\n') ;

for m = [0 1 5 100]
    for n = [0 1 5 100]
        I1 = randperm (m) ;
        J1 = randperm (n) ;
        I0 = uint64 (I1) - 1 ;
        J0 = uint64 (J1) - 1 ;
        for d = [0 0.1 0.5 1]
            for is_csc = 0:1
                for is_hyper = 0:1

                    if (is_hyper)
                        hyper_ratio = 1 ;
                    else
                        hyper_ratio = 0 ;
                    end

                    A = GB_spec_random (m, n, d, 100, 'double', ...
                        is_csc, is_hyper, hyper_ratio) ;
                    B = GB_spec_random (m, n, d, 100, 'double', ...
                        is_csc, is_hyper, hyper_ratio) ;
                    C = GB_spec_random (m, n, d, 100, 'double', ...
                        is_csc, is_hyper, hyper_ratio) ;
                    M = GB_spec_random (m, n, d, 100, 'double', ...
                        is_csc, is_hyper, hyper_ratio) ;

save gunk C I0 J0 I1 J1 A B C M hyper_ratio is_csc is_hyper d

                    C1a = GB_mex_subassign  (C, [ ], [ ],  C,  I0, J0, [ ]) ;
                    C2  = GB_spec_subassign (C, [ ], [ ],  C,  I1, J1, [ ], 0) ;
                    GB_spec_compare (C1a, C2) ;
                    C1b = GB_mex_subassign  (C, [ ], [ ], 'C', I0, J0, [ ]) ;
                    GB_spec_compare (C1b, C2) ;

                    C1a = GB_mex_subassign  (C,  C,  [ ], A, I0, J0, [ ]) ;
                    C2  = GB_spec_subassign (C,  C,  [ ], A, I1, J1, [ ], 0) ;
                    GB_spec_compare (C1a, C2) ;
                    C1b = GB_mex_subassign  (C, 'C', [ ], A, I0, J0, [ ]) ;
                    GB_spec_compare (C1b, C2) ;

                end
            end
        end
    end
end

fprintf ('\ntest106: all tests passed\n') ;

