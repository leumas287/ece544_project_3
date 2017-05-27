`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/13/2017 11:39:11 AM
// Design Name: 
// Module Name: MOTOR_SPEED_DETECTOR
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

module MOTOR_SPEED_DETECTOR
    #( 
        parameter COUNT = 100_000_000,        // Set the default parameter to 100e6, or 1 second for the 100MHz input clock
        parameter WIDTH = 10
    )    
    (
    input clk,
    input reset_n,
    input motor_sensor_input_a,
    input motor_sensor_input_b,
    output [WIDTH-1:0] motor_speed_count
    );
    
    
    reg prev_a;                     // Previous value of the SA sensor, for edge detection
    reg prev_b;                     // Previous value of the SB sensor, for edge detection
    reg [31:0] counter;             // 32-bit counter to track the 1 second period value
    reg [WIDTH-1:0] counter_a;            // 10-bit counter for the SA signal, meaning we don't expect more than 1023Hz signal or a min 1ms period 
    reg [WIDTH-1:0] counter_b;            // 10-bit counter for the SB signal
    reg [WIDTH-1:0] counter_out_a;        // 10-bit output for the counter_a, output every 1 second
    reg [WIDTH-1:0] counter_out_b;        // 10-bit output for the counter_b, output every 1 second
    
    // Continually assign counter_out_a to the motor_speed_count
    assign motor_speed_count = counter_out_a;
        
    // Initialize everything to zero
    initial begin
        counter = 0;
        prev_a = 0;
        prev_a = 0;
        counter_a = 0;
        counter_b = 0;
        counter_out_a = 0;
        counter_out_b = 0;
    end

    // At every 10MHz clock or 100ns, or the asynchronous negative edge triggered active low reset, enter the block
    always @(posedge clk or negedge reset_n) begin
        // If we are resetting, set everything to zero
        if (!reset_n) begin
            counter <= 0;
            counter_a <= 0;
            counter_b <= 0;
            counter_out_a <= 0;
            counter_out_b <= 0;
            prev_a <= 0;
            prev_b <= 0;
        // If the counter is reached (every 1 second), store the sensor counters and reset them to zero
        // and reset the 1 second counter
        end else if (counter >= COUNT) begin
            counter <= 0;
            counter_out_a <= counter_a;
            counter_out_b <= counter_b;
            counter_a <= 0;
            counter_b <= 0;
        end else begin
            // Check for a positive edge on the SA input
            if (motor_sensor_input_a > prev_a)
                counter_a <= counter_a + 1;
                
            // Check for a positive edge on the SB input
            if (motor_sensor_input_b > prev_b)
                counter_b <= counter_b + 1;
                
            // Store the current sensor outputs as the previous, for positive edge detection
            prev_a <= motor_sensor_input_a;
            prev_b <= motor_sensor_input_b;
            
            // Increment the counter
            counter <= counter + 1;
        end
    end
endmodule
