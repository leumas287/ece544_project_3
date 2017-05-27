onbreak {quit -force}
onerror {quit -force}

asim -t 1ps +access +r +m+embsys -L unisims_ver -L unimacro_ver -L secureip -L xil_defaultlib -L xpm -L microblaze_v9_6_1 -L dist_mem_gen_v8_0_10 -L lib_pkg_v1_0_2 -L lib_cdc_v1_0_2 -L lib_srl_fifo_v1_0_2 -L fifo_generator_v13_1_1 -L lib_fifo_v1_0_5 -L axi_lite_ipif_v3_0_4 -L interrupt_control_v3_1_4 -L axi_quad_spi_v3_2_8 -L axi_gpio_v2_0_11 -L axi_timer_v2_0_11 -L fit_timer_v2_0_7 -L axi_uartlite_v2_0_13 -L axi_timebase_wdt_v3_0_1 -L lmb_v10_v3_0_8 -L lmb_bram_if_cntlr_v4_0_9 -L blk_mem_gen_v8_3_3 -L axi_intc_v4_1_7 -L mdm_v3_2_6 -L proc_sys_reset_v5_0_9 -L generic_baseblocks_v2_1_0 -L axi_infrastructure_v1_1_0 -L axi_register_slice_v2_1_9 -L axi_data_fifo_v2_1_8 -L axi_crossbar_v2_1_10 -O5 xil_defaultlib.embsys xil_defaultlib.glbl

do {wave.do}

view wave
view structure
view signals

do {embsys.udo}

run -all

endsim

quit -force
