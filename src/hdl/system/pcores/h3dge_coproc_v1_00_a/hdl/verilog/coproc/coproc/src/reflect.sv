import definitions_pack::*;
import light_pack::*;
module reflectRay
(
    input ray r, input triangle trig, input vector normal, input point intersectionPoint, input integer of, output ray refl
); 
    
	assign refl = functionReflectedRay( r, trig, intersectionPoint, normal, of );

endmodule