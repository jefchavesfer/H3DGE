import definitions_pack::*;
import math_pack::*;
module vectorialProduct
(
    input vector a, input vector b, output vector c
); 
	assign c.x = fMul( a.y, b.z ) - fMul( a.z, b.y );
	assign c.y = fMul( b.x, a.z ) - fMul( a.x, b.z );
	assign c.z = fMul( a.x, b.y ) - fMul( b.x, a.y); 
	always @( c )
	begin 
		$display("produto vetorial");
		$display("c.x %x, c.y %x, c.z %x", c.x, c.y, c.z );
		$display("a.x %x, a.y %x, a.z %x", a.x, a.y, a.z );
		$display("b.x %x, b.y %x, b.z %x", b.x, b.y, b.z );	 
	end
endmodule