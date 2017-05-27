`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/15/2017 07:49:56 PM
// Design Name: 
// Module Name: MOTOR_DIRECTION_CONTROLLER
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


module MOTOR_DIRECTION_CONTROLLER
    #( parameter WIDTH = 10)
	(
    input clk,
    input reset_n,
    input [WIDTH-1:0] motor_speed_in,
    input motor_direction_in,
    output motor_direction_out
    );
    
    reg motor_direction;
    reg prev_motor_direction;
    
    assign motor_direction_out = motor_direction;
    
    initial begin
        motor_direction = 0;
        prev_motor_direction = 0;
    end
    
    // setup a free running counter that counts up to PWM_COUNT, the PWM period
    always @(posedge clk)
        if (!reset_n) begin
            motor_direction <= 0;
            prev_motor_direction <= 0;
        end else if (motor_speed_in == 0) begin
            motor_direction <= motor_direction_in;
            prev_motor_direction = motor_direction_in;
        end else begin
            motor_direction <= prev_motor_direction;
        end
        
endmodule
