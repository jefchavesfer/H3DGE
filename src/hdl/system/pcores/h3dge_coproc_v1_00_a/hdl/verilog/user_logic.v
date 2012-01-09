//----------------------------------------------------------------------------
// user_logic.vhd - module
//----------------------------------------------------------------------------
//
// ***************************************************************************
// ** Copyright (c) 1995-2010 Xilinx, Inc.  All rights reserved.            **
// **                                                                       **
// ** Xilinx, Inc.                                                          **
// ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
// ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
// ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
// ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
// ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
// ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
// ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
// ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
// ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
// ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
// ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
// ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
// ** FOR A PARTICULAR PURPOSE.                                             **
// **                                                                       **
// ***************************************************************************
//
//----------------------------------------------------------------------------
// Filename:          user_logic.vhd
// Version:           1.00.a
// Description:       User logic module.
// Date:              Wed Nov 30 20:33:35 2011 (by Create and Import Peripheral Wizard)
// Verilog Standard:  Verilog-2001
//----------------------------------------------------------------------------
// Naming Conventions:
//   active low signals:                    "*_n"
//   clock signals:                         "clk", "clk_div#", "clk_#x"
//   reset signals:                         "rst", "rst_n"
//   generics:                              "C_*"
//   user defined types:                    "*_TYPE"
//   state machine next state:              "*_ns"
//   state machine current state:           "*_cs"
//   combinatorial signals:                 "*_com"
//   pipelined or register delay signals:   "*_d#"
//   counter signals:                       "*cnt*"
//   clock enable signals:                  "*_ce"
//   internal version of output port:       "*_i"
//   device pins:                           "*_pin"
//   ports:                                 "- Names begin with Uppercase"
//   processes:                             "*_PROCESS"
//   component instantiations:              "<ENTITY_>I_<#|FUNC>"
//----------------------------------------------------------------------------

