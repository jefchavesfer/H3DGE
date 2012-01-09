xmd -tcl genace.tcl -ace h3dge.ace -board ml507 -elf Debug/colorbar.elf -hw ../h3dge_hw_platform/system.bit -jprog -target ppc_hw
mv h3dge.ace h:/ml50X/cfg6/
rm -rf *.ace *.svf *.scr *.log
