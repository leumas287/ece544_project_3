
/**
*
* @file lab1.c
*
* @author Dakota Ward(dakward@pdx.edu)
* @influenced/informed by file ece544_IP_Test.c
*							   @author Roy Kravitz (roy.kravitz@pdx.edu)
* 							   @modified by Shrivatsa Yogendra (srivatsa@pdx.edu)
* 							   @modified by Chetan Bornarkar (bchetan@pdx.edu)
* 							   @copyright Portland State University, 2014-2015, 2016-2017
*
* This file implements a pulse width modulation detection program utilizing the PmodOLEDrgb, PmodENC and Nexys4IO
* custom peripherals. Slide switch, SW[0] is used to toggle between the hardware/software PWM implementation. The
* Seven segment Display will output the RGB duty cycles: RED channel on Digits [7:6], Green channel on digits[4:3],
* and BLUE on digits [1:0]. LED [0] will be lit when the Hardware PWM methos is selected.
*
* USAGE:
* Rotary encoder : turn the knob to increment/decrement the HUE value on the OLEDrgb scrren
* Buttons (Nexys4DDR board):
*			BtnU/BtnD: increment/decrements the VALUE on the OLEDrgb scrren
*			BtnL/BtnR: increment/decrements the SATURATION on the OLEDrgb scrren
*			BtnC: exits the program
*
*
* @note
* The minimal hardware configuration for this program is a Microblaze-based system with at least 32KB of memory,
* an instance of Nexys4IO, an instance of the PMod544IOR2,  and an instance of the Xilinx
* UARTLite (used for xil_printf() console output)
*
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xparameters.h"
#include "xstatus.h"
#include "nexys4IO.h"
#include "PmodHB3.h"
#include "pmodENC.h"
#include "xgpio.h"
#include "xintc.h"
#include "xtmrctr.h"
#include "PmodOLEDrgb.h"

/************************** Constant Definitions ****************************/
// Clock frequencies
#define CPU_CLOCK_FREQ_HZ		XPAR_CPU_CORE_CLOCK_FREQ_HZ
#define AXI_CLOCK_FREQ_HZ		XPAR_CPU_M_AXI_DP_FREQ_HZ

// AXI timer parameters
#define AXI_TIMER_DEVICE_ID		XPAR_AXI_TIMER_0_DEVICE_ID
#define AXI_TIMER_BASEADDR		XPAR_AXI_TIMER_0_BASEADDR
#define AXI_TIMER_HIGHADDR		XPAR_AXI_TIMER_0_HIGHADDR
#define TmrCtrNumber			0

// Definitions for peripheral NEXYS4IO
#define NX4IO_DEVICE_ID		XPAR_NEXYS4IO_0_DEVICE_ID
#define NX4IO_BASEADDR		XPAR_NEXYS4IO_0_S00_AXI_BASEADDR
#define NX4IO_HIGHADDR		XPAR_NEXYS4IO_0_S00_AXI_HIGHADDR

// Definitions for peripheral PMODOLEDRGB
#define RGBDSPLY_DEVICE_ID		XPAR_PMODOLEDRGB_0_DEVICE_ID
#define RGBDSPLY_GPIO_BASEADDR	XPAR_PMODOLEDRGB_0_AXI_LITE_GPIO_BASEADDR
#define RGBDSPLY_GPIO_HIGHADDR	XPAR_PMODOLEDRGB_0_AXI_LITE_GPIO_HIGHADD
#define RGBDSPLY_SPI_BASEADDR	XPAR_PMODOLEDRGB_0_AXI_LITE_SPI_BASEADDR
#define RGBDSPLY_SPI_HIGHADDR	XPAR_PMODOLEDRGB_0_AXI_LITE_SPI_HIGHADDR

// Definitions for peripheral PMODENC
#define PMODENC_DEVICE_ID		XPAR_PMODENC_0_DEVICE_ID
#define PMODENC_BASEADDR		XPAR_PMODENC_0_S00_AXI_BASEADDR
#define PMODENC_HIGHADDR		XPAR_PMODENC_0_S00_AXI_HIGHADDR

// Definitions for peripheral PMODHB3
#define PMODHB3_DEVICE_ID		XPAR_PMODHB3_0_DEVICE_ID
#define PMODHB3_BASEADDR		XPAR_PMODHB3_0_S00_AXI_BASEADDR
#define PMODHB3_HIGHADDR		XPAR_PMODHB3_0_S00_AXI_HIGHADDR

// Fixed Interval timer - 100 MHz input clock, 40KHz output clock
// FIT_COUNT_1MSEC = FIT_CLOCK_FREQ_HZ * .001
#define FIT_IN_CLOCK_FREQ_HZ	CPU_CLOCK_FREQ_HZ
#define FIT_CLOCK_FREQ_HZ		40000
#define FIT_COUNT				(FIT_IN_CLOCK_FREQ_HZ / FIT_CLOCK_FREQ_HZ)
#define FIT_COUNT_1MSEC			40

