-------------------------------------------------------------------------------
-- system_stub.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity system_stub is
  port (
    fpga_0_RS232_Uart_1_RX_pin : in std_logic;
    fpga_0_RS232_Uart_1_TX_pin : out std_logic;
    fpga_0_LEDs_8Bit_GPIO_IO_pin : inout std_logic_vector(0 to 7);
    fpga_0_LEDs_Positions_GPIO_IO_pin : inout std_logic_vector(0 to 4);
    fpga_0_Push_Buttons_5Bit_GPIO_IO_pin : inout std_logic_vector(0 to 4);
    fpga_0_DIP_Switches_8Bit_GPIO_IO_pin : inout std_logic_vector(0 to 7);
    fpga_0_SRAM_Mem_A_pin : out std_logic_vector(7 to 30);
    fpga_0_SRAM_Mem_CEN_pin : out std_logic;
    fpga_0_SRAM_Mem_OEN_pin : out std_logic;
    fpga_0_SRAM_Mem_WEN_pin : out std_logic;
    fpga_0_SRAM_Mem_BEN_pin : out std_logic_vector(0 to 3);
    fpga_0_SRAM_Mem_ADV_LDN_pin : out std_logic;
    fpga_0_SRAM_Mem_DQ_pin : inout std_logic_vector(0 to 31);
    fpga_0_SRAM_ZBT_CLK_OUT_pin : out std_logic;
    fpga_0_SRAM_ZBT_CLK_FB_pin : in std_logic;
    fpga_0_DDR2_SDRAM_DDR2_DQ_pin : inout std_logic_vector(63 downto 0);
    fpga_0_DDR2_SDRAM_DDR2_DQS_pin : inout std_logic_vector(7 downto 0);
    fpga_0_DDR2_SDRAM_DDR2_DQS_N_pin : inout std_logic_vector(7 downto 0);
    fpga_0_DDR2_SDRAM_DDR2_A_pin : out std_logic_vector(12 downto 0);
    fpga_0_DDR2_SDRAM_DDR2_BA_pin : out std_logic_vector(1 downto 0);
    fpga_0_DDR2_SDRAM_DDR2_RAS_N_pin : out std_logic;
    fpga_0_DDR2_SDRAM_DDR2_CAS_N_pin : out std_logic;
    fpga_0_DDR2_SDRAM_DDR2_WE_N_pin : out std_logic;
    fpga_0_DDR2_SDRAM_DDR2_CS_N_pin : out std_logic;
    fpga_0_DDR2_SDRAM_DDR2_ODT_pin : out std_logic_vector(1 downto 0);
    fpga_0_DDR2_SDRAM_DDR2_CKE_pin : out std_logic;
    fpga_0_DDR2_SDRAM_DDR2_DM_pin : out std_logic_vector(7 downto 0);
    fpga_0_DDR2_SDRAM_DDR2_CK_pin : out std_logic_vector(1 downto 0);
    fpga_0_DDR2_SDRAM_DDR2_CK_N_pin : out std_logic_vector(1 downto 0);
    fpga_0_clk_1_sys_clk_pin : in std_logic;
    fpga_0_rst_1_sys_rst_pin : in std_logic;
    xps_tft_0_TFT_VSYNC_pin : out std_logic;
    xps_tft_0_TFT_HSYNC_pin : out std_logic;
    xps_tft_0_TFT_DE_pin : out std_logic;
    xps_tft_0_TFT_DVI_CLK_P_pin : out std_logic;
    xps_tft_0_TFT_DVI_CLK_N_pin : out std_logic;
    xps_tft_0_TFT_DVI_DATA_pin : out std_logic_vector(11 downto 0);
    xps_tft_0_TFT_IIC_SCL : inout std_logic;
    xps_tft_0_TFT_IIC_SDA : inout std_logic;
    vga_reset_pin : out std_logic
  );
end system_stub;

