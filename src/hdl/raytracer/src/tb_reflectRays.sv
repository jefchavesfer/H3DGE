import light_pack::*;

module tb_reflectedRayTest;
	
	
	ray r, refl; 
	triangle trig;
	fixed t;
	integer of; //arquivo de saida
	wire [ _WIDTH-1 : 0 ] wdistInt;
	logic[1:0] code;	
	vector normal;
	point intersectionPoint;
	
	intersectionTest hitTriangle( .r(r), .trig(trig), .of(of), .t(wdistInt), .code(code) );
	prepareIntersectionAndNormalVector prepareIntNormal( .trig( trig ), .r(r), .distInt(wdistInt), .of(of), .normal( normal ), .intersectionPoint( intersectionPoint ) );
	reflectRay reflectedRayModule( .r(r), .trig(trig), .normal(normal), .intersectionPoint(intersectionPoint), .of(of) , .refl( refl ) );
																									  

initial begin	   
	  
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 2 );
	trig.v2.x = `fixed( 2 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 0 );
	trig.v3.x = `fixed( 0 );
	trig.v3.y = `fixed( 2 );
	trig.v3.z = `fixed( 0 ); 

	r.start.x = `fn0p5;
	r.start.y = `fixed2( 0, `fp0p5 );	   
	r.start.z = `fixed2( 0, `fp0p5 );
	
	r.dir.x = `fixed( 1 );
	r.dir.y = `fixed( 0 );
	r.dir.z = `fixed( 0 );
	
	#10ms;	  
	
	of = $fopen("reflectionTest.dat");
	$fdisplay(of, "square root test:"); 
	
	t = squareRoot( `fixed( 1 ), of );
	$fdisplay(of," sqrt (1): %x", t );
	t = squareRoot( `fixed( 4 ), of );
	$fdisplay(of," sqrt (4): %x", t );
	t = squareRoot( `fixed( 2 ), of );
	$fdisplay(of," sqrt (2): %x", t );
	t = squareRoot( `fixed(2.1), of);
	$fdisplay(of," sqrt (2.1): %x", t );
	
	$fdisplay(of, "the test begins");	
	
	$fdisplay(of,"=============case obliquous triangle with normal changing==================");
	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, wdistInt );	
	$fdisplay(of,"reflected ray - start: %x %x %x dir: %x %x %x", refl.start.x, refl.start.y, refl.start.z, refl.dir.x, refl.dir.y, refl.dir.z );				   
	$fdisplay(of,"=============end test==================");
	
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 2 );
	trig.v2.x = `fixed( 2 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 0 );
	trig.v3.x = `fixed( 0 );
	trig.v3.y = `fixed( 2 );
	trig.v3.z = `fixed( 0 ); 	 
	
	r.start.x = `fixed2( 1, `fp0p5 );
	r.start.y = `fixed2( 0, `fp0p5 );	   
	r.start.z = `fixed2( 0, `fp0p5 );	  
	
	r.dir.x = `fixed( -1 );
	r.dir.y = `fixed( 0 );
	r.dir.z = `fixed( 0 ); 
	
	#10ms;	  
	
	$fdisplay(of,"=============case obliquous triangle with NO normal changing==================");
	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, wdistInt );	
	$fdisplay(of,"reflected ray - start: %x %x %x dir: %x %x %x", refl.start.x, refl.start.y, refl.start.z, refl.dir.x, refl.dir.y, refl.dir.z );				   
	$fdisplay(of,"=============end test==================");
	
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 0 );
	trig.v2.x = `fixed( 0 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 2 );
	trig.v3.x = `fixed( 0 );
	trig.v3.y = `fixed( 2 );
	trig.v3.z = `fixed( 0 ); 	 
	
	r.start.x = `fixed2( 1, `fp0p5 );
	r.start.y = `fixed2( 0, `fp0p5 );	   
	r.start.z = `fixed2( 0, `fp0p5 );	  
	
	r.dir.x = `fixed( -1 );
	r.dir.y = `fixed( 0 );
	r.dir.z = `fixed( 0 ); 
	
	#10ms;	  
	
	$fdisplay(of,"=============case triangle OYZ with normal changing triangle perpendicular ray reflection==================");
	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, wdistInt );	
	$fdisplay(of,"reflected ray - start: %x %x %x dir: %x %x %x", refl.start.x, refl.start.y, refl.start.z, refl.dir.x, refl.dir.y, refl.dir.z );				   
	$fdisplay(of,"=============end test=================="); 
		
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 0 );
	trig.v2.x = `fixed( 0 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 2 );
	trig.v3.x = `fixed( 2 );
	trig.v3.y = `fixed( 0 );
	trig.v3.z = `fixed( 0 ); 	 	 
	
	r.start.x = `fixed2( 0, `fp0p5 );
	r.start.y = `fixed2( 0, `fp0p5 );	   
	r.start.z = `fixed2( 0, `fp0p5 );	  
	
	r.dir.x = `fixed( 1 );
	r.dir.y = `fixed( -1 );
	r.dir.z = `fixed( 0 ); 
	
	#10ms;	  
	
	$fdisplay(of,"=============case triangle OXZ with NO normal changing triangle 45deg ray reflection==================");
	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, wdistInt );	
	$fdisplay(of,"reflected ray - start: %x %x %x dir: %x %x %x", refl.start.x, refl.start.y, refl.start.z, refl.dir.x, refl.dir.y, refl.dir.z );				   
	$fdisplay(of,"=============end test==================");
	
	trig.v1.x = `fixed( 0 );
	trig.v1.y = `fixed( 0 );
	trig.v1.z = `fixed( 0 );
	trig.v2.x = `fixed( 0 );
	trig.v2.y = `fixed( 0 );
	trig.v2.z = `fixed( 2 );
	trig.v3.x = `fixed( 2 );
	trig.v3.y = `fixed( 0 );
	trig.v3.z = `fixed( 0 ); 	 	 
	
	r.start.x = `fixed2( 0, `fp0p5 );
	r.start.y = `fixed2( 0, `fp0p5 );	   
	r.start.z = `fixed2( 0, `fp0p5 );	  
	
	r.dir.x = `fixed( 1 );
	r.dir.y = `fixed( -1 );
	r.dir.z = `fixed( -1 ); 
	
	#10ms;	  
	
	$fdisplay(of,"=============case triangle OXZ with NO normal changing triangle 45deg ray reflection in all axis (border intersection)==================");
	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, wdistInt );	
	$fdisplay(of,"reflected ray - start: %x %x %x dir: %x %x %x", refl.start.x, refl.start.y, refl.start.z, refl.dir.x, refl.dir.y, refl.dir.z );				   
	$fdisplay(of,"=============end test==================");	
	
	$fclose(of);
	$finish;
end	

endmodule