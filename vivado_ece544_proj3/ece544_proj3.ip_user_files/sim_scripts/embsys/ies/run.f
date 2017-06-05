-makelib ies/xil_defaultlib -sv \
  "C:/Xilinx/Vivado/2016.2/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
  "C:/Xilinx/Vivado/2016.2/data/ip/xpm/xpm_memory/hdl/xpm_memory_base.sv" \
  "C:/Xilinx/Vivado/2016.2/data/ip/xpm/xpm_memory/hdl/xpm_memory_dpdistram.sv" \
  "C:/Xilinx/Vivado/2016.2/data/ip/xpm/xpm_memory/hdl/xpm_memory_dprom.sv" \
  "C:/Xilinx/Vivado/2016.2/data/ip/xpm/xpm_memory/hdl/xpm_memory_sdpram.sv" \
  "C:/Xilinx/Vivado/2016.2/data/ip/xpm/xpm_memory/hdl/xpm_memory_spram.sv" \
  "C:/Xilinx/Vivado/2016.2/data/ip/xpm/xpm_memory/hdl/xpm_memory_sprom.sv" \
  "C:/Xilinx/Vivado/2016.2/data/ip/xpm/xpm_memory/hdl/xpm_memory_tdpram.sv" \
-endlib
-makelib ies/xpm \
  "C:/Xilinx/Vivado/2016.2/data/ip/xpm/xpm_VCOMP.vhd" \
-endlib
-makelib ies/microblaze_v9_6_1 \
  "../../../ipstatic/microblaze_v9_6/hdl/microblaze_v9_6_vh_rfs.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_microblaze_0_0/sim/embsys_microblaze_0_0.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../ipstatic/nexys4io_v2_0/src/rgbpwm.v" \
  "../../../ipstatic/nexys4io_v2_0/src/sevensegment.v" \
  "../../../ipstatic/nexys4io_v2_0/src/debounce.v" \
  "../../../ipstatic/nexys4io_v2_0/hdl/nexys4IO_v2_0_S00_AXI.v" \
  "../../../ipstatic/nexys4io_v2_0/hdl/nexys4IO_v2_0.v" \
  "../../../bd/embsys/ip/embsys_nexys4IO_0_0/sim/embsys_nexys4IO_0_0.v" \
  "../../../bd/embsys/ipshared/ece.pdx.edu/pmodenc_v1_0/hdl/PmodENC_v1_0_S00_AXI.v" \
  "../../../bd/embsys/ipshared/ece.pdx.edu/pmodenc_v1_0/hdl/PmodENC_v1_0.v" \
  "../../../bd/embsys/ipshared/ece.pdx.edu/pmodenc_v1_0/src/debouncer.v" \
  "../../../bd/embsys/ipshared/ece.pdx.edu/pmodenc_v1_0/src/encoder.v" \
  "../../../bd/embsys/ip/embsys_PmodENC_0_0/sim/embsys_PmodENC_0_0.v" \
-endlib
-makelib ies/dist_mem_gen_v8_0_10 \
  "../../../ipstatic/dist_mem_gen_v8_0/simulation/dist_mem_gen_v8_0.v" \
-endlib
-makelib ies/lib_pkg_v1_0_2 \
  "../../../ipstatic/lib_pkg_v1_0/hdl/src/vhdl/lib_pkg.vhd" \
-endlib
-makelib ies/lib_cdc_v1_0_2 \
  "../../../ipstatic/lib_cdc_v1_0/hdl/src/vhdl/cdc_sync.vhd" \
-endlib
-makelib ies/lib_srl_fifo_v1_0_2 \
  "../../../ipstatic/lib_srl_fifo_v1_0/hdl/src/vhdl/cntr_incr_decr_addn_f.vhd" \
  "../../../ipstatic/lib_srl_fifo_v1_0/hdl/src/vhdl/dynshreg_f.vhd" \
  "../../../ipstatic/lib_srl_fifo_v1_0/hdl/src/vhdl/srl_fifo_rbu_f.vhd" \
  "../../../ipstatic/lib_srl_fifo_v1_0/hdl/src/vhdl/srl_fifo_f.vhd" \
