`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/13/2017 10:23:07 AM
// Design Name: 
// Module Name: MOTOR_SPEED_CONTROLLER
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module MOTOR_SPEED_CONTROLLER
    #( parameter WIDTH = 12)
    (
    input clk,
    input reset_n,
    input [WIDTH-1:0] motor_speed_in,
    output motor_enable_out
    );
    
	parameter PWM_COUNT = 2**WIDTH - 1;   // max motor speed input count
    
    reg [WIDTH-1:0] counter;
    reg [WIDTH-1:0] pwm_counter;
    
    reg motor_enable;
    
    // Connect motor_enable to motor_enable_out
    assign motor_enable_out = motor_enable;
    
    // Reset the enable and counter to zero
    initial begin
        counter = 0;
        motor_enable = 0;
    end
    
    // setup a free running counter that counts up to PWM_COUNT, the PWM period
    always @(posedge clk)
        if ((counter < PWM_COUNT) & (reset_n))
            counter <= counter + 1;
        else
            counter <= 0;
         
    // while the requested motor speed is larger than the counter, output a 1 to the motor enable
    always @(posedge clk) begin
        if ((counter < motor_speed_in) & (reset_n))
            motor_enable <= 1;
        else
            motor_enable <= 0;
    end
endmodule
