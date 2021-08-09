# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/austin/Documents/TSC/bare_metal_vitis_ws/ArtyZ7/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/austin/Documents/TSC/bare_metal_vitis_ws/ArtyZ7/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {ArtyZ7}\
-hw {/home/austin/Documents/TSC/src/artyZ7_linux/ArtyZ7_top_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -fsbl-target {psu_cortexa53_0} -out {/home/austin/Documents/TSC/bare_metal_vitis_ws}

platform write
platform generate -domains 
platform active {ArtyZ7}
domain active {zynq_fsbl}
bsp reload
domain active {standalone_domain}
bsp reload
bsp setlib -name libmetal -ver 2.1
bsp config stdin "ps7_uart_0"
bsp config stdin "ps7_uart_0"
bsp write
bsp reload
catch {bsp regenerate}
domain active {zynq_fsbl}
bsp reload
platform clean
platform generate
domain active {standalone_domain}
bsp config stdin "ps7_coresight_comp_0"
bsp config ttc_select_cntr "2"
bsp config stdout "ps7_coresight_comp_0"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains standalone_domain 
platform active {ArtyZ7}
bsp reload
platform generate -domains 
domain active {zynq_fsbl}
bsp reload
platform active {ArtyZ7}
platform config -updatehw {/home/austin/Documents/TSC/src/artyZ7_linux/ArtyZ7_top_wrapper.xsa}
bsp reload
platform generate -domains 
platform active {ArtyZ7}
platform config -updatehw {/home/austin/Documents/TSC/src/artyZ7_linux/ArtyZ7_top_wrapper.xsa}
platform clean
platform generate
domain active {zynq_fsbl}
bsp reload
domain active {standalone_domain}
bsp reload
bsp reload
catch {bsp regenerate}
platform clean
platform clean
platform clean
platform clean
platform generate
platform clean
platform generate
bsp reload
bsp config stdin "ps7_coresight_comp_0"
bsp config stdout "ps7_coresight_comp_0"
bsp write
domain active {zynq_fsbl}
bsp reload
bsp config stdin "ps7_coresight_comp_0"
bsp config stdout "ps7_coresight_comp_0"
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains zynq_fsbl 
platform config -updatehw {/home/austin/Documents/TSC/src/artyZ7_linux/ArtyZ7_top_wrapper.xsa}
platform generate -domains 
platform clean
platform clean
platform clean
