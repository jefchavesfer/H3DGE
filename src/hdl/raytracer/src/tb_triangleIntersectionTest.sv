import light_pack::*;

module tb_triangleIntersectionTest;
	
	fixed t;
	ray r;		   
	triangle trig;
	logic[1:0] code;
	integer of;//arquivo de saida
	
	intersectionTest hitTriangle(.r(r), .trig(trig), .of(of), .t(t), .code(code) );
																									  

initial begin	   
	vector v1;
	of = $fopen("intersectionTest.dat");
	$fdisplay(of, "the test begins");
		
	$fdisplay(of,"===Test intersection case not intercept near border===");
	r.start.x = `fn0p001;
	r.start.y = `fixed( 0 );
	r.start.z = `fixed2( 0, `fp0p002 );
	r.dir.x = `fixed( 0 );
	r.dir.y = `fixed( 0 );
	r.dir.z = `fixed( -1 );
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 0 );
	trig.v2.x = `fixed( 2 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 0 );
	trig.v3.x = `fixed( 0 );
	trig.v3.y = `fixed( 2 );
	trig.v3.z = `fixed( 0 );																							   
	#10ms;																												   
	
	$fdisplay(of,"code intersection :%03d value intersection :%x", code, t );
	$display("test intersection complete\n");
	
	$fdisplay(of,"===Test intersection case intercept in border===");
	r.start.x = `fixed( 0 );
	r.start.y = `fixed( 0 );
	r.start.z = `fixed2( 0, `fp0p002 );
	r.dir.x = `fixed( 0 );
	r.dir.y = `fixed( 0 );
	r.dir.z = `fixed( -1 );
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 0 );
	trig.v2.x = `fixed( 2 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 0 );
	trig.v3.x = `fixed( 0 );
	trig.v3.y = `fixed( 2 );
	trig.v3.z = `fixed( 0 );																							   
	#10ms;																												   
	
	$fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	$display("test intersection complete\n");	 
	
	$fdisplay(of,"===Test intersection case intercept===");
	
	r.start.x = `fixed2( 0, `fp0p5 );		
	r.start.y = `fixed2( 0, `fp0p5 );
	r.start.z = `fixed2( 0, `fp0p5 );
	r.dir.x = `fixed( 0 );
	r.dir.y = `fixed( 0 );
	r.dir.z = `fixed( -1 );
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 0 );
	trig.v2.x = `fixed( 2 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 0 );
	trig.v3.x = `fixed( 0 );
	trig.v3.y = `fixed( 2 );
	trig.v3.z = `fixed( 0 );
	#10ms;
	
	$fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	$display("test intersection complete\n");
	
	$fdisplay(of,"===Test intersection case paralel ==="); 
	
	r.start.x = `fixed(1);
	r.start.y = `fixed2(0, `fp0p5);
	r.start.z = `fixed2(0, `fp0p5);
	r.dir.x = `fixed(-1);
	r.dir.y = `fixed(0);
	r.dir.z = `fixed(0);
	trig.v1.x = `fixed2( 0, `fp0p5 );
	trig.v1.y = `fixed2( 0, `fp0p5 );
	trig.v1.z = `fixed2( 0, `fp0p5 );
	trig.v2.x = `fixed2( 0, `fp0p5 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed2( 0, `fp0p5 );
	trig.v3.x = `fixed( 0 );
	trig.v3.y = `fixed( 0 );
	trig.v3.z = `fixed2( 0, `fp0p5 );
	#10ms;
	
	$fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	$display("test intersection complete\n");				
	
	$fdisplay(of,"===Test intersection not intersect ray outside triangle range==="); 	
	
	r.start.x = `fixed( 0 );
	r.start.y = `fixed2( 0, `fp0p5 );
	r.start.z = `fixed2( 0, `fp0p5 );
	r.dir.x = `fixed( 1 );
	r.dir.y = `fixed2( 0, `fp0p002 );
	r.dir.z = `fn0p2;
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 0 );
	trig.v2.x = `fixed( 2 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 0 );
	trig.v3.x = `fixed( 0 );
	trig.v3.y = `fixed( 2 );
	trig.v3.z = `fixed( 0 );
	#10ms;
	
	$fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	$display("test intersection complete\n");	
	
	$fdisplay(of,"===Test intersection intersect obliquous ray==="); 	
	
	r.start.x = `fixed( 0 );
	r.start.y = `fixed2( 0, `fp0p5 );
	r.start.z = `fixed2( 0, `fp0p5 );
	r.dir.x = `fixed2( 0, `fp0p5 );
	r.dir.y = `fixed2( 0, `fp0p5); 
	r.dir.z = `fn0p5;
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 0 );
	trig.v2.x = `fixed( 2 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 0 );
	trig.v3.x = `fixed( 0 );
	trig.v3.y = `fixed( 2 );
	trig.v3.z = `fixed( 0 );
	#10ms;
	
	$fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	$display("test intersection complete\n");
	
	$fdisplay(of,"===Test intersection intersect obliquous ray normalised==="); 	
	
	r.start.x = `fixed( 0 );
	r.start.y = `fixed2( 0, `fp0p5 );
	r.start.z = `fixed2( 0, `fp0p5 );
	r.dir.x = 32'h093cd3a0;
	r.dir.y = 32'h093cd3a0;
	r.dir.z = 32'hf6c32c60;
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 0 );
	trig.v2.x = `fixed( 2 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 0 );
	trig.v3.x = `fixed( 0 );
	trig.v3.y = `fixed( 2 );
	trig.v3.z = `fixed( 0 );
	#10ms;
	
	$fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	$display("test intersection complete\n");
	
	$fclose(of);
	$finish;
end	

endmodule