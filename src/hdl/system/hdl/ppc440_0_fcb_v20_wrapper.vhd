-------------------------------------------------------------------------------
-- ppc440_0_fcb_v20_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library fcb_v20_v1_00_a;
use fcb_v20_v1_00_a.all;

entity ppc440_0_fcb_v20_wrapper is
  port (
    FCB_CLK : in std_logic;
    SYS_RST : in std_logic;
    FCB_RST : out std_logic;
    M_DECUDI : in std_logic_vector(0 to 3);
    M_DECUDIVALID : in std_logic_vector(0 to 0);
    M_ENDIAN : in std_logic_vector(0 to 0);
    M_FLUSH : in std_logic_vector(0 to 0);
    M_INSTRUCTION : in std_logic_vector(0 to 31);
    M_INSTRVALID : in std_logic;
    M_LOADBYTEADDR : in std_logic_vector(0 to 3);
    M_LOADDATA : in std_logic_vector(0 to 127);
    M_LOADDVALID : in std_logic;
    M_OPERANDVALID : in std_logic_vector(0 to 0);
    M_RADATA : in std_logic_vector(0 to 31);
    M_RBDATA : in std_logic_vector(0 to 31);
    M_WRITEBACKOK : in std_logic_vector(0 to 0);
    M_DECFPUOP : in std_logic;
    M_DECLOAD : in std_logic;
    M_DECSTORE : in std_logic;
    M_DECLDSTXFERSIZE : in std_logic_vector(0 to 2);
    M_DECNONAUTON : in std_logic;
    M_NEXTINSTRREADY : in std_logic;
    M_MSRFE0 : in std_logic;
    M_MSRFE1 : in std_logic;
    FCB_CR : out std_logic_vector(0 to 3);
    FCB_DONE : out std_logic;
    FCB_EXCEPTION : out std_logic;
    FCB_RESULT : out std_logic_vector(0 to 31);
    FCB_RESULTVALID : out std_logic;
    FCB_SLEEPNOTREADY : out std_logic;
    FCB_STOREDATA : out std_logic_vector(0 to 127);
    FCB_CONFIRMINSTR : out std_logic;
    FCB_FPSCRFEX : out std_logic;
    FCB_DECUDI : out std_logic_vector(0 to 3);
    FCB_DECUDIVALID : out std_logic;
    FCB_ENDIAN : out std_logic;
    FCB_FLUSH : out std_logic;
    FCB_INSTRUCTION : out std_logic_vector(0 to 31);
    FCB_INSTRVALID : out std_logic;
    FCB_LOADBYTEADDR : out std_logic_vector(0 to 3);
    FCB_LOADDATA : out std_logic_vector(0 to 127);
    FCB_LOADDVALID : out std_logic;
    FCB_OPERANDVALID : out std_logic;
    FCB_RADATA : out std_logic_vector(0 to 31);
    FCB_RBDATA : out std_logic_vector(0 to 31);
    FCB_WRITEBACKOK : out std_logic;
    FCB_DECFPUOP : out std_logic;
    FCB_DECLOAD : out std_logic;
    FCB_DECSTORE : out std_logic;
    FCB_DECLDSTXFERSIZE : out std_logic_vector(0 to 2);
    FCB_DECNONAUTON : out std_logic;
    FCB_NEXTINSTRREADY : out std_logic;
    FCB_MSRFE0 : out std_logic;
    FCB_MSRFE1 : out std_logic;
    Sl_CR : in std_logic_vector(0 to 3);
    Sl_DONE : in std_logic_vector(0 to 0);
    Sl_EXCEPTION : in std_logic_vector(0 to 0);
    Sl_RESULT : in std_logic_vector(0 to 31);
    Sl_RESULTVALID : in std_logic_vector(0 to 0);
    Sl_SLEEPNOTREADY : in std_logic_vector(0 to 0);
    Sl_STOREDATA : in std_logic_vector(0 to 127);
    Sl_CONFIRMINSTR : in std_logic_vector(0 to 0);
    Sl_FPSCRFEX : in std_logic_vector(0 to 0)
  );

  attribute x_core_info : STRING;
  attribute x_core_info of ppc440_0_fcb_v20_wrapper : entity is "fcb_v20_v1_00_a";

end ppc440_0_fcb_v20_wrapper;