-endlib
-makelib ies/fifo_generator_v13_1_1 \
  "../../../ipstatic/fifo_generator_v13_1/simulation/fifo_generator_vlog_beh.v" \
-endlib
-makelib ies/fifo_generator_v13_1_1 \
  "../../../ipstatic/fifo_generator_v13_1/hdl/fifo_generator_v13_1_rfs.vhd" \
-endlib
-makelib ies/fifo_generator_v13_1_1 \
  "../../../ipstatic/fifo_generator_v13_1/hdl/fifo_generator_v13_1_rfs.v" \
-endlib
-makelib ies/lib_fifo_v1_0_5 \
  "../../../ipstatic/lib_fifo_v1_0/hdl/src/vhdl/async_fifo_fg.vhd" \
  "../../../ipstatic/lib_fifo_v1_0/hdl/src/vhdl/sync_fifo_fg.vhd" \
-endlib
-makelib ies/axi_lite_ipif_v3_0_4 \
  "../../../ipstatic/axi_lite_ipif_v3_0/hdl/src/vhdl/ipif_pkg.vhd" \
  "../../../ipstatic/axi_lite_ipif_v3_0/hdl/src/vhdl/pselect_f.vhd" \
  "../../../ipstatic/axi_lite_ipif_v3_0/hdl/src/vhdl/address_decoder.vhd" \
  "../../../ipstatic/axi_lite_ipif_v3_0/hdl/src/vhdl/slave_attachment.vhd" \
  "../../../ipstatic/axi_lite_ipif_v3_0/hdl/src/vhdl/axi_lite_ipif.vhd" \
-endlib
-makelib ies/interrupt_control_v3_1_4 \
  "../../../ipstatic/interrupt_control_v3_1/hdl/src/vhdl/interrupt_control.vhd" \
-endlib
-makelib ies/axi_quad_spi_v3_2_8 \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/comp_defs.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/pselect_f.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/counter_f.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/soft_reset.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/xip_cross_clk_sync.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/reset_sync_module.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_status_slave_sel_reg.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_startup_block.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_receive_transmit_reg.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_occupancy_reg.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_mode_control_logic.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_mode_0_module.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_look_up_logic.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_fifo_ifmodule.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_cntrl_reg.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_address_decoder.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/cross_clk_sync_fifo_1.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/cross_clk_sync_fifo_0.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/xip_status_reg.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/xip_cntrl_reg.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/qspi_core_interface.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/axi_qspi_xip_if.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/axi_qspi_enhanced_mode.vhd" \
  "../../../ipstatic/axi_quad_spi_v3_2/hdl/src/vhdl/axi_quad_spi.vhd" \
  "../../../bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_quad_spi_0_0/sim/PmodOLEDrgb_axi_quad_spi_0_0.vhd" \
-endlib
-makelib ies/axi_gpio_v2_0_11 \
  "../../../ipstatic/axi_gpio_v2_0/hdl/src/vhdl/gpio_core.vhd" \
  "../../../ipstatic/axi_gpio_v2_0/hdl/src/vhdl/axi_gpio.vhd" \
  "../../../bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_axi_gpio_0_1/sim/PmodOLEDrgb_axi_gpio_0_1.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../../ece544_proj3.srcs/sources_1/bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ipshared/digilentinc.com/pmod_bridge_v1_0/src/pmod_concat.v" \
  "../../../bd/embsys/ip/embsys_PmodOLEDrgb_0_0/ip/PmodOLEDrgb_pmod_bridge_0_0/sim/PmodOLEDrgb_pmod_bridge_0_0.v" \
  "../../../../ece544_proj3.srcs/sources_1/bd/embsys/ipshared/digilentinc.com/pmodoledrgb_v1_0/hdl/PmodOLEDrgb_v1_0.v" \
  "../../../bd/embsys/ip/embsys_PmodOLEDrgb_0_0/sim/embsys_PmodOLEDrgb_0_0.v" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_axi_gpio_0_0/sim/embsys_axi_gpio_0_0.vhd" \
-endlib
-makelib ies/axi_timer_v2_0_11 \
  "../../../ipstatic/axi_timer_v2_0/hdl/src/vhdl/counter_f.vhd" \
  "../../../ipstatic/axi_timer_v2_0/hdl/src/vhdl/mux_onehot_f.vhd" \
  "../../../ipstatic/axi_timer_v2_0/hdl/src/vhdl/tc_types.vhd" \
  "../../../ipstatic/axi_timer_v2_0/hdl/src/vhdl/timer_control.vhd" \
  "../../../ipstatic/axi_timer_v2_0/hdl/src/vhdl/count_module.vhd" \
  "../../../ipstatic/axi_timer_v2_0/hdl/src/vhdl/tc_core.vhd" \
  "../../../ipstatic/axi_timer_v2_0/hdl/src/vhdl/axi_timer.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_axi_timer_0_0/sim/embsys_axi_timer_0_0.vhd" \
