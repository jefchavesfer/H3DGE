import light_pack::*;

module tb_refractedRayTest;
	
	
	ray r, refr; 
	triangle trig;
	fixed t;
	integer of; //arquivo de saida
	wire [ _WIDTH-1 : 0 ] wdistInt;
	logic[1:0] code, codeRefr;	
	vector normal;
	point intersectionPoint;
	
	intersectionTest hitTriangle( .r(r), .trig(trig), .of(of), .t(wdistInt), .code(code) );
	prepareIntersectionAndNormalVector prepareIntNormal( .trig( trig ), .r(r), .distInt(wdistInt), .of(of), .normal( normal ), .intersectionPoint( intersectionPoint ) );
	refractRay refractedRayModule( .r(r), .trig(trig), .normal(normal), .intersectionPoint(intersectionPoint), .of(of) , .refr( refr ), .code( codeRefr ) );
																									  

initial begin	   
	  
	of = $fopen("refractionTest.dat");
	
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
	
	$fdisplay(of,"=============case triangle OYZ with normal changing triangle perpendicular ray refraction==================");
	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, wdistInt );	
	$fdisplay(of,"reflected ray - start: %x %x %x dir: %x %x %x", refr.start.x, refr.start.y, refr.start.z, refr.dir.x, refr.dir.y, refr.dir.z );				   
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
	
	r.dir.x = fDiv( `fixed( 1 ), squareRoot( `fixed(2), of ) );
	r.dir.y = fDiv( `fixed( -1 ), squareRoot( `fixed(2), of ) );
	r.dir.z = `fixed( 0 ); 
	
	#10ms;	  
	
	$fdisplay(of,"=============case triangle OXZ with NO normal changing triangle 45deg ray refraction in aix XY==================");
	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, wdistInt );	
	$fdisplay(of,"refracted ray - codeRefr: %x start: %x %x %x dir: %x %x %x", codeRefr, refr.start.x, refr.start.y, refr.start.z, refr.dir.x, refr.dir.y, refr.dir.z );				   
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
	
	r.dir.x = `fixed( 0 );
	r.dir.y = fDiv( `fixed( -1 ), squareRoot( `fixed(2), of ) );
	r.dir.z = fDiv( `fixed( -1 ), squareRoot( `fixed(2), of ) );
	
	#10ms;	  
	
	$fdisplay(of,"=============case triangle OXZ with NO normal changing triangle 45deg ray refraction in axis YZ (border intersection)==================");
	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, wdistInt );	
	$fdisplay(of,"reflected ray - start: %x %x %x dir: %x %x %x", refr.start.x, refr.start.y, refr.start.z, refr.dir.x, refr.dir.y, refr.dir.z );				   
	$fdisplay(of,"=============end test==================");	
	
	$fclose(of);
	$finish;
end	

endmodule