import definitions_pack::*;
import math_pack::*;
import light_pack::*;
module prepareIntersectionAndNormalVector
(
    input triangle trig, input ray r, input fixed distInt, input integer of, output vector normal, output point intersectionPoint
); 

	assign normal = generateNormalVector( trig, of );
	assign intersectionPoint.x = r.start.x + fMul( distInt, r.dir.x ); 
	assign intersectionPoint.y = r.start.y + fMul( distInt, r.dir.y );
	assign intersectionPoint.z = r.start.z + fMul( distInt, r.dir.z ); 
endmodule