-endlib
-makelib ies/fit_timer_v2_0_7 \
  "../../../ipstatic/fit_timer_v2_0/hdl/vhdl/divide_part.vhd" \
  "../../../ipstatic/fit_timer_v2_0/hdl/vhdl/fit_timer.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_fit_timer_0_0/sim/embsys_fit_timer_0_0.vhd" \
-endlib
-makelib ies/axi_uartlite_v2_0_13 \
  "../../../ipstatic/axi_uartlite_v2_0/hdl/src/vhdl/dynshreg_i_f.vhd" \
  "../../../ipstatic/axi_uartlite_v2_0/hdl/src/vhdl/uartlite_tx.vhd" \
  "../../../ipstatic/axi_uartlite_v2_0/hdl/src/vhdl/uartlite_rx.vhd" \
  "../../../ipstatic/axi_uartlite_v2_0/hdl/src/vhdl/baudrate.vhd" \
  "../../../ipstatic/axi_uartlite_v2_0/hdl/src/vhdl/uartlite_core.vhd" \
  "../../../ipstatic/axi_uartlite_v2_0/hdl/src/vhdl/axi_uartlite.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_axi_uartlite_0_0/sim/embsys_axi_uartlite_0_0.vhd" \
-endlib
-makelib ies/axi_timebase_wdt_v3_0_1 \
  "../../../ipstatic/axi_timebase_wdt_v3_0/hdl/src/vhdl/window_wdt_counter.vhd" \
  "../../../ipstatic/axi_timebase_wdt_v3_0/hdl/src/vhdl/window_wdt_fail_cnt.vhd" \
  "../../../ipstatic/axi_timebase_wdt_v3_0/hdl/src/vhdl/timebase_wdt_core.vhd" \
  "../../../ipstatic/axi_timebase_wdt_v3_0/hdl/src/vhdl/axi_timebase_wdt.vhd" \
  "../../../ipstatic/axi_timebase_wdt_v3_0/hdl/src/vhdl/axi_window_wdt.vhd" \
  "../../../ipstatic/axi_timebase_wdt_v3_0/hdl/src/vhdl/axi_timebase_wdt_top.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_axi_timebase_wdt_0_0/sim/embsys_axi_timebase_wdt_0_0.vhd" \
-endlib
-makelib ies/lmb_v10_v3_0_8 \
  "../../../ipstatic/lmb_v10_v3_0/hdl/vhdl/lmb_v10.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_dlmb_v10_0/sim/embsys_dlmb_v10_0.vhd" \
  "../../../bd/embsys/ip/embsys_ilmb_v10_0/sim/embsys_ilmb_v10_0.vhd" \
