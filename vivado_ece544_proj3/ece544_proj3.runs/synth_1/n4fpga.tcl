# 
# Synthesis run script generated by Vivado
# 

set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
create_project -in_memory -part xc7a100tcsg324-1

set_param project.singleFileAddWarning.threshold 0
set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_msg_config -source 4 -id {IP_Flow 19-2162} -severity warning -new_severity info
set_property webtalk.parent_dir C:/VivadoProjects/ece544_proj3/ece544_proj3.cache/wt [current_project]
set_property parent.project_path C:/VivadoProjects/ece544_proj3/ece544_proj3.xpr [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
set_property ip_repo_paths {
  c:/VivadoProjects/ip_repo/PmodHB3_1.0
  c:/Users/sburkhar/Desktop/Personal/School/Classes/Spring-2017/Projects/Project1/Project_1_release/ip_repo_544_p1/ip_repo_544_p1
} [current_project]
add_files C:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/embsys.bd
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_0/embsys_microblaze_0_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_0/embsys_microblaze_0_0_ooc_debug.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_0/embsys_microblaze_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/embsys_PmodOLEDrgb_0_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/PmodOLEDrgb_axi_quad_spi_0_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/PmodOLEDrgb_axi_quad_spi_0_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/PmodOLEDrgb_axi_quad_spi_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/PmodOLEDrgb_axi_quad_spi_0_0_clocks.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_gpio_0_1/PmodOLEDrgb_axi_gpio_0_1_board.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_gpio_0_1/PmodOLEDrgb_axi_gpio_0_1_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_gpio_0_1/PmodOLEDrgb_axi_gpio_0_1.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_pmod_bridge_0_0/PmodOLEDrgb_pmod_bridge_0_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_pmod_bridge_0_0/src/pmod_concat_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_gpio_0_0/embsys_axi_gpio_0_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_gpio_0_0/embsys_axi_gpio_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_gpio_0_0/embsys_axi_gpio_0_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_timer_0_0/embsys_axi_timer_0_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_timer_0_0/embsys_axi_timer_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_fit_timer_0_0/embsys_fit_timer_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_uartlite_0_0/embsys_axi_uartlite_0_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_uartlite_0_0/embsys_axi_uartlite_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_uartlite_0_0/embsys_axi_uartlite_0_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_timebase_wdt_0_0/embsys_axi_timebase_wdt_0_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_timebase_wdt_0_0/embsys_axi_timebase_wdt_0_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_dlmb_v10_0/embsys_dlmb_v10_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_dlmb_v10_0/embsys_dlmb_v10_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_ilmb_v10_0/embsys_ilmb_v10_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_ilmb_v10_0/embsys_ilmb_v10_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_dlmb_bram_if_cntlr_0/embsys_dlmb_bram_if_cntlr_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_ilmb_bram_if_cntlr_0/embsys_ilmb_bram_if_cntlr_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_lmb_bram_0/embsys_lmb_bram_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_axi_intc_0/embsys_microblaze_0_axi_intc_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_axi_intc_0/embsys_microblaze_0_axi_intc_0_clocks.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_axi_intc_0/embsys_microblaze_0_axi_intc_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_mdm_1_0/embsys_mdm_1_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_mdm_1_0/embsys_mdm_1_0_ooc_trace.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_clk_wiz_1_0/embsys_clk_wiz_1_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_clk_wiz_1_0/embsys_clk_wiz_1_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_clk_wiz_1_0/embsys_clk_wiz_1_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_rst_clk_wiz_1_100M_0/embsys_rst_clk_wiz_1_100M_0_board.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_rst_clk_wiz_1_100M_0/embsys_rst_clk_wiz_1_100M_0.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_rst_clk_wiz_1_100M_0/embsys_rst_clk_wiz_1_100M_0_ooc.xdc]
set_property used_in_implementation false [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_xbar_0/embsys_xbar_0_ooc.xdc]
set_property used_in_implementation false [get_files -all C:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/embsys_ooc.xdc]
set_property is_locked true [get_files C:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/embsys.bd]

read_verilog -library xil_defaultlib {
  C:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/imports/hardware/debounce.v
  C:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/imports/hardware/n4fpga.v
}
foreach dcp [get_files -quiet -all *.dcp] {
  set_property used_in_implementation false $dcp
}
read_xdc C:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/constrs_1/imports/constraints/n4DDRfpga.xdc
set_property used_in_implementation false [get_files C:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/constrs_1/imports/constraints/n4DDRfpga.xdc]

read_xdc dont_touch.xdc
set_property used_in_implementation false [get_files dont_touch.xdc]

synth_design -top n4fpga -part xc7a100tcsg324-1


write_checkpoint -force -noxdef n4fpga.dcp

catch { report_utilization -file n4fpga_utilization_synth.rpt -pb n4fpga_utilization_synth.pb }
