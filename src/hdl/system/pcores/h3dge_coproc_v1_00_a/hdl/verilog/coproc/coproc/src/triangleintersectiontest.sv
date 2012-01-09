import definitions_pack::*;
import light_pack::*;
module intersectionTest
(
    input ray r, input triangle trig, input integer of, output fixed t, output code[ 1 : 0 ]
); 
    
    wire [ _WIDTH + 1 : 0 ] w_return;    
    wire [ 1 : 0 ] w_code;
    wire [ _WIDTH-1 : 0 ] w_result;
    
    
    assign w_return = functionIntersectionTest( r, trig, of );
	assign w_code = w_return[ _WIDTH + 1: _WIDTH ];
    assign w_result = w_return[ _WIDTH - 1: 0 ];		
    
    assign t = w_result;
    assign code = w_code;

endmodule