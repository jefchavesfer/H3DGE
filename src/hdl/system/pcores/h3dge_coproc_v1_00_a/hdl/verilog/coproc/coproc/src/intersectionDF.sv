import definitions_pack::*;
import math_pack::*;
module intersectionDF
(
	input logic clock, input vector rdir, input vector t1, input vector e2, input vector e1, input logic sel1, input logic sel2, /*input integer of,*/ output fixed det, output fixed u, output fixed v, output fixed t
); 
    
	vector w_mux1_vp11, w_mux2_vp12, w_mux5_mux32; 
	vector w_vp1_dm1;
	vector rp, rq;						  		
	vector w_mux3_sc21, w_mux4_sc22;
	
	fixed w_sc2_md11, w_md1_dm2, w_dm22_dm3, w_dm21_rf1, w_dm31_rf2, w_dm32_rf3;
	
	mux mux1( .a( rdir  ), .b( t1           ), .c( w_mux1_vp11  ), .sel( sel1 )/*, .of(of)*/ );	 
	mux mux2( .a( e2    ), .b( e1           ), .c( w_mux2_vp12  ), .sel( sel1 )/*, .of(of)*/ );
	mux mux3( .a( rp    ), .b( w_mux5_mux32 ), .c( w_mux3_sc21  ), .sel( sel1 )/*, .of(of)*/ );	   
	mux mux4( .a( t1    ), .b( rq           ), .c( w_mux4_sc22  ), .sel( sel1 )/*, .of(of)*/ );	 
	mux mux5( .a( rdir ), .b ( e2           ), .c( w_mux5_mux32 ), .sel( sel2 )/*, .of(of)*/ );
	
	vectorialProduct vp1( .a( w_mux1_vp11 ), .b( w_mux2_vp12 ), .c( w_vp1_dm1 ) );
	
	demuxVector dm1( .c( w_vp1_dm1 ), .a( rp ), .b ( rq ), .sel( sel1 ) );
	
	scalarProduct sc1( .a( e1          ), .b( rp          ), .c( det        ) /*, .of(of)*/ );
	scalarProduct sc2( .a( w_mux3_sc21 ), .b( w_mux4_sc22 ), .c( w_sc2_md11 )/*, .of(of)*/ );
	
	mDiv md1( .a( w_sc2_md11 ), .b( det ), .c( w_md1_dm2 ) /*, .of(of)*/ );
	
	demuxFixed dm2( .c( w_md1_dm2  ), .a( w_dm21_rf1 ), .b( w_dm22_dm3 ), .sel ( sel1 ) /*, .of(of)*/ );
	demuxFixed dm3( .c( w_dm22_dm3 ), .a( w_dm31_rf2 ), .b( w_dm32_rf3 ), .sel ( sel2 ) /*, .of(of)*/ );
	
	registerFixed rf1( .clock(clock & ~sel1 ) , .a( w_dm21_rf1 ), .b(u) );
	registerFixed rf2( .clock(clock & sel1 ) , .a( w_dm31_rf2 ), .b(v) );
	registerFixed rf3( .clock(clock & sel2 ) , .a( w_dm32_rf3 ), .b(t) );
endmodule