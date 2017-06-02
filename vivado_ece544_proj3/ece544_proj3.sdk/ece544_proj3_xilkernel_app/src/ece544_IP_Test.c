/* project3_starter_code.c - ECE 544 Project #3 Application template and starter code
 *
 *
 *	Author:		Srivatsa Yogendra (modified by Roy Kravitz)
 *	Date:		20-May-2017
 *	Revision:	1.0
 *
 * 	Description:
 * 	------------
 * 	This program implements ECE 544 Project #3.
 * 	The application sets up 5 independent threads:
 *		o Thread 1	(Parameter_input_thread) Reads the switches,buttons and rotary count on the FPGA development board
 *		 			and sends an update message to the PID thread.
 * 		o Thread 2 	(Motor_input_thread) Receives the Feedback value (the Hall sensor) from the motor and sends
 *					an update message to the PID thread.
 * 		o Thread 3 	(PID_thread) Implements the PID controller and sends a message to the Motor_output_thread. A message q
 * 		o Thread 4	(Motor_output_thread) Receives the control value message from the PID controller and drives the motor.
 * 		o Thread 5	(Display_thread) Receives messages from the Parameter_input_thread and the Motor_input_thread and updates
 *					the 7-segment display and the LEDs.
 *
 * 	The threads communicate via a message queues. Thread 1 and 3 uses interrupt handlers to sample the inputs
 *	and update the PID control constants.  Semaphores are used (if necessary) to synchronize the interrupt handlers
 *	and their associated threads.
 *
 * 	There is a 6th thread, the Master thread. The Master_thread sets up the Microblaze, initializes the peripherals, and creates
 *	the other threads, messages queues, and semaphores before entering a while(1) loop.
 *
 *  We use priority based scheduling of threads where master thread has priority 0 and all the other threads have
 *  priority of 1.
 *
 *  The application makes use of the Xilinx watchdog timer.  You application should handle restarting the WDT timer and
 *	handling a simulated crash. Turning Sw[15]on (up) should force a crash.  Don't forget to Turn SW[15] off after the crash
 *	or the system will just crash again.
 *
 *	Status messages should be sent to the USB serial port (STDOUT) which can be connected to a PC running a terminal
 *	emulator such as Putty.
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

// TODO:  ***Adjust these constants to reflect your paremeters.h***

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
// switches masks
#define SW_0					0x00000001
#define SW_1					0x00000002
#define SW_proj3				0x0000003F //enables sw[5:0]
#define SW_01_on				0x00000003 // sw[1:0] both open

// Motor Speed/Direction/Control constants
#define MAX_MOTOR_SPEED			255
#define MIN_MOTOR_SPEED			0
#define MAX_RPM					12000
#define MAX_KP					1000
#define MIN_KP					0
#define MAX_KI					100
#define MIN_KI					0
#define MAX_KD					100
#define MIN_KD					0
//Led mask
#define LEDS_OFF				0x00000000

// Peripheral instances
PmodHB3		pmodHB3_inst;				// PmodHB3 instance ref
PmodOLEDrgb	pmodOLEDrgb_inst;			// PmodOLED instance ref
XGpio 	BoardINPUT;
XTmrCtr TMRCTR0Inst,TMRCTR1Inst;
XIntc 	IntrptCtlrInst;
XWdtTb 	WDTInst;
PmodENC pmodENC_inst;				// PmodENC instance ref
XTmrCtr	AXITimerInst;				// PWM timer instance

//global variables
volatile u32 force_crash = 0;	// Force crash flag
volatile u32 system_running = 1;// System running flag

// Message passing variables
typedef struct
{
	int msg_src;
	u8 SetPoint;
	u8 kp,ki,kd;
	u16 switch_value;
	u8 motor_direction;
} t_PID_message, *t_PID_messageptr;

typedef struct
{
	int msg_src;
	u16 motor_input;
	u8 motor_drive;
} t_motor_message, *t_motor_messageptr;

// message queue keys for LED message queue
const int motor_input_key = 1;
const int control_input_key = 2;
const int motor_output_key = 3;

// statistics from message queue
t_PID_message motor_control_input, motor_control_input_receive, motor_control_input_receive_dis;
t_motor_message motor_input,motor_input_receive,motor_input_receive_dis,motor_control_output, motor_control_output_receive;

// semaphore between the Nexys4 DDR board inputs and PMODENC inputs
// interrupt handler and the Parameter_input_thread
sem_t control_input_change_sema;



// Function declarations
void* master_thread(void *arg);
void* Motor_output_thread(void *arg);
void* Parameter_input_thread(void *arg);
void* PID_thread(void *arg);
void* Display_thread(void *arg);
void* Motor_input_thread(void *arg);

void  control_handler(void);
void  wdt_handler(void);
void  FIT_Handler(void);
XStatus init_peripherals(void);
XStatus do_init();

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

		// reset force_crash to zero
		force_crash = 0;
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

	// u32 WDT_status;

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

	/** Declare and initialize other semaphores as shown above **/

	// Register the interrupt handlers
	ret = register_int_handler(WDT_INTR_NUM, (void*) wdt_handler, NULL);
	if (ret != XST_SUCCESS)
	{
		return (void*) -4;
	}

	// TODO: *** Register additional interrupt handlers



	// set the priority of all but the master thread to 1.  The master thread runs at priority 0
	// because it is responsible for tickling the WDT.
	pthread_attr_init (&attr);
	spar.sched_priority = 1;
	pthread_attr_setschedparam(&attr, &spar);

	// create the Parameter_input_thread  (an example of how to do it)
	ret = pthread_create (&control_input, &attr, (void*) Parameter_input_thread, NULL);
	if (ret != 0)
	{
		xil_printf("ERROR (%d) IN MASTER THREAD: could not launch %s\r\n", ret, "button thread");
		xil_printf("FATAL ERROR: Master Thread Terminating\r\n");
		return (void*) -3;
	}

	// TODO:  ***Create the other threads
	ret = pthread_create (&display_thread, &attr, (void*) Display_thread, NULL);
	if (ret != 0)
	{
		xil_printf("ERROR (%d) IN MASTER THREAD: could not launch %s\r\n", ret, "display thread");
		xil_printf("FATAL ERROR: Master Thread Terminating\r\n");
		return (void*) -3;
	}
	
	ret = pthread_create (&motor_input, &attr, (void*) Motor_input_thread, NULL);
	if (ret != 0)
	{
		xil_printf("ERROR (%d) IN MASTER THREAD: could not launch %s\r\n", ret, "motor input thread");
		xil_printf("FATAL ERROR: Master Thread Terminating\r\n");
		return (void*) -3;
	}

	ret = pthread_create (&PID, &attr, (void*) PID_thread, NULL);
	if (ret != 0)
	{
		xil_printf("ERROR (%d) IN MASTER THREAD: could not launch %s\r\n", ret, "PID thread");
		xil_printf("FATAL ERROR: Master Thread Terminating\r\n");
		return (void*) -3;
	}

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

	// TODO:  Enable the other interrupt handlers here (ex: Nexys4 DDR input handler)
	enable_interrupt(N4INPUT_GPIO_INTR_NUM);

	// master thread main loop
	while(1)
	{

		// TODO:  ***Add the code for restarting the WDT, forcing a crash, and
		// whatever other processing you are delegating to the Master thread.
		system_running = 1;

		// Read the WDT status and print to the screen
		// WDT_status = XWdtTb_GetTbValue(&WDTInst);
		// xil_printf("WDT Status: %d\n\r", WDT_status);
		// usleep(3000);
	}

	xil_printf("MASTER THREAD: Outside of wait loop");

	return NULL;
}