// GPIO parameters
#define GPIO_0_DEVICE_ID			XPAR_AXI_GPIO_0_DEVICE_ID
#define GPIO_0_INPUT_0_CHANNEL		1
#define GPIO_0_OUTPUT_0_CHANNEL		2


// Interrupt Controller parameters
#define INTC_DEVICE_ID			XPAR_INTC_0_DEVICE_ID
#define FIT_INTERRUPT_ID		XPAR_MICROBLAZE_0_AXI_INTC_FIT_TIMER_0_INTERRUPT_INTR

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

/************************** Variable Definitions ****************************/
//unsigned long timeStamp = 0;
volatile u32			gpio_in;				// GPIO input port

// HEX to BCD buffers for SSEG display
unsigned char sseg_constant[10]   = {0};
unsigned char sseg_rpm[10]  = {0};

u16 					sw = 0;					// storage to read in the switches
uint16_t  				RotaryCnt;
uint64_t 				timestamp = 0L;			// used in delay msec

//u16 sw = 0;
u8 RotaryIncr					= 1;
bool RotaryNoNeg 				= true;
u8 prevRotary 					= 0;

volatile int desiredRpm					= 0;
volatile int actualRpm					= 0;
u8 desiredMotorSpeed 			= 0;
u8 prev_desiredMotorSpeed 		= 0;
u8 desiredMotorSpeedIncrement	= 1;
u8 desiredMotorDirection		= 0;
u8 actualMotorSpeed				= 0;
u8 prev_actualMotorSpeed		= 0;

volatile int signedError					= 0;
volatile int prev_signedError				= 0;
volatile int dt								= 0;
volatile float p_term						= 0;
volatile float i_term						= 0;
volatile float d_term						= 0;
volatile u8 motorOut						= 0;
u8 prev_motorOut							= 0;
volatile int motorOutRpm					= 0;

bool kP_flag					;
u8 kP_incr						= 1;
int pcntrl_const 				= 0;
int prev_kP						= 0;

bool kI_flag					;
u8 kI_incr						=1;
int icntrl_const				=0;
int prev_kI						=0;

bool kD_flag					;
u8 kD_incr						=1;
int dcntrl_const				=0;
int prev_kD						=0;

u8 pEnc_switch 					= 0;
u8 prev_pEnc_switch 			= 0;


u32 switch_on; 						//variable to store either software/hardware PMW
unsigned long KP,KI,KD,RPM;				//PWM variables for display

/************************** Function Prototypes *****************************/
void usleep(u32 usecs);
void PMDIO_itoa(int32_t value, char *string, int32_t radix);
void PMDIO_puthex(PmodOLEDrgb* InstancePtr, uint32_t num);
void PMDIO_putnum(PmodOLEDrgb* InstancePtr, int32_t num, int32_t radix);
void bin2bcd(unsigned long bin, unsigned char *bcd); //had to include definition otherwise "implicit error message"
void MotorControl(void);
void FIT_Handler(void);
int AXI_Timer_initialize(void);
int do_init();

PmodHB3		pmodHB3_inst;				// PmodHB3 instance ref
PmodENC 	pmodENC_inst;				// PmodENC instance ref
PmodOLEDrgb	pmodOLEDrgb_inst;			// PmodOLED instance ref
XGpio		GPIOInst0;					// GPIO instance
XIntc 		IntrptCtlrInst;				// Interrupt Controller instance
XTmrCtr		AXITimerInst;				// PWM timer instance


