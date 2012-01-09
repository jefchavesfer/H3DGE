import definitions_pack::*;
import light_pack::*;
module intersectionDF
(
    input logic clock, input vector rdir, input vector t1, input vector e2, input vector e1, input logic sel1, input logic sel2, output fixed det, output fixed u, output fixed v, output fixed t); 
    
	vector w_mux_vp11, w_mux_vp12, w_vp1_dm1;
	vector w_mux5_mux32; 
	vector w_mux3_sc21, w_mux4_sc22;
	
	vector p, q;  
	fixed det;
	wire sel1;
    vector e1;	
	
	mux mux1( .a(r.dir), .b(t1          ), .c(w_mux_vp11)   , .sel(sel1 ) );
	mux mux2( .a(e2   ), .b(e1          ), .c(w_mux_vp12)   , .sel(sel1 ) );
	mux mux3( .a(p    ), .b(w_mux5_mux32), .c( w_mux3_sc21 ), .sel(sel1) );	   
	mux mux4( .a(t1   ), .b(q           ), .c( w_mux4_sc22 ), .sel(sel1) );	 
	mux mux5( .a(r.dir), .b(e2          ), .c( w_mux5_mux32), .sel(sel2) );
	
	vectorialProduct vp1( .a(w_mux_vp11), .b(w_mux_vp12), .c(w_vp_dm) );
	
	demux dm1( .c( w_vp_dm ), .a( w_dm_regp ), .b ( w_dm_regq ), .sel( sel1 );
	
	scalarProduct sc1( .a(p            ), .b( e1          ), .c( det)       );
	scalarProduct sc2( .a( w_mux3_sc21 ), .b( w_mux4_sc22 ), .c( w_sc2_div1 );
	
	mDiv md1( .a( w_sc2_md11 ), .b( det ), .c( w_md1_dm2 ) );
	
	demux dm2( .c( w_md1_dm2  ), .a( u ), .b( w_dm22_dm3 ), .sel (wsel ) );
	demux dm3( .c( w_dm22, dm3), .a( v ), .b( t          ), .sel (wsel2) );
	
	mux2( .a( p ), .b( e1 ) ); 
	 										 
	
	scalarProduct sp1 ( a.( w_vp1_mm11 ), .b( w_vp1_mm12 ), .c( w_vp1_mm13 ) );
	
	dm1( .a(resultadodoescalar), .b(resultadoprodescalar), .c(u);
	
	logic state[1:0];
	always(  @ (posedge clk) )
    begin
	    if (start) 
		begin 
			state = 0;		
		end
		
		e1.x = trig.v2.x - trig.v1.x;
		e1.y = trig.v2.y - trig.v1.y;
		e1.z = trig.v2.z - trig.v1.z;				
		
		e2.x = trig.v3.x - trig.v1.x;
		e2.y = trig.v3.y - trig.v1.y;
		e2.z = trig.v3.z - trig.v1.z;				
	
		$fdisplay(of, "e2: %x %x %x", e2.x, e2.y, e2.z );
	
		t1.x = r.start.x - trig.v1.x;
		t1.y = r.start.y - trig.v1.y;
		t1.z = r.start.z - trig.v1.z;
		
		if( u [_WIDTH - 1 ] == 1 || u > `fixed2(1, `fp0p001) )
		begin 
			resultado = 0;
		end
		else
		begin 
			p1 = sp1 ( r.dir, q );
			v = mDiv( p1, det );
			$fdisplay(of, "v: %x", v); 
			begin
				$fdisplay(of, "parametric value v outside limits %x, %x", u, v);
				resultado = 0;
			end
			else
			begin
				t = fMul(e2.x, q.x) + fMul( e2.y, q.y ) + fMul( e2.z, q.z ); 
				t = fDiv( t, det );
				$display(of, "t: %x", t);
				if( t > `fixed2( 0, `fp0p001 ) && t < `fixed( 2 )  )
				begin  
					$fdisplay(of, "value t inside limits");
					$fdisplay(of, "e2: %x, %x, %x, e1: %x, %x, %x t1: %x, %x, %x", e2.x, e2.y, e2.z, e1.x, e1.y, e1.z, t1.x, t1.y, t1.z);
					$fdisplay(of, "q %x, %x, %x, det %x, u %x, v %x", q.x, q.y, q.z, det, u, v );
					$fdisplay(of, "t: %x", t);
					resultado[ _WIDTH + 1 : _WIDTH ] = 'b10;
					resultado[ _WIDTH - 1 : 0     ] = t;	
				end																													  
				else
				begin
					$fdisplay(of, "value t outside limits: %x", t);
					resultado = 0;
				end
			end
		end
		
		
	end
    assign w_return = functionIntersectionTest( r, trig, of );
	assign w_code = w_return[ _WIDTH + 1: _WIDTH ];
    assign w_result = w_return[ _WIDTH - 1: 0 ];		
    
    assign t = w_result;
    assign code = w_code;

endmodule