import definitions_pack::*;
import math_pack::*;
module registerVector
(
    input logic clock, input vector a, output vector b
); 

always @(posedge clock)	
	begin
		b = a;
	end
endmodule	 


module registerFixed
(
    input logic clock, input fixed a, output fixed b
); 

always @(posedge clock)	
	begin
		b = a;
	end
endmodule