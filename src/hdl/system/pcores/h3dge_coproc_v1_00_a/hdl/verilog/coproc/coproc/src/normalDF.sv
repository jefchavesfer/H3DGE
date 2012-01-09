import definitions_pack::*;
import math_pack::*;
module normalDF
(
	input logic clock, input point trigV1, input point trigV2, input point trigV3, input logic sel1, input logic sel2, input sqrtReady, output vector normal, input integer of, input logic clockSqrt, input logic startSqrt, output logic readySqrt, output vector n1, output vector n2  
); 
    												   									   
	vector w_mux1_sub11, w_sub1_mux21; 
	vector w_vp1_mux22, w_rv1_vp11, w_rv1_vp12, w_mux2_sc1_md;		
	
	fixed w_sc1_sqrt1, w_sqrt1_dm, w_dm_nx, w_dm_ny, w_dm_nz, w_mux2_sc1_mdx, w_mux2_sc1_mdy, w_mux2_sc1_mdz;
	
	vector n_dm1, w_dm12_rv2, w_dm11_rv1, w_vp1_m2;
	
	mux mux1( .a( trigV2 ), .b( trigV3 ), .c( w_mux1_sub11 ), .sel( sel1 ) );
	
	assign w_sub1_mux21.x = w_mux1_sub11.x - trigV1.x;
	assign w_sub1_mux21.y = w_mux1_sub11.y - trigV1.y;
	assign w_sub1_mux21.z = w_mux1_sub11.z - trigV1.z;
	assign n1 = w_rv1_vp11; 
	assign n2 = w_rv1_vp12;
	
	mux mux2( .a( w_sub1_mux21 ), .b( w_vp1_mux22 ), .c( w_mux2_sc1_md ), .sel( sel2 ) );
	
	scalarProduct sc1( .a( w_mux2_sc1_md ), .b( w_mux2_sc1_md ), .c( w_sc1_sqrt1 ) );


	mSqrt sqrt1( .clock(clockSqrt), .start(startSqrt), .a( w_sc1_sqrt1 ), .b( w_sqrt1_dm ), .ready(readySqrt), .of(of) );
	
	assign w_mux2_sc1_mdx = w_mux2_sc1_md.x;
	assign w_mux2_sc1_mdy = w_mux2_sc1_md.y;
	assign w_mux2_sc1_mdz = w_mux2_sc1_md.z;
	
	mDiv md1( .a( w_mux2_sc1_mdx ), .b( w_sqrt1_dm ), .c( w_dm_nx ) );
	mDiv md2( .a( w_mux2_sc1_mdy ), .b( w_sqrt1_dm ), .c( w_dm_ny ) );
	mDiv md3( .a( w_mux2_sc1_mdz ), .b( w_sqrt1_dm ), .c( w_dm_nz ) );
	
	assign n_dm1.x = w_dm_nx;
	assign n_dm1.y = w_dm_ny;
    assign n_dm1.z = w_dm_nz;  
	
	assign normal = w_rv1_vp12;
	
	demuxVector dm1( .c( n_dm1 ), .a( w_dm11_rv1 ), .b ( w_dm12_rv2 ), .sel( sel1 ) );
	
	registerVector rv1( .clock( sqrtReady ), .a( w_dm11_rv1 ), .b( w_rv1_vp11 ) ); 
	registerVector rv2( .clock( sqrtReady ), .a( w_dm12_rv2 ), .b( w_rv1_vp12 ) ); 
	
	vectorialProduct vp1( .a( w_rv1_vp11 ), .b( w_rv1_vp12 ), .c( w_vp1_mux22 ) );
	

	
endmodule