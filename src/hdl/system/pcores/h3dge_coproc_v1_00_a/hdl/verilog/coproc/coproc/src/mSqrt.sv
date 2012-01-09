import definitions_pack::*;
import math_pack::*;
module mSqrt
(
    input logic clock, input logic start, input fixed a, output fixed b, output logic ready, input integer of
); 

	fixed number, n, r, temp1, temp2, temp3, aux;
	logic[8:0] count, maxcount;	
	logic prv_start;

always @( posedge clock)
  begin
	if( start && !prv_start )
	begin
		number = a;
		$fdisplay(of, "squareRoot starting");
		$fdisplay(of, "number: %x", number);
		maxcount = _WIDTH - BI/2;

		r = 0;
		n = 0;	
		count = 0;
		ready = 0;
	end
	else
	//for ( count=0; count <= maxcount; count = count + 1 )
	begin
		$fdisplay(of, "squareRoot calculating");
		if( count < maxcount )
		begin
			temp1 = n << 2;
			//$fdisplay(of, "temp1 %x",temp1 );
	   		 temp2 = number >> ( _WIDTH - 2 );
	    	//$fdisplay(of, "temp2 %x",temp2 );
			temp3 = 3;
	    	n = temp1 | ( temp2 & temp3 );
			//$fdisplay(of, "n %x temp3: %x",n, temp3 );
	    	number = number << 2;
			//$fdisplay(of, "number %x",number );
	    	temp1 = r << 2;	
			//$fdisplay(of, "temp1 %x r %x",temp1, r );
	    	temp1 = temp1 + 1;	   
			//$fdisplay(of, "temp1 %x",temp1 );
	    	aux = n - temp1;				  
		
			if( aux[_WIDTH - 1] == 0 )
	    	begin
	      		n = n - temp1;
		   		//$fdisplay(of, "n %x",n );
	       		r = ( r << 1 ) + 1; 
		   		//$fdisplay(of, "r %x",r );
	    	end
	    	else
	    	begin
				r = r << 1;
				//$fdisplay(of, "r %x",r );
	    	end
			count = count + 1;
			ready = 0;
		end
		else
		begin 
			 b = r;
			 ready = 1;
		end
  	end
	prv_start = start;
  end
endmodule