/****************************************************************************************/
/***************************************** Parameter Input thread ***********************/
/****************************************************************************************/
void* Parameter_input_thread(void *arg)
{
	// TODO: ***Add your code to read the Nexys4 DDR and PmodENC inputs and send the
	// values to the PID_thread and the Display_thread
	u8 btnR, btnL, btnD, btnU, btnC;
//	u8 btnCpuReset;
	u16 switches;
	u8 rotary_press, rotary_sw, rotary_a, rotary_b;

	u8 motor_increment_size = 1;
	u8 constant_increment_size = 1;
	u8 constant_select = 1;
	u8 motor_direction = 0;

	u8 SetPoint = 0;
	u8 kP = 0;
	u8 kI = 0;
	u8 kD = 0;

	int msgid1;
	int consret;

	// obtain the id for the control input message queue
	msgid1 = msgget(control_input_key, IPC_CREAT);
	if( msgid1 == -1 )
	{
		xil_printf ("PARAMETER INPUT THREAD: ERROR while opening Control Input message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	while (1){
		sem_wait(&control_input_change_sema);
		Control_input_old = Control_input;
		// btnCpuReset = Control_input_old & 0x1;
		btnR = (Control_input_old & 0x2) >> 1;
		btnL = (Control_input_old & 0x4) >> 2;
		btnD = (Control_input_old & 0x8) >> 3;
		btnU = (Control_input_old & 0x10) >> 4;
		btnC = (Control_input_old & 0x20) >> 5;

		switches = (Control_input_old & 0x3fffc0) >> 6;

		rotary_b = (Control_input_old & 0x400000) >> 22;
		rotary_a = (Control_input_old & 0x800000) >> 23;
		rotary_sw = (Control_input_old & 0x1000000) >> 24;
		rotary_press = (Control_input_old & 0x2000000) >> 25;

		// Determine if we are forcing a crash based on sw[15] value
		if (((switches & 0x8000) >> 15) == 0x1){
			xil_printf("force a crash!!!\n\r");
			force_crash = 1;
		}

		// Determine the motor increment size
		if ((switches & 0x3) > 1){
			motor_increment_size = 10;
		}else if ((switches & 0x3) == 1){
			motor_increment_size = 5;
		}else{
			motor_increment_size = 1;
		}

		// Determine the constant increment size
		if (((switches >> 4) & 0x3) > 1){
			constant_increment_size = 10;
		}else if (((switches >> 4) & 0x3) == 1){
			constant_increment_size = 5;
		}else{
			constant_increment_size = 1;
		}

		// Determine the constant select
		if (((switches >> 2) & 0x3) > 1){
			constant_select = 3;
		}else if (((switches >> 2) & 0x3) == 1){
			constant_select = 2;
		}else{
			constant_select = 1;
		}

		// Increment the selected constant if the Up Button is pressed
		if (btnU){
			if (constant_select == 1){
				if ((kP + constant_increment_size) >= 255){
					kP = 255;
				} else {
					kP += constant_increment_size;
				}
			} else if (constant_select == 2){
				if ((kI + constant_increment_size) >= 255){
					kI = 255;
				} else {
					kI += constant_increment_size;
				}
			} else if (constant_select == 3){
				if ((kD + constant_increment_size) >= 255){
					kD = 255;
				} else {
					kD += constant_increment_size;
				}
			}
		}

		// Decrement the selected constant if the Down Button is pressed
		if (btnD){
			if (constant_select == 1){
				if ((kP - constant_increment_size) <= 0){
					kP = 0;
				} else {
					kP -= constant_increment_size;
				}
			} else if (constant_select == 2){
				if ((kI - constant_increment_size) <= 0){
					kI = 0;
				} else {
					kI -= constant_increment_size;
				}
			} else if (constant_select == 3){
				if ((kD - constant_increment_size) <= 0){
					kD = 0;
				} else {
					kD -= constant_increment_size;
				}
			}
		}

		// Increment or Decrement SetPoint based on rotary encoder change and motor increment size
//		if (rotary_a > rotary_b) {
		if (btnR){
			if ((SetPoint + motor_increment_size) >= 255){
				SetPoint = 255;
			} else {
				SetPoint += motor_increment_size;
			}
		}
//		if (rotary_a < rotary_b) {
		if (btnL){
			if ((SetPoint - motor_increment_size) <= 0){
				SetPoint = 0;
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

		xil_printf("PARAMETER INPUT THREAD: Control_input: \t\t\t%x\n\r", Control_input_old);
		xil_printf("PARAMETER INPUT THREAD: Button Right: \t\t\t%x\n\r", btnR);
		xil_printf("PARAMETER INPUT THREAD: Button Left: \t\t\t%x\n\r", btnL);
		xil_printf("PARAMETER INPUT THREAD: Button Down: \t\t\t%x\n\r", btnD);
		xil_printf("PARAMETER INPUT THREAD: Button Up: \t\t\t%x\n\r", btnU);
		xil_printf("PARAMETER INPUT THREAD: Button Center: \t\t\t%x\n\r", btnC);
		xil_printf("PARAMETER INPUT THREAD: Switches: \t\t\t%x\n\r", switches);
		xil_printf("PARAMETER INPUT THREAD: Rotary Press: \t\t\t%x\n\r", rotary_press);
		xil_printf("PARAMETER INPUT THREAD: Rotary Switch: \t\t\t%x\n\r", rotary_sw);
		xil_printf("PARAMETER INPUT THREAD: Rotary A: \t\t\t%x\n\r", rotary_a);
		xil_printf("PARAMETER INPUT THREAD: Rotary B: \t\t\t%x\n\r", rotary_b);
		xil_printf("PARAMETER INPUT THREAD: Motor Increment Size: \t\t%d\n\r", motor_increment_size);
		xil_printf("PARAMETER INPUT THREAD: Constant Increment Size: \t%d\n\r", constant_increment_size);
		xil_printf("PARAMETER INPUT THREAD: Constant Select: \t\t%d\n\r", constant_select);
		xil_printf("PARAMETER INPUT THREAD: Motor Direction: \t\t%d\n\r", motor_direction);
		xil_printf("PARAMETER INPUT THREAD: kP: \t\t\t\t%d\n\r", kP);
		xil_printf("PARAMETER INPUT THREAD: kI: \t\t\t\t%d\n\r", kI);
		xil_printf("PARAMETER INPUT THREAD: kD: \t\t\t\t%d\n\r", kD);
		xil_printf("PARAMETER INPUT THREAD: SetPoint: \t\t\t%d\n\r", SetPoint);
		// usleep(3000);

		motor_control_input.SetPoint = SetPoint;
		motor_control_input.kp = kP;
		motor_control_input.ki = kI;
		motor_control_input.kd = kD;
		motor_control_input.switch_value = switches;
		motor_control_input.motor_direction = motor_direction;

		// send a new control input message
		if( msgsnd( msgid1, &motor_control_input, sizeof(motor_control_input), 0 ) < 0 )
		{
			xil_printf ("PARAMETER INPUT THREAD: msgsnd ran into ERROR.Errno: %d. Halting...\r\n", errno);
			consret = errno;
			pthread_exit(&consret);
		}
	}
	return NULL;
}



/****************************************************************************************/
/**************************************** PID thread ************************************/
/****************************************************************************************/
void* PID_thread(void *arg)
{
	// TODO: ***Add your code to receive messages from the Parameter_input_thread and the Motor_input_thread.  Calculate
	// the motor speed output using the PID control algorithm and send the updated output to the Motor_output_thread.
	int msgid1, msgid2, msgid3;
	u16 switches;
	volatile int32_t actual_speed = 0;
	volatile u8 actual_motor_direction = 0;
	volatile u8 SetPoint = 0;
	volatile u8 kP = 0;
	volatile u8 kI = 0;
	volatile u8 kD = 0;
	volatile u8 motor_direction = 0;

	int error;
	int error_sum;
	int error_diff;
	int prev_error;

	u8 motor_direction_output = 0;
	u16 motor_speed_output = 0;

	int consret;

	// obtain the id for the control input message queue
	msgid1 = msgget(control_input_key, IPC_CREAT);
	if( msgid1 == -1 )
	{
		xil_printf ("PID THREAD: ERROR while opening Control Input message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	// obtain the id for the motor input message queue
	msgid2 = msgget(motor_input_key, IPC_CREAT);
	if( msgid2 == -1 )
	{
		xil_printf ("PID THREAD: ERROR while opening Motor input message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	// obtain the id for the motor output message queue
	msgid3 = msgget(motor_output_key, IPC_CREAT);
	if( msgid3 == -1 )
	{
		xil_printf ("PID THREAD: ERROR while opening Motor output message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	xil_printf("\nPID THREAD: in PID thread \r\n");

	while (1)
	{
		// receive a new control input message if one is available
		if( msgrcv( msgid1, &motor_control_input_receive, sizeof(motor_control_input_receive), 0,0 ) != sizeof(motor_control_input) )
		{
			xil_printf ("PID THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
			consret = errno;
			pthread_exit(&consret);
		}

		// receive a new motor input message if one is available
		if( msgrcv( msgid2, &motor_input_receive, sizeof(motor_input_receive), 0,0 ) != sizeof(motor_input) )
		{
			xil_printf ("PID THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
			consret = errno;
			pthread_exit(&consret);
		}

		actual_speed = motor_input_receive.motor_input;
		actual_motor_direction = motor_input_receive.motor_drive;
		SetPoint = motor_control_input_receive.SetPoint;
		kP = motor_control_input_receive.kp;
		kI = motor_control_input_receive.ki;
		kD = motor_control_input_receive.kd;
		motor_direction = motor_control_input_receive.motor_direction;
		switches = motor_control_input_receive.switch_value;

		//Perform calculation of motor_speed_output and motor_direction_output
		error = SetPoint - actual_speed;
		error_sum += error;
		error_diff = error - prev_error;

		motor_direction_output = motor_direction;
		motor_speed_output = SetPoint + (kP*error) + (kI*error_sum) + (kD*error_diff);

		motor_control_output.motor_drive = motor_direction_output;
		motor_control_output.motor_input = motor_speed_output;

		// send a new control input message
		if( msgsnd( msgid3, &motor_control_output, sizeof(motor_control_output), 0 ) < 0 )
		{
			xil_printf ("PID THREAD: msgsnd ran into ERROR.Errno: %d. Halting...\r\n", errno);
			consret = errno;
			pthread_exit(&consret);
		}
		prev_error = error;
	}

	return NULL;
}



/****************************************************************************************/
/************************************** Display thread **********************************/
/****************************************************************************************/

// TODO: Check the functionality of this thread and modify if appropriate

void* Display_thread(void *arg)
{
	int msgid1,msgid2;
	u16 switches;
	volatile int32_t actual_speed = 0;
	int consret;

	// obtain the id for the control input message queue
	msgid1 = msgget(control_input_key, IPC_CREAT);
	if( msgid1 == -1 )
	{
		xil_printf ("DISPLAY THREAD: ERROR while opening Control Input message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	// obtain the id for the motor input message queue
	msgid2 = msgget(motor_input_key, IPC_CREAT);
	if( msgid2 == -1 )
	{
		xil_printf ("DISPLAY_THREAD: ERROR while opening Motor input message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	xil_printf("\nDISPLAY THREAD: in Display thread \r\n");

	while (1)
	{
		// receive a new control input message if one is available
		if( msgrcv( msgid1, &motor_control_input_receive_dis, sizeof(motor_control_input_receive_dis), 0,0 ) != sizeof(motor_control_input) )
		{
			xil_printf ("DISPLAY THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
			consret = errno;
			pthread_exit(&consret);
		}

		// receive a new motor input message if one is available
		if( msgrcv( msgid2, &motor_input_receive_dis, sizeof(motor_input_receive_dis), 0,0 ) != sizeof(motor_input) )
		{
			xil_printf ("DISPLAY THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
			consret = errno;
			pthread_exit(&consret);
		}

		actual_speed = motor_input_receive_dis.motor_input;
		switches = motor_control_input_receive_dis.switch_value;

		u32 tU;

		if (((switches >> 2) & 0x3) > 1){
			tU = motor_control_input_receive_dis.kd;
		}else if (((switches >> 2) & 0x3) == 1){
			tU = motor_control_input_receive_dis.ki;
		}else{
			tU = motor_control_input_receive_dis.kp;
		}

		NX4IO_setLEDs(switches);

		NX4IO_SSEG_setDigit(SSEGHI, DIGIT7,(int)  (tU / 10)%10);
		NX4IO_SSEG_setDigit(SSEGHI, DIGIT6,(int)  tU % 10);

		NX4IO_SSEG_setDigit(SSEGHI, DIGIT4,(int) (actual_speed / 10000) % 10);
		NX4IO_SSEG_setDigit(SSEGLO, DIGIT3,(int) (actual_speed / 1000) % 10);
		NX4IO_SSEG_setDigit(SSEGLO, DIGIT2,(int) (actual_speed /100) % 10) ;
		NX4IO_SSEG_setDigit(SSEGLO, DIGIT1,(int) (actual_speed % 100) / 10);
		NX4IO_SSEG_setDigit(SSEGLO, DIGIT0,(int)  actual_speed % 10);
	}
	return NULL;
}




/****************************************************************************************/
/************************************** Motor Input thread ******************************/
/****************************************************************************************/
void* Motor_input_thread(void *arg)
{
	// TODO:  ***Add your code to receive the feedback (Hall sensor value) from your H-Bridge
	// custom peripheral and send a message to the PID_thread.
	u16 actual_motor_speed = 100;
	u8 actual_motor_direction = 0;
	int consret;
	int msgid2;

	// obtain the id for the motor input message queue
	msgid2 = msgget(motor_input_key, IPC_CREAT);
	if( msgid2 == -1 )
	{
		xil_printf ("MOTOR INPUT THREAD: ERROR while opening Motor input message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	xil_printf("\nMOTOR INPUT THREAD: in MOTOR INPUT thread \r\n");

	while (1)
	{
		motor_input.motor_drive = actual_motor_direction;
		motor_input.motor_input = actual_motor_speed;

		// send a new motor input message
		if( msgsnd( msgid2, &motor_input, sizeof(motor_input), 0) < 0 )
		{
			xil_printf ("MOTOR INPUT THREAD: msgsnd ran into ERROR.Errno: %d. Halting...\r\n", errno);
			consret = errno;
			pthread_exit(&consret);
		}

		usleep(3000);
	}
	return NULL;
}



/****************************************************************************************/
/************************************** Motor Output thread *****************************/
/****************************************************************************************/
void* Motor_output_thread(void *arg)
{
	// TODO:  ***Add your code to receive the new motor control value(s) from the PID_thread
	// and drive the output(s) from your H-Bridge custom peripheral.
	int consret;
	int msgid3;

	u16 motor_speed = 0;
	u8 motor_direction = 0;

	// obtain the id for the motor output message queue
	msgid3 = msgget(motor_output_key, IPC_CREAT);
	if( msgid3 == -1 )
	{
		xil_printf ("MOTOR OUTPUT THREAD: ERROR while opening Motor output message queue. Errno: %d \r\n", errno);
		consret = errno;
		pthread_exit(&consret) ;
	}

	xil_printf("\nMOTOR OUTPUT THREAD: in MOTOR OUTPUT thread \r\n");

	while (1)
	{
		// receive a new control input message if one is available
		if( msgrcv( msgid3, &motor_control_output_receive, sizeof(motor_control_output_receive), 0,0 ) != sizeof(motor_control_output) )
		{
			xil_printf ("MOTOR OUTPUT THREAD: msgrcv ran into ERROR.Errno: %d. Halting...\r\n", errno);
			consret = errno;
			pthread_exit(&consret);
		}
		motor_direction = motor_control_output_receive.motor_drive;
		motor_speed = motor_control_output_receive.motor_input;

		xil_printf("MOTOR OUTPUT THREAD: Setting motor speed to: %d\n\r", motor_speed);
		xil_printf("MOTOR OUTPUT THREAD: Setting motor direction to: %d\n\r", motor_direction);
		xil_printf("\n\r");
		usleep(3000);
	}
	return NULL;

}



/****************************************************************************************/
/******************************* initialization function ********************************/
/****************************************************************************************/
XStatus do_init()
{
	// TODO:  ***Modify this function if necessary

	XStatus status;

	// initialize the Nexys4 driver and (some of)the devices
	status = (uint32_t) NX4IO_initialize(NX4IO_BASEADDR);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	// initialize the PMod544IO driver and the PmodENC and PmodCLP
//	status = pmodENC_initialize(&pmodENC_inst, PMODENC_BASEADDR);
//	if (status != XST_SUCCESS)
//	{
//		return XST_FAILURE;
//	}

	// initialize the PModHB3 driver
//	status = pmodHB3_initialize(&pmodHB3_inst, PMODHB3_BASEADDR);
//	if (status == XST_FAILURE)
//	{
//		xil_printf("Exiting due to error returned from PMODHB3 initialize\n\r");
//		return XST_FAILURE;
//	}

	// Initialize the PmodHB3 with the max and min motor speed (from 0-4096, 12-bit)
//	pmodHB3_init(&pmodHB3_inst, 4000, 0);

	// set all of the display digits to blanks and turn off
	// the decimal points using the "raw" set functions.
	// These registers are formatted according to the spec
	// and should remain unchanged when written to Nexys4IO...
	// something else to check w/ the debugger when we bring the
	// drivers up for the first time
	// NX4IO_SSEG_setSSEG_DATA(SSEGHI, 0x0058E30E);
	// NX4IO_SSEG_setSSEG_DATA(SSEGLO, 0x00144116);
	NX4IO_SSEG_setSSEG_DATA(SSEGHI, 0x00000000);
	NX4IO_SSEG_setSSEG_DATA(SSEGLO, 0x00000000);

	// Initialize the OLED display
//	OLEDrgb_begin(&pmodOLEDrgb_inst, RGBDSPLY_GPIO_BASEADDR, RGBDSPLY_SPI_BASEADDR);

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

	// connect the fixed interval timer (FIT) handler to the interrupt
	status = XIntc_Connect(&IntrptCtlrInst, FIT_INTERRUPT_ID,
						   (XInterruptHandler)FIT_Handler,
						   (void *)0);
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

	// TODO:  ***Modify this code if necessary

	//Interrupt Clear function
	XGpio_InterruptClear(&BoardINPUT, 1);
	xil_printf("\r\n Control Input interrupt handler \r\n");

	Control_input = XGpio_DiscreteRead(&BoardINPUT, GPIO0_INPUT_0_CHANNEL);

	semaphore_status = sem_post(&control_input_change_sema);
		if (semaphore_status !=0)
			xil_printf("\r\n semaphore release failed \r\n");
		else
			xil_printf("\r\n semaphore released \r\n");

	// Acknowledge the interrupt
	acknowledge_interrupt(N4INPUT_GPIO_INTR_NUM);
}


/****************************************************************************************/
/************************* Watchdog timer interrupt handler *****************************/
/****************************************************************************************/
// WDT interrupt handler

void wdt_handler(void)
{
	// TODO:  ***Add your code to handle a WDT interrupt
	// xil_printf("Entered WDT interrupt handler.\n\r");

	// Restart the WDT clearing the trigger state
	if (!force_crash){
		XWdtTb_RestartWdt(&WDTInst);
	}

	// Acknowledge the interrupt
	acknowledge_interrupt(WDT_INTR_NUM);
}


/****************************************************************************/
/**
* Fixed interval timer interrupt handler
*
* Updates the PID state based on the FIT interrupt
 *****************************************************************************/

void FIT_Handler(void)
{
	// TODO: ***Add code for FIT handler
	acknowledge_interrupt(FIT_INTERRUPT_ID);
}


/**************************************************************/
/******************** Helper Functions ************************/
/**************************************************************/

// TODO:  ***Add code as necessary