architecture STRUCTURE of ppc440_0_fcb_v20_wrapper is

  component fcb_v20 is
    generic (
      C_FCB2_NUM_MASTERS : INTEGER;
      C_FCB2_NUM_SLAVES : INTEGER;
      C_DATA_WIDTH : INTEGER;
      C_EXT_RESET_HIGH : INTEGER;
      C_DECUDI_WIDTH : INTEGER;
      C_LBA_WIDTH : INTEGER;
      C_LDSDATA_WIDTH : INTEGER
    );
    port (
      FCB_CLK : in std_logic;
      SYS_RST : in std_logic;
      FCB_RST : out std_logic;
      M_DECUDI : in std_logic_vector(0 to (C_DECUDI_WIDTH-1));
      M_DECUDIVALID : in std_logic_vector(0 to (C_FCB2_NUM_MASTERS-1));
      M_ENDIAN : in std_logic_vector(0 to (C_FCB2_NUM_MASTERS-1));
      M_FLUSH : in std_logic_vector(0 to (C_FCB2_NUM_MASTERS-1));
      M_INSTRUCTION : in std_logic_vector(0 to ((C_DATA_WIDTH*C_FCB2_NUM_MASTERS)-1));
      M_INSTRVALID : in std_logic;
      M_LOADBYTEADDR : in std_logic_vector(0 to (C_LBA_WIDTH-1));
      M_LOADDATA : in std_logic_vector(0 to (C_LDSDATA_WIDTH-1));
      M_LOADDVALID : in std_logic;
      M_OPERANDVALID : in std_logic_vector(0 to (C_FCB2_NUM_MASTERS-1));
      M_RADATA : in std_logic_vector(0 to ((C_DATA_WIDTH*C_FCB2_NUM_MASTERS)-1));
      M_RBDATA : in std_logic_vector(0 to ((C_DATA_WIDTH*C_FCB2_NUM_MASTERS)-1));
      M_WRITEBACKOK : in std_logic_vector(0 to (C_FCB2_NUM_MASTERS-1));
      M_DECFPUOP : in std_logic;
      M_DECLOAD : in std_logic;
      M_DECSTORE : in std_logic;
      M_DECLDSTXFERSIZE : in std_logic_vector(0 to 2);
      M_DECNONAUTON : in std_logic;
      M_NEXTINSTRREADY : in std_logic;
      M_MSRFE0 : in std_logic;
      M_MSRFE1 : in std_logic;
      FCB_CR : out std_logic_vector(0 to 3);
      FCB_DONE : out std_logic;
      FCB_EXCEPTION : out std_logic;
      FCB_RESULT : out std_logic_vector(0 to C_DATA_WIDTH-1);
      FCB_RESULTVALID : out std_logic;
      FCB_SLEEPNOTREADY : out std_logic;
      FCB_STOREDATA : out std_logic_vector(0 to (C_LDSDATA_WIDTH-1));
      FCB_CONFIRMINSTR : out std_logic;
      FCB_FPSCRFEX : out std_logic;
      FCB_DECUDI : out std_logic_vector(0 to 3);
      FCB_DECUDIVALID : out std_logic;
      FCB_ENDIAN : out std_logic;
      FCB_FLUSH : out std_logic;
      FCB_INSTRUCTION : out std_logic_vector(0 to C_DATA_WIDTH-1);
      FCB_INSTRVALID : out std_logic;
      FCB_LOADBYTEADDR : out std_logic_vector(0 to C_DATA_WIDTH/8-1);
      FCB_LOADDATA : out std_logic_vector(0 to C_LDSDATA_WIDTH-1);
      FCB_LOADDVALID : out std_logic;
      FCB_OPERANDVALID : out std_logic;
      FCB_RADATA : out std_logic_vector(0 to C_DATA_WIDTH-1);
      FCB_RBDATA : out std_logic_vector(0 to C_DATA_WIDTH-1);
      FCB_WRITEBACKOK : out std_logic;
      FCB_DECFPUOP : out std_logic;
      FCB_DECLOAD : out std_logic;
      FCB_DECSTORE : out std_logic;
      FCB_DECLDSTXFERSIZE : out std_logic_vector(0 to 2);
      FCB_DECNONAUTON : out std_logic;
      FCB_NEXTINSTRREADY : out std_logic;
      FCB_MSRFE0 : out std_logic;
      FCB_MSRFE1 : out std_logic;
      Sl_CR : in std_logic_vector(0 to ((4*C_FCB2_NUM_SLAVES)-1));
      Sl_DONE : in std_logic_vector(0 to (C_FCB2_NUM_SLAVES-1));
      Sl_EXCEPTION : in std_logic_vector(0 to (C_FCB2_NUM_SLAVES-1));
      Sl_RESULT : in std_logic_vector(0 to ((C_DATA_WIDTH*C_FCB2_NUM_SLAVES)-1));
      Sl_RESULTVALID : in std_logic_vector(0 to (C_FCB2_NUM_SLAVES-1));
      Sl_SLEEPNOTREADY : in std_logic_vector(0 to (C_FCB2_NUM_SLAVES-1));
      Sl_STOREDATA : in std_logic_vector(0 to ((C_LDSDATA_WIDTH*C_FCB2_NUM_SLAVES)-1));
      Sl_CONFIRMINSTR : in std_logic_vector(0 to (C_FCB2_NUM_SLAVES-1));
      Sl_FPSCRFEX : in std_logic_vector(0 to (C_FCB2_NUM_SLAVES-1))
    );
  end component;

