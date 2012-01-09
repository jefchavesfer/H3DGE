import definitions_pack::*;
import math_pack::*;
module mux
(
    input point a, input point b, input logic sel, output point c/*, input integer of */
); 
	always @ (a or b or sel)
	begin
  	if (sel == 0) 
   	begin
      c = a;
   	end 
  	else 
  	begin
      c = b; 
   	end
	//$fdisplay( of, "mux: sel %x a %x b %x c %x ", sel, a, b, c);
end
endmodule