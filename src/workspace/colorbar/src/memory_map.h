/******************************************************************************
*
*       XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*       AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*       SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*       OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*       APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*       THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*       AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*       FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*       WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*       IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*       REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*       INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*       FOR A PARTICULAR PURPOSE.
*
*       (c) Copyright 2007 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/

/**********************************************************************
* Header file that translates existing constants in xparameters.h
* into constants that are used by the software applications.
* Note: xparameters.h must be included before this file.
**********************************************************************/

#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#define UART_BASEADDR              XPAR_RS232_UART_1_BASEADDR
#define UART2_BASEADDR             XPAR_RS232_UART_2_BASEADDR
#define UART_BAUDRATE              9600
#define UART_CLOCK                 XPAR_XUARTNS550_CLOCK_HZ
#define TMRCTR_BASEADDR            XPAR_XPS_TIMER_0_BASEADDR

#define SYSACE_BASEADDR            XPAR_SYSACE_COMPACTFLASH_BASEADDR
#define SRAM_BASEADDR              XPAR_SRAM_MEM0_BASEADDR
#define FLASH_BASEADDR             XPAR_SRAM_MEM1_BASEADDR

#define PUSHB_CWSEN_BASEADDR       XPAR_PUSH_BUTTONS_5BIT_BASEADDR
#define PUSHB_CWSEN_DEVICE_ID      XPAR_PUSH_BUTTONS_5BIT_DEVICE_ID

#define DIPSW_BASEADDR             XPAR_DIP_SWITCHES_8BIT_BASEADDR
#define DIPSW_DEVICE_ID            XPAR_DIP_SWITCHES_8BIT_DEVICE_ID

#define LEDS_CWSEN_BASEADDR        XPAR_LEDS_POSITIONS_BASEADDR
#define LEDS_CWSEN_DEVICE_ID       XPAR_LEDS_POSITIONS_DEVICE_ID

#define GPIO_LEDS_BASEADDR         XPAR_LEDS_8BIT_BASEADDR
#define GPIO_LEDS_DEVICE_ID        XPAR_LEDS_8BIT_DEVICE_ID

#define ERR_LEDS_BASEADDR          XPAR_XPS_GPIO_0_BASEADDR
#define ERR_LEDS_DEVICE_ID         XPAR_XPS_GPIO_0_DEVICE_ID

#define PIEZO_BASEADDR             XPAR_XPS_GPIO_0_BASEADDR + 0x08
#define ROTARY_ENCODER_BASEADDR    XPAR_XPS_GPIO_1_BASEADDR
#define LCD_BASEADDR               XPAR_XPS_GPIO_2_BASEADDR

#define IIC_1_BASE_ADDRESS         XPAR_XPS_IIC_0_BASEADDR
#define IIC_2_BASE_ADDRESS         XPAR_XPS_IIC_1_BASEADDR

#define IIC_BASE_ADDRESS           XPAR_IIC_EEPROM_BASEADDR
#define TFT_DEVICE_ID              XPAR_XPS_TFT_0_DEVICE_ID
#define TFT_BASEADDR               XPAR_XPS_TFT_0_DCR_BASEADDR
#define ETHERNET_BASEADDR          XPAR_LLTEMAC_0_BASEADDR

#define XPAR_INTC_0_PS2_0_VEC_ID   XPAR_INTC_0_PS2_0_IP2INTC_IRPT_1_VEC_ID // Mouse Port
#define XPAR_INTC_0_PS2_1_VEC_ID   XPAR_INTC_0_PS2_0_IP2INTC_IRPT_2_VEC_ID // Keyboard Port

#define PPC440

#ifdef  PPC440
#define PLB_CLK_MHZ                100
#define DDR_BASEADDR               XPAR_DDR2_SDRAM_MEM_BASEADDR
#define CPU_CORE_FREQUENCY		   XPAR_CPU_PPC440_CORE_CLOCK_FREQ_HZ
#else
#define PLB_CLK_MHZ                125
#define DDR_BASEADDR               XPAR_DDR2_SDRAM_MPMC_BASEADDR
#define CPU_CORE_FREQUENCY		   XPAR_CPU_CORE_CLOCK_FREQ_HZ
#endif

//#define PPC440CACHE
#ifdef  PPC440CACHE
#define PPC440_ICACHE              0x00000030
#define PPC440_DCACHE              0x00000030
#endif

/* Cache Ranges
Range						Cache Setting
0x0000 0000 - 0x0FFF FFFF	0xC000_0000
0x1000 0000 - 0x1FFF FFFF	0x3000_0000
0x2000 0000 - 0x2FFF FFFF	0x0C00_0000
0x3000 0000 - 0x3FFF FFFF	0x0300_0000
0x4000 0000 - 0x4FFF FFFF	0x00C0_0000
0x5000 0000 - 0x5FFF FFFF	0x0030_0000
0x6000 0000 - 0x6FFF FFFF	0x000C_0000
0x7000 0000 - 0x7FFF FFFF	0x0003_0000
0x8000 0000 - 0x8FFF FFFF	0x0000_C000
0x9000 0000 - 0x9FFF FFFF	0x0000_3000
0xA000 0000 - 0xAFFF FFFF	0x0000_0C00
0xB000 0000 - 0xBFFF FFFF	0x0000_0300
0xC000 0000 - 0xCFFF FFFF	0x0000_00C0
0xD000 0000 - 0xDFFF FFFF	0x0000_0030
0xE000 0000 - 0xEFFF FFFF	0x0000_000C
0xF000 0000 - 0xFFFF FFFF	0x0000_0003
*/

#endif

