import definitions_pack::*;
import math_pack::*;
module scalarProduct
(
    input point a, input point b, output fixed c/*, input integer of*/
); 
	assign c = fMul( a.x, b.x ) +  fMul( a.y, b.y ) + fMul( a.z, b.z );  
	/*always @( a or b or c )
	begin 
		$fdisplay( of, "scalar product: c %x, a %x, b %x", c, a, b );	 
	end*/
endmodule