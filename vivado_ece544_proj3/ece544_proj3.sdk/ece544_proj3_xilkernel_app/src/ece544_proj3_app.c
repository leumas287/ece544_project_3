/* ece544_proj3_app.c - ECE 544 Project #3 Application
 *
 *
 *	Author:		Samuel K Burkhart, Dakota Ward (Modified from Srivatsa and Roy's sample code)
 *	Date:		4-June-2017
 *	Revision:	1.0
 *
 * 	Description:
 * 	------------
 * 	This program implements ECE 544 Project #3.
 * 	The application sets up 5 independent threads:
 *		o Thread 1	(Parameter_input_thread) Reads the switches, buttons and rotary count on the FPGA development board
 *		 			and sends an update message to the PID thread.
 * 		o Thread 2 	(Motor_input_thread) Receives the Feedback value (the Hall sensor) from the motor and sends
 *					an update message to the PID thread.
 * 		o Thread 3 	(PID_thread) Implements the PID controller and sends a message to the Motor_output_thread.
 * 		o Thread 4	(Motor_output_thread) Receives the control value message from the PID controller and drives the motor.
 * 		o Thread 5	(Display_thread) Receives messages from the Parameter_input_thread and the Motor_input_thread and updates
 *					the 7-segment display and the LEDs.
 *
 * 	The threads communicate via a message queues. Thread 1 uses an interrupt handler to sample the inputs
 *	and update the PID control constants.  Semaphores are used (if necessary) to synchronize the interrupt handlers
 *	and their associated threads.
 *
 * 	There is a 6th thread, the Master thread. The Master_thread sets up the Microblaze, initializes the peripherals, and creates
 *	the other threads, messages queues, and semaphores before entering a while(1) loop.
 *
 *  We use priority based scheduling of threads where master thread has priority 0 and all the other threads have
 *  priority of 1.
 *
 *  The application makes use of the Xilinx watchdog timer. Restarting the WDT timer is performed in the WDT interrupt handler and forces a crash if the
 *  force_crash flag is set by setting sw[15]. Don't forget to Turn SW[15] off after the crash
 *	or the system will just crash again.
 *
 *	Status messages should be sent to the USB serial port (STDOUT) which can be connected to a PC running a terminal
 *	emulator such as Putty.  A BAUD rate of 115200 is used for connecting to the UART output.
 *
 */

// Includes
#include "xmk.h"
#include "os_config.h"
#include "config/config_param.h"
#include "sys/ksched.h"
#include "sys/init.h"
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <sys/intr.h>
#include <sys/timer.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include "xintc.h"
#include "xparameters.h"
#include "platform_config.h"
#include "platform.h"
#include "stdbool.h"
#include "xgpio.h"
#include "xwdttb.h"
#include "xtmrctr.h"
#include "xstatus.h"
#include "nexys4IO.h"
#include "pmodENC.h"
#include "PmodHB3.h"
#include "PmodOLEDrgb.h"

/************************** Constant Definitions ****************************/

// Interrupt Declarations
#define INTC_DEVICEID			XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID
#define WDT_DEVICEID			XPAR_WDTTB_0_DEVICE_ID
#define N4INPUT_GPIO_INTR_NUM	XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define WDT_INTR_NUM			XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMEBASE_WDT_0_WDT_INTERRUPT_INTR
#define FIT_INTERRUPT_ID		XPAR_MICROBLAZE_0_AXI_INTC_FIT_TIMER_0_INTERRUPT_INTR

// Clock frequencies
#define CPU_CLOCK_FREQ_HZ		XPAR_CPU_CORE_CLOCK_FREQ_HZ
#define AXI_CLOCK_FREQ_HZ		XPAR_CPU_M_AXI_DP_FREQ_HZ

// AXI timer parameters
#define TMRCTR0_DEVICEID		XPAR_AXI_TIMER_0_DEVICE_ID
#define AXI_TIMER_DEVICE_ID		XPAR_AXI_TIMER_1_DEVICE_ID
#define AXI_TIMER_BASEADDR		XPAR_AXI_TIMER_1_BASEADDR
#define AXI_TIMER_HIGHADDR		XPAR_AXI_TIMER_1_HIGHADDR
#define TmrCtrNumber			0

// Definitions for peripheral NEXYS4IO
#define NX4IO_DEVICE_ID		XPAR_NEXYS4IO_0_DEVICE_ID
#define NX4IO_BASEADDR		XPAR_NEXYS4IO_0_S00_AXI_BASEADDR
#define NX4IO_HIGHADDR		XPAR_NEXYS4IO_0_S00_AXI_HIGHADDR

// Definitions for peripheral PMODENC
#define PMODENC_DEVICE_ID		XPAR_PMODENC_0_DEVICE_ID
#define PMODENC_BASEADDR		XPAR_PMODENC_0_S00_AXI_BASEADDR
#define PMODENC_HIGHADDR		XPAR_PMODENC_0_S00_AXI_HIGHADDR

// Fixed Interval timer - 100 MHz input clock, 5KHz output clock
// FIT_COUNT_1MSEC = FIT_CLOCK_FREQ_HZ * .001
#define FIT_IN_CLOCK_FREQ_HZ	CPU_CLOCK_FREQ_HZ
#define FIT_CLOCK_FREQ_HZ		5000
#define FIT_COUNT				(FIT_IN_CLOCK_FREQ_HZ / FIT_CLOCK_FREQ_HZ)
#define FIT_COUNT_1MSEC			5

// GPIO Declarations (Change According to your implementation)
#define NEXYS4_INPUT_GPIO_DEVICEID		XPAR_AXI_GPIO_0_DEVICE_ID
#define GPIO0_INPUT_0_CHANNEL		1

// Definitions for peripheral PMODHB3
#define PMODHB3_DEVICE_ID		XPAR_PMODHB3_0_DEVICE_ID
#define PMODHB3_BASEADDR		XPAR_PMODHB3_0_S00_AXI_BASEADDR
#define PMODHB3_HIGHADDR		XPAR_PMODHB3_0_S00_AXI_HIGHADDR

// Definitions for peripheral PMODOLEDRGB
#define RGBDSPLY_DEVICE_ID		XPAR_PMODOLEDRGB_0_DEVICE_ID
#define RGBDSPLY_GPIO_BASEADDR	XPAR_PMODOLEDRGB_0_AXI_LITE_GPIO_BASEADDR
#define RGBDSPLY_GPIO_HIGHADDR	XPAR_PMODOLEDRGB_0_AXI_LITE_GPIO_HIGHADD
#define RGBDSPLY_SPI_BASEADDR	XPAR_PMODOLEDRGB_0_AXI_LITE_SPI_BASEADDR
#define RGBDSPLY_SPI_HIGHADDR	XPAR_PMODOLEDRGB_0_AXI_LITE_SPI_HIGHADDR

