import light_pack::*;
module refractRay
(
    input ray r, input triangle trig, input point intersectionPoint, input vector normal, input integer of, output ray refr, output logic[1:0]code
); 
	wire [ 6 * _WIDTH + 1 : 0 ] exit;
	
	assign exit = functionRefractedRay( r, trig, intersectionPoint, normal, of );
	assign code = exit[ 6 * _WIDTH + 1 : 6 * _WIDTH];
	assign refr = exit[ 6 * _WIDTH - 1 : 0];
endmodule