architecture STRUCTURE of system_stub is

  component system is
    port (
      fpga_0_RS232_Uart_1_RX_pin : in std_logic;
      fpga_0_RS232_Uart_1_TX_pin : out std_logic;
      fpga_0_LEDs_8Bit_GPIO_IO_pin : inout std_logic_vector(0 to 7);
      fpga_0_LEDs_Positions_GPIO_IO_pin : inout std_logic_vector(0 to 4);
      fpga_0_Push_Buttons_5Bit_GPIO_IO_pin : inout std_logic_vector(0 to 4);
      fpga_0_DIP_Switches_8Bit_GPIO_IO_pin : inout std_logic_vector(0 to 7);
      fpga_0_SRAM_Mem_A_pin : out std_logic_vector(7 to 30);
      fpga_0_SRAM_Mem_CEN_pin : out std_logic;
      fpga_0_SRAM_Mem_OEN_pin : out std_logic;
      fpga_0_SRAM_Mem_WEN_pin : out std_logic;
      fpga_0_SRAM_Mem_BEN_pin : out std_logic_vector(0 to 3);
      fpga_0_SRAM_Mem_ADV_LDN_pin : out std_logic;
      fpga_0_SRAM_Mem_DQ_pin : inout std_logic_vector(0 to 31);
      fpga_0_SRAM_ZBT_CLK_OUT_pin : out std_logic;
      fpga_0_SRAM_ZBT_CLK_FB_pin : in std_logic;
      fpga_0_DDR2_SDRAM_DDR2_DQ_pin : inout std_logic_vector(63 downto 0);
      fpga_0_DDR2_SDRAM_DDR2_DQS_pin : inout std_logic_vector(7 downto 0);
      fpga_0_DDR2_SDRAM_DDR2_DQS_N_pin : inout std_logic_vector(7 downto 0);
      fpga_0_DDR2_SDRAM_DDR2_A_pin : out std_logic_vector(12 downto 0);
      fpga_0_DDR2_SDRAM_DDR2_BA_pin : out std_logic_vector(1 downto 0);
      fpga_0_DDR2_SDRAM_DDR2_RAS_N_pin : out std_logic;
      fpga_0_DDR2_SDRAM_DDR2_CAS_N_pin : out std_logic;
      fpga_0_DDR2_SDRAM_DDR2_WE_N_pin : out std_logic;
      fpga_0_DDR2_SDRAM_DDR2_CS_N_pin : out std_logic;
      fpga_0_DDR2_SDRAM_DDR2_ODT_pin : out std_logic_vector(1 downto 0);
      fpga_0_DDR2_SDRAM_DDR2_CKE_pin : out std_logic;
      fpga_0_DDR2_SDRAM_DDR2_DM_pin : out std_logic_vector(7 downto 0);
      fpga_0_DDR2_SDRAM_DDR2_CK_pin : out std_logic_vector(1 downto 0);
      fpga_0_DDR2_SDRAM_DDR2_CK_N_pin : out std_logic_vector(1 downto 0);
      fpga_0_clk_1_sys_clk_pin : in std_logic;
      fpga_0_rst_1_sys_rst_pin : in std_logic;
      xps_tft_0_TFT_VSYNC_pin : out std_logic;
      xps_tft_0_TFT_HSYNC_pin : out std_logic;
      xps_tft_0_TFT_DE_pin : out std_logic;
      xps_tft_0_TFT_DVI_CLK_P_pin : out std_logic;
      xps_tft_0_TFT_DVI_CLK_N_pin : out std_logic;
      xps_tft_0_TFT_DVI_DATA_pin : out std_logic_vector(11 downto 0);
      xps_tft_0_TFT_IIC_SCL : inout std_logic;
      xps_tft_0_TFT_IIC_SDA : inout std_logic;
      vga_reset_pin : out std_logic
    );
  end component;

  attribute BOX_TYPE : STRING;
  attribute BOX_TYPE of system : component is "user_black_box";

