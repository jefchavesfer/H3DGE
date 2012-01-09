##############################################################################
## Filename:          C:\git\h3dge\src\hdl\system/drivers/h3dge_coproc_v1_00_a/data/h3dge_coproc_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Wed Nov 30 20:33:52 2011 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "h3dge_coproc" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
