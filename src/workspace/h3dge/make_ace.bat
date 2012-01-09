xmd -tcl genace.tcl -ace h3dge_system.ace -board ml507 -elf Debug/h3dge.elf -hw ../h3dge_hw_platform/system.bit -jprog -target ppc_hw
rm h:/ml50X/cfg6/*.ace
mv h3dge_system.ace h:/ml50X/cfg6/
rm -rf *.ace *.svf *.scr *.log