begin

  system_i : system
    port map (
      fpga_0_RS232_Uart_1_RX_pin => fpga_0_RS232_Uart_1_RX_pin,
      fpga_0_RS232_Uart_1_TX_pin => fpga_0_RS232_Uart_1_TX_pin,
      fpga_0_LEDs_8Bit_GPIO_IO_pin => fpga_0_LEDs_8Bit_GPIO_IO_pin,
      fpga_0_LEDs_Positions_GPIO_IO_pin => fpga_0_LEDs_Positions_GPIO_IO_pin,
      fpga_0_Push_Buttons_5Bit_GPIO_IO_pin => fpga_0_Push_Buttons_5Bit_GPIO_IO_pin,
      fpga_0_DIP_Switches_8Bit_GPIO_IO_pin => fpga_0_DIP_Switches_8Bit_GPIO_IO_pin,
      fpga_0_SRAM_Mem_A_pin => fpga_0_SRAM_Mem_A_pin,
      fpga_0_SRAM_Mem_CEN_pin => fpga_0_SRAM_Mem_CEN_pin,
      fpga_0_SRAM_Mem_OEN_pin => fpga_0_SRAM_Mem_OEN_pin,
      fpga_0_SRAM_Mem_WEN_pin => fpga_0_SRAM_Mem_WEN_pin,
      fpga_0_SRAM_Mem_BEN_pin => fpga_0_SRAM_Mem_BEN_pin,
      fpga_0_SRAM_Mem_ADV_LDN_pin => fpga_0_SRAM_Mem_ADV_LDN_pin,
      fpga_0_SRAM_Mem_DQ_pin => fpga_0_SRAM_Mem_DQ_pin,
      fpga_0_SRAM_ZBT_CLK_OUT_pin => fpga_0_SRAM_ZBT_CLK_OUT_pin,
      fpga_0_SRAM_ZBT_CLK_FB_pin => fpga_0_SRAM_ZBT_CLK_FB_pin,
      fpga_0_DDR2_SDRAM_DDR2_DQ_pin => fpga_0_DDR2_SDRAM_DDR2_DQ_pin,
      fpga_0_DDR2_SDRAM_DDR2_DQS_pin => fpga_0_DDR2_SDRAM_DDR2_DQS_pin,
      fpga_0_DDR2_SDRAM_DDR2_DQS_N_pin => fpga_0_DDR2_SDRAM_DDR2_DQS_N_pin,
      fpga_0_DDR2_SDRAM_DDR2_A_pin => fpga_0_DDR2_SDRAM_DDR2_A_pin,
      fpga_0_DDR2_SDRAM_DDR2_BA_pin => fpga_0_DDR2_SDRAM_DDR2_BA_pin,
      fpga_0_DDR2_SDRAM_DDR2_RAS_N_pin => fpga_0_DDR2_SDRAM_DDR2_RAS_N_pin,
      fpga_0_DDR2_SDRAM_DDR2_CAS_N_pin => fpga_0_DDR2_SDRAM_DDR2_CAS_N_pin,
      fpga_0_DDR2_SDRAM_DDR2_WE_N_pin => fpga_0_DDR2_SDRAM_DDR2_WE_N_pin,
      fpga_0_DDR2_SDRAM_DDR2_CS_N_pin => fpga_0_DDR2_SDRAM_DDR2_CS_N_pin,
      fpga_0_DDR2_SDRAM_DDR2_ODT_pin => fpga_0_DDR2_SDRAM_DDR2_ODT_pin,
      fpga_0_DDR2_SDRAM_DDR2_CKE_pin => fpga_0_DDR2_SDRAM_DDR2_CKE_pin,
      fpga_0_DDR2_SDRAM_DDR2_DM_pin => fpga_0_DDR2_SDRAM_DDR2_DM_pin,
      fpga_0_DDR2_SDRAM_DDR2_CK_pin => fpga_0_DDR2_SDRAM_DDR2_CK_pin,
      fpga_0_DDR2_SDRAM_DDR2_CK_N_pin => fpga_0_DDR2_SDRAM_DDR2_CK_N_pin,
      fpga_0_clk_1_sys_clk_pin => fpga_0_clk_1_sys_clk_pin,
      fpga_0_rst_1_sys_rst_pin => fpga_0_rst_1_sys_rst_pin,
      xps_tft_0_TFT_VSYNC_pin => xps_tft_0_TFT_VSYNC_pin,
      xps_tft_0_TFT_HSYNC_pin => xps_tft_0_TFT_HSYNC_pin,
      xps_tft_0_TFT_DE_pin => xps_tft_0_TFT_DE_pin,
      xps_tft_0_TFT_DVI_CLK_P_pin => xps_tft_0_TFT_DVI_CLK_P_pin,
      xps_tft_0_TFT_DVI_CLK_N_pin => xps_tft_0_TFT_DVI_CLK_N_pin,
      xps_tft_0_TFT_DVI_DATA_pin => xps_tft_0_TFT_DVI_DATA_pin,
      xps_tft_0_TFT_IIC_SCL => xps_tft_0_TFT_IIC_SCL,
      xps_tft_0_TFT_IIC_SDA => xps_tft_0_TFT_IIC_SDA,
      vga_reset_pin => vga_reset_pin
    );

end architecture STRUCTURE;