-endlib
-makelib ies/lmb_bram_if_cntlr_v4_0_9 \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/lmb_bram_if_funcs.vhd" \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/lmb_bram_if_primitives.vhd" \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/xor18.vhd" \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/parity.vhd" \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/parityenable.vhd" \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/checkbit_handler.vhd" \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/correct_one_bit.vhd" \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/pselect_mask.vhd" \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/axi_interface.vhd" \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/lmb_mux.vhd" \
  "../../../ipstatic/lmb_bram_if_cntlr_v4_0/hdl/vhdl/lmb_bram_if_cntlr.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_dlmb_bram_if_cntlr_0/sim/embsys_dlmb_bram_if_cntlr_0.vhd" \
  "../../../bd/embsys/ip/embsys_ilmb_bram_if_cntlr_0/sim/embsys_ilmb_bram_if_cntlr_0.vhd" \
-endlib
-makelib ies/blk_mem_gen_v8_3_3 \
  "../../../ipstatic/blk_mem_gen_v8_3/simulation/blk_mem_gen_v8_3.v" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_lmb_bram_0/sim/embsys_lmb_bram_0.v" \
-endlib
-makelib ies/axi_intc_v4_1_7 \
  "../../../ipstatic/axi_intc_v4_1/hdl/src/vhdl/double_synchronizer.vhd" \
  "../../../ipstatic/axi_intc_v4_1/hdl/src/vhdl/shared_ram_ivar.vhd" \
  "../../../ipstatic/axi_intc_v4_1/hdl/src/vhdl/pulse_synchronizer.vhd" \
  "../../../ipstatic/axi_intc_v4_1/hdl/src/vhdl/intc_core.vhd" \
  "../../../ipstatic/axi_intc_v4_1/hdl/src/vhdl/axi_intc.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_microblaze_0_axi_intc_0/sim/embsys_microblaze_0_axi_intc_0.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ipshared/xilinx.com/xlconcat_v2_1/xlconcat.v" \
  "../../../bd/embsys/ip/embsys_microblaze_0_xlconcat_0/sim/embsys_microblaze_0_xlconcat_0.v" \
-endlib
-makelib ies/mdm_v3_2_6 \
  "../../../ipstatic/mdm_v3_2/hdl/vhdl/mdm_primitives.vhd" \
  "../../../ipstatic/mdm_v3_2/hdl/vhdl/arbiter.vhd" \
  "../../../ipstatic/mdm_v3_2/hdl/vhdl/srl_fifo.vhd" \
  "../../../ipstatic/mdm_v3_2/hdl/vhdl/bus_master.vhd" \
  "../../../ipstatic/mdm_v3_2/hdl/vhdl/jtag_control.vhd" \
  "../../../ipstatic/mdm_v3_2/hdl/vhdl/mdm_core.vhd" \
  "../../../ipstatic/mdm_v3_2/hdl/vhdl/mdm.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_mdm_1_0/sim/embsys_mdm_1_0.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_clk_wiz_1_0/embsys_clk_wiz_1_0_clk_wiz.v" \
  "../../../bd/embsys/ip/embsys_clk_wiz_1_0/embsys_clk_wiz_1_0.v" \
