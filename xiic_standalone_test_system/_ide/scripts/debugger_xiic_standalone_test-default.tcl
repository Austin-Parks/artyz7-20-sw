# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: /home/austin/Documents/TSC/bare_metal_vitis_ws/xiic_standalone_test_system/_ide/scripts/debugger_xiic_standalone_test-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source /home/austin/Documents/TSC/bare_metal_vitis_ws/xiic_standalone_test_system/_ide/scripts/debugger_xiic_standalone_test-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Arty Z7 003017A6FC2EA" && level==0 && jtag_device_ctx=="jsn-Arty Z7-003017A6FC2EA-23727093-0"}
fpga -file /home/austin/Documents/TSC/bare_metal_vitis_ws/xiic_standalone_test/_ide/bitstream/ArtyZ7_top_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw /home/austin/Documents/TSC/bare_metal_vitis_ws/ArtyZ7/export/ArtyZ7/hw/ArtyZ7_top_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source /home/austin/Documents/TSC/bare_metal_vitis_ws/xiic_standalone_test/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow /home/austin/Documents/TSC/bare_metal_vitis_ws/xiic_standalone_test/Debug/xiic_standalone_test.elf
configparams force-mem-access 0
bpadd -addr &main
