import definitions_pack::*;
import math_pack::*;
module demuxVector
(
    input vector c, input logic sel, output vector a, output vector b
); 
	always @ (c or sel)
	begin
  	if (sel == 0) 
   	begin
      a = c;
   	end 
  	else 
  	begin
      b = c;
   	end
end
endmodule

module demuxFixed
(
    input fixed c, input logic sel, output fixed a, output fixed b/*, input integer of*/
); 
	always @ (c or sel)
	begin  
  	if (sel == 0) 
   	begin
      a = c;
   	end 
  	else 
  	begin
      b = c;
   	end
	//$fdisplay( of, "demux: c %x, sel %x, a %x, b %x", c, sel, a, b );
end
endmodule