/************************** MAIN PROGRAM ************************************/
int main()
{
	int sts;
    init_platform();

    sts = do_init();		// initialize the peripherals
    if (XST_SUCCESS != sts)
    {
    	exit(1);
    }

    microblaze_enable_interrupts();		// enable the interrupts

	xil_printf("ECE 544 Project2: Motor Control with Proportional Feedback\r\n");
	xil_printf("By Dakota Ward and Sam Burkhart.  17-May 2017\r\n");

	// blank the display digits and turn off the decimal points
	NX410_SSEG_setAllDigits(SSEGLO, CC_BLANK, CC_BLANK, CC_BLANK, CC_BLANK, DP_NONE);
	NX410_SSEG_setAllDigits(SSEGHI, CC_BLANK, CC_BLANK, CC_BLANK, CC_BLANK, DP_NONE);

	// Run the motor control function
	MotorControl();

	// blank the display digits and turn off the decimal points
	NX410_SSEG_setAllDigits(SSEGLO, CC_BLANK, CC_BLANK, CC_BLANK, CC_BLANK, DP_NONE);
	NX410_SSEG_setAllDigits(SSEGHI, CC_BLANK, CC_BLANK, CC_BLANK, CC_BLANK, DP_NONE);
	// loop the test until the user presses the center button

	xil_printf("\nThat's All Folks!\n\n\r");
	// display the message "Bye Bye" on the OLED screen
	OLEDrgb_Clear(&pmodOLEDrgb_inst);	// Clear the display
	OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 4, 2);	//
	OLEDrgb_SetFontColor(&pmodOLEDrgb_inst ,OLEDrgb_BuildRGB(0, 0, 255));  // green font
	OLEDrgb_PutString(&pmodOLEDrgb_inst,"Cya Bye");
	usleep(05000000);

	// Clear all the display digits and the OLED display at the end of the program
	NX410_SSEG_setAllDigits(SSEGHI, CC_BLANK, CC_B, CC_LCY, CC_E, DP_NONE);
	NX410_SSEG_setAllDigits(SSEGLO, CC_B, CC_LCY, CC_E, CC_BLANK, DP_NONE);
	OLEDrgb_Clear(&pmodOLEDrgb_inst);
	OLEDrgb_end(&pmodOLEDrgb_inst);

    cleanup_platform();
    exit(0);
}
/****************************************************************************/
/**
* Test 4 - Test the PmodENC and PmodCLP
*
* Performs some basic tests on the PmodENC and PmodCLP.  Includes the following tests
* 	1.	check the rotary encoder by displaying the rotary encoder
* 		count in decimal and hex on the LCD display.  Rotate the knob
* 		to change the values up or down.  The pushbuttons can be used
* 		as follows:
* 			o 	press the rotary encoder pushbutton to exit
* 			o 	press BtnUp to clear the count
* 			o 	press BtnR to change rotary encoder
* 				mode to "stop at zero".  This does not appear
* 				to be reversible - not sure why.
* 			o 	press BTNL to change the increment/decrement
* 				value.  Use sw[3:0] to set the new value
*	6.	display the string "357#&CFsw" on the LCD display.  These values
* 		were chosen to check that the bit order is correct.  The screen will
* 		clear in about 5 seconds.
* 	7.	display "Exiting Test 4" on the LCD.  The screen will clear
* 		in about 5 seconds.
*
*
* @param	*NONE*
*
* @return	*NONE*
*
*****************************************************************************/
void MotorControl(void)
{
	// Initialize the rotary encoder
	// clear the counter of the encoder if initialized to garbage pcntrl_const on power on
	pmodENC_init(&pmodENC_inst, RotaryIncr, RotaryNoNeg);
	pmodENC_clear_count(&pmodENC_inst);

	//Set up the  LEDS
	NX4IO_setLEDs(LEDS_OFF);

	//TODO: Oledrgb used for debugging
	// Set up the OLED display output
	OLEDrgb_Clear(&pmodOLEDrgb_inst);
	OLEDrgb_SetFontColor(&pmodOLEDrgb_inst,OLEDrgb_BuildRGB(255, 0, 0));

	OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 0, 1);
	OLEDrgb_PutString(&pmodOLEDrgb_inst,"Mot:");
	OLEDrgb_SetFontColor(&pmodOLEDrgb_inst,OLEDrgb_BuildRGB(0, 255, 0));

	OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 0, 2);
	OLEDrgb_PutString(&pmodOLEDrgb_inst,"Kp:");
	OLEDrgb_SetFontColor(&pmodOLEDrgb_inst,OLEDrgb_BuildRGB(0, 0, 255));

	OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 0, 3);
	OLEDrgb_PutString(&pmodOLEDrgb_inst,"Ki:");
	OLEDrgb_SetFontColor(&pmodOLEDrgb_inst,OLEDrgb_BuildRGB(0, 0, 255));

	OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 0, 4);
	OLEDrgb_PutString(&pmodOLEDrgb_inst,"Kd:");
	OLEDrgb_SetFontColor(&pmodOLEDrgb_inst,OLEDrgb_BuildRGB(0, 0, 255));
	
	OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 0, 5);
	OLEDrgb_PutString(&pmodOLEDrgb_inst,"Psw:");			//Psw = 1 if switch open

	OLEDrgb_SetFontColor(&pmodOLEDrgb_inst,OLEDrgb_BuildRGB(200, 180, 180));

	while(1)
	{
		/********************************************************************
		 * 				Step 0: Check for stop/reset
		 * 					Exit on press of the RotaryENC button
		 * 					Reset on press of the center button
		 ********************************************************************/
		// check if the rotary encoder pushbutton or BTNC is pressed
		// exit the loop if either one is pressed.
		if(pmodENC_is_button_pressed(&pmodENC_inst) ){break;}
		if(NX4IO_isPressed(BTNC)){
			pcntrl_const = 0;
			icntrl_const = 0;
			dcntrl_const = 0;
			desiredMotorSpeed =0;
			actualMotorSpeed = 0;
			XGpio_DiscreteWrite(&GPIOInst0, GPIO_0_OUTPUT_0_CHANNEL, 0);
			pmodENC_clear_count(&pmodENC_inst);
			//NX410_SSEG_setAllDigits(SSEGLO, CC_BLANK, CC_BLANK, CC_BLANK, CC_BLANK, DP_NONE);

		}
		/********************************************************************
		 * 				Step 1: Read the constant KP,KI,KD being selected
		 * 					This drives the increment values of the
		 * 					kP,kI,kD (U/D buttons) and desiredMotorSpeed (RotaryEnc)
		 ********************************************************************/
		if (switch_on & 0x1){		//sw[0] open, KP constant to be incremented/decremented/displayed
			kP_flag = true;
			kI_flag = false;
			kD_flag = false;
		}
		else if ((switch_on >> 2) & 0x01){ //sw[2] open, KP constant to be incremented/decremented/displayed
			kI_flag = true;
			kP_flag = false;
			kD_flag = false;
		}
		else if ((switch_on >> 3) & 0x01){ //sw[3] open, KP constant to be incremented/decremented/displayed
			kD_flag = true;
			kP_flag = false;
			kI_flag = false;
		}
		
		/********************************************************************
		 * 				Step 2: Read Switches
		 * 					This drives the increment values of the
		 * 					kP (U/D buttons) and desiredMotorSpeed (RotaryEnc)
		 ********************************************************************/
		sw = NX4IO_getSwitches();							// get switch info

		//Slider switch logic
		switch_on = (sw & SW_proj3);								//turn on sw[5,4,3,2,1,0]
		NX4IO_setLEDs(switch_on);									// display led over switch
		
		// Check switch[1] to determine if we increment desired motor speed by 10
		if ((switch_on >> 1) & 0x1){
			desiredMotorSpeedIncrement = 10;

		// Else check switch[0] to determine if we increment desired motor speed by 1 or 5
		}else{
			if ((switch_on & 0x1)){
				desiredMotorSpeedIncrement = 5;
			} else {
				desiredMotorSpeedIncrement = 1;
			}
		}
		if (kP_flag){
			// Check switch[5] to determine if we increment kP by 10
			if ((switch_on >> 5) & 0x1){
				kP_incr = 10;

			// Else check switch[4] to determine if we increment kP by 1 or 5
			}else{
				if (((switch_on >> 4) & 0x1)){
					kP_incr = 5;
				} else {
					kP_incr = 1;
				}
			}
		}
		else if (kI_flag){
			//Check switch[5] to determine if we increment KI by 10
			if ((switch_on >> 5) & 0x01){
				kI_incr = 10;

			// Else check switch[4] to determine if we increment KI by 1 or 5
			}else{
				if ((( switch_on >> 4) & 0x01)){
					kI_incr = 5;
				} else {
					kI_incr = 1;
				}
			}
		}
		
		else if (kD_flag){
			//Check switch[5] to determine if we increment KD
			if ((switch_on >> 5) & 0x01){
				kD_incr = 10;

			// Else check switch[4] to determine if we increment KI by 1 or 5
			}else{
				if ((( switch_on >> 4) & 0x01)){
					kD_incr = 5;
				} else {
					kD_incr = 1;
				}
			}
		}
		/********************************************************************
		 * 				Step 2: Update and display the kP, kI, kD value
		 * 					The up/down buttons control the constant values by
		 * 					incrementing/decrementing the kP by the value read
		 * 					from switch[5:4] in step 1.
		 ********************************************************************/
		// Check if button is pressed and adjust kP,kI,kD accordingly
		if(NX4IO_isPressed(BTNU))
		{
			int a = pcntrl_const;
			int b = kP_incr;

			int c = icntrl_const;
			int d = kI_incr;

			int e = dcntrl_const;
			int f = kD_incr;

			int sumP = a + b;
			int sumI = c + d;
			int sumD = e + f;

			// increment pcntrl_const of Kp
			if (sumP <= MAX_KP){
				pcntrl_const += kP_incr;
			} else {
				pcntrl_const = MAX_KP;
			}

			// increment icntrl_const of KI
			if (sumI <= MAX_KI){
				icntrl_const += kI_incr;
			} else {
				icntrl_const = MAX_KI;
			}

			// increment dcntrl_const of KD
			if (sumD <= MAX_KD){
				dcntrl_const += kD_incr;
			} else {
				dcntrl_const = MAX_KD;
			}

		} else if(NX4IO_isPressed(BTND))
		{
			int a = pcntrl_const;
			int b = kP_incr;

			int c = icntrl_const;
			int d = kI_incr;

			int e = dcntrl_const;
			int f = kD_incr;

			int diffP = a - b;
			int diffI = c - d;
			int diffD = e - f;

			// decrement pcntrl_const of Kp
			if (diffP >= MIN_KP){
				pcntrl_const -= kP_incr;
			} else {
				pcntrl_const = MIN_KP;
			}
			// decrement pcntrl_const of KI
			if (diffI >= MIN_KI){
				icntrl_const -= kI_incr;
			} else {
				icntrl_const = MIN_KI;
			}
			// decrement pcntrl_const of KD
			if (diffD >= MIN_KD){
				dcntrl_const -= kD_incr;
			} else {
				dcntrl_const = MIN_KD;
			}
		}
			//SSEG Display Logic
			
			
			if((kP_flag) | (NX4IO_isPressed(BTNC))){
				if (pcntrl_const != prev_kP){
					// Display to OLED
					OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 4, 2);				// reset the cursor after "Kp:"
					OLEDrgb_PutString(&pmodOLEDrgb_inst, "   ");
					OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 4, 2);				// reset the cursor after "Kp:"
					PMDIO_putnum(&pmodOLEDrgb_inst, pcntrl_const, 10);		// show the number in dec format
					KP = pcntrl_const;
					bin2bcd(KP, sseg_constant);
				}

			}
			if ((kI_flag) | (NX4IO_isPressed(BTNC))){
				if (icntrl_const != prev_kI){
					OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 4, 3);				// reset the cursor after "Ki:"
					OLEDrgb_PutString(&pmodOLEDrgb_inst, "   ");
					OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 4, 3);				// reset the cursor after "Ki:"
					PMDIO_putnum(&pmodOLEDrgb_inst, icntrl_const, 10);		// show the number in dec format
					bin2bcd(KI, sseg_constant);
					KI = icntrl_const;
				}
			}
			
			if ((kD_flag)| (NX4IO_isPressed(BTNC))){
				if (dcntrl_const != prev_kD){
					OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 4, 4);				// reset the cursor after "Kd:"
					OLEDrgb_PutString(&pmodOLEDrgb_inst, "   ");
					OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 4, 4);				// reset the cursor after "Kd:"
					PMDIO_putnum(&pmodOLEDrgb_inst, dcntrl_const, 10);		// show the number in dec format
					KD = dcntrl_const;
					bin2bcd(KD, sseg_constant);
				}
			}

			if(sseg_constant[6] == 0)
			{
				sseg_constant[6] = CC_BLANK;
			}

			NX4IO_SSEG_setDigit(SSEGHI, DIGIT7, sseg_constant[6]);	// display thousand digit
			NX4IO_SSEG_setDigit(SSEGHI, DIGIT6, sseg_constant[7]);	// display hundred digit
			NX4IO_SSEG_setDigit(SSEGHI, DIGIT5, sseg_constant[8]);	// display tens digit
			NX4IO_SSEG_setDigit(SSEGHI, DIGIT4, sseg_constant[9]);	// display ones digit

		



		/********************************************************************
		 * 				Step 3: Update and display the desired Motor Speed
		 * 					The Rotary Encoder increments/decrements the
		 * 					desired motor speed by the values specified on
		 * 					switch[1:0]
		 ********************************************************************/
		// read the value from the rotary encoder
		pmodENC_read_count(&pmodENC_inst, &RotaryCnt);

		// Rotary Count if: detects transition in prev/current RotaryCnt
		if (RotaryCnt > prevRotary){
			int a = desiredMotorSpeed;
			int b = desiredMotorSpeedIncrement;
			int sum = a + b;
			if (sum <= MAX_MOTOR_SPEED){
				desiredMotorSpeed += desiredMotorSpeedIncrement;
			} else {
				desiredMotorSpeed = MAX_MOTOR_SPEED;
			}
		}else if (RotaryCnt < prevRotary){
			int a = desiredMotorSpeed;
			int b = desiredMotorSpeedIncrement;
			int diff = a - b;
			if (diff >= MIN_MOTOR_SPEED){
				desiredMotorSpeed -= desiredMotorSpeedIncrement;
			} else {
				desiredMotorSpeed = MIN_MOTOR_SPEED;
			}
		// If the Rotary Count is 0, set the motor speed to zero
		// This synchronizes the rotary count and desired motor speed, allowing the scaling from
		// 1, 5, 10 increment values.
		} else if (RotaryCnt == 0){
			desiredMotorSpeed = 0;
		}

		// Display the value if the value changed
		if(desiredMotorSpeed != prev_desiredMotorSpeed)
		{

			// Display the Rotary Count on the OLED
			OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 4, 1);
			OLEDrgb_PutString(&pmodOLEDrgb_inst, "   ");	// clear the row of the display

			// avoid flickering while hue pcntrl_const updates
			OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 4, 1);			// reset the cursor after "Mot:"
			PMDIO_putnum(&pmodOLEDrgb_inst, desiredMotorSpeed, 10);				// show the number in decimal form
		}

		/********************************************************************
		 * 				Step 4: Update and display the desired motor direction
		 * 					The Rotary Encoder switch sets the desired motor
		 * 					direction.  The motor can only change when the RPM is 0,
		 * 					so the affects of this value have an inherent delay
		 * 					until after the motor goes to 0 RPM.
		 ********************************************************************/
		//Check to see if PmodEnc switch is on
		if(pmodENC_is_switch_on(&pmodENC_inst) ){
			pEnc_switch = 1;
		}else{
			pEnc_switch = 0;
		}

		// Display new RotaryEnc switch value
		if (prev_pEnc_switch != pEnc_switch){
			OLEDrgb_SetCursor(&pmodOLEDrgb_inst, 4, 5);			// reset the cursor after "Psw:"
			PMDIO_putnum(&pmodOLEDrgb_inst,pEnc_switch, 10);	// show the number in dec format
		}
		// Set the desired motor direction using the pmodENC switch
		pmodHB3_set_motor_direction(&pmodHB3_inst, (pEnc_switch & 0x1));

		/********************************************************************
		 * 				Step 5: Perform calculation of motor control signal
		 * 					The motor control signal is a function of the
		 * 					desired motor speed, the proportional constant (kP),
		 * 					and the actual motor speed.
		 ********************************************************************/


		// actualMotorSpeed = XGpio_DiscreteRead(&GPIOInst0, GPIO_0_INPUT_0_CHANNEL);
		// actualRpm = actualMotorSpeed * 60;
		// actualRpm = pmodHB3_read_motor_speed(&pmodHB3_inst);
