
 PARAMETER VERSION = 2.2.0


BEGIN OS
 PARAMETER OS_NAME = xilkernel
 PARAMETER OS_VER = 6.3
 PARAMETER PROC_INSTANCE = microblaze_0
 PARAMETER config_bufmalloc = true
 PARAMETER config_debug_support = true
 PARAMETER config_msgq = true
 PARAMETER config_sema = true
 PARAMETER config_shm = true
 PARAMETER config_time = true
 PARAMETER mem_table = (8,20)
 PARAMETER msgq_capacity = 100
 PARAMETER n_prio = 5
 PARAMETER sched_type = SCHED_PRIO
 PARAMETER stdin = axi_uartlite_0
 PARAMETER stdout = axi_uartlite_0
 PARAMETER sysintc_spec = microblaze_0_axi_intc
 PARAMETER systmr_dev = axi_timer_0
 PARAMETER use_malloc = true
END


BEGIN PROCESSOR
 PARAMETER DRIVER_NAME = cpu
 PARAMETER DRIVER_VER = 2.5
 PARAMETER HW_INSTANCE = microblaze_0
END


BEGIN DRIVER
 PARAMETER DRIVER_NAME = PmodENC
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = PmodENC_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = PmodHB3
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = PmodHB3_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = PmodOLEDrgb
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = PmodOLEDrgb_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = gpio
 PARAMETER DRIVER_VER = 4.1
 PARAMETER HW_INSTANCE = axi_gpio_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = wdttb
 PARAMETER DRIVER_VER = 4.0
 PARAMETER HW_INSTANCE = axi_timebase_wdt_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = tmrctr
 PARAMETER DRIVER_VER = 4.1
 PARAMETER HW_INSTANCE = axi_timer_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = uartlite
 PARAMETER DRIVER_VER = 3.2
 PARAMETER HW_INSTANCE = axi_uartlite_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = intc
 PARAMETER DRIVER_VER = 3.5
 PARAMETER HW_INSTANCE = microblaze_0_axi_intc
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.1
 PARAMETER HW_INSTANCE = microblaze_0_local_memory_dlmb_bram_if_cntlr
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.1
 PARAMETER HW_INSTANCE = microblaze_0_local_memory_ilmb_bram_if_cntlr
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = nexys4IO
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = nexys4IO_0
END