begin

  ppc440_0_fcb_v20 : fcb_v20
    generic map (
      C_FCB2_NUM_MASTERS => 1,
      C_FCB2_NUM_SLAVES => 1,
      C_DATA_WIDTH => 32,
      C_EXT_RESET_HIGH => 1,
      C_DECUDI_WIDTH => 4,
      C_LBA_WIDTH => 4,
      C_LDSDATA_WIDTH => 128
    )
    port map (
      FCB_CLK => FCB_CLK,
      SYS_RST => SYS_RST,
      FCB_RST => FCB_RST,
      M_DECUDI => M_DECUDI,
      M_DECUDIVALID => M_DECUDIVALID,
      M_ENDIAN => M_ENDIAN,
      M_FLUSH => M_FLUSH,
      M_INSTRUCTION => M_INSTRUCTION,
      M_INSTRVALID => M_INSTRVALID,
      M_LOADBYTEADDR => M_LOADBYTEADDR,
      M_LOADDATA => M_LOADDATA,
      M_LOADDVALID => M_LOADDVALID,
      M_OPERANDVALID => M_OPERANDVALID,
      M_RADATA => M_RADATA,
      M_RBDATA => M_RBDATA,
      M_WRITEBACKOK => M_WRITEBACKOK,
      M_DECFPUOP => M_DECFPUOP,
      M_DECLOAD => M_DECLOAD,
      M_DECSTORE => M_DECSTORE,
      M_DECLDSTXFERSIZE => M_DECLDSTXFERSIZE,
      M_DECNONAUTON => M_DECNONAUTON,
      M_NEXTINSTRREADY => M_NEXTINSTRREADY,
      M_MSRFE0 => M_MSRFE0,
      M_MSRFE1 => M_MSRFE1,
      FCB_CR => FCB_CR,
      FCB_DONE => FCB_DONE,
      FCB_EXCEPTION => FCB_EXCEPTION,
      FCB_RESULT => FCB_RESULT,
      FCB_RESULTVALID => FCB_RESULTVALID,
      FCB_SLEEPNOTREADY => FCB_SLEEPNOTREADY,
      FCB_STOREDATA => FCB_STOREDATA,
      FCB_CONFIRMINSTR => FCB_CONFIRMINSTR,
      FCB_FPSCRFEX => FCB_FPSCRFEX,
      FCB_DECUDI => FCB_DECUDI,
      FCB_DECUDIVALID => FCB_DECUDIVALID,
      FCB_ENDIAN => FCB_ENDIAN,
      FCB_FLUSH => FCB_FLUSH,
      FCB_INSTRUCTION => FCB_INSTRUCTION,
      FCB_INSTRVALID => FCB_INSTRVALID,
      FCB_LOADBYTEADDR => FCB_LOADBYTEADDR,
      FCB_LOADDATA => FCB_LOADDATA,
      FCB_LOADDVALID => FCB_LOADDVALID,
      FCB_OPERANDVALID => FCB_OPERANDVALID,
      FCB_RADATA => FCB_RADATA,
      FCB_RBDATA => FCB_RBDATA,
      FCB_WRITEBACKOK => FCB_WRITEBACKOK,
      FCB_DECFPUOP => FCB_DECFPUOP,
      FCB_DECLOAD => FCB_DECLOAD,
      FCB_DECSTORE => FCB_DECSTORE,
      FCB_DECLDSTXFERSIZE => FCB_DECLDSTXFERSIZE,
      FCB_DECNONAUTON => FCB_DECNONAUTON,
      FCB_NEXTINSTRREADY => FCB_NEXTINSTRREADY,
      FCB_MSRFE0 => FCB_MSRFE0,
      FCB_MSRFE1 => FCB_MSRFE1,
      Sl_CR => Sl_CR,
      Sl_DONE => Sl_DONE,
      Sl_EXCEPTION => Sl_EXCEPTION,
      Sl_RESULT => Sl_RESULT,
      Sl_RESULTVALID => Sl_RESULTVALID,
      Sl_SLEEPNOTREADY => Sl_SLEEPNOTREADY,
      Sl_STOREDATA => Sl_STOREDATA,
      Sl_CONFIRMINSTR => Sl_CONFIRMINSTR,
      Sl_FPSCRFEX => Sl_FPSCRFEX
    );

end architecture STRUCTURE;