//		desiredRpm = desiredMotorSpeed * MAX_RPM/MAX_MOTOR_SPEED;
//		signedError = desiredRpm - actualRpm;
//		p_term = (float)(pcntrl_const)/10 * signedError;
//		motorOutRpm = p_term + desiredRpm;
//		if(motorOutRpm > MAX_RPM){
//				motorOutRpm = MAX_RPM;
//		}
//		else if (motorOutRpm < 0){
//			motorOutRpm = 0;
//		}
//
//		motorOut = (u8)(motorOutRpm* MAX_MOTOR_SPEED /MAX_RPM );
//
		xil_printf("Desired RPM: %d \n\n\r", desiredRpm);
		xil_printf("actual Rpm: %d \n\r", actualRpm);
		xil_printf("Motor Out: %d \n\r", motorOut);

		// Calculate RPM based on actual motor speed and pring to SSEG Display
		RPM = actualRpm;
		bin2bcd(RPM, sseg_rpm);

		if(sseg_rpm[6] == 0)
		{
			sseg_rpm[6] = CC_BLANK;
		}

		NX4IO_SSEG_setDigit(SSEGLO, DIGIT3, sseg_rpm[6]);	// display thousand digit
		NX4IO_SSEG_setDigit(SSEGLO, DIGIT2, sseg_rpm[7]);	// display hundred digit
		NX4IO_SSEG_setDigit(SSEGLO, DIGIT1, sseg_rpm[8]);	// display tens digit
		NX4IO_SSEG_setDigit(SSEGLO, DIGIT0, sseg_rpm[9]);	// display ones digit

		/********************************************************************
		 * 				Step 6: Send the motor control signals to the motor
		 * 					Use the motor control handles to control the motor.
		 ********************************************************************/
		// pmodHB3_set_motor_speed(&pmodHB3_inst, motorOut);

		/********************************************************************
		 * 				Step 7: Update the previous values before waiting for the next iteration
		 ********************************************************************/
		prev_pEnc_switch = pEnc_switch;
		prev_kP = pcntrl_const;
		prev_kI = icntrl_const;
		prev_kD = dcntrl_const;
		prev_desiredMotorSpeed = desiredMotorSpeed;
		prev_motorOut = motorOut;
		prev_actualMotorSpeed = actualMotorSpeed;
		prevRotary = RotaryCnt;

		usleep(100000);
	}

	xil_printf("\n\rMotor Controller App Completed!!!\n\r");

	// Write one final string
	OLEDrgb_Clear(&pmodOLEDrgb_inst);
	return;
}
/**
 * Function Name: do_init()
 *
 * Return: XST_FAILURE or XST_SUCCESS
 *
 * Description: Initialize the AXI timer, gpio, interrupt, FIT timer, Encoder,
 * 				OLED display
 */