// potentially useful macro functions
#define MIN(a, b)  				( ((a) <= (b)) ? (a) : (b) )
#define MAX(a, b)  				( ((a) >= (b)) ? (a) : (b) )


/*****Global variables, peripheral instances, function prototypes, etc. *****/

// Motor Speed/Direction/Control constants
#define MAX_MOTOR_SPEED			255			//Used for max SetPoint value
#define MIN_MOTOR_SPEED			0			//Used for min SetPoint value
#define MAX_RPM					12000		//Used for max RPM to convert SetPoint to RPM for error calculation, could have been measured/set by the motor driver.
#define CONSTANT_MULTIPLIER		100			//Used for the fixed precision (2 decimal point) counts for each PID constant
#define MAX_KP					9999		//Used for the max PID constants, since this is a fixed precision, 2-decimal value the max is 99.99 and min is 00.00 with 00.01 resolution
#define MIN_KP					0
#define MAX_KI					9999
#define MIN_KI					0
#define MAX_INTEGRATE_ERROR		MAX_RPM		//Used to cap the integrate error since the error of the integrate is a continual sum
#define MIN_INTEGRATE_ERROR		0
#define MAX_KD					9999
#define MIN_KD					0

//Led mask
#define LEDS_OFF				0x00000000

// GPIO Control_output mask/shift constants
#define btnR_mask						0x2
#define btnR_shift						1
#define btnL_mask						0x4
#define btnL_shift						2
#define btnD_mask						0x8
#define btnD_shift						3
#define btnU_mask						0x10
#define btnU_shift						4
#define btnC_mask						0x20
#define btnC_shift						5

#define switches_mask					0x3fffc0
#define switches_shift					6

#define rotary_b_mask					0x400000
#define rotary_b_shift					22
#define rotary_a_mask					0x800000
#define rotary_a_shift					23
#define rotary_sw_mask					0x1000000
#define rotary_sw_shift					24
#define rotary_press_mask				0x2000000
#define rotary_press_shift				25

// Switches mask/shift constants
#define force_crash_mask				0x8000
#define force_crash_shift				15

#define motor_increment_size_mask		0x3
#define motor_increment_size_shift		0

#define constant_select_mask			0xc
#define constant_select_shift			2

#define constant_increment_size_mask	0x30
#define constant_increment_size_shift	4

#define DISPLAY_THREAD_MSG_ID			1
#define PID_THREAD_MSG_ID				2

// Peripheral instances
PmodHB3			pmodHB3_inst;				// PmodHB3 instance ref
PmodOLEDrgb		pmodOLEDrgb_inst;			// PmodOLED instance ref
XGpio 			BoardINPUT;
XTmrCtr 		TMRCTR0Inst,TMRCTR1Inst;
XIntc 			IntrptCtlrInst;
XWdtTb 			WDTInst;
PmodENC 		pmodENC_inst;				// PmodENC instance ref
XTmrCtr			AXITimerInst;				// PWM timer instance

//global variables
volatile u32 force_crash = 0;	// Force crash flag
volatile u32 system_running = 1;// System running flag

// Message passing variables
typedef struct
{
	int msg_src;		// Used to specify the message type, for having multiple destination threads in the same message queue
	u8 SetPoint;		// Desired Motor Speed, 0-255
	int kp,ki,kd;		// PID constants
	u16 switch_value;	// switch values for sw[15:0]
	u8 motor_direction;	// motor direction
	u8 test_flag;		// test flag used to run a test (aka stop the motor, wait a few seconds, and
} t_PID_message, *t_PID_messageptr;

typedef struct
{
	int msg_src;		// Used to specify the message type, for having multiple destination threads in the same message queue
	u16 motor_input;	// Motor speed input setting
	u8 motor_drive;		// Motor direction setting
} t_motor_message, *t_motor_messageptr;

// message queue keys for LED message queue
const int motor_input_key = 1;		// Key for queue written by motor_input_thread (actual motor speed/direction) and read by Display and PID threads
const int control_input_key = 2;	// Key for queue written by the parameter input thread (buttons/rotary encoder/switches) and read by Display and PID threads
const int motor_output_key = 3;		// Key for queue written by the PID thread and read by the motor output thread

// statistics from message queue
t_PID_message motor_control_input;				// Message used by the Parameter Input Thread to write it's current state (Buttons/Switches/Rotary Encoder) to the message queue
t_PID_message motor_control_input_receive;		// Message used by the PID thread to read the message sent by the Parameters Input Thread
t_PID_message motor_control_input_receive_dis;	// Message used by the Display thread to read the message sent by the Parameters Input Thread
t_motor_message motor_input;					// Message used by the Motor Input Thread to write it's current state (actual motor speed/direction) to the message queue.
t_motor_message motor_input_receive;			// Message used by the PID thread to read the message sent by the Motor Input Thread.
t_motor_message motor_input_receive_dis;		// Message used by the Display thread to read the message sent by the Motor Input Thread.
t_motor_message motor_control_output;			// Message used by the PID thread to write it's current motor drive to the motor output thread.
t_motor_message motor_control_output_receive;	// Message used by the Motor Output Thread to read the message sent by the PID thread.

// semaphore between the Nexys4 DDR board inputs and PMODENC inputs
// interrupt handler and the Parameter_input_thread
sem_t control_input_change_sema;

// Function declarations (descriptions before function contents)
void* master_thread(void *arg);
void* Motor_output_thread(void *arg);
void* Parameter_input_thread(void *arg);
void* PID_thread(void *arg);
void* Display_thread(void *arg);
void* Motor_input_thread(void *arg);
void  control_handler(void);
void  wdt_handler(void);
XStatus init_peripherals(void);
XStatus do_init();
void bin2bcd(unsigned long bin, unsigned char *bcd); //had to include definition otherwise "implicit error message"

u32 Control_input,Control_input_old;

/****************************************************************************************/
/****************************************** main program ********************************/
/****************************************************************************************/
int main()
{
	XStatus sts;

	// initialize the platform and the peripherals
	init_platform();
	sts = do_init();
	if (sts != XST_SUCCESS)
	{
		xil_printf("FATAL ERROR: Could not initialize the peripherals\n\r");
		xil_printf("Please power cycle or reset the system\n\r");
		return -1;
	}

	xil_printf("MAIN: Before checking the WDT\n \r");


	if (XWdtTb_IsWdtExpired(&WDTInst))
	{
		// it's true, the WDT expired.
		// stop the WDT and clear the bits that are set to indicate the reason for the last reset.
		XWdtTb_Stop(&WDTInst);

		xil_printf("\r\n MAIN:Watchdog timer previous state reset \r\n");

		sts = XWdtTb_Initialize(&WDTInst, WDT_DEVICEID);

		if (sts != XST_SUCCESS)
		{
			return XST_FAILURE;
		}

		// reset force_crash to zero and setup the system as running
		force_crash = 0;
		system_running = 1;
	}

	// Initialize xilkernel
	xilkernel_init();

	// Create the master thread
	xmk_add_static_thread(master_thread, 1);

	// Start the kernel
	xilkernel_start();

	// Should never be reached
	cleanup_platform();

	return 0;
}


