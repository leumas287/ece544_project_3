//Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2016.2 (win64) Build 1577090 Thu Jun  2 16:32:40 MDT 2016
//Date        : Sun Jun 04 17:52:51 2017
//Host        : sburkhar-MOBL2 running 64-bit major release  (build 9200)
//Command     : generate_target embsys_wrapper.bd
//Design      : embsys_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module embsys_wrapper
   (RGB1_Blue,
    RGB1_Green,
    RGB1_Red,
    RGB2_Blue,
    RGB2_Green,
    RGB2_Red,
    an,
    btnC,
    btnD,
    btnL,
    btnR,
    btnU,
    clk_100,
    dp,
    gpio_0_gpio_tri_i,
    led,
    motor_direction_out,
    motor_enable_out,
    motor_sensor_input_a,
    motor_sensor_input_b,
    pmodENC_A,
    pmodENC_B,
    pmodENC_btn,
    pmodENC_sw,
    pmodoledrgb_out_pin10_io,
    pmodoledrgb_out_pin1_io,
    pmodoledrgb_out_pin2_io,
    pmodoledrgb_out_pin3_io,
    pmodoledrgb_out_pin4_io,
    pmodoledrgb_out_pin7_io,
    pmodoledrgb_out_pin8_io,
    pmodoledrgb_out_pin9_io,
    sec1_int,
    seg,
    sw,
    sysclk,
    sysreset_n,
    uart_rtl_rxd,
    uart_rtl_txd);
  output RGB1_Blue;
  output RGB1_Green;
  output RGB1_Red;
  output RGB2_Blue;
  output RGB2_Green;
  output RGB2_Red;
  output [7:0]an;
  input btnC;
  input btnD;
  input btnL;
  input btnR;
  input btnU;
  output clk_100;
  output dp;
  input [31:0]gpio_0_gpio_tri_i;
  output [15:0]led;
  output motor_direction_out;
  output motor_enable_out;
  input motor_sensor_input_a;
  input motor_sensor_input_b;
  input pmodENC_A;
  input pmodENC_B;
  input pmodENC_btn;
  input pmodENC_sw;
  inout pmodoledrgb_out_pin10_io;
  inout pmodoledrgb_out_pin1_io;
  inout pmodoledrgb_out_pin2_io;
  inout pmodoledrgb_out_pin3_io;
  inout pmodoledrgb_out_pin4_io;
  inout pmodoledrgb_out_pin7_io;
  inout pmodoledrgb_out_pin8_io;
  inout pmodoledrgb_out_pin9_io;
  input [0:0]sec1_int;
  output [6:0]seg;
  input [15:0]sw;
  input sysclk;
  input sysreset_n;
  input uart_rtl_rxd;
  output uart_rtl_txd;

  wire RGB1_Blue;
  wire RGB1_Green;
  wire RGB1_Red;
  wire RGB2_Blue;
  wire RGB2_Green;
  wire RGB2_Red;
  wire [7:0]an;
  wire btnC;
  wire btnD;
  wire btnL;
  wire btnR;
  wire btnU;
  wire clk_100;
  wire dp;
  wire [31:0]gpio_0_gpio_tri_i;
  wire [15:0]led;
  wire motor_direction_out;
  wire motor_enable_out;
  wire motor_sensor_input_a;
  wire motor_sensor_input_b;
  wire pmodENC_A;
  wire pmodENC_B;
  wire pmodENC_btn;
  wire pmodENC_sw;
  wire pmodoledrgb_out_pin10_i;
  wire pmodoledrgb_out_pin10_io;
  wire pmodoledrgb_out_pin10_o;
  wire pmodoledrgb_out_pin10_t;
  wire pmodoledrgb_out_pin1_i;
  wire pmodoledrgb_out_pin1_io;
  wire pmodoledrgb_out_pin1_o;
  wire pmodoledrgb_out_pin1_t;
  wire pmodoledrgb_out_pin2_i;
  wire pmodoledrgb_out_pin2_io;
  wire pmodoledrgb_out_pin2_o;
  wire pmodoledrgb_out_pin2_t;
  wire pmodoledrgb_out_pin3_i;
  wire pmodoledrgb_out_pin3_io;
  wire pmodoledrgb_out_pin3_o;
  wire pmodoledrgb_out_pin3_t;
  wire pmodoledrgb_out_pin4_i;
  wire pmodoledrgb_out_pin4_io;
  wire pmodoledrgb_out_pin4_o;
  wire pmodoledrgb_out_pin4_t;
  wire pmodoledrgb_out_pin7_i;
  wire pmodoledrgb_out_pin7_io;
  wire pmodoledrgb_out_pin7_o;
  wire pmodoledrgb_out_pin7_t;
  wire pmodoledrgb_out_pin8_i;
  wire pmodoledrgb_out_pin8_io;
  wire pmodoledrgb_out_pin8_o;
  wire pmodoledrgb_out_pin8_t;
  wire pmodoledrgb_out_pin9_i;
  wire pmodoledrgb_out_pin9_io;
  wire pmodoledrgb_out_pin9_o;
  wire pmodoledrgb_out_pin9_t;
  wire [0:0]sec1_int;
  wire [6:0]seg;
  wire [15:0]sw;
  wire sysclk;
  wire sysreset_n;
  wire uart_rtl_rxd;
  wire uart_rtl_txd;

  embsys embsys_i
       (.PmodOLEDrgb_out_pin10_i(pmodoledrgb_out_pin10_i),
        .PmodOLEDrgb_out_pin10_o(pmodoledrgb_out_pin10_o),
        .PmodOLEDrgb_out_pin10_t(pmodoledrgb_out_pin10_t),
        .PmodOLEDrgb_out_pin1_i(pmodoledrgb_out_pin1_i),
        .PmodOLEDrgb_out_pin1_o(pmodoledrgb_out_pin1_o),
        .PmodOLEDrgb_out_pin1_t(pmodoledrgb_out_pin1_t),
        .PmodOLEDrgb_out_pin2_i(pmodoledrgb_out_pin2_i),
        .PmodOLEDrgb_out_pin2_o(pmodoledrgb_out_pin2_o),
        .PmodOLEDrgb_out_pin2_t(pmodoledrgb_out_pin2_t),
        .PmodOLEDrgb_out_pin3_i(pmodoledrgb_out_pin3_i),
        .PmodOLEDrgb_out_pin3_o(pmodoledrgb_out_pin3_o),
        .PmodOLEDrgb_out_pin3_t(pmodoledrgb_out_pin3_t),
        .PmodOLEDrgb_out_pin4_i(pmodoledrgb_out_pin4_i),
        .PmodOLEDrgb_out_pin4_o(pmodoledrgb_out_pin4_o),
        .PmodOLEDrgb_out_pin4_t(pmodoledrgb_out_pin4_t),
        .PmodOLEDrgb_out_pin7_i(pmodoledrgb_out_pin7_i),
        .PmodOLEDrgb_out_pin7_o(pmodoledrgb_out_pin7_o),
        .PmodOLEDrgb_out_pin7_t(pmodoledrgb_out_pin7_t),
        .PmodOLEDrgb_out_pin8_i(pmodoledrgb_out_pin8_i),
        .PmodOLEDrgb_out_pin8_o(pmodoledrgb_out_pin8_o),
        .PmodOLEDrgb_out_pin8_t(pmodoledrgb_out_pin8_t),
        .PmodOLEDrgb_out_pin9_i(pmodoledrgb_out_pin9_i),
        .PmodOLEDrgb_out_pin9_o(pmodoledrgb_out_pin9_o),
        .PmodOLEDrgb_out_pin9_t(pmodoledrgb_out_pin9_t),
        .RGB1_Blue(RGB1_Blue),
        .RGB1_Green(RGB1_Green),
        .RGB1_Red(RGB1_Red),
        .RGB2_Blue(RGB2_Blue),
        .RGB2_Green(RGB2_Green),
        .RGB2_Red(RGB2_Red),
        .an(an),
        .btnC(btnC),
        .btnD(btnD),
        .btnL(btnL),
        .btnR(btnR),
        .btnU(btnU),
        .clk_100(clk_100),
        .dp(dp),
        .gpio_0_GPIO_tri_i(gpio_0_gpio_tri_i),
        .led(led),
        .motor_direction_out(motor_direction_out),
        .motor_enable_out(motor_enable_out),
        .motor_sensor_input_a(motor_sensor_input_a),
        .motor_sensor_input_b(motor_sensor_input_b),
        .pmodENC_A(pmodENC_A),
        .pmodENC_B(pmodENC_B),
        .pmodENC_btn(pmodENC_btn),
        .pmodENC_sw(pmodENC_sw),
        .sec1_int(sec1_int),
        .seg(seg),
        .sw(sw),
        .sysclk(sysclk),
        .sysreset_n(sysreset_n),
        .uart_rtl_rxd(uart_rtl_rxd),
        .uart_rtl_txd(uart_rtl_txd));
  IOBUF pmodoledrgb_out_pin10_iobuf
       (.I(pmodoledrgb_out_pin10_o),
        .IO(pmodoledrgb_out_pin10_io),
        .O(pmodoledrgb_out_pin10_i),
        .T(pmodoledrgb_out_pin10_t));
  IOBUF pmodoledrgb_out_pin1_iobuf
       (.I(pmodoledrgb_out_pin1_o),
        .IO(pmodoledrgb_out_pin1_io),
        .O(pmodoledrgb_out_pin1_i),
        .T(pmodoledrgb_out_pin1_t));
  IOBUF pmodoledrgb_out_pin2_iobuf
       (.I(pmodoledrgb_out_pin2_o),
        .IO(pmodoledrgb_out_pin2_io),
        .O(pmodoledrgb_out_pin2_i),
        .T(pmodoledrgb_out_pin2_t));
  IOBUF pmodoledrgb_out_pin3_iobuf
       (.I(pmodoledrgb_out_pin3_o),
        .IO(pmodoledrgb_out_pin3_io),
        .O(pmodoledrgb_out_pin3_i),
        .T(pmodoledrgb_out_pin3_t));
  IOBUF pmodoledrgb_out_pin4_iobuf
       (.I(pmodoledrgb_out_pin4_o),
        .IO(pmodoledrgb_out_pin4_io),
        .O(pmodoledrgb_out_pin4_i),
        .T(pmodoledrgb_out_pin4_t));
  IOBUF pmodoledrgb_out_pin7_iobuf
       (.I(pmodoledrgb_out_pin7_o),
        .IO(pmodoledrgb_out_pin7_io),
        .O(pmodoledrgb_out_pin7_i),
        .T(pmodoledrgb_out_pin7_t));
  IOBUF pmodoledrgb_out_pin8_iobuf
       (.I(pmodoledrgb_out_pin8_o),
        .IO(pmodoledrgb_out_pin8_io),
        .O(pmodoledrgb_out_pin8_i),
        .T(pmodoledrgb_out_pin8_t));
  IOBUF pmodoledrgb_out_pin9_iobuf
       (.I(pmodoledrgb_out_pin9_o),
        .IO(pmodoledrgb_out_pin9_io),
        .O(pmodoledrgb_out_pin9_i),
        .T(pmodoledrgb_out_pin9_t));
endmodule
