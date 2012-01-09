import definitions_pack::*;

module tb_triangleIntersectionTest;
   fixed t, u, v, det;
   ray r;
   triangle trig;
   logic[1:0] code;
   integer 	  of;//arquivo de saida
   logic 	  clk, reset, start, ready, sel1, sel2;	   
   point trigV1, trigV2, trigV3; 
   
   intersectionModule hitTriangle(.clock(clk), .reset(reset), .start(start), .r(r), .trigV1(trigV1), .trigV2(trigV2), .trigV3( trigV3 ), .of(of), .ready(ready), .t(t), .code(code), .u(u), .v(v), .det(det), .sel1(sel1), .sel2(sel2) );

   always
	 begin
		#1ms;
		clk = ~clk;	 	 
	 end

   initial begin
	  vector v1;
	  of = $fopen("intersectionTest.dat");
	  $fdisplay(of, "the test begins");
	  
	  clk = 0;
	  start = 0;
	  
	  $fdisplay(of,"===Test intersection case not intercept near border===");
	  
	  reset = 0;  
	  #2ms;	 
	  reset = 1;
	  #2ms;
	  reset = 0;
	  
	  r.start.x = `fMMRDIRX;
	  r.start.y = `fixed( 1 );
	  r.start.z = `fixed( 0 );
	  r.dir.x = `fixed( 0 );
	  r.dir.y = `fMMRDIRY;
	  r.dir.z = `fMMRDIRZ;
	  trigV1.x = `fMMTRIGV1X;
	  trigV1.y = `fMMTRIGV1Y;
	  trigV1.z = `fMMTRIGV1Z;
	  trigV2.x = `fMMTRIGV2X;
	  trigV2.y = `fMMTRIGV2Y;
	  trigV2.z = `fMMTRIGV2Z;
	  trigV3.x = `fMMTRIGV3X;
	  trigV3.y = `fMMTRIGV3Y;
	  trigV3.z = `fMMTRIGV3Z;
	  
	  
	  //r.start.x = `fixed( 0 )`fn0p001;
//	  r.start.y = `fixed( 1 );
//	  r.start.z = `fixed2( 0 );
//	  r.dir.x = `fixed( 0 );
//	  r.dir.y = `fixed( 0 );
//	  r.dir.z = `fixed( -1 );
//	  trigV1.x = `fixed( 0 );
//	  trigV1.y = `fixed( 0 );
//	  trigV1.z = `fixed( 0 );
//	  trigV2.x = `fixed( 2 );
//	  trigV2.y = `fixed( 0 );
//	  trigV2.z = `fixed( 0 );
//	  trigV3.x = `fixed( 0 );
//	  trigV3.y = `fixed( 2 );
//	  trigV3.z = `fixed( 0 );
	  
	  start = 1;
	  #2ms;
	  start = 0;
	  
	  #8ms;
	  
	  $fdisplay(of,"code intersection :%03d value intersection :%x", code, t );
	  $display("test intersection complete\n");
	  
	  $fdisplay(of,"===Test intersection case intercept in border===");
 
	  r.start.x = `fixed( 0 );
	  r.start.y = `fixed( 0 );
	  r.start.z = `fixed2( 0, `fp0p002 );
	  r.dir.x = `fixed( 0 );
	  r.dir.y = `fixed( 0 );
	  r.dir.z = `fixed( -1 );
	  trigV1.x = `fixed( 0 );
	  trigV1.y = `fixed( 0 );
	  trigV1.z = `fixed( 0 );
	  trigV2.x = `fixed( 2 );
	  trigV2.y = `fixed( 0 );
	  trigV2.z = `fixed( 0 );
	  trigV3.x = `fixed( 0 );
	  trigV3.y = `fixed( 2 );
	  trigV3.z = `fixed( 0 );
	  
	  start = 1;
	  #2ms;
	  start = 0;
	  
	  #8ms;
	  
	  $fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	  $display("test intersection complete\n");
	  
	  $fdisplay(of,"===Test intersection case intercept===");
	  
      r.start.x = `fixed2( 0, `fp0p5 );
	  r.start.y = `fixed2( 0, `fp0p5 );
	  r.start.z = `fixed2( 0, `fp0p5 );
	  r.dir.x = `fixed( 0 );
	  r.dir.y = `fixed( 0 );
	  r.dir.z = `fixed( -1 );
	  trigV1.x = `fixed( 0 );
	  trigV1.y = `fixed( 0 );
	  trigV1.z = `fixed( 0 );
	  trigV2.x = `fixed( 2 );
	  trigV2.y = `fixed( 0 );
	  trigV2.z = `fixed( 0 );
	  trigV3.x = `fixed( 0 );
	  trigV3.y = `fixed( 2 );
	  trigV3.z = `fixed( 0 );
	  
	  start = 1;
	  #2ms;
	  start = 0;
	  
	  #8ms;

	  $fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
      $display("test intersection complete\n");
	  
	  $fdisplay(of,"===Test intersection case paralel ===");
	  
	  r.start.x = `fixed(1);
	  r.start.y = `fixed2(0, `fp0p5);
	  r.start.z = `fixed2(0, `fp0p5);
	  r.dir.x = `fixed(-1);
	  r.dir.y = `fixed(0);
	  r.dir.z = `fixed(0);
	  trigV1.x = `fixed2( 0, `fp0p5 );
	  trigV1.y = `fixed2( 0, `fp0p5 );
	  trigV1.z = `fixed2( 0, `fp0p5 );
	  trigV2.x = `fixed2( 0, `fp0p5 );
	  trigV2.y = `fixed( 0 );
	  trigV2.z = `fixed2( 0, `fp0p5 );
	  trigV3.x = `fixed( 0 );
	  trigV3.y = `fixed( 0 );
	  trigV3.z = `fixed2( 0, `fp0p5 );
  
	  start = 1;
	  #2ms;
	  start = 0;
	  
	  #8ms;

	  $fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	  $display("test intersection complete\n");

	  $fdisplay(of,"===Test intersection not intersect ray outside triangle range===");
	  
	  r.start.x = `fixed( 0 );
	  r.start.y = `fixed2( 0, `fp0p5 );
	  r.start.z = `fixed2( 0, `fp0p5 );
	  r.dir.x = `fixed( 1 );
	  r.dir.y = `fixed2( 0, `fp0p002 );
	  r.dir.z = `fn0p2;
	  trigV1.x = `fixed( 0 );
	  trigV1.y = `fixed( 0 );
	  trigV1.z = `fixed( 0 );
	  trigV2.x = `fixed( 2 );
	  trigV2.y = `fixed( 0 );
	  trigV2.z = `fixed( 0 );
	  trigV3.x = `fixed( 0 );
	  trigV3.y = `fixed( 2 );
	  trigV3.z = `fixed( 0 );
	  
	  start = 1;
	  #2ms;
	  start = 0;
	  
	  #8ms;
	  
	  $fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	  $display("test intersection complete\n");

	  $fdisplay(of,"===Test intersection intersect obliquous ray===");
	  
	  r.start.x = `fixed( 0 );
	  r.start.y = `fixed2( 0, `fp0p5 );
	  r.start.z = `fixed2( 0, `fp0p5 );
	  r.dir.x = `fixed2( 0, `fp0p5 );
	  r.dir.y = `fixed2( 0, `fp0p5);
	  r.dir.z = `fn0p5;
	  trigV1.x = `fixed( 0 );
	  trigV1.y = `fixed( 0 );
	  trigV1.z = `fixed( 0 );
	  trigV2.x = `fixed( 2 );
	  trigV2.y = `fixed( 0 );
	  trigV2.z = `fixed( 0 );
	  trigV3.x = `fixed( 0 );
	  trigV3.y = `fixed( 2 );
	  trigV3.z = `fixed( 0 );
	  
	  start = 1;
	  #2ms;
	  start = 0;
	  
	  #8ms;
	  
	  $fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	  $display("test intersection complete\n");

	  $fdisplay(of,"===Test intersection intersect obliquous ray normalised===");
	  
	  r.start.x = `fixed( 0 );
	  r.start.y = `fixed2( 0, `fp0p5 );
	  r.start.z = `fixed2( 0, `fp0p5 );
	  r.dir.x = 32'h093cd3a0;
	  r.dir.y = 32'h093cd3a0;
	  r.dir.z = 32'hf6c32c60;
	  trigV1.x = `fixed( 0 );
	  trigV1.y = `fixed( 0 );
	  trigV1.z = `fixed( 0 );
	  trigV2.x = `fixed( 2 );
	  trigV2.y = `fixed( 0 );
	  trigV2.z = `fixed( 0 );
	  trigV3.x = `fixed( 0 );
	  trigV3.y = `fixed( 2 );
	  trigV3.z = `fixed( 0 );
	  
	  start = 1;
	  #2ms;
	  start = 0;
	  
	  #8ms;

	  $fdisplay(of,"code intersection :%03d value intersection :%x", code, t ) ;
	  $display("test intersection complete\n");

	  $fclose(of);
	  $finish;
   end

endmodule // tb_triangleIntersectionTest