module user_logic
  (
   // -- ADD USER PORTS BELOW THIS LINE ---------------

   // -- ADD USER PORTS ABOVE THIS LINE ---------------

   // -- DO NOT EDIT BELOW THIS LINE ------------------
   // -- Bus protocol ports, do not add to or delete 
   Bus2IP_Clk,                     // Bus to IP clock
   Bus2IP_Reset,                   // Bus to IP reset
   Bus2IP_Data,                    // Bus to IP data bus
   Bus2IP_BE,                      // Bus to IP byte enables
   Bus2IP_RdCE,                    // Bus to IP read chip enable
   Bus2IP_WrCE,                    // Bus to IP write chip enable
   IP2Bus_Data,                    // IP to Bus data bus
   IP2Bus_RdAck,                   // IP to Bus read transfer acknowledgement
   IP2Bus_WrAck,                   // IP to Bus write transfer acknowledgement
   IP2Bus_Error,                   // IP to Bus error response
   IP2Bus_MstRd_Req,               // IP to Bus master read request
   IP2Bus_MstWr_Req,               // IP to Bus master write request
   IP2Bus_Mst_Addr,                // IP to Bus master address bus
   IP2Bus_Mst_BE,                  // IP to Bus master byte enables
   IP2Bus_Mst_Length,              // IP to Bus master transfer length
   IP2Bus_Mst_Type,                // IP to Bus master transfer type
   IP2Bus_Mst_Lock,                // IP to Bus master lock
   IP2Bus_Mst_Reset,               // IP to Bus master reset
   Bus2IP_Mst_CmdAck,              // Bus to IP master command acknowledgement
   Bus2IP_Mst_Cmplt,               // Bus to IP master transfer completion
   Bus2IP_Mst_Error,               // Bus to IP master error response
   Bus2IP_Mst_Rearbitrate,         // Bus to IP master re-arbitrate
   Bus2IP_Mst_Cmd_Timeout,         // Bus to IP master command timeout
   Bus2IP_MstRd_d,                 // Bus to IP master read data bus
   Bus2IP_MstRd_rem,               // Bus to IP master read remainder
   Bus2IP_MstRd_sof_n,             // Bus to IP master read start of frame
   Bus2IP_MstRd_eof_n,             // Bus to IP master read end of frame
   Bus2IP_MstRd_src_rdy_n,         // Bus to IP master read source ready
   Bus2IP_MstRd_src_dsc_n,         // Bus to IP master read source discontinue
   IP2Bus_MstRd_dst_rdy_n,         // IP to Bus master read destination ready
   IP2Bus_MstRd_dst_dsc_n,         // IP to Bus master read destination discontinue
   IP2Bus_MstWr_d,                 // IP to Bus master write data bus
   IP2Bus_MstWr_rem,               // IP to Bus master write remainder
   IP2Bus_MstWr_sof_n,             // IP to Bus master write start of frame
   IP2Bus_MstWr_eof_n,             // IP to Bus master write end of frame
   IP2Bus_MstWr_src_rdy_n,         // IP to Bus master write source ready
   IP2Bus_MstWr_src_dsc_n,         // IP to Bus master write source discontinue
   Bus2IP_MstWr_dst_rdy_n,         // Bus to IP master write destination ready
   Bus2IP_MstWr_dst_dsc_n,         // Bus to IP master write destination discontinue
   IP2Bus_IntrEvent                // IP to Bus interrupt event
   // -- DO NOT EDIT ABOVE THIS LINE ------------------
   ); // user_logic

   // -- ADD USER PARAMETERS BELOW THIS LINE ------------
   // --USER parameters added here 
   // -- ADD USER PARAMETERS ABOVE THIS LINE ------------

   // -- DO NOT EDIT BELOW THIS LINE --------------------
   // -- Bus protocol parameters, do not add to or delete
   parameter C_SLV_DWIDTH                   = 32;
   parameter C_MST_AWIDTH                   = 32;
   parameter C_MST_DWIDTH                   = 32;
   parameter C_NUM_REG                      = 24;
   parameter C_NUM_INTR                     = 1;
   // -- DO NOT EDIT ABOVE THIS LINE --------------------

   // -- ADD USER PORTS BELOW THIS LINE -----------------

   // -- ADD USER PORTS ABOVE THIS LINE -----------------

   // -- DO NOT EDIT BELOW THIS LINE --------------------
   // -- Bus protocol ports, do not add to or delete
   input                                     Bus2IP_Clk;
   input                                     Bus2IP_Reset;
   input [0 : C_SLV_DWIDTH-1]                Bus2IP_Data;
   input [0 : C_SLV_DWIDTH/8-1]              Bus2IP_BE;
   input [0 : C_NUM_REG-1]                   Bus2IP_RdCE;
   input [0 : C_NUM_REG-1]                   Bus2IP_WrCE;
   output [0 : C_SLV_DWIDTH-1]               IP2Bus_Data;
   output                                    IP2Bus_RdAck;
   output                                    IP2Bus_WrAck;
   output                                    IP2Bus_Error;
   output                                    IP2Bus_MstRd_Req;
   output                                    IP2Bus_MstWr_Req;
   output [0 : C_MST_AWIDTH-1]               IP2Bus_Mst_Addr;
   output [0 : C_MST_DWIDTH/8-1]             IP2Bus_Mst_BE;
   output [0 : 11]                           IP2Bus_Mst_Length;
   output                                    IP2Bus_Mst_Type;
   output                                    IP2Bus_Mst_Lock;
   output                                    IP2Bus_Mst_Reset;
   input                                     Bus2IP_Mst_CmdAck;
   input                                     Bus2IP_Mst_Cmplt;
   input                                     Bus2IP_Mst_Error;
   input                                     Bus2IP_Mst_Rearbitrate;
   input                                     Bus2IP_Mst_Cmd_Timeout;
   input [0 : C_MST_DWIDTH-1]                Bus2IP_MstRd_d;
   input [0 : C_MST_DWIDTH/8-1]              Bus2IP_MstRd_rem;
   input                                     Bus2IP_MstRd_sof_n;
   input                                     Bus2IP_MstRd_eof_n;
   input                                     Bus2IP_MstRd_src_rdy_n;
   input                                     Bus2IP_MstRd_src_dsc_n;
   output                                    IP2Bus_MstRd_dst_rdy_n;
   output                                    IP2Bus_MstRd_dst_dsc_n;
   output [0 : C_MST_DWIDTH-1]               IP2Bus_MstWr_d;
   output [0 : C_MST_DWIDTH/8-1]             IP2Bus_MstWr_rem;
   output                                    IP2Bus_MstWr_sof_n;
   output                                    IP2Bus_MstWr_eof_n;
   output                                    IP2Bus_MstWr_src_rdy_n;
   output                                    IP2Bus_MstWr_src_dsc_n;
   input                                     Bus2IP_MstWr_dst_rdy_n;
   input                                     Bus2IP_MstWr_dst_dsc_n;
   output [0 : C_NUM_INTR-1]                 IP2Bus_IntrEvent;
   // -- DO NOT EDIT ABOVE THIS LINE --------------------

   //----------------------------------------------------------------------------
   // Implementation
   //----------------------------------------------------------------------------

   // --USER nets declarations added here, as needed for user logic
   integer                                   Bus2IP_Clk_div13_count;
   reg                                       Bus2IP_Clk_div13;
   wire [0 : C_SLV_DWIDTH-1]                 intersection_t;
   wire [0 : C_SLV_DWIDTH-1]                 intersection_x;
   wire [0 : C_SLV_DWIDTH-1]                 intersection_y;
   wire [0 : C_SLV_DWIDTH-1]                 intersection_z;
   wire [0 : 1]                              code;
   wire                                      ready;
   wire [0 : C_SLV_DWIDTH-1]                 status;
   wire [0 : C_SLV_DWIDTH-1]                 sum;

   // Nets for user logic slave model s/w accessible register example
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg0;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg1;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg2;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg3;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg4;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg5;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg6;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg7;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg8;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg9;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg10;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg11;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg12;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg13;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg14;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg15;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg16;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg17;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg18;
   reg [0 : C_SLV_DWIDTH-1]                  slv_reg19;
   wire [0 : 19]                             slv_reg_write_sel;
   wire [0 : 19]                             slv_reg_read_sel;
   reg [0 : C_SLV_DWIDTH-1]                  slv_ip2bus_data;
   wire                                      slv_read_ack;
   wire                                      slv_write_ack;
   integer                                   byte_index, bit_index;

   initial
     begin
        Bus2IP_Clk_div13_count = 0;
        Bus2IP_Clk_div13 = 0;
     end

   always @(posedge Bus2IP_Clk)
     if (Bus2IP_Clk_div13_count < 14)
       Bus2IP_Clk_div13_count = Bus2IP_Clk_div13_count + 1;
     else
       begin
          Bus2IP_Clk_div13 = ~Bus2IP_Clk_div13;
          Bus2IP_Clk_div13_count = 0;
       end

   // --USER logic implementation added here
   coproc myCoproc(Bus2IP_Clk_div13,
                   slv_reg0[C_SLV_DWIDTH - 2],
                   slv_reg0[C_SLV_DWIDTH - 1],
                   slv_reg1,
                   slv_reg2,
                   slv_reg3,
                   slv_reg4,
                   slv_reg5,
                   slv_reg6,
                   slv_reg7,
                   slv_reg8,
                   slv_reg9,
                   slv_reg10,
                   slv_reg11,
                   slv_reg12,
                   slv_reg13,
                   slv_reg14,
                   slv_reg15,
                   ready,
                   code,
                   intersection_t,
                   intersection_x,
                   intersection_y,
                   intersection_z);

   assign
     status[C_SLV_DWIDTH - 1] = ready,
     status[C_SLV_DWIDTH - 3 : C_SLV_DWIDTH - 2] = code,
     status[0 : C_SLV_DWIDTH - 4] = 0;

   adder myAdder(slv_reg19, slv_reg18, sum);

   // ------------------------------------------------------
   // Example code to read/write user logic slave model s/w accessible registers
   // 
   // Note:
   // The example code presented here is to show you one way of reading/writing
   // software accessible registers implemented in the user logic slave model.
   // Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
   // to one software accessible register by the top level template. For example,
   // if you have four 32 bit software accessible registers in the user logic,
   // you are basically operating on the following memory mapped registers:
   // 
   //    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
   //                     "1000"   C_BASEADDR + 0x0
   //                     "0100"   C_BASEADDR + 0x4
   //                     "0010"   C_BASEADDR + 0x8
   //                     "0001"   C_BASEADDR + 0xC
   // 
   // ------------------------------------------------------

   assign
     slv_reg_write_sel = Bus2IP_WrCE[0:19],
     slv_reg_read_sel  = Bus2IP_RdCE[0:19],
     slv_write_ack     = Bus2IP_WrCE[0] || Bus2IP_WrCE[1] || Bus2IP_WrCE[2] || Bus2IP_WrCE[3] || Bus2IP_WrCE[4] || Bus2IP_WrCE[5] || Bus2IP_WrCE[6] || Bus2IP_WrCE[7] || Bus2IP_WrCE[8] || Bus2IP_WrCE[9] || Bus2IP_WrCE[10] || Bus2IP_WrCE[11] || Bus2IP_WrCE[12] || Bus2IP_WrCE[13] || Bus2IP_WrCE[14] || Bus2IP_WrCE[15] || Bus2IP_WrCE[16] || Bus2IP_WrCE[17] || Bus2IP_WrCE[18] || Bus2IP_WrCE[19],
     slv_read_ack      = Bus2IP_RdCE[0] || Bus2IP_RdCE[1] || Bus2IP_RdCE[2] || Bus2IP_RdCE[3] || Bus2IP_RdCE[4] || Bus2IP_RdCE[5] || Bus2IP_RdCE[6] || Bus2IP_RdCE[7] || Bus2IP_RdCE[8] || Bus2IP_RdCE[9] || Bus2IP_RdCE[10] || Bus2IP_RdCE[11] || Bus2IP_RdCE[12] || Bus2IP_RdCE[13] || Bus2IP_RdCE[14] || Bus2IP_RdCE[15] || Bus2IP_RdCE[16] || Bus2IP_RdCE[17] || Bus2IP_RdCE[18] || Bus2IP_RdCE[19];

   // implement slave model register(s)
   always @( posedge Bus2IP_Clk )
     begin: SLAVE_REG_WRITE_PROC

        if ( Bus2IP_Reset == 1 )
          begin
             slv_reg0 <= 0;
             slv_reg1 <= 0;
             slv_reg2 <= 0;
             slv_reg3 <= 0;
             slv_reg4 <= 0;
             slv_reg5 <= 0;
             slv_reg6 <= 0;
             slv_reg7 <= 0;
             slv_reg8 <= 0;
             slv_reg9 <= 0;
             slv_reg10 <= 0;
             slv_reg11 <= 0;
             slv_reg12 <= 0;
             slv_reg13 <= 0;
             slv_reg14 <= 0;
             slv_reg15 <= 0;
             slv_reg16 <= 0;
             slv_reg17 <= 0;
             slv_reg18 <= 0;
             slv_reg19 <= 0;
          end
        else
          case ( slv_reg_write_sel )
            20'b10000000000000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg0[bit_index] <= Bus2IP_Data[bit_index];
            20'b01000000000000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg1[bit_index] <= Bus2IP_Data[bit_index];
            20'b00100000000000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg2[bit_index] <= Bus2IP_Data[bit_index];
            20'b00010000000000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg3[bit_index] <= Bus2IP_Data[bit_index];
            20'b00001000000000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg4[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000100000000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg5[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000010000000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg6[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000001000000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg7[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000100000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg8[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000010000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg9[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000001000000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg10[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000000100000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg11[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000000010000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg12[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000000001000000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg13[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000000000100000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg14[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000000000010000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg15[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000000000001000 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg16[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000000000000100 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg17[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000000000000010 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg18[bit_index] <= Bus2IP_Data[bit_index];
            20'b00000000000000000001 :
              for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
                if ( Bus2IP_BE[byte_index] == 1 )
                  for ( bit_index = byte_index*8; bit_index <= byte_index*8+7; bit_index = bit_index+1 )
                    slv_reg19[bit_index] <= Bus2IP_Data[bit_index];
            default : ;
          endcase

     end // SLAVE_REG_WRITE_PROC

   // implement slave model register read mux
   always @( slv_reg_read_sel or slv_reg0 or slv_reg1 or slv_reg2 or slv_reg3 or slv_reg4 or slv_reg5 or slv_reg6 or slv_reg7 or slv_reg8 or slv_reg9 or slv_reg10 or slv_reg11 or slv_reg12 or slv_reg13 or slv_reg14 or slv_reg15 or slv_reg16 or slv_reg17 or slv_reg18 or slv_reg19 )
     begin: SLAVE_REG_READ_PROC

        case ( slv_reg_read_sel )
          20'b10000000000000000000 : slv_ip2bus_data <= status;
          20'b01000000000000000000 : slv_ip2bus_data <= intersection_t;
          20'b00100000000000000000 : slv_ip2bus_data <= intersection_x;
          20'b00010000000000000000 : slv_ip2bus_data <= intersection_y;
          20'b00001000000000000000 : slv_ip2bus_data <= intersection_z;
          20'b00000100000000000000 : slv_ip2bus_data <= slv_reg5;
          20'b00000010000000000000 : slv_ip2bus_data <= slv_reg6;
          20'b00000001000000000000 : slv_ip2bus_data <= slv_reg7;
          20'b00000000100000000000 : slv_ip2bus_data <= slv_reg8;
          20'b00000000010000000000 : slv_ip2bus_data <= slv_reg9;
          20'b00000000001000000000 : slv_ip2bus_data <= slv_reg10;
          20'b00000000000100000000 : slv_ip2bus_data <= slv_reg11;
          20'b00000000000010000000 : slv_ip2bus_data <= slv_reg12;
          20'b00000000000001000000 : slv_ip2bus_data <= slv_reg13;
          20'b00000000000000100000 : slv_ip2bus_data <= slv_reg14;
          20'b00000000000000010000 : slv_ip2bus_data <= slv_reg15;
          20'b00000000000000001000 : slv_ip2bus_data <= slv_reg16;
          20'b00000000000000000100 : slv_ip2bus_data <= slv_reg17;
          20'b00000000000000000010 : slv_ip2bus_data <= slv_reg18;
          20'b00000000000000000001 : slv_ip2bus_data <= sum;
          default : slv_ip2bus_data <= 0;
        endcase

     end // SLAVE_REG_READ_PROC

   // ------------------------------------------------------------
   // Example code to drive IP to Bus signals
   // ------------------------------------------------------------

   assign IP2Bus_Data    = slv_ip2bus_data;
   assign IP2Bus_WrAck   = slv_write_ack;
   assign IP2Bus_RdAck   = slv_read_ack;
   assign IP2Bus_Error   = 0;

endmodule
