/** @file PmodHB3.c
*
* @author Samuel K Burkhart (skb@pdx.edu)
* @copyright Portland State University, 2017
*
* @brief
* This header file contains the driver functions for the PmodHB3 custom AXI Slave peripheral.  
* The peripheral provides access to a Digilent PmodHB3.  The PmodHB3 contains motor enable and direction control
* signals, and two sensor inputs (SA, SB) to determine the speed/direction of the motor.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a	skb	5/15/17	First release of driver
* </pre>
*
******************************************************************************/


/***************************** Include Files *******************************/
#include "PmodHB3.h"
/************************** Constant Definitions *****************************/

// bit masks for the pmodHB3 peripheral

// status register
#define PMODHB3_MTR_SPD_DET_MSK	    0x000003FF
#define PMODHB3_MTR_SPD_DET_SCALING	(60*4)	//4 to get to Hz, 60 to go from Hz to RPM

// control register
#define PMODHB3_MTR_SPD_CTRL_MSK 	0x00000FFF
#define PMODHB3_MTR_DIR_CTRL_MSK		0x00001000
#define PMODHB3_MTR_DIR_BIT_POS		12
#define PMODHB3_MTR_SPEED_IN_MSK		0xFF
#define PMODHB3_MTR_SPEED_IN_MAX		0xFF
#define PMODHB3_MTR_SPEED_MAX			0xFFF
#define PMODHB3_MTR_SPEED_MIN			0

/**************************** Type definitions ******************************/

/***************** Macros (Inline function) Definitions *********************/

/******************** Static variable declarations **************************/


/*********************** Private function prototypes ************************/

/************************** Public functions ********************************/

/****************************************************************************/

/**
* @brief Initialize the pmodHB3 peripheral driver
*
* Saves the base address of the pmodHB3 registers and runs the selftest
* (only the first time the peripheral is initialized). If the self-test passes
* the function sets the Motor Speed and Direction to zero.
*
* @param	p_instance is a pointer to the pmodHB3 driver instance 
* @param	baseaddr is the base address of the pmodHB3 perioheral register set
*
* @return
* 		- XST_SUCCESS	Initialization was successful.
*
* @note		This function can hang if the peripheral was not created correctly
* @note		The Base Address of the pmodHB3 peripheral registers will be in xparameters.h
*****************************************************************************/
uint32_t pmodHB3_initialize(p_pmodHB3 p_instance, uint32_t baseaddr)
{
	
	// Save the Base Address of the pmodHB3 register set so we know where to point the driver
	p_instance->base_address = baseaddr;
	
	// Run the driver self-test.
	if ( XST_SUCCESS == pmodHB3_selftest(p_instance->base_address ) )
	{
		p_instance->is_ready = true;
	}
	else
	{
		p_instance->is_ready = false;
	}
	
	// if pmodHB3 is ready configure the motor enable/direction
	if (p_instance->is_ready)
	{
		pmodHB3_init(p_instance, PMODHB3_MTR_SPEED_MAX, PMODHB3_MTR_SPEED_MIN);
	}
	
	return (p_instance->is_ready) ? XST_SUCCESS : XST_FAILURE;
}

/****************************************************************************/
/**
* @brief Set PmodHB3 Max/Min motor speed
*
* Configures the PmodHB3 Max/Min motor speed
*
* @param	p_instance is a pointer to the pmodHB3 driver instance 
*
* @return	XST_SUCCESS
*
*****************************************************************************/
uint32_t pmodHB3_set_max_min_motor_speed(p_pmodHB3 p_instance, u32 max_motor_speed, u32 min_motor_speed)
{
	p_instance->max_motor_speed = max_motor_speed;			    // 2**12 - 1 = 4095 (used to scale the desiredMotorSpeed)
	p_instance->min_motor_speed = min_motor_speed;				// Used to scale the desiredMotorSpeed
	
	return XST_SUCCESS;
}