-endlib
-makelib ies/proc_sys_reset_v5_0_9 \
  "../../../ipstatic/proc_sys_reset_v5_0/hdl/src/vhdl/upcnt_n.vhd" \
  "../../../ipstatic/proc_sys_reset_v5_0/hdl/src/vhdl/sequence_psr.vhd" \
  "../../../ipstatic/proc_sys_reset_v5_0/hdl/src/vhdl/lpf.vhd" \
  "../../../ipstatic/proc_sys_reset_v5_0/hdl/src/vhdl/proc_sys_reset.vhd" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_rst_clk_wiz_1_100M_0/sim/embsys_rst_clk_wiz_1_100M_0.vhd" \
-endlib
-makelib ies/generic_baseblocks_v2_1_0 \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_carry_and.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_carry_latch_and.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_carry_latch_or.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_carry_or.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_carry.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_command_fifo.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_comparator_mask_static.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_comparator_mask.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_comparator_sel_mask_static.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_comparator_sel_mask.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_comparator_sel_static.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_comparator_sel.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_comparator_static.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_comparator.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_mux_enc.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_mux.v" \
  "../../../ipstatic/generic_baseblocks_v2_1/hdl/verilog/generic_baseblocks_v2_1_nto1_mux.v" \
-endlib
-makelib ies/axi_infrastructure_v1_1_0 \
  "../../../ipstatic/axi_infrastructure_v1_1/hdl/verilog/axi_infrastructure_v1_1_axi2vector.v" \
  "../../../ipstatic/axi_infrastructure_v1_1/hdl/verilog/axi_infrastructure_v1_1_axic_srl_fifo.v" \
  "../../../ipstatic/axi_infrastructure_v1_1/hdl/verilog/axi_infrastructure_v1_1_vector2axi.v" \
-endlib
-makelib ies/axi_register_slice_v2_1_9 \
  "../../../ipstatic/axi_register_slice_v2_1/hdl/verilog/axi_register_slice_v2_1_axic_register_slice.v" \
  "../../../ipstatic/axi_register_slice_v2_1/hdl/verilog/axi_register_slice_v2_1_axi_register_slice.v" \
-endlib
-makelib ies/axi_data_fifo_v2_1_8 \
  "../../../ipstatic/axi_data_fifo_v2_1/hdl/verilog/axi_data_fifo_v2_1_axic_fifo.v" \
  "../../../ipstatic/axi_data_fifo_v2_1/hdl/verilog/axi_data_fifo_v2_1_fifo_gen.v" \
  "../../../ipstatic/axi_data_fifo_v2_1/hdl/verilog/axi_data_fifo_v2_1_axic_srl_fifo.v" \
  "../../../ipstatic/axi_data_fifo_v2_1/hdl/verilog/axi_data_fifo_v2_1_axic_reg_srl_fifo.v" \
  "../../../ipstatic/axi_data_fifo_v2_1/hdl/verilog/axi_data_fifo_v2_1_ndeep_srl.v" \
  "../../../ipstatic/axi_data_fifo_v2_1/hdl/verilog/axi_data_fifo_v2_1_axi_data_fifo.v" \
-endlib
-makelib ies/axi_crossbar_v2_1_10 \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_addr_arbiter_sasd.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_addr_arbiter.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_addr_decoder.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_arbiter_resp.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_crossbar_sasd.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_crossbar.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_decerr_slave.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_si_transactor.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_splitter.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_wdata_mux.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_wdata_router.v" \
  "../../../ipstatic/axi_crossbar_v2_1/hdl/verilog/axi_crossbar_v2_1_axi_crossbar.v" \
-endlib
-makelib ies/xil_defaultlib \
  "../../../bd/embsys/ip/embsys_xbar_0/sim/embsys_xbar_0.v" \
  "../../../bd/embsys/hdl/embsys.v" \
  "../../../bd/embsys/ipshared/pdx.edu/pmodhb3_v1_0/hdl/PmodHB3_v1_0_S00_AXI.v" \
  "../../../bd/embsys/ipshared/pdx.edu/pmodhb3_v1_0/src/MOTOR_DIRECTION_CONTROLLER.v" \
  "../../../bd/embsys/ipshared/pdx.edu/pmodhb3_v1_0/src/MOTOR_SPEED_CONTROLLER.v" \
  "../../../bd/embsys/ipshared/pdx.edu/pmodhb3_v1_0/src/MOTOR_SPEED_DETECTOR.v" \
  "../../../bd/embsys/ipshared/pdx.edu/pmodhb3_v1_0/hdl/PmodHB3_v1_0.v" \
  "../../../bd/embsys/ip/embsys_PmodHB3_0_2/sim/embsys_PmodHB3_0_2.v" \
-endlib
-makelib ies/xil_defaultlib \
  glbl.v
-endlib

