import definitions_pack::*;
import math_pack::*;
module mDiv
(
    input fixed a, input fixed b, output fixed c/*, input integer of*/
); 
  assign c = fDiv( a, b/*, of*/ );
endmodule