/****************************************************************************/
/**
* @brief Initialize the PmodHB3
*
* Configures the PmodHB3
*
* @param	p_instance is a pointer to the pmodHB3 driver instance 
*
* @return	XST_SUCCESS
*
*****************************************************************************/
uint32_t pmodHB3_init(p_pmodHB3 p_instance, u32 max_motor_speed, u32 min_motor_speed)
{
	// Set the max/min motor speed
	pmodHB3_set_max_min_motor_speed(p_instance, max_motor_speed, min_motor_speed);
	
	// Set the current motor speed and direction to 0
	pmodHB3_mWriteReg(p_instance->base_address, PMODHB3_CNTRL_OFFSET, 0x00000000);
	
	return XST_SUCCESS;
}

/**
* @brief read and return the motor speed from the PmodHB3
*
* Returns the current motor speed.
*
* @param	p_instance is a pointer to the pmodHB3 driver instance
*
* @return	motor_speed
*****************************************************************************/
uint32_t pmodHB3_read_motor_speed(p_pmodHB3 p_instance)
{
	uint32_t speed;
	
	// Read motor speed from the status register
	speed = pmodHB3_mReadReg(p_instance->base_address, PMODHB3_STS_OFFSET);
	
	// Mask the motor speed before returning
	speed = (speed & PMODHB3_MTR_SPD_DET_MSK);
	
	// Calculate RPM from input motor speed (in Hz)
	u32 RPM = speed * PMODHB3_MTR_SPD_DET_SCALING;
	return RPM;
}

/****************************************************************************/
/**
* @brief Write motor speed to the PmodHB3
*
* @param	p_instance is a pointer to the pmodHB3 driver instance 
*
* @param	motor speed to set the motor PWM signal to, 8-bit value 
*
* @return	XST_SUCCESS
*
*****************************************************************************/
uint32_t pmodHB3_set_motor_speed(p_pmodHB3 p_instance, u32 motor_speed_in)
{
    u32 control_out;
	
	u32 motor_speed;					// Motor Control output
	float motor_speed_range = (float)(p_instance->max_motor_speed) - (float)(p_instance->min_motor_speed);

	float duty_cycle = ((float)(motor_speed_in & PMODHB3_MTR_SPEED_IN_MSK) / (float)(PMODHB3_MTR_SPEED_IN_MAX));
	motor_speed = (int)((duty_cycle * motor_speed_range) + p_instance->min_motor_speed);
    
    // Read in the current control values
    control_out = pmodHB3_mReadReg(p_instance->base_address, PMODHB3_CNTRL_OFFSET);
    
    // Clear the motor speed and set it to the incoming speed value
    control_out = (control_out & ~PMODHB3_MTR_SPD_CTRL_MSK) | (motor_speed & PMODHB3_MTR_SPD_CTRL_MSK);
    
    // Write the new control signal out
	pmodHB3_mWriteReg(p_instance->base_address, PMODHB3_CNTRL_OFFSET, control_out);
	
	return XST_SUCCESS;
}

/****************************************************************************/
/**
* @brief Write motor direction to the PmodHB3
*
* @param	p_instance is a pointer to the pmodHB3 driver instance 
*
* @param	motor direction is to set 
*
* @return	XST_SUCCESS
*
*****************************************************************************/
uint32_t pmodHB3_set_motor_direction(p_pmodHB3 p_instance, u32 motor_direction)
{
    u32 control_out;
    
    // Read the current control state
    control_out = pmodHB3_mReadReg(p_instance->base_address, PMODHB3_CNTRL_OFFSET);

    // Clear and set the motor direction
    control_out = (control_out & ~PMODHB3_MTR_DIR_CTRL_MSK) | ((motor_direction & 0x1) << PMODHB3_MTR_DIR_BIT_POS);

    // Write the new control state
	pmodHB3_mWriteReg(p_instance->base_address, PMODHB3_CNTRL_OFFSET, control_out);
	
	return XST_SUCCESS;
}