/****************************************************************************************/
/************************************** Master thread ***********************************/
/****************************************************************************************/
/**
 * This thread sets up the semaphore, interrupts, and other lower priority threads.
 */
void* master_thread(void *arg)
{
	pthread_t control_input;
	pthread_t PID;
	pthread_t motor_input;
	pthread_t motor_output;
	pthread_t display_thread;

	pthread_attr_t attr;
	struct sched_param spar;

	int ret;

	xil_printf("MASTER THREAD: Master Thread Starting\r\n");

	// initialize the button press semaphore
	ret = sem_init (&control_input_change_sema, 0, 0);
	if (ret != 0)
	{
		xil_printf("ERROR (%d) IN MASTER THREAD: could not initialize %s\r\n", errno, "button press semaphore");
		xil_printf("FATAL ERROR: Master Thread Terminating\r\n");
		return (void*) -3;
	}

	xil_printf ("MASTER THREAD: Control input change semaphore has been initialized\n\r");

	// Register the interrupt handlers
	ret = register_int_handler(WDT_INTR_NUM, (void*) wdt_handler, NULL);
	if (ret != XST_SUCCESS)
	{
		return (void*) -4;
	}

	// set the priority of all but the master thread to 1.  The master thread runs at priority 0
	// because it is responsible for tickling the WDT.
	pthread_attr_init (&attr);
	spar.sched_priority = 1;
	pthread_attr_setschedparam(&attr, &spar);

	// create the Parameter_input_thread
	ret = pthread_create (&control_input, &attr, (void*) Parameter_input_thread, NULL);
	if (ret != 0)
	{
		xil_printf("ERROR (%d) IN MASTER THREAD: could not launch %s\r\n", ret, "button thread");
		xil_printf("FATAL ERROR: Master Thread Terminating\r\n");
		return (void*) -3;
	}

	// create the Display_thread
	ret = pthread_create (&display_thread, &attr, (void*) Display_thread, NULL);
	if (ret != 0)
	{
		xil_printf("ERROR (%d) IN MASTER THREAD: could not launch %s\r\n", ret, "display thread");
		xil_printf("FATAL ERROR: Master Thread Terminating\r\n");
		return (void*) -3;
	}
	
	// create the Motor_input_thread
	ret = pthread_create (&motor_input, &attr, (void*) Motor_input_thread, NULL);
	if (ret != 0)
	{
		xil_printf("ERROR (%d) IN MASTER THREAD: could not launch %s\r\n", ret, "motor input thread");
		xil_printf("FATAL ERROR: Master Thread Terminating\r\n");
		return (void*) -3;
	}

	// create the PID_thread
	ret = pthread_create (&PID, &attr, (void*) PID_thread, NULL);
	if (ret != 0)
	{
		xil_printf("ERROR (%d) IN MASTER THREAD: could not launch %s\r\n", ret, "PID thread");
		xil_printf("FATAL ERROR: Master Thread Terminating\r\n");
		return (void*) -3;
	}

	// create the Motor_output_thread
	ret = pthread_create (&motor_output, &attr, (void*) Motor_output_thread, NULL);
	if (ret != 0)
	{
		xil_printf("ERROR (%d) IN MASTER THREAD: could not launch %s\r\n", ret, "motor output thread");
		xil_printf("FATAL ERROR: Master Thread Terminating\r\n");
		return (void*) -3;
	}
	
	// Enable the WDT interrupt handler and start the WDT...we're off to the races
	enable_interrupt(WDT_INTR_NUM);
	xil_printf("MASTER THREAD: Interrupts have been enabled\r\n");

	XWdtTb_Start(&WDTInst);
	xil_printf("MASTER THREAD: Watchdog timer has been started\r\n");

	// Enable the other interrupt handlers here (ex: Nexys4 DDR input handler)
	enable_interrupt(N4INPUT_GPIO_INTR_NUM);

	// master thread main loop
	while(1)
	{

		// Check if system is running, if it is not running print a message and exit
		if (!system_running){
			xil_printf("MAIN THREAD: System is not running.\r\n");
			return NULL;
		}
	}

	xil_printf("MASTER THREAD: Outside of wait loop");

	return NULL;
}


/****************************************************************************************/
/***************************************** Parameter Input thread ***********************/
/****************************************************************************************/
/**
 *	The Parameter_input_thread set's up the input parameter capture and formatting for
 *	downstream changes.
 */
