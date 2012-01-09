import definitions_pack::*;
import math_pack::*;
module intersectionModule
(
    input logic clock, input logic reset, input logic start, input ray r, input point trigV1, input point trigV2, input point trigV3,  input integer of, output logic ready, output fixed t, output logic[1:0] code, output fixed u, output fixed v, output fixed det, output logic sel1, output logic sel2
); 				   
	fixed tUCDF;
	vector t1, e1, e2;
	vector rdir;
	assign rdir = r.dir;
	intersectionCU intCU( .clock( clock ), .reset( reset ), .start ( start ), .r ( r ), .trigV1 ( trigV1 ), .trigV2( trigV2 ), .trigV3( trigV3 ),  .tDF(tUCDF), .det( det ), .u(u), .v(v), .of(of), .t1( t1 ), .e2( e2 ), .e1( e1 ), .sel1( sel1 ), .sel2( sel2 ), .ready( ready ), .t(t), .code(code) );
	intersectionDF intDF( .clock( clock ), .rdir( rdir ) , .t1( t1 ), .e2 ( e2 ), .e1( e1 ), .sel1( sel1 ), .sel2( sel2 ), .det ( det ), .u( u ), .v(v), .t(tUCDF)/*, .of(of)*/ );
endmodule