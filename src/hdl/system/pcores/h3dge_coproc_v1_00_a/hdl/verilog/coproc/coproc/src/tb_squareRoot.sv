import definitions_pack::*;

module tb_sqrt;
   integer 	  of;//arquivo de saida
   logic 	  clk, reset, start, ready;
   fixed entry, exit;
   
   mSqrt sqrt( .clock(clk), .start(start), .a(entry), .b(exit), .ready(ready), .of(of) ); 
   
   always
	 begin
		#1ms;
		clk = ~clk;	 	 
	 end

   initial begin
	of = $fopen("sqrtTest.dat");
	$fdisplay(of, "the test begins");
	
	clk = 0;
	start = 0;
			  
	entry = `fixed(4);

	#2ms;
	start = 1;
	#2ms;
	start = 0;
	  
	#31ms;
	
	$fdisplay(of,"sqrt 4: %x", exit );	  
	
	entry = `fixed(2);

	#2ms;
	start = 1;
	#2ms;
	start = 0;
	  
	#31ms;
	
	$fdisplay(of,"sqrt 2: %x", exit );
		
	$fclose(of);
	$finish;
   end

endmodule // tb_normalAndIntersectionPoint