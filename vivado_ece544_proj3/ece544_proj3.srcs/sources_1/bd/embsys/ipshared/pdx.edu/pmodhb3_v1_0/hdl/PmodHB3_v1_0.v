//////////////////////////////////////////////////////////////////////////////////
// Company: Portland Statue University
// Engineer: Samuel K Burkhart (skb@pedx.edu)
//
// Description: 
// ------------
// This module is the top level for the pmodHB3 module.
//
// Dependencies: 
// -------------
// 	This module is dependent on MOTOR_SPEED_CONTROLLER.v (PWM motor controller), MOTOR_SPEED_DETECTOR.v (uses hall effect sensor to detect speed of the motor),
// and MOTOR_DIRECTION_CONTROLLER.v (uses the detected speed and user input to change the direction of the motor) and makes use
//	of signals produced in the AXI interface module for the periphal
// 
// Revision:
// ---------
// 1.0	SKB	File Created
//
// Additional Comments:
// --------------------
// 
//////////////////////////////////////////////////////////////////////////////////
`timescale 1 ns / 1 ps

	module PmodHB3_v1_0 #
	(
		// Users to add parameters here
		parameter integer MOTOR_SPEED_CONTROL_WIDTH		= 12,
		parameter integer MOTOR_SPEED_DETECT_WIDTH		= 10,
		parameter integer MOTOR_SPEED_DETECT_COUNT		= 25_000_000,
		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer C_S00_AXI_DATA_WIDTH	= 32,
		parameter integer C_S00_AXI_ADDR_WIDTH	= 4
	)
	(
		// Users to add ports here
        input wire motor_sensor_input_a,    // SA input from the PmodHB3
        input wire motor_sensor_input_b,    // SB input from the PmodHB3
        output wire motor_enable_out,       // Motor Enable output to the PmodHB3
        output wire motor_direction_out,    // Motor Direction output to the PmodHB3
        
		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Slave Bus Interface S00_AXI
		input wire  s00_axi_aclk,
		input wire  s00_axi_aresetn,
		input wire [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_awaddr,
		input wire [2 : 0] s00_axi_awprot,
		input wire  s00_axi_awvalid,
		output wire  s00_axi_awready,
		input wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_wdata,
		input wire [(C_S00_AXI_DATA_WIDTH/8)-1 : 0] s00_axi_wstrb,
		input wire  s00_axi_wvalid,
		output wire  s00_axi_wready,
		output wire [1 : 0] s00_axi_bresp,
		output wire  s00_axi_bvalid,
		input wire  s00_axi_bready,
		input wire [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_araddr,
		input wire [2 : 0] s00_axi_arprot,
		input wire  s00_axi_arvalid,
		output wire  s00_axi_arready,
		output wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_rdata,
		output wire [1 : 0] s00_axi_rresp,
		output wire  s00_axi_rvalid,
		input wire  s00_axi_rready
	);
	
	// declare the interconnect wires
	wire [MOTOR_SPEED_CONTROL_WIDTH-1:0] motor_speed_control;         // User output for the current motor speed from the PmodHB3
	wire motor_direction_control;           // User input for the motor direction used to change motor direction of rotation on the PmodHB3
    wire [MOTOR_SPEED_DETECT_WIDTH-1:0] motor_speed_detect;      // User input for the motor speed, used to drive the motor enable with a PWM signal to the Pmod HB3
	
// Instantiation of Axi Bus Interface S00_AXI
	PmodHB3_v1_0_S00_AXI # ( 
		.MOTOR_SPEED_CONTROL_WIDTH(MOTOR_SPEED_CONTROL_WIDTH),
		.MOTOR_SPEED_DETECT_WIDTH(MOTOR_SPEED_DETECT_WIDTH),
		.C_S_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH)
	) PmodHB3_v1_0_S00_AXI_inst (
	    // Peripheral-specific signals (to/from register bits)
	    .MOTOR_SPEED_DETECT(motor_speed_detect),
	    .MOTOR_DIRECTION_CONTROL(motor_direction_control),
	    .MOTOR_SPEED_CONTROL(motor_speed_control),
	    
	    // AXI Lite Signals
		.S_AXI_ACLK(s00_axi_aclk),
		.S_AXI_ARESETN(s00_axi_aresetn),
		.S_AXI_AWADDR(s00_axi_awaddr),
		.S_AXI_AWPROT(s00_axi_awprot),
		.S_AXI_AWVALID(s00_axi_awvalid),
		.S_AXI_AWREADY(s00_axi_awready),
		.S_AXI_WDATA(s00_axi_wdata),
		.S_AXI_WSTRB(s00_axi_wstrb),
		.S_AXI_WVALID(s00_axi_wvalid),
		.S_AXI_WREADY(s00_axi_wready),
		.S_AXI_BRESP(s00_axi_bresp),
		.S_AXI_BVALID(s00_axi_bvalid),
		.S_AXI_BREADY(s00_axi_bready),
		.S_AXI_ARADDR(s00_axi_araddr),
		.S_AXI_ARPROT(s00_axi_arprot),
		.S_AXI_ARVALID(s00_axi_arvalid),
		.S_AXI_ARREADY(s00_axi_arready),
		.S_AXI_RDATA(s00_axi_rdata),
		.S_AXI_RRESP(s00_axi_rresp),
		.S_AXI_RVALID(s00_axi_rvalid),
		.S_AXI_RREADY(s00_axi_rready)
	);

	// Add user logic here
    // instantiate the motor speed controller
    MOTOR_SPEED_CONTROLLER #(
		.WIDTH(MOTOR_SPEED_CONTROL_WIDTH)
	) motor_speed_controller (
		.clk(s00_axi_aclk),
		.reset_n(s00_axi_aresetn),
		.motor_speed_in(motor_speed_control),
		.motor_enable_out(motor_enable_out)
	);
    
    // instantiate the motor speed detector    
    MOTOR_SPEED_DETECTOR #(
		.COUNT(MOTOR_SPEED_DETECT_COUNT), 
		.WIDTH(MOTOR_SPEED_DETECT_WIDTH)
	) motor_speed_detector (
		.clk(s00_axi_aclk),
		.reset_n(s00_axi_aresetn),
		.motor_sensor_input_a(motor_sensor_input_a),
		.motor_sensor_input_b(motor_sensor_input_b),
		.motor_speed_count(motor_speed_detect)
	);
        
    // instantiate the motor direction controller        
    MOTOR_DIRECTION_CONTROLLER #(
		.WIDTH(MOTOR_SPEED_DETECT_WIDTH)
	) motor_direction_controller (
		.clk(s00_axi_aclk),
		.reset_n(s00_axi_aresetn),
		.motor_speed_in(motor_speed_detect),
		.motor_direction_in(motor_direction_control),
		.motor_direction_out(motor_direction_out)
	);
    
	// User logic ends

	endmodule
