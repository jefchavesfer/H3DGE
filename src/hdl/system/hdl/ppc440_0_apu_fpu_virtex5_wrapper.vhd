-------------------------------------------------------------------------------
-- ppc440_0_apu_fpu_virtex5_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library apu_fpu_virtex5_v1_01_a;
use apu_fpu_virtex5_v1_01_a.all;

entity ppc440_0_apu_fpu_virtex5_wrapper is
  port (
    FCB_CLK : in std_logic;
    FCB_RST : in std_logic;
    FCMAPURESULT : out std_logic_vector(0 to 31);
    FCMAPUDONE : out std_logic;
    FCMAPUSLEEPNOTREADY : out std_logic;
    FCMAPURESULTVALID : out std_logic;
    FCMAPUCR : out std_logic_vector(0 to 3);
    FCMAPUEXCEPTION : out std_logic;
    FCMAPUSTOREDATA : out std_logic_vector(0 to 127);
    FCMAPUCONFIRMINSTR : out std_logic;
    FCMAPUFPSCRFEX : out std_logic;
    APUFCMINSTRUCTION : in std_logic_vector(0 to 31);
    APUFCMINSTRVALID : in std_logic;
    APUFCMRADATA : in std_logic_vector(0 to 31);
    APUFCMRBDATA : in std_logic_vector(0 to 31);
    APUFCMOPERANDVALID : in std_logic;
    APUFCMFLUSH : in std_logic;
    APUFCMWRITEBACKOK : in std_logic;
    APUFCMLOADDATA : in std_logic_vector(0 to 127);
    APUFCMLOADDVALID : in std_logic;
    APUFCMLOADBYTEADDR : in std_logic_vector(0 to 3);
    APUFCMENDIAN : in std_logic;
    APUFCMDECUDI : in std_logic_vector(0 to 3);
    APUFCMDECUDIVALID : in std_logic;
    APUFCMDECFPUOP : in std_logic;
    APUFCMDECLOAD : in std_logic;
    APUFCMDECSTORE : in std_logic;
    APUFCMDECLDSTXFERSIZE : in std_logic_vector(0 to 2);
    APUFCMDECNONAUTON : in std_logic;
    APUFCMNEXTINSTRREADY : in std_logic;
    APUFCMMSRFE0 : in std_logic;
    APUFCMMSRFE1 : in std_logic
  );

  attribute x_core_info : STRING;
  attribute x_core_info of ppc440_0_apu_fpu_virtex5_wrapper : entity is "apu_fpu_virtex5_v1_01_a";

end ppc440_0_apu_fpu_virtex5_wrapper;

architecture STRUCTURE of ppc440_0_apu_fpu_virtex5_wrapper is

  component apu_fpu_virtex5 is
    generic (
      C_USE_RLOCS : integer;
      C_DOUBLE_PRECISION : integer;
      C_LATENCY_CONF : integer
    );
    port (
      FCB_CLK : in std_logic;
      FCB_RST : in std_logic;
      FCMAPURESULT : out std_logic_vector(0 to 31);
      FCMAPUDONE : out std_logic;
      FCMAPUSLEEPNOTREADY : out std_logic;
      FCMAPURESULTVALID : out std_logic;
      FCMAPUCR : out std_logic_vector(0 to 3);
      FCMAPUEXCEPTION : out std_logic;
      FCMAPUSTOREDATA : out std_logic_vector(0 to 127);
      FCMAPUCONFIRMINSTR : out std_logic;
      FCMAPUFPSCRFEX : out std_logic;
      APUFCMINSTRUCTION : in std_logic_vector(0 to 31);
      APUFCMINSTRVALID : in std_logic;
      APUFCMRADATA : in std_logic_vector(0 to 31);
      APUFCMRBDATA : in std_logic_vector(0 to 31);
      APUFCMOPERANDVALID : in std_logic;
      APUFCMFLUSH : in std_logic;
      APUFCMWRITEBACKOK : in std_logic;
      APUFCMLOADDATA : in std_logic_vector(0 to 127);
      APUFCMLOADDVALID : in std_logic;
      APUFCMLOADBYTEADDR : in std_logic_vector(0 to 3);
      APUFCMENDIAN : in std_logic;
      APUFCMDECUDI : in std_logic_vector(0 to 3);
      APUFCMDECUDIVALID : in std_logic;
      APUFCMDECFPUOP : in std_logic;
      APUFCMDECLOAD : in std_logic;
      APUFCMDECSTORE : in std_logic;
      APUFCMDECLDSTXFERSIZE : in std_logic_vector(0 to 2);
      APUFCMDECNONAUTON : in std_logic;
      APUFCMNEXTINSTRREADY : in std_logic;
      APUFCMMSRFE0 : in std_logic;
      APUFCMMSRFE1 : in std_logic
    );
  end component;

begin

  ppc440_0_apu_fpu_virtex5 : apu_fpu_virtex5
    generic map (
      C_USE_RLOCS => 0,
      C_DOUBLE_PRECISION => 1,
      C_LATENCY_CONF => 1
    )
    port map (
      FCB_CLK => FCB_CLK,
      FCB_RST => FCB_RST,
      FCMAPURESULT => FCMAPURESULT,
      FCMAPUDONE => FCMAPUDONE,
      FCMAPUSLEEPNOTREADY => FCMAPUSLEEPNOTREADY,
      FCMAPURESULTVALID => FCMAPURESULTVALID,
      FCMAPUCR => FCMAPUCR,
      FCMAPUEXCEPTION => FCMAPUEXCEPTION,
      FCMAPUSTOREDATA => FCMAPUSTOREDATA,
      FCMAPUCONFIRMINSTR => FCMAPUCONFIRMINSTR,
      FCMAPUFPSCRFEX => FCMAPUFPSCRFEX,
      APUFCMINSTRUCTION => APUFCMINSTRUCTION,
      APUFCMINSTRVALID => APUFCMINSTRVALID,
      APUFCMRADATA => APUFCMRADATA,
      APUFCMRBDATA => APUFCMRBDATA,
      APUFCMOPERANDVALID => APUFCMOPERANDVALID,
      APUFCMFLUSH => APUFCMFLUSH,
      APUFCMWRITEBACKOK => APUFCMWRITEBACKOK,
      APUFCMLOADDATA => APUFCMLOADDATA,
      APUFCMLOADDVALID => APUFCMLOADDVALID,
      APUFCMLOADBYTEADDR => APUFCMLOADBYTEADDR,
      APUFCMENDIAN => APUFCMENDIAN,
      APUFCMDECUDI => APUFCMDECUDI,
      APUFCMDECUDIVALID => APUFCMDECUDIVALID,
      APUFCMDECFPUOP => APUFCMDECFPUOP,
      APUFCMDECLOAD => APUFCMDECLOAD,
      APUFCMDECSTORE => APUFCMDECSTORE,
      APUFCMDECLDSTXFERSIZE => APUFCMDECLDSTXFERSIZE,
      APUFCMDECNONAUTON => APUFCMDECNONAUTON,
      APUFCMNEXTINSTRREADY => APUFCMNEXTINSTRREADY,
      APUFCMMSRFE0 => APUFCMMSRFE0,
      APUFCMMSRFE1 => APUFCMMSRFE1
    );

end architecture STRUCTURE;

