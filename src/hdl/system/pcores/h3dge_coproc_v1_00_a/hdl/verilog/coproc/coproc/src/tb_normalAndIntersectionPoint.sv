import definitions_pack::*;

module tb_normalAndIntersectionPoint;
   triangle trig; 
   ray r;
   integer 	  of, state;//arquivo de saida
   logic 	  clk, reset, start, readyInt, readyNorm, clockSqrt, startSqrt, readySqrt, sel1, sel2;
   vector normal, n1, n2;
   point intersectionPoint, trigV1, trigV2, trigV3;
   logic[1:0] code;	
   fixed t, u, v, det;
   
   intersectionModule hitTriangle( .clock(clk), .reset(reset), .start(start), .r(r), .trigV1( trigV1 ), .trigV2( trigV2 ), .trigV3( trigV3 ), .of(of), .ready(readyInt), .t(t), .code(code), .u(u), .v(v), .det(det) );
   normalAndIntersectionPoint normInt( .clockSqrt(clockSqrt), .clock(clk), .reset(reset), .start(start), .trigV1( trigV1 ), .trigV2( trigV2 ) , .trigV3( trigV3 ), .r(r), .t(t), .of(of), .ready(readyNorm), .normal(normal), .intersectionPoint(intersectionPoint), .startSqrt(startSqrt), .readySqrt(readySqrt), .n1(n1), .n2(n2), .sel1(sel1), .sel2(sel2), .state(state) );

   always
	 begin
		#1ms;
				clockSqrt = ~clockSqrt;
				clk = ~clk;	 
	 end

   initial begin
	of = $fopen("intersectionAndNormalTest.dat");
	$fdisplay(of, "the test begins");
	
	clk = 0; 
	clockSqrt = 0;
	start = 0; 						   
	reset = 1;
		  
	trigV1.x = `fixed( 0 );
	trigV1.y = `fixed( 0 );
	trigV1.z = `fixed( 0 );
	trigV2.x = `fixed( 2 );
	trigV2.y = `fixed( 0 );
	trigV2.z = `fixed( 0 );
	trigV3.x = `fixed( 0 );
	trigV3.y = `fixed( 2 );
	trigV3.z = `fixed( 0 ); 

	r.start.x = `fn0p5;
	r.start.y = `fixed2( 0, `fp0p5 );	   
	r.start.z = `fixed2( 0, `fp0p5 );
	
	r.dir.x = `fixed( 1 );
	r.dir.y = `fixed( 0 );
	r.dir.z = `fixed( 0 );	   
	
	#2ms;
	reset = 0;
	start = 1;
	#2ms;
	start = 0;
	  
	#250ms;
	
	$fdisplay(of,"=============case obliquous triangle==================");
	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, t );	
	$fdisplay(of,"normal: %x %x %x intersectionPoint: %x %x %x", normal.x, normal.y, normal.z, intersectionPoint.x, intersectionPoint.y, intersectionPoint.z );		   
	$fdisplay(of,"=============end test==================");
	
	reset = 1;
	
	//trigV1.x = `fixed( 0 );
//	trigV1.y = `fixed( 0 );
//	trigV1.z = `fixed( 0 );
//	trigV2.x = `fixed( 0 );
//	trigV2.y = `fixed( 0 );
//	trigV2.z = `fixed( 2 );
//	trigV3.x = `fixed( 0 );
//	trigV3.y = `fixed( 2 );
//	trigV3.z = `fixed( 0 ); 	 
//	
//	r.start.x = `fixed2( 1, `fp0p5 );
//	r.start.y = `fixed2( 0, `fp0p5 );	   
//	r.start.z = `fixed2( 0, `fp0p5 );	  
//	
//	r.dir.x = `fixed( -1 );
//	r.dir.y = `fixed( 0 );
//	r.dir.z = `fixed( 0 ); 
//	
//	#2ms;
//	reset = 0;
//	start = 1;
//	#2ms;
//	start = 0;
//	  
//	#6ms	  
//	
//	$fdisplay(of,"=============case triangle OYZ triangle perpendicular ==================");
//	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, t );	
//	$fdisplay(of,"normal: %x %x %x intersectionPoint: %x %x %x", normal.x, normal.y, normal.z, intersectionPoint.x, intersectionPoint.y, intersectionPoint.z );		   
//	$fdisplay(of,"=============end test=================="); 
//	
//	reset = 1;
//	
//	trigV1.x = `fixed( 0 );
//	trigV1.y = `fixed( 0 );
//	trigV1.z = `fixed( 0 );
//	trigV2.x = `fixed( 0 );
//	trigV2.y = `fixed( 0 );
//	trigV2.z = `fixed( 2 );
//	trigV3.x = `fixed( 2 );
//	trigV3.y = `fixed( 0 );
//	trigV3.z = `fixed( 0 ); 	 	 
//	
//	r.start.x = `fixed2( 0, `fp0p5 );
//	r.start.y = `fixed2( 0, `fp0p5 );	   
//	r.start.z = `fixed2( 0, `fp0p5 );	  
//	
//	r.dir.x = `fixed( 1 );
//	r.dir.y = `fixed( -1 );
//	r.dir.z = `fixed( 0 ); 
//	
//	#2ms;
//	reset = 0;
//	start = 1;
//	#2ms;
//	start = 0;
//	  
//	#6ms	  
//	
//	$fdisplay(of,"=============case triangle OXZ with triangle 45deg ray==================");
//	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, t );	
//	$fdisplay(of,"normal: %x %x %x intersectionPoint: %x %x %x", normal.x, normal.y, normal.z, intersectionPoint.x, intersectionPoint.y, intersectionPoint.z );		   
//	$fdisplay(of,"=============end test==================");
//	
//	reset = 1;
//	
//	trigV1.x = `fixed( 0 );
//	trigV1.y = `fixed( 0 );
//	trigV1.z = `fixed( 0 );
//	trigV2.x = `fixed( 0 );
//	trigV2.y = `fixed( 0 );
//	trigV2.z = `fixed( 2 );
//	trigV3.x = `fixed( 2 );
//	trigV3.y = `fixed( 0 );
//	trigV3.z = `fixed( 0 ); 	 	 
//	
//	r.start.x = `fixed2( 0, `fp0p5 );
//	r.start.y = `fixed2( 0, `fp0p5 );	   
//	r.start.z = `fixed2( 0, `fp0p5 );	  
//	
//	r.dir.x = `fixed( 1 );
//	r.dir.y = `fixed( -1 );
//	r.dir.z = `fixed( -1 ); 
//	
//	#2ms;
//	reset = 0;
//	start = 1;
//	#2ms;
//	start = 0;
//	  
//	#6ms	  
//	
//	$fdisplay(of,"=============case triangle OXZ with NO normal changing triangle 45deg in all axis ==================");
//	$fdisplay(of,"code intersection normal:%x value intersection :%x", code, t );	
//	$fdisplay(of,"normal: %x %x %x intersectionPoint: %x %x %x", normal.x, normal.y, normal.z, intersectionPoint.x, intersectionPoint.y, intersectionPoint.z );		   
//	$fdisplay(of,"=============end test==================");	
	
	$fclose(of);
	$finish;
   end

endmodule // tb_normalAndIntersectionPoint