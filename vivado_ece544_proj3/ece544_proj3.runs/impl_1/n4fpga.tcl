proc start_step { step } {
  set stopFile ".stop.rst"
  if {[file isfile .stop.rst]} {
    puts ""
    puts "*** Halting run - EA reset detected ***"
    puts ""
    puts ""
    return -code error
  }
  set beginFile ".$step.begin.rst"
  set platform "$::tcl_platform(platform)"
  set user "$::tcl_platform(user)"
  set pid [pid]
  set host ""
  if { [string equal $platform unix] } {
    if { [info exist ::env(HOSTNAME)] } {
      set host $::env(HOSTNAME)
    }
  } else {
    if { [info exist ::env(COMPUTERNAME)] } {
      set host $::env(COMPUTERNAME)
    }
  }
  set ch [open $beginFile w]
  puts $ch "<?xml version=\"1.0\"?>"
  puts $ch "<ProcessHandle Version=\"1\" Minor=\"0\">"
  puts $ch "    <Process Command=\".planAhead.\" Owner=\"$user\" Host=\"$host\" Pid=\"$pid\">"
  puts $ch "    </Process>"
  puts $ch "</ProcessHandle>"
  close $ch
}

proc end_step { step } {
  set endFile ".$step.end.rst"
  set ch [open $endFile w]
  close $ch
}

proc step_failed { step } {
  set endFile ".$step.error.rst"
  set ch [open $endFile w]
  close $ch
}

set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000

start_step init_design
set rc [catch {
  create_msg_db init_design.pb
  set_property design_mode GateLvl [current_fileset]
  set_param project.singleFileAddWarning.threshold 0
  set_property webtalk.parent_dir C:/VivadoProjects/ece544_proj3/ece544_proj3.cache/wt [current_project]
  set_property parent.project_path C:/VivadoProjects/ece544_proj3/ece544_proj3.xpr [current_project]
  set_property ip_repo_paths {
  c:/VivadoProjects/ece544_proj3/ece544_proj3.cache/ip
  C:/VivadoProjects/ip_repo/PmodHB3_1.0
  C:/Users/sburkhar/Desktop/Personal/School/Classes/Spring-2017/Projects/Project1/Project_1_release/ip_repo_544_p1/ip_repo_544_p1
} [current_project]
  set_property ip_output_repo c:/VivadoProjects/ece544_proj3/ece544_proj3.cache/ip [current_project]
  set_property XPM_LIBRARIES {XPM_CDC XPM_MEMORY} [current_project]
  add_files -quiet C:/VivadoProjects/ece544_proj3/ece544_proj3.runs/synth_1/n4fpga.dcp
  add_files C:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/embsys.bmm
  set_property SCOPED_TO_REF embsys [get_files -all C:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/embsys.bmm]
  add_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_0/data/mb_bootloop_le.elf
  set_property SCOPED_TO_REF embsys [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_0/data/mb_bootloop_le.elf]
  set_property SCOPED_TO_CELLS microblaze_0 [get_files -all c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_0/data/mb_bootloop_le.elf]
  read_xdc -ref embsys_microblaze_0_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_0/embsys_microblaze_0_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_0/embsys_microblaze_0_0.xdc]
  read_xdc -prop_thru_buffers -ref PmodOLEDrgb_axi_quad_spi_0_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/PmodOLEDrgb_axi_quad_spi_0_0_board.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/PmodOLEDrgb_axi_quad_spi_0_0_board.xdc]
  read_xdc -ref PmodOLEDrgb_axi_quad_spi_0_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/PmodOLEDrgb_axi_quad_spi_0_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/PmodOLEDrgb_axi_quad_spi_0_0.xdc]
  read_xdc -prop_thru_buffers -ref PmodOLEDrgb_axi_gpio_0_1 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_gpio_0_1/PmodOLEDrgb_axi_gpio_0_1_board.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_gpio_0_1/PmodOLEDrgb_axi_gpio_0_1_board.xdc]
  read_xdc -ref PmodOLEDrgb_axi_gpio_0_1 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_gpio_0_1/PmodOLEDrgb_axi_gpio_0_1.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_gpio_0_1/PmodOLEDrgb_axi_gpio_0_1.xdc]
  read_xdc -prop_thru_buffers -ref PmodOLEDrgb_pmod_bridge_0_0 -cells inst c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_pmod_bridge_0_0/PmodOLEDrgb_pmod_bridge_0_0_board.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_pmod_bridge_0_0/PmodOLEDrgb_pmod_bridge_0_0_board.xdc]
  read_xdc -prop_thru_buffers -ref embsys_PmodOLEDrgb_0_0 -cells inst c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/embsys_PmodOLEDrgb_0_0_board.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/embsys_PmodOLEDrgb_0_0_board.xdc]
  read_xdc -prop_thru_buffers -ref embsys_axi_gpio_0_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_gpio_0_0/embsys_axi_gpio_0_0_board.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_gpio_0_0/embsys_axi_gpio_0_0_board.xdc]
  read_xdc -ref embsys_axi_gpio_0_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_gpio_0_0/embsys_axi_gpio_0_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_gpio_0_0/embsys_axi_gpio_0_0.xdc]
  read_xdc -ref embsys_axi_timer_0_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_timer_0_0/embsys_axi_timer_0_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_timer_0_0/embsys_axi_timer_0_0.xdc]
  read_xdc -prop_thru_buffers -ref embsys_axi_uartlite_0_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_uartlite_0_0/embsys_axi_uartlite_0_0_board.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_uartlite_0_0/embsys_axi_uartlite_0_0_board.xdc]
  read_xdc -ref embsys_axi_uartlite_0_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_uartlite_0_0/embsys_axi_uartlite_0_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_uartlite_0_0/embsys_axi_uartlite_0_0.xdc]
  read_xdc -ref embsys_axi_timebase_wdt_0_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_timebase_wdt_0_0/embsys_axi_timebase_wdt_0_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_axi_timebase_wdt_0_0/embsys_axi_timebase_wdt_0_0.xdc]
  read_xdc -ref embsys_dlmb_v10_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_dlmb_v10_0/embsys_dlmb_v10_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_dlmb_v10_0/embsys_dlmb_v10_0.xdc]
  read_xdc -ref embsys_ilmb_v10_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_ilmb_v10_0/embsys_ilmb_v10_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_ilmb_v10_0/embsys_ilmb_v10_0.xdc]
  read_xdc -ref embsys_microblaze_0_axi_intc_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_axi_intc_0/embsys_microblaze_0_axi_intc_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_axi_intc_0/embsys_microblaze_0_axi_intc_0.xdc]
  read_xdc -ref embsys_mdm_1_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_mdm_1_0/embsys_mdm_1_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_mdm_1_0/embsys_mdm_1_0.xdc]
  read_xdc -prop_thru_buffers -ref embsys_clk_wiz_1_0 -cells inst c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_clk_wiz_1_0/embsys_clk_wiz_1_0_board.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_clk_wiz_1_0/embsys_clk_wiz_1_0_board.xdc]
  read_xdc -ref embsys_clk_wiz_1_0 -cells inst c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_clk_wiz_1_0/embsys_clk_wiz_1_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_clk_wiz_1_0/embsys_clk_wiz_1_0.xdc]
  read_xdc -prop_thru_buffers -ref embsys_rst_clk_wiz_1_100M_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_rst_clk_wiz_1_100M_0/embsys_rst_clk_wiz_1_100M_0_board.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_rst_clk_wiz_1_100M_0/embsys_rst_clk_wiz_1_100M_0_board.xdc]
  read_xdc -ref embsys_rst_clk_wiz_1_100M_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_rst_clk_wiz_1_100M_0/embsys_rst_clk_wiz_1_100M_0.xdc
  set_property processing_order EARLY [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_rst_clk_wiz_1_100M_0/embsys_rst_clk_wiz_1_100M_0.xdc]
  read_xdc C:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/constrs_1/imports/constraints/n4DDRfpga.xdc
  read_xdc -ref PmodOLEDrgb_axi_quad_spi_0_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/PmodOLEDrgb_axi_quad_spi_0_0_clocks.xdc
  set_property processing_order LATE [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/PmodOLEDrgb_axi_quad_spi_0_0_clocks.xdc]
  read_xdc -ref embsys_microblaze_0_axi_intc_0 -cells U0 c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_axi_intc_0/embsys_microblaze_0_axi_intc_0_clocks.xdc
  set_property processing_order LATE [get_files c:/VivadoProjects/ece544_proj3/ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_microblaze_0_axi_intc_0/embsys_microblaze_0_axi_intc_0_clocks.xdc]
  link_design -top n4fpga -part xc7a100tcsg324-1
  write_hwdef -file n4fpga.hwdef
  close_msg_db -file init_design.pb
} RESULT]
if {$rc} {
  step_failed init_design
  return -code error $RESULT
} else {
  end_step init_design
}

