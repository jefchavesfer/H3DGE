import definitions_pack::*;
import math_pack::*;
module normalAndIntersectionPoint
(
    input logic clockSqrt, input logic clock, input logic reset, input logic start, input point trigV1, input point trigV2, input point trigV3, input ray r, input fixed t, input integer of, output logic ready, output vector normal, output point intersectionPoint, output logic startSqrt, output logic readySqrt, output vector n1, output vector n2, output logic sel1, output logic sel2, output integer state
); 				   

	//logic sel1, sel2;
	
	assign intersectionPoint.x = r.start.x + fMul( r.dir.x, t ); 
	assign intersectionPoint.y = r.start.y + fMul( r.dir.y, t ); 
	assign intersectionPoint.z = r.start.z + fMul( r.dir.z, t ); 
	
	normalCU normCU( .clock( clock ), .reset( reset ), .start ( start ), .of ( of ), .sel1 ( sel1 ),  .sel2(sel2), .ready( ready ), .startSqrt( startSqrt ), .readySqrt( readySqrt ), .state(state) );
	normalDF normDF( .clock( clock ), .trigV1( trigV1 ) , .trigV2( trigV2 ), .trigV3 ( trigV3 ), .sel1( sel1 ), .sel2( sel2 ), .sqrtReady(readySqrt ), .normal( normal ), .clockSqrt( clockSqrt ), .startSqrt( startSqrt ), .readySqrt( readySqrt ), .of(of), .n1(n1), .n2(n2) );
endmodule