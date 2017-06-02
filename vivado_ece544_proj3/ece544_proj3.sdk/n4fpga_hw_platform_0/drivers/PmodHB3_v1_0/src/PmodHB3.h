/** @file PmodHB3.h
*
* @author Samuel K Burkhart (skb@pdx.edu)
* @copyright Portland State University, 2016, 2017
*
* @brief
* This header file contains the constants and low level function for the PmodHB3 custom AXI Slave
* peripheral driver.  The peripheral provides access to a Digilent PmodHB3.  The PmodHB3 contains
* a motor enable and direction signal to drive a motor, and two sensor input (SA/SB) for detecting the
* motor speed and direction.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a	rhk	15-May-2017	First release of driver
* </pre>
*
******************************************************************************/

#ifndef PMODHB3_H
#define PMODHB3_H


/****************** Include Files ********************/
#include "stdint.h"
#include "stdbool.h"
#include "xil_types.h"
#include "xil_io.h"
#include "xstatus.h"

#define PMODHB3_S00_AXI_SLV_REG0_OFFSET 0
#define PMODHB3_S00_AXI_SLV_REG1_OFFSET 4
#define PMODHB3_S00_AXI_SLV_REG2_OFFSET 8
#define PMODHB3_S00_AXI_SLV_REG3_OFFSET 12

// canonical register declaration
#define PMODHB3_STS_OFFSET  PMODHB3_S00_AXI_SLV_REG0_OFFSET
#define PMODHB3_CNTRL_OFFSET PMODHB3_S00_AXI_SLV_REG1_OFFSET
#define PMODHB3_RSVD00_OFFSET PMODHB3_S00_AXI_SLV_REG2_OFFSET
#define PMODHB3_RSVD01_OFFSET PMODHB3_S00_AXI_SLV_REG2_OFFSET

/**************************** Type Definitions *****************************/

typedef struct
{
	uint32_t	base_address;					// base address for pmodENC peripheral registers
	bool			is_ready;							// pmodHB3 driver has been successfully initialized
	u32 			max_motor_speed;			// 2**12 - 1 = 4095 (used to scale the desiredMotorSpeed)
	u32 			min_motor_speed;				// Used to scale the desiredMotorSpeed
} PmodHB3, *p_pmodHB3;

/***************** Macros (Inline function) Definitions *********************/

/**
 *
 * Write a value to a PMODHB3 register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the PMODHB3device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void PMODHB3_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define pmodHB3_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a PMODHB3 register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the PMODHB3 device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 PMODHB3_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define pmodHB3_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the PMODHB3 instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
 
 // self test and initialization functions
uint32_t pmodHB3_selftest(uint32_t baseaddr);
uint32_t pmodHB3_initialize(p_pmodHB3 p_instance, uint32_t baseaddr);

// PmodHB3 functions
uint32_t pmodHB3_set_max_min_motor_speed(p_pmodHB3 p_instance, u32 max_motor_speed, u32 min_motor_speed);
uint32_t pmodHB3_init(p_pmodHB3 p_instance, u32 max_motor_speed, u32 min_motor_speed);
uint32_t pmodHB3_read_motor_speed(p_pmodHB3 p_instance);
uint32_t pmodHB3_set_motor_speed(p_pmodHB3 p_instance, u32 motor_speed);
uint32_t pmodHB3_set_motor_direction(p_pmodHB3 p_instance, u32 motor_direction);

#endif // PMODHB3_H
