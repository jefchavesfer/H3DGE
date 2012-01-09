module coproc
  (
   Clk,
   Reset,
   Start,
   RayStart_X,
   RayStart_Y,
   RayStart_Z,
   RayDir_X,
   RayDir_Y,
   RayDir_Z,
   TriangleV1_X,
   TriangleV1_Y,
   TriangleV1_Z,
   TriangleV2_X,
   TriangleV2_Y,
   TriangleV2_Z,
   TriangleV3_X,
   TriangleV3_Y,
   TriangleV3_Z,
   Ready,
   Code,
   IntersectionT,
   Intersection_X,
   Intersection_Y,
   Intersection_Z
   );

   parameter C_SLV_DWIDTH                   = 32;

   input Clk;
   input Reset;
   input Start;
   input [0 : C_SLV_DWIDTH - 1] RayStart_X;
   input [0 : C_SLV_DWIDTH - 1] RayStart_Y;
   input [0 : C_SLV_DWIDTH - 1] RayStart_Z;
   input [0 : C_SLV_DWIDTH - 1] RayDir_X;
   input [0 : C_SLV_DWIDTH - 1] RayDir_Y;
   input [0 : C_SLV_DWIDTH - 1] RayDir_Z;
   input [0 : C_SLV_DWIDTH - 1] TriangleV1_X;
   input [0 : C_SLV_DWIDTH - 1] TriangleV1_Y;
   input [0 : C_SLV_DWIDTH - 1] TriangleV1_Z;
   input [0 : C_SLV_DWIDTH - 1] TriangleV2_X;
   input [0 : C_SLV_DWIDTH - 1] TriangleV2_Y;
   input [0 : C_SLV_DWIDTH - 1] TriangleV2_Z;
   input [0 : C_SLV_DWIDTH - 1] TriangleV3_X;
   input [0 : C_SLV_DWIDTH - 1] TriangleV3_Y;
   input [0 : C_SLV_DWIDTH - 1] TriangleV3_Z;
   output                       Ready;
   output [0 : 1]               Code;
   output [0 : C_SLV_DWIDTH - 1] IntersectionT;
   output [0 : C_SLV_DWIDTH - 1] Intersection_X;
   output [0 : C_SLV_DWIDTH - 1] Intersection_Y;
   output [0 : C_SLV_DWIDTH - 1] Intersection_Z;

endmodule // coproc
