function C = uint8 (G)
%UINT8 cast a GraphBLAS matrix to MATLAB full uint8 matrix.
% C = uint8 (G) typecasts the GrB matrix G to a MATLAB full uint8 matrix.
% The result C is full since MATLAB does not support sparse uint8
% matrices.
%
% To typecast the matrix G to a GraphBLAS sparse uint8 matrix instead,
% use C = GrB (G, 'uint8').
%
% See also GrB, GrB/double, GrB/complex, GrB/single, GrB/logical, GrB/int8,
% GrB/int16, GrB/int32, GrB/int64, GrB/uint16, GrB/uint32, GrB/uint64.

% SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2020, All Rights Reserved.
% SPDX-License-Identifier: Apache-2.0

G = G.opaque ;
desc.kind = 'full' ;
C = gbfull (G, 'uint8', uint8 (0), desc) ;