start_step opt_design
set rc [catch {
  create_msg_db opt_design.pb
  opt_design 
  write_checkpoint -force n4fpga_opt.dcp
  report_drc -file n4fpga_drc_opted.rpt
  close_msg_db -file opt_design.pb
} RESULT]
if {$rc} {
  step_failed opt_design
  return -code error $RESULT
} else {
  end_step opt_design
}

start_step place_design
set rc [catch {
  create_msg_db place_design.pb
  implement_debug_core 
  place_design 
  write_checkpoint -force n4fpga_placed.dcp
  report_io -file n4fpga_io_placed.rpt
  report_utilization -file n4fpga_utilization_placed.rpt -pb n4fpga_utilization_placed.pb
  report_control_sets -verbose -file n4fpga_control_sets_placed.rpt
  close_msg_db -file place_design.pb
} RESULT]
if {$rc} {
  step_failed place_design
  return -code error $RESULT
} else {
  end_step place_design
}

start_step route_design
set rc [catch {
  create_msg_db route_design.pb
  route_design 
  write_checkpoint -force n4fpga_routed.dcp
  report_drc -file n4fpga_drc_routed.rpt -pb n4fpga_drc_routed.pb
  report_timing_summary -warn_on_violation -max_paths 10 -file n4fpga_timing_summary_routed.rpt -rpx n4fpga_timing_summary_routed.rpx
  report_power -file n4fpga_power_routed.rpt -pb n4fpga_power_summary_routed.pb -rpx n4fpga_power_routed.rpx
  report_route_status -file n4fpga_route_status.rpt -pb n4fpga_route_status.pb
  report_clock_utilization -file n4fpga_clock_utilization_routed.rpt
  close_msg_db -file route_design.pb
} RESULT]
if {$rc} {
  step_failed route_design
  return -code error $RESULT
} else {
  end_step route_design
}

start_step write_bitstream
set rc [catch {
  create_msg_db write_bitstream.pb
  catch { write_mem_info -force n4fpga.mmi }
  catch { write_bmm -force n4fpga_bd.bmm }
  write_bitstream -force n4fpga.bit 
  catch { write_sysdef -hwdef n4fpga.hwdef -bitfile n4fpga.bit -meminfo n4fpga.mmi -file n4fpga.sysdef }
  catch {write_debug_probes -quiet -force debug_nets}
  close_msg_db -file write_bitstream.pb
} RESULT]
if {$rc} {
  step_failed write_bitstream
  return -code error $RESULT
} else {
  end_step write_bitstream
}