int do_init()
{
	int status;

	// initialize the Nexys4 driver and (some of)the devices
	status = (uint32_t) NX4IO_initialize(NX4IO_BASEADDR);
	if (status == XST_FAILURE)
	{
		exit(1);
	}

	// initialize the PMod544IO driver and the PmodENC and PmodCLP
	status = pmodENC_initialize(&pmodENC_inst, PMODENC_BASEADDR);
	if (status == XST_FAILURE)
	{
		exit(1);
	}

	// initialize the PModHB3 driver
	status = pmodHB3_initialize(&pmodHB3_inst, PMODHB3_BASEADDR);
	if (status == XST_FAILURE)
	{
		xil_printf("Exiting due to error returned from PMODHB3 initialize\n\r");
		exit(1);
	}

	pmodHB3_init(&pmodHB3_inst, 500, 50);

	// initialize the GPIO instances
	status = XGpio_Initialize(&GPIOInst0, GPIO_0_DEVICE_ID);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	// Initialize the AXI Timer
	status = AXI_Timer_initialize();
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	// set all of the display digits to blanks and turn off
	// the decimal points using the "raw" set functions.
	// These registers are formatted according to the spec
	// and should remain unchanged when written to Nexys4IO...
	// something else to check w/ the debugger when we bring the
	// drivers up for the first time
	NX4IO_SSEG_setSSEG_DATA(SSEGHI, 0x0058E30E);
	NX4IO_SSEG_setSSEG_DATA(SSEGLO, 0x00144116);

	// Initialize the OLED display
	OLEDrgb_begin(&pmodOLEDrgb_inst, RGBDSPLY_GPIO_BASEADDR, RGBDSPLY_SPI_BASEADDR);

	// initialize the GPIO instances
	status = XGpio_Initialize(&GPIOInst0, GPIO_0_DEVICE_ID);
	if (status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}
	// GPIO0 channel 1 is an 32-bit input port.
	// GPIO0 channel 2 is an 8-bit output port.
	XGpio_SetDataDirection(&GPIOInst0, GPIO_0_INPUT_0_CHANNEL, 0xFFFFFFFF);
	XGpio_SetDataDirection(&GPIOInst0, GPIO_0_OUTPUT_0_CHANNEL, 0x00);

	// initialize the interrupt controller
	status = XIntc_Initialize(&IntrptCtlrInst, INTC_DEVICE_ID);
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

/****************************************************************************
*
* AXI timer initializes it to generate out a 4Khz signal, Which is given to the Nexys4IO module as clock input.
* DO NOT MODIFY
*
*****************************************************************************/
int AXI_Timer_initialize(void){

	uint32_t status;				// status from Xilinx Lib calls
	u32		ctlsts;		// control/status register or mask

	status = XTmrCtr_Initialize(&AXITimerInst,AXI_TIMER_DEVICE_ID);
		if (status != XST_SUCCESS) {
			return XST_FAILURE;
		}
	status = XTmrCtr_SelfTest(&AXITimerInst, TmrCtrNumber);
		if (status != XST_SUCCESS) {
			return XST_FAILURE;
		}
	ctlsts = XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_EXT_GENERATE_MASK | XTC_CSR_LOAD_MASK |XTC_CSR_DOWN_COUNT_MASK ;
	XTmrCtr_SetControlStatusReg(AXI_TIMER_BASEADDR, TmrCtrNumber,ctlsts);

	//Set the value that is loaded into the timer counter and cause it to be loaded into the timer counter
	XTmrCtr_SetLoadReg(AXI_TIMER_BASEADDR, TmrCtrNumber, 24998);
	XTmrCtr_LoadTimerCounterReg(AXI_TIMER_BASEADDR, TmrCtrNumber);
	ctlsts = XTmrCtr_GetControlStatusReg(AXI_TIMER_BASEADDR, TmrCtrNumber);
	ctlsts &= (~XTC_CSR_LOAD_MASK);
	XTmrCtr_SetControlStatusReg(AXI_TIMER_BASEADDR, TmrCtrNumber, ctlsts);

	ctlsts = XTmrCtr_GetControlStatusReg(AXI_TIMER_BASEADDR, TmrCtrNumber);
	ctlsts |= XTC_CSR_ENABLE_TMR_MASK;
	XTmrCtr_SetControlStatusReg(AXI_TIMER_BASEADDR, TmrCtrNumber, ctlsts);

	XTmrCtr_Enable(AXI_TIMER_BASEADDR, TmrCtrNumber);
	return XST_SUCCESS;

}

/************************ TEST FUNCTIONS ************************************/




/*********************** HELPER FUNCTIONS ***********************************/

/****************************************************************************/
/**
* insert delay (in microseconds) between instructions.
*
* This function should be in libc but it seems to be missing.  This emulation implements
* a delay loop with (really) approximate timing; not perfect but it gets the job done.
*
* @param	usec is the requested delay in microseconds
*
* @return	*NONE*
*
* @note
* This emulation assumes that the microblaze is running @ 100MHz and takes 15 clocks
* per iteration - this is probably totally bogus but it's a start.
*
*****************************************************************************/

static const u32	DELAY_1US_CONSTANT	= 15;	// constant for 1 microsecond delay

void usleep(u32 usec)
{
	volatile u32 i, j;

	for (i = 0; i < usec; i++)
	{
		for (j = 0; j < DELAY_1US_CONSTANT; j++);
	}
	return;
}

/*********************** DISPLAY-RELATED FUNCTIONS ***********************************/

/****************************************************************************/
/**
* Converts an integer to ASCII characters
*
* algorithm borrowed from ReactOS system libraries
*
* Converts an integer to ASCII in the specified base.  Assumes string[] is
* long enough to hold the result plus the terminating null
*
* @param 	value is the integer to convert
* @param 	*string is a pointer to a buffer large enough to hold the converted number plus
*  			the terminating null
* @param	radix is the base to use in conversion,
*
* @return  *NONE*
*
* @note
* No size check is done on the return string size.  Make sure you leave room
* for the full string plus the terminating null in string
*****************************************************************************/
void PMDIO_itoa(int32_t value, char *string, int32_t radix)
{
	char tmp[33];
	char *tp = tmp;
	int32_t i;
	uint32_t v;
	int32_t  sign;
	char *sp;

	if (radix > 36 || radix <= 1)
	{
		return;
	}

	sign = ((10 == radix) && (value < 0));
	if (sign)
	{
		v = -value;
	}
	else
	{
		v = (uint32_t) value;
	}

  	while (v || tp == tmp)
  	{
		i = v % radix;
		v = v / radix;
		if (i < 10)
		{
			*tp++ = i+'0';
		}
		else
		{
			*tp++ = i + 'a' - 10;
		}
	}
	sp = string;

	if (sign)
		*sp++ = '-';

	while (tp > tmp)
		*sp++ = *--tp;
	*sp = 0;

  	return;
}


/****************************************************************************/
/**
* Write a 32-bit unsigned hex number to PmodOLEDrgb in Hex
*
* Writes  32-bit unsigned number to the pmodOLEDrgb display starting at the current
* cursor position.
*
* @param num is the number to display as a hex value
*
* @return  *NONE*
*
* @note
* No size checking is done to make sure the string will fit into a single line,
* or the entire display, for that matter.  Watch your string sizes.
*****************************************************************************/
void PMDIO_puthex(PmodOLEDrgb* InstancePtr, uint32_t num)
{
  char  buf[9];
  int32_t   cnt;
  char  *ptr;
  int32_t  digit;

  ptr = buf;
  for (cnt = 7; cnt >= 0; cnt--) {
    digit = (num >> (cnt * 4)) & 0xF;

    if (digit <= 9)
	{
      *ptr++ = (char) ('0' + digit);
	}
    else
	{
      *ptr++ = (char) ('a' - 10 + digit);
	}
  }

  *ptr = (char) 0;
  OLEDrgb_PutString(InstancePtr,buf);

  return;
}


/****************************************************************************/
/**
* Write a 32-bit number in Radix "radix" to LCD display
*
* Writes a 32-bit number to the LCD display starting at the current
* cursor position. "radix" is the base to output the number in.
*
* @param num is the number to display
*
* @param radix is the radix to display number in
*
* @return *NONE*
*
* @note
* No size checking is done to make sure the string will fit into a single line,
* or the entire display, for that matter.  Watch your string sizes.
*****************************************************************************/
void PMDIO_putnum(PmodOLEDrgb* InstancePtr, int32_t num, int32_t radix)
{
  char  buf[16];

  PMDIO_itoa(num, buf, radix);
  OLEDrgb_PutString(InstancePtr,buf);

  return;
}
/**
* Converts an unsigned integer to BCD so that it can be displayed
* on the Seven Segment display
*
* @param	bin is the unsigned integer to convert.
*
* @param	*bcd is a pointer to the buffer holding the result. The buffer
*			should be at least 10 bytes long. the BCD digits are returned
*			with the largest digit being in bcd[0] and the least signifcant
*			digit being in bcd[9]
*
* @return	NONE
*
* @note
*	Source:  http://www.keil.com/forum/14621/
*/


/**************************** INTERRUPT HANDLERS ******************************/

/****************************************************************************/
/**
* Fixed interval timer interrupt handler
*
* Reads the GPIO port which acquires the hardware PWM duty cycle data for the RGB Leds
* Implements the software PWM detection using edge detection.

 *****************************************************************************/

void FIT_Handler(void)
{
	// Read actualRpm from the motor
	actualRpm = pmodHB3_read_motor_speed(&pmodHB3_inst);

	// Calculate Desired and Motor Output...
	
	// Proportional term
	desiredRpm = desiredMotorSpeed * MAX_RPM/MAX_MOTOR_SPEED;
	
	prev_signedError = signedError;
	signedError = desiredRpm - actualRpm;
	p_term = (float)(pcntrl_const)/100 * signedError;
	
	//Integral term
	
	i_term = (float)(icntrl_const)/1000 * signedError * dt;
	// Derivative term
	d_term = (float)(dcntrl_const)/100 * signedError - prev_signedError;
	
	//motorOutRpm = p_term + desiredRpm;
	motorOutRpm = p_term + i_term + d_term;
	
	if(motorOutRpm > MAX_RPM){
			motorOutRpm = MAX_RPM;
	}
	else if (motorOutRpm < 0){
		motorOutRpm = 0;
	}

	motorOut = (u8)(motorOutRpm* MAX_MOTOR_SPEED /MAX_RPM );

	pmodHB3_set_motor_speed(&pmodHB3_inst, motorOut);
}