void* Parameter_input_thread(void *arg)
{
	// Parameters for GPIO inputs
	volatile u8 btnL, btnD, btnU, btnC;
	volatile u16 switches;
	volatile u8 rotary_sw;

	// Parameters from switch values
	u8 force_crash_switch;
	u8 motor_increment_size_switch;
	u8 constant_increment_size_switch;
	u8 constant_select_switch = 0;

	u8 motor_increment_size = 1;
	u8 constant_increment_size = 1;
	u8 constant_select = 1;

	u8 motor_direction = 0;
	u8 SetPoint = 0;

	// Parameters stored and adjusted by input parameters
	int kP = 0;
	int kI = 0;
	int kD = 0;

	uint16_t RotaryCnt;
	u8 RotaryIncr = 1;
	bool RotaryNoNeg = true;
	uint16_t prevRotary = 0;

	// Parameter used for setting up a test
	u8 test_flag = 0;

	// Parameters for error detection and messaging
	int msgid1;
	int consret;

	// Initialize the rotary encoder
	// clear the counter of the encoder if initialized to garbage pcntrl_const on power on
	pmodENC_init(&pmodENC_inst, RotaryIncr, RotaryNoNeg);
	pmodENC_clear_count(&pmodENC_inst);

	// obtain the id for the control input message queue
	msgid1 = msgget(control_input_key, IPC_CREAT);
	if( msgid1 == -1 )
	{
		xil_printf ("PARAMETER INPUT THREAD: ERROR while opening Control Input message queue. Errno: %d \r\n", errno);
		consret = errno;
		system_running = 0;
		pthread_exit(&consret) ;
	}

	// Clear the message queue from a reset
	// receive a new control input message if one is available
	t_PID_message temp;
	while (1){
		if( msgrcv( msgid1, &temp, sizeof(temp), DISPLAY_THREAD_MSG_ID, IPC_NOWAIT) != sizeof(temp) )
		{
			// If there is no message in the queue, break out of the loop
			if ((errno == ENOMSG) || (errno == EAGAIN)){
				xil_printf("PARAMETER INPUT THREAD: Parameter Input Message Queue for Display thread types.\n\r");
				break;

			// If there is any other error, print that error and exit
			} else {
				xil_printf ("PARAMETER INPUT THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				system_running = 0;
				pthread_exit(&consret);
			}
		}
	}

	while (1){
		if( msgrcv( msgid1, &temp, sizeof(temp), PID_THREAD_MSG_ID, IPC_NOWAIT) != sizeof(temp) )
		{
			// If there is no message in the queue, break out of the loop
			if ((errno == ENOMSG) || (errno == EAGAIN)){
				xil_printf("PARAMETER INPUT THREAD: Parameter Input Message Queue for PID thread types.\n\r");
				break;

			// If there is any other error, print that error and exit
			} else {
				xil_printf ("PARAMETER INPUT THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				system_running = 0;
				pthread_exit(&consret);
			}
		}
	}


	// Main while loop
	while (1){
		// Wait for a GPIO interrupt from the control_handler() function.
		sem_wait(&control_input_change_sema);

		// read the value from the rotary encoder, rotary_a and rotary_b are not able to be used for rotary values as they change to frequently to be sampled in software
		pmodENC_read_count(&pmodENC_inst, &RotaryCnt);

		// Only enter the block if the inputs changed, used to settle rotary encoder changes
		if ((Control_input != Control_input_old) || (prevRotary != RotaryCnt)){
			Control_input_old = Control_input;

			// Read the buttons from the GPIO state
			btnL = (Control_input_old & btnL_mask) >> btnL_shift;
			btnD = (Control_input_old & btnD_mask) >> btnD_shift;
			btnU = (Control_input_old & btnU_mask) >> btnU_shift;
			btnC = (Control_input_old & btnC_mask) >> btnC_shift;

			// Read the switches from the GPIO state
			switches = (Control_input_old & switches_mask) >> switches_shift;

			// Read the rotary switch from the GPIO state
			rotary_sw = (Control_input_old & rotary_sw_mask) >> rotary_sw_shift;

			// Determine if we are forcing a crash based on sw[15] value
			force_crash_switch = ((switches & force_crash_mask) >> force_crash_shift);
			if (force_crash_switch == 0x1){
				force_crash = 1;
			}

			// Determine the motor increment size:
			// sw[1:0] == 0, increment by 1
			// sw[1:0] == 1, increment by 5
			// sw[1:0] == 2 or 3, increment by 10
			motor_increment_size_switch = ((switches & motor_increment_size_mask) >> motor_increment_size_shift);
			if (motor_increment_size_switch > 1){
				motor_increment_size = 10;
			}else if (motor_increment_size_switch == 1){
				motor_increment_size = 5;
			}else{
				motor_increment_size = 1;
			}

			// Determine the constant increment size
			// sw[5:4] == 0, increment by 1
			// sw[5:4] == 1, increment by 5
			// sw[5:4] == 2, increment by 10
			// sw[5:4] == 3, increment by 100
			constant_increment_size_switch = ((switches & constant_increment_size_mask) >> constant_increment_size_shift);
			if (constant_increment_size_switch == 3){
				constant_increment_size = 100;
			}else if (constant_increment_size_switch == 2){
				constant_increment_size = 10;
			}else if (constant_increment_size_switch == 1){
				constant_increment_size = 5;
			}else{
				constant_increment_size = 1;
			}

			// Determine the constant select
			// sw[3:2] == 0, kP is selected
			// sw[3:2] == 1, kI is selected
			// sw[3:2] == 2 or 3, kD is selected
			constant_select_switch = ((switches & constant_select_mask) >> constant_select_shift);
			if (constant_select_switch > 1){
				constant_select = 3;
			}else if (constant_select_switch == 1){
				constant_select = 2;
			}else{
				constant_select = 1;
			}

			// Increment the selected constant if the Up Button is pressed
			// Saturate the constant by the MAX value (9999) or MIN value (0) allowing 10000 values
			// These are fixed precision 2-decimal values
			if (btnU){
				if (constant_select == 1){
					if ((kP + constant_increment_size) >= MAX_KP){
						kP = MAX_KP;
					} else {
						kP += constant_increment_size;
					}
				} else if (constant_select == 2){
					if ((kI + constant_increment_size) >= MAX_KI){
						kI = MAX_KI;
					} else {
						kI += constant_increment_size;
					}
				} else if (constant_select == 3){
					if ((kD + constant_increment_size) >= MAX_KD){
						kD = MAX_KD;
					} else {
						kD += constant_increment_size;
					}
				}
			}

			// Decrement the selected constant if the Down Button is pressed
			if (btnD){
				if (constant_select == 1){
					if ((kP - constant_increment_size) <= MIN_KP){
						kP = MIN_KP;
					} else {
						kP -= constant_increment_size;
					}
				} else if (constant_select == 2){
					if ((kI - constant_increment_size) <= MIN_KI){
						kI = MIN_KI;
					} else {
						kI -= constant_increment_size;
					}
				} else if (constant_select == 3){
					if ((kD - constant_increment_size) <= MIN_KD){
						kD = MIN_KD;
					} else {
						kD -= constant_increment_size;
					}
				}
			}

			// The left button is used to set a test_flag, which zero's the motor output keeping the SetPoint and PID constants, wait for 2 seconds, and resume
			if (btnL){
				test_flag = 1;
			} else {
				test_flag = 0;
			}

			// Increment or Decrement SetPoint based on rotary encoder change and motor increment size
			// Saturate the value to the Max (255) or Min (0)
			if (RotaryCnt > prevRotary) {
				if ((SetPoint + motor_increment_size) >= MAX_MOTOR_SPEED){
					SetPoint = MAX_MOTOR_SPEED;
				} else {
					SetPoint += motor_increment_size;
				}
			}

			if (RotaryCnt < prevRotary) {
				if ((SetPoint - motor_increment_size) <= MIN_MOTOR_SPEED){
					SetPoint = MIN_MOTOR_SPEED;
				} else {
					SetPoint -= motor_increment_size;
				}
			}

			// Reset constants and set point when center button is pressed
			if (btnC){
				SetPoint = 0;
				kP = 0;
				kI = 0;
				kD = 0;
			}

			// the motor direction is directly linked to the rotary switch
			motor_direction = rotary_sw;

			// Setup motor_control_input message for the display thread (msg_src = 1)
			motor_control_input.msg_src = DISPLAY_THREAD_MSG_ID;
			motor_control_input.SetPoint = SetPoint;
			motor_control_input.kp = kP;
			motor_control_input.ki = kI;
			motor_control_input.kd = kD;
			motor_control_input.switch_value = switches;
			motor_control_input.motor_direction = motor_direction;
			motor_control_input.test_flag = test_flag;

			// send a new control input message
			if( msgsnd( msgid1, &motor_control_input, sizeof(motor_control_input), 0 ) < 0 )
			{
				if(errno == ENOSPC){
					xil_printf ("PARAMETER INPUT THREAD: Too many messages, need to wait...\r\n");
				} else {
					xil_printf ("PARAMETER INPUT THREAD: msgsnd ran into ERROR.Errno: %d. Halting...\r\n", errno);
					consret = errno;
					system_running = 0;
					pthread_exit(&consret);
				}
			}

			// Setup motor_control_input message for the PID thread (msg_src = 2)
			motor_control_input.msg_src = PID_THREAD_MSG_ID;
			if( msgsnd( msgid1, &motor_control_input, sizeof(motor_control_input), 0 ) < 0 )
			{
				if(errno == ENOSPC){
					xil_printf ("PARAMETER INPUT THREAD: Too many messages, need to wait...\r\n");
				} else {
					xil_printf ("PARAMETER INPUT THREAD: msgsnd ran into ERROR.Errno: %d. Halting...\r\n", errno);
					consret = errno;
					system_running = 0;
					pthread_exit(&consret);
				}
			}

			// Store the previous rotary count for change detection
			prevRotary = RotaryCnt;
		}
	}
	return NULL;
}



/****************************************************************************************/
/**************************************** PID thread ************************************/
/****************************************************************************************/
/**
 * 	The PID thread reads form the parameter input thread and motor input thread, calculates the
 * 	motor output drive using PID control, and pushes the motor output drive to the motor output thread.
 */
void* PID_thread(void *arg)
{
	// Code to receive messages from the Parameter_input_thread and the Motor_input_thread.  Calculate
	// the motor speed output using the PID control algorithm and send the updated output to the Motor_output_thread.

	// Parameters for message queue's
	int msgid1, msgid2, msgid3;

	// Parameters to store message contents from motor input thread
	volatile int actualRpm = 0;

	// Parameters to store message contents from the parameters input thread
	volatile u8 SetPoint = 0;
	volatile int kP = 0;
	volatile int kI = 0;
	volatile int kD = 0;
	volatile u8 motor_direction = 0;
	volatile u8 test_flag;

	// Parameters for writing to the motor output thread message queue
	u8 motor_direction_output = 0;
	u16 motor_speed_output = 0;

	// Parameters for calculating the PID control
	volatile int desiredRpm = 0;
	volatile int signedError				= 0;
	volatile int prev_signedError			= 0;
	volatile int integrate_err				= 0;
	volatile float p_term					= 0;
	volatile float i_term					= 0;
	volatile float d_term					= 0;
	volatile int motorOutRpm				= 0;

	// Flags used to skip the read of message queue's that aren't ready (aka empty)
	volatile int no_motor_input_flag = 0;
	volatile int no_parameter_input_flag = 0;

	// Parameter for error/debug
	int consret;

	// obtain the id for the control input message queue (Parameter input thread)
	msgid1 = msgget(control_input_key, IPC_CREAT);
	if( msgid1 == -1 )
	{
		xil_printf ("PID THREAD: ERROR while opening Control Input message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	// obtain the id for the motor input message queue (Motor input thread)
	msgid2 = msgget(motor_input_key, IPC_CREAT);
	if( msgid2 == -1 )
	{
		xil_printf ("PID THREAD: ERROR while opening Motor input message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	// obtain the id for the motor output message queue (Motor output thread)
	msgid3 = msgget(motor_output_key, IPC_CREAT);
	if( msgid3 == -1 )
	{
		xil_printf ("PID THREAD: ERROR while opening Motor output message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	// Clear the message queue from a reset
	// receive a new control input message if one is available
	t_motor_message temp;
	while (1){
		if( msgrcv( msgid3, &temp, sizeof(temp), 0, IPC_NOWAIT) != sizeof(temp) )
		{
			// If there is no message in the queue, break out of the loop
			if ((errno == ENOMSG) || (errno == EAGAIN)){
				xil_printf("PID THREAD: Cleared Motor Output Message Queue\n\r");
				break;

			// If there is any other error, print that error and exit
			} else {
				xil_printf ("PID THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				system_running = 0;
				pthread_exit(&consret);
			}
		}
	}

	xil_printf("\nPID THREAD: in PID thread \r\n");

	// Main while loop
	while (1)
	{
		// receive a new control input message if one is available (from parameter_input_thread)
		// Using IPC_NOWAIT allows for returning immediately and not waiting/blocking if the queue is empty, allowing for the PID to respond to motor speed changes
		// seperately/independently from parameter input changes.
		// The DISPLAY_THREAD_MSG_ID is used to only receive message of a certain type, since the parameter input thread writes two identical messages, one for
		// the display thread and one for the PID thread, the image type must be used to distinguish the two on a recieve.
		if( msgrcv( msgid1, &motor_control_input_receive, sizeof(motor_control_input_receive), PID_THREAD_MSG_ID, IPC_NOWAIT) != sizeof(motor_control_input) )
		{
			if ((errno == ENOMSG) || (errno == EAGAIN) || (errno == ENOSPC)){
				no_parameter_input_flag = 1;
			} else {
				xil_printf ("PID THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				system_running = 0;
				pthread_exit(&consret);
			}
		}

		// receive a new motor input message if one is available (from Motor input thread)
		if( msgrcv( msgid2, &motor_input_receive, sizeof(motor_input_receive), PID_THREAD_MSG_ID, IPC_NOWAIT) != sizeof(motor_input) )
		{
			if ((errno == ENOMSG) || (errno == EAGAIN) || (errno == ENOSPC)){
				no_motor_input_flag = 1;
			} else {
				xil_printf ("PID THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				system_running = 0;
				pthread_exit(&consret);
			}
		}

		// Grab the parameter inputs if the no_parameter_input_flag is not set
		if (!no_parameter_input_flag) {
			SetPoint = motor_control_input_receive_dis.SetPoint;
			kP = motor_control_input_receive_dis.kp;
			kI = motor_control_input_receive_dis.ki;
			kD = motor_control_input_receive_dis.kd;
			motor_direction = motor_control_input_receive_dis.motor_direction;
			test_flag = motor_control_input_receive.test_flag;

		// else skip reading inputs (none available) and clear the no_parameter_input_flag
		} else {
			no_parameter_input_flag = 0;
		}

		// Grab the motor inputs if the no_motor_input_flag is not set
		if (!no_motor_input_flag) {
			actualRpm = motor_input_receive_dis.motor_input;

		// else skip reading inputs (none available) and clear the no_motor_input_flag
		} else {
			no_motor_input_flag = 0;
		}

		motor_direction_output = motor_direction;

		// Test-Case logic
		if(test_flag){
			xil_printf("Executing test.\n\r");

			// Force motor input to zero and send message to the motor output
			motor_control_output.motor_drive = motor_direction_output;
			motor_control_output.motor_input = 0;

			// send a new control input message
			if( msgsnd( msgid3, &motor_control_output, sizeof(motor_control_output), 0 ) < 0 )
			{
				if ((errno == ENOMSG) || (errno == EAGAIN) || (errno == ENOSPC)){
					xil_printf("PID THREAD: ERRNO (motor outputs): %d\n\r", errno);
					no_motor_input_flag = 1;
				} else {
					xil_printf ("PID THREAD: msgsnd ran into ERROR.Errno: %d. Halting...\r\n", errno);
					consret = errno;
					pthread_exit(&consret);
				}
			}

			// wait for 2 seconds
			usleep(2000);
			xil_printf("Finished halting the motor.\n\r");

			// clear test flag to resume normal functionality
			test_flag = 0;
		} else {
			// Calculate Desired and Motor Output...

			// Convert SetPoint to RPM
			desiredRpm = SetPoint * MAX_RPM/MAX_MOTOR_SPEED;

			// Proportional term
			signedError = desiredRpm - actualRpm;
			p_term = (float)(kP)/CONSTANT_MULTIPLIER * signedError;

			//Integral term (sum of the error)
			integrate_err += signedError;

			//saturate min/max integrate error (set to min/max motor RPM)
			if(integrate_err > MAX_INTEGRATE_ERROR){
				integrate_err = MAX_INTEGRATE_ERROR;
			}
			else if(integrate_err < MIN_INTEGRATE_ERROR){
				integrate_err = MIN_INTEGRATE_ERROR;
			}

			i_term = (float)(kI)/CONSTANT_MULTIPLIER * integrate_err;

			// Derivative term (Difference of errors, could have used difference of absolute motor speeds too...)
			d_term = (float)(kD)/CONSTANT_MULTIPLIER * (signedError - prev_signedError);

			//Calculate motor output
			motorOutRpm = desiredRpm + p_term + i_term + d_term;

			// Bound the max/min output RPM before sending to the motor.
			if(motorOutRpm > MAX_RPM){
					motorOutRpm = MAX_RPM;
			}
			else if (motorOutRpm < 0){
				motorOutRpm = 0;
			}

			// Convert the output motor RPM to 0-255 based value for writing to the motor output thread
			motor_speed_output = (u8)(motorOutRpm * MAX_MOTOR_SPEED / MAX_RPM );

			// Print the desired/actual/output values for charting
			xil_printf("Desired: %d, DesiredRpm: %d, ActualRpm: %d, MotorOut: %d, MotorOutRpm: %d \n\r", SetPoint, desiredRpm, actualRpm, motor_speed_output, motorOutRpm);

			// Create a motor output thread structure for writing to the motor output thread
			motor_control_output.motor_drive = motor_direction_output;
			motor_control_output.motor_input = motor_speed_output;

			// send a new control input message
			if( msgsnd( msgid3, &motor_control_output, sizeof(motor_control_output), 0 ) < 0 )
			{
				if (errno == ENOSPC){
					// Non-fatal errors for sending...if the queue is full (aka not read fast enough)
					xil_printf("PID THREAD: msgsnd ran into ERROR. Errno: %d\n\r", errno);
				} else {
					xil_printf ("PID THREAD: msgsnd ran into ERROR.Errno: %d. Halting...\r\n", errno);
					consret = errno;
					pthread_exit(&consret);
				}
			}

			// Store previous error for Difference Calculation
			prev_signedError = signedError;
		}
	}

	return NULL;
}



/****************************************************************************************/
/************************************** Display thread **********************************/
/****************************************************************************************/
/**
 * Display thread, reads from the parameters input thread and motor input thread and displays
 * the current motor speed and kP/kI/or kD value on the seven segment display and lights the LED's
 * with the current switch values.
 */

void* Display_thread(void *arg)
{
	// Parameters for message queues
	int msgid1,msgid2;

	// variables to store parameters input message contents
	u16 switches;
	volatile u8 SetPoint = 0;
	volatile int kP = 0;
	volatile int kI = 0;
	volatile int kD = 0;
	volatile u8 motor_direction = 0;

	// Used to select the constant for displaying on the seven segment display
	u8 constant_select_switch = 0;
	u32 tU;

	// variables to store motor_input message contents
	volatile u16 actual_speed = 0;
	volatile u8 actual_direction = 0;
	int consret;

	// HEX to BCD buffers for SSEG display
	unsigned char sseg_constant[10]   = {0};
	unsigned char sseg_rpm[10]  = {0};
	
	// Flags to update the display of the parameter input thread and motor input thread seperately
	volatile int no_motor_input_flag = 0;
	volatile int no_parameter_input_flag = 0;

	// obtain the id for the control input message queue (Parameter input thread)
	msgid1 = msgget(control_input_key, IPC_CREAT);
	if( msgid1 == -1 )
	{
		xil_printf ("DISPLAY THREAD: ERROR while opening Control Input message queue. Errno: %d \r\n", errno);
		consret = errno;
		system_running = 0;
		pthread_exit(&consret) ;
	}

	// obtain the id for the motor input message queue (Motor input thread)
	msgid2 = msgget(motor_input_key, IPC_CREAT);
	if( msgid2 == -1 )
	{
		xil_printf ("DISPLAY_THREAD: ERROR while opening Motor input message queue. Errno: %d \r\n", errno);
		consret = errno;
		system_running = 0;
		pthread_exit(&consret) ;
	}

	xil_printf("\nDISPLAY THREAD: in Display thread \r\n");

	// Main while loopo
	while (1)
	{
		// receive a new control input message if one is available
		if( msgrcv( msgid1, &motor_control_input_receive_dis, sizeof(motor_control_input_receive_dis), DISPLAY_THREAD_MSG_ID, IPC_NOWAIT) != sizeof(motor_control_input) )
		{
			if ((errno == ENOMSG) || (errno == EAGAIN)){
				no_parameter_input_flag = 1;
			} else {
				xil_printf ("DISPLAY THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				system_running = 0;
				pthread_exit(&consret);
			}
		}

		// receive a new motor input message if one is available
		if( msgrcv( msgid2, &motor_input_receive_dis, sizeof(motor_input_receive_dis), DISPLAY_THREAD_MSG_ID, IPC_NOWAIT) != sizeof(motor_input) )
		{
			if ((errno == ENOMSG) || (errno == EAGAIN)){
				no_motor_input_flag = 1;
			} else {
				xil_printf ("DISPLAY THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				system_running = 0;
				pthread_exit(&consret);
			}
		}

		// Read in the parameter input thread if we got a new message, else don't and clear the flag.
		if (!no_parameter_input_flag) {
			switches = motor_control_input_receive_dis.switch_value;
			SetPoint = motor_control_input_receive_dis.SetPoint;
			kP = motor_control_input_receive_dis.kp;
			kI = motor_control_input_receive_dis.ki;
			kD = motor_control_input_receive_dis.kd;
			motor_direction = motor_control_input_receive_dis.motor_direction;
		} else {
			no_parameter_input_flag = 0;
		}
		
		// Read in the motor input thread if we got a new message, else don't and clear the flag.
		if (!no_motor_input_flag) {
			actual_speed = motor_input_receive_dis.motor_input;
			actual_direction = motor_input_receive_dis.motor_drive;
		} else {
			no_motor_input_flag = 0;
		}

		// Set tU to the constant selected from the switches, used for displaying on the seven segment display
		constant_select_switch = ((switches & constant_select_mask) >> constant_select_shift);
		if (constant_select_switch > 1){
			tU = motor_control_input_receive_dis.kd;
		}else if (constant_select_switch == 1){
			tU = motor_control_input_receive_dis.ki;
		}else{
			tU = motor_control_input_receive_dis.kp;
		}

		// Set the LED's to the switch values
		NX4IO_setLEDs(switches);

		//SSEG Display Logic
		bin2bcd(tU, sseg_constant);

		if(sseg_constant[6] == 0)
		{
			sseg_constant[6] = CC_BLANK;
		}


		NX4IO_SSEG_setDigit(SSEGHI, DIGIT7, sseg_constant[6]);	// display thousand digit
		NX4IO_SSEG_setDigit(SSEGHI, DIGIT6, sseg_constant[7]);	// display hundred digit
		NX4IO_SSEG_setDigit(SSEGHI, DIGIT5, sseg_constant[8]);	// display tens digit
		NX4IO_SSEG_setDigit(SSEGHI, DIGIT4, sseg_constant[9]);	// display ones digit

		// fixed precision 2-digit value means we put the decimal point on digit 6
		NX4IO_SSEG_setDecPt(SSEGHI, DIGIT6, true);

		// Calculate RPM based on actual motor speed and print to SSEG Display
		bin2bcd(actual_speed, sseg_rpm);

		if(sseg_rpm[6] == 0)
		{
			sseg_rpm[6] = CC_BLANK;
		}

		NX4IO_SSEG_setDigit(SSEGLO, DIGIT3, sseg_rpm[6]);	// display thousand digit
		NX4IO_SSEG_setDigit(SSEGLO, DIGIT2, sseg_rpm[7]);	// display hundred digit
		NX4IO_SSEG_setDigit(SSEGLO, DIGIT1, sseg_rpm[8]);	// display tens digit
		NX4IO_SSEG_setDigit(SSEGLO, DIGIT0, sseg_rpm[9]);	// display ones digit
	}

	// Clear all the display digits and the OLED display at the end of the program
	NX410_SSEG_setAllDigits(SSEGHI, CC_BLANK, CC_B, CC_LCY, CC_E, DP_NONE);
	NX410_SSEG_setAllDigits(SSEGLO, CC_B, CC_LCY, CC_E, CC_BLANK, DP_NONE);

	return NULL;
}




/****************************************************************************************/
/************************************** Motor Input thread ******************************/
/****************************************************************************************/
/**
 * The motor input thread reads the current motor speed from the PmodHB3, and sends the
 * value to the Display Thread and the PID thread.  This is performed after a sleep of 100
 * such that we don't over-run the message queues.  Another implementation would have been
 * to attach an interrupt handler connect to the FIT timer.
 */
void* Motor_input_thread(void *arg)
{
	// Parameters used to store the PmodHB3 values
	volatile u16 actualRpm = 0;
	volatile u8 actual_motor_direction = 0;

	// Parameters for message queues and error detection
	int consret;
	int msgid2;


	// obtain the id for the motor input message queue
	msgid2 = msgget(motor_input_key, IPC_CREAT);
	if( msgid2 == -1 )
	{
		xil_printf ("MOTOR INPUT THREAD: ERROR while opening Motor input message queue. Errno: %d \r\n", errno);
		consret = errno;
		system_running = 0;
		pthread_exit(&consret) ;
	}

	// Clear the message queue from a reset
	// receive a new control input message if one is available
	t_motor_message temp;
	while (1){
		if( msgrcv( msgid2, &temp, sizeof(temp), DISPLAY_THREAD_MSG_ID, IPC_NOWAIT) != sizeof(temp) )
		{
			// If there is no message in the queue, break out of the loop
			if ((errno == ENOMSG) || (errno == EAGAIN)){
				xil_printf("MOTOR INPUT THREAD: Parameter Input Message Queue for Display thread types.\n\r");
				break;

			// If there is any other error, print that error and exit
			} else {
				xil_printf ("MOTOR INPUT THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				system_running = 0;
				pthread_exit(&consret);
			}
		}
	}

	while (1){
		if( msgrcv( msgid2, &temp, sizeof(temp), PID_THREAD_MSG_ID, IPC_NOWAIT) != sizeof(temp) )
		{
			// If there is no message in the queue, break out of the loop
			if ((errno == ENOMSG) || (errno == EAGAIN)){
				xil_printf("MOTOR INPUT THREAD: Parameter Input Message Queue for Display thread types.\n\r");
				break;

			// If there is any other error, print that error and exit
			} else {
				xil_printf ("MOTOR INPUT THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				system_running = 0;
				pthread_exit(&consret);
			}
		}
	}

	xil_printf("\nMOTOR INPUT THREAD: in MOTOR INPUT thread \r\n");

	// Main while loop
	while (1)
	{
		// First, read the pmodHB3 motor speed
		actualRpm = pmodHB3_read_motor_speed(&pmodHB3_inst);

		// Send motor inputs to PID thread (msg_src = 1)
		motor_input.msg_src = DISPLAY_THREAD_MSG_ID;
		motor_input.motor_drive = actual_motor_direction;
		motor_input.motor_input = actualRpm;

		if( msgsnd( msgid2, &motor_input, sizeof(motor_input), 0) < 0 )
		{
			if (errno == ENOSPC){
				xil_printf("MOTOR INPUT THREAD: Too many messages, need to wait.\r\n");
			} else {
				xil_printf ("MOTOR INPUT THREAD: msgsnd ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				pthread_exit(&consret);
			}
		}

		// Send motor inputs to PID thread (msg_src = 2)
		motor_input.msg_src = PID_THREAD_MSG_ID;
		if( msgsnd( msgid2, &motor_input, sizeof(motor_input), 0) < 0 )
		{
			if (errno == ENOSPC){
				xil_printf("MOTOR INPUT THREAD: Too many messages, need to wait.\r\n");
			} else {
				xil_printf ("MOTOR INPUT THREAD: msgsnd ran into ERROR.Errno: %d. Halting...\r\n", errno);
				consret = errno;
				system_running = 0;
				pthread_exit(&consret);
			}
		}
		usleep(100);
	}
	return NULL;
}



/****************************************************************************************/
/************************************** Motor Output thread *****************************/
/****************************************************************************************/
/**
 * Motor output thread reads a message from the PID thread (blocking read, it doesn't have to wait on any other thread)
 * The PID message is used to output directly to the PmodHB3 motor controller
 */
void* Motor_output_thread(void *arg)
{
	// Parameters for error detection and message passing
	int consret;
	int msgid3;

	// Parameters to store the PID thread message values
	volatile u16 motor_speed = 0;
	volatile u8 motor_direction = 0;

	// obtain the id for the motor output message queue
	msgid3 = msgget(motor_output_key, IPC_CREAT);
	if( msgid3 == -1 )
	{
		xil_printf ("MOTOR OUTPUT THREAD: ERROR while opening Motor output message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	xil_printf("\nMOTOR OUTPUT THREAD: in MOTOR OUTPUT thread \r\n");

	// Main while loop
	while (1)
	{
		// receive a new control input message if one is available
		if( msgrcv( msgid3, &motor_control_output_receive, sizeof(motor_control_output_receive), 0,0 ) != sizeof(motor_control_output) )
		{
			xil_printf ("MOTOR OUTPUT THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
			consret = errno;
			system_running = 0;
			pthread_exit(&consret);
		}

		motor_direction = motor_control_output_receive.motor_drive;
		motor_speed = motor_control_output_receive.motor_input;

		// Set the desired motor direction using the pmodENC switch
		pmodHB3_set_motor_direction(&pmodHB3_inst, motor_direction);
		pmodHB3_set_motor_speed(&pmodHB3_inst, motor_speed);
	}
	return NULL;

}



/****************************************************************************************/
/******************************* initialization function ********************************/
/****************************************************************************************/
XStatus do_init()
{
	XStatus status;

	// initialize the Nexys4 driver and (some of)the devices
	status = (uint32_t) NX4IO_initialize(NX4IO_BASEADDR);
	if (status != XST_SUCCESS)
	{
		xil_printf("Failed to initialize NX4IO\n\r");
		return XST_FAILURE;
	}

	// initialize the PMod544IO driver and the PmodENC and PmodCLP
	status = pmodENC_initialize(&pmodENC_inst, PMODENC_BASEADDR);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	// initialize the PModHB3 driver
	status = pmodHB3_initialize(&pmodHB3_inst, PMODHB3_BASEADDR);
	if (status == XST_FAILURE)
	{
		xil_printf("Exiting due to error returned from PMODHB3 initialize\n\r");
		return XST_FAILURE;
	}

	// Initialize the PmodHB3 with the max and min motor speed (from 0-4096, 12-bit)
	pmodHB3_init(&pmodHB3_inst, 4000, 0);

	// set all of the display digits to blanks and turn off
	// the decimal points using the "raw" set functions.
	// These registers are formatted according to the spec
	// and should remain unchanged when written to Nexys4IO...
	// something else to check w/ the debugger when we bring the
	// drivers up for the first time
	NX4IO_SSEG_setSSEG_DATA(SSEGHI, 0x00000000);
	NX4IO_SSEG_setSSEG_DATA(SSEGLO, 0x00000000);

	// Initilize Timer 0 for xilkernel
	status = XTmrCtr_Initialize(&TMRCTR0Inst,TMRCTR0_DEVICEID);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	// initialize the button GPIO instance
	status = XGpio_Initialize(&BoardINPUT, NEXYS4_INPUT_GPIO_DEVICEID);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	//all ports configured as input ports
	XGpio_SetDataDirection(&BoardINPUT, 1, 0xFFFF);

	// Enable global interrupt for button
	XGpio_InterruptGlobalEnable(&BoardINPUT);

	// Enabling the GPIO interrupt
	XGpio_InterruptEnable(&BoardINPUT,1);

	// initialize the watchdog timer
	status = XWdtTb_Initialize(&WDTInst,WDT_DEVICEID);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	// initialize the interrupt controller
	status = XIntc_Initialize(&IntrptCtlrInst, INTC_DEVICEID);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	//Connect the control input handler to the interrupt structure
	status = XIntc_Connect(&IntrptCtlrInst, N4INPUT_GPIO_INTR_NUM,
							   (XInterruptHandler)control_handler,
							   (void *)0);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}


	// start the interrupt controller such that interrupts are enabled for
	// all devices that cause interrupts.
	status = XIntc_Start(&IntrptCtlrInst, XIN_REAL_MODE);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	// enable the FIT interrupt
	XIntc_Enable(&IntrptCtlrInst, FIT_INTERRUPT_ID);

	return XST_SUCCESS;
}


/****************************************************************************************/
/******************************* Control interrupt handler *******************************/
/****************************************************************************************/
// Control interrupt handler

void control_handler(void)
{
	int semaphore_status;

	// This interrupt handler responds to an interrupt generated whenever
	// a Nexys4 DDR button or switch changes values

	//Interrupt Clear function
	XGpio_InterruptClear(&BoardINPUT, 1);

	// Read the GPIO values
	Control_input = XGpio_DiscreteRead(&BoardINPUT, GPIO0_INPUT_0_CHANNEL);

	// Post the semaphore for other threads to read.
	semaphore_status = sem_post(&control_input_change_sema);
		if (semaphore_status !=0)
			xil_printf("\r\n semaphore release failed \r\n");

	// Acknowledge the interrupt
	acknowledge_interrupt(N4INPUT_GPIO_INTR_NUM);
}


/****************************************************************************************/
/************************* Watchdog timer interrupt handler *****************************/
/****************************************************************************************/
// WDT interrupt handler

void wdt_handler(void)
{
	// Restart the WDT clearing the trigger state if force_crash is false
	if (!force_crash){
		XWdtTb_RestartWdt(&WDTInst);
	}

	// Acknowledge the interrupt
	acknowledge_interrupt(WDT_INTR_NUM);
}
