import definitions_pack::*;
import definitions_pack::*;
import math_pack::*;

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

   logic                         int_start;
   ray                           int_ray;
   triangle                      int_triangle;
   logic                         int_ready;
   logic [1 : 0]                 int_code;
   fixed                         int_t;

   intersectionModule  intersector(Clk, Reset, int_start,
                                   int_ray, int_triangle, 0, int_ready,
                                   int_t, int_code);

   assign
     int_start = Start,
     int_ray.start.x  = RayStart_X,
     int_ray.start.y  = RayStart_Y,
     int_ray.start.z  = RayStart_Z,
     int_ray.dir.x  = RayDir_X,
     int_ray.dir.y  = RayDir_Y,
     int_ray.dir.z  = RayDir_Z,
     int_triangle.v1.x  = TriangleV1_X,
     int_triangle.v1.y  = TriangleV1_Y,
     int_triangle.v1.z  = TriangleV1_Z,
     int_triangle.v2.x  = TriangleV2_X,
     int_triangle.v2.y  = TriangleV2_Y,
     int_triangle.v2.z  = TriangleV2_Z,
     int_triangle.v3.x  = TriangleV3_X,
     int_triangle.v3.y  = TriangleV3_Y,
     int_triangle.v3.z  = TriangleV3_Z,
     IntersectionT = int_t,
     Code = int_code,
     Ready = int_ready,
     Intersection_X = RayStart_X + IntersectionT * RayDir_X,
     Intersection_Y = RayStart_Y + IntersectionT * RayDir_Y,
     Intersection_Z = RayStart_Z + IntersectionT * RayDir_Z;

endmodule // coproc
