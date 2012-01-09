import definitions_pack::*;
import math_pack::*;
module intersectionCU
(
    input logic clock, input logic reset, input logic start, input ray r, input point trigV1, input point trigV2, input point trigV3,  input fixed tDF, input fixed det, input fixed u, input fixed v, input integer of, output vector t1, output vector e2, output vector e1, output logic sel1, output logic sel2, output logic ready, output fixed t, output logic[1:0] code
); 				   
	integer state; 
	logic prv_start;	 
	fixed test, test2, test3;
	
	always@( posedge clock )
    begin
		if (reset)
		  begin	  
			 ready = 0;
			 state = 0;	  
			 sel1 = 0;
			 sel2 = 0;		 
			 code = 0;
			 t = 0;
			 prv_start = start;	
			 $fdisplay( of, "reset" );
		  end
		else
		begin
		  case (state)
			0:
			  begin
			  	$fdisplay( of, "state0" );
			 	if (start && !prv_start)
				begin
					ready = 0;
			 		state = 0;	  
			 		sel1 = 0;
			 		sel2 = 0;		 
			 		code = 0;
			 		t = 0;
					
					e1.x = trigV2.x - trigV1.x;
					e1.y = trigV2.y - trigV1.y;
					e1.z = trigV2.z - trigV1.z;				
		
					e2.x = trigV3.x - trigV1.x;
					e2.y = trigV3.y - trigV1.y;
					e2.z = trigV3.z - trigV1.z;				
	
					$fdisplay(of, "e2: %x %x %x", e2.x, e2.y, e2.z );
	
					t1.x = r.start.x - trigV1.x;
					t1.y = r.start.y - trigV1.y;
					t1.z = r.start.z - trigV1.z;
				   	
					state = 1;	 
					$fdisplay( of, "started" );
				end	 
			  end
			1:
			  begin
				 $fdisplay( of, "det %x", det );
				 //if( det >= `fixed(`fn0p001) || det <= `fixed2(0, `fp0p001 ) )
				 if( det >= 32'hfffbe76d || det <= 32'h00041893 )
				 begin
					ready = 1;	 
					state = 0;
					$fdisplay( of, "state1: det not good" );
					test  = 32'hf8000000; //-0,5
					test2 = 32'hf3333330; //-0.8
					test3 = 32'h08000000; //0,5
					
					if( test2 >= test || test2 <= test3 )
					begin
						$fdisplay( of, "considera sinal" );
					end
					else
					begin
						$fdisplay( of, "nao considera sinal" );
					end
					
				 end  
				 else
				 begin
					 $fdisplay( of, "u %x", u );
					 if( u [_WIDTH - 1 ] == 1 || u > 32'h10041893 )
				 	 begin
					 	ready = 1;
						state = 0; 
						$fdisplay( of, "state1: u not good" );
					 end
					 else
					 begin
						$fdisplay( of, "state1: det and u ok, going to state2 %x %x ", u, det );
						state = 2;	 
						sel1 = 1;
				 		sel2 = 0;
				     end
				 end
			  end
			2:
			  begin	
				 if( v[ _WIDTH - 1 ] == 1 || ( u + v ) > 32'h10041893 )
				 begin	
					ready = 1;
					state = 0;	 
					$fdisplay( of, "state2: u or v not good u: %x v: %x", u, v );
				 end
				 else
				 begin
					$fdisplay( of, "state2: u and v ok going to state3 %x %x", u, v );
					state = 3; 
					sel1 = 1;
				 	sel2 = 1;
				 end
			  end
			3:
			  begin		
				if( tDF > 32'h00041893 && tDF < 32'h20000000 )
				begin
					ready = 1;
					t = tDF;
					code = 1; 
					state = 0; 
					$fdisplay( of, "state3: t good. Waiting for reset" );
				end	
				else
				begin
					state = 0;
					ready = 1;
					$fdisplay( of, "state3: t not good %x", tDF );
				end
			  end	
		  endcase
        end	 
		prv_start = start;
	end
endmodule