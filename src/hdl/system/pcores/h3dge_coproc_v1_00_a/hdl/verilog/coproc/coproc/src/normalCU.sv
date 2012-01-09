import definitions_pack::*;
import math_pack::*;
module normalCU
(
    input logic clock, input logic reset, input logic start, input integer of, output logic sel1, output logic sel2, output logic ready, output logic startSqrt, input logic readySqrt, output integer state
); 				   
	//integer state; 
	logic prv_start;

	always@( posedge clock )
    begin
		if (reset)
		  begin	
			 startSqrt = 0;
			 ready = 0;	 
			 state = 0;	  
			 sel1 = 0;
			 sel2 = 0;		 
			 prv_start = start;	
			 $fdisplay( of, "normal - reset" );
		  end
		else
		begin
		  case (state)
			0:
			  begin
			  	$fdisplay( of, "normal - state0" );
			 	if (start && !prv_start)
				begin
					sel1 = 0;
			 		sel2 = 0;
					$fdisplay( of, "norma; - started - state0" );
					state = 1;
					startSqrt = 0;
				end	 
			  end
			1:
			  begin
				 $fdisplay( of, "normal - state1" );
				 startSqrt = 1;
				 state = 2;	
			  end
			  2: 
			  begin	 
				 $fdisplay( of, "normal - state2" );
				 startSqrt = 0;
				 state = 3;				  
			  end
			  3: 
			  begin
				 $fdisplay( of, "normal - state3" );
				 if( readySqrt == 1 )
			     begin
					 state = 4;
			     end
				 else
				 begin
					 state = 3;
				 end
		 	  end
			  4:
			  begin
				 $fdisplay( of, "normal - state4" );
			     sel1 = 1;
				 sel2 = 0;
				 state = 5;	
			  end
			  5:
			  begin
				 $fdisplay( of, "normal - state5" );
				 startSqrt = 1;
				 state = 6;
			  end
			  6:
			  begin	 
				 $fdisplay( of, "normal - state6" );
				 startSqrt=0;
				 state = 7;
			  end
			  7:
			  begin	
				 $fdisplay( of, "normal - state7" );
				 if( readySqrt == 1 )
			     begin
					state = 8; 
				 end
				 else
			     begin
					state = 7; 
				 end
			   end
			   8:
			   begin
				 $fdisplay( of, "normal - state8" );
				 sel1 = 1;
				 sel2 = 1;
				 state = 9;
			   end 
			   9:
			   begin
				 $fdisplay( of, "normal - state9" );
			     startSqrt = 1;
			     state = 10;
			   end
			   10:
			   begin 
				 $fdisplay( of, "normal - state10" );
				 startSqrt = 0;
				 state = 11;
			   end 
			   11:
			   begin
				  $fdisplay( of, "normal - state11" );
				  if(readySqrt == 1)
				  begin 
					 state = 12; 
				  end
				  else
			      begin
					 state = 11; 
				  end
			   end
			   12:
			   begin
				 $fdisplay( of, "normal - state12" );
				 ready = 1;
				 state = 12;
			   end
			   default:
			   begin
				 ready = 0;
			 	 state = 0;	  
			     sel1 = 0;
			     sel2 = 0;		 
			     prv_start = start;   
			   end //default
		  endcase
		end	//if
		prv_start = start;
	end	//always
endmodule