// WARNING: Do NOT edit the input and output ports in this file in a text
// editor if you plan to continue editing the block that represents it in
// the Block Editor! File corruption is VERY likely to occur.

// Copyright (C) 1991-2010 Altera Corporation
// Your use of Altera Corporation's design tools, logic functions 
// and other software and tools, and its AMPP partner logic 
// functions, and any output files from any of the foregoing 
// (including device programming or simulation files), and any 
// associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License 
// Subscription Agreement, Altera MegaCore Function License 
// Agreement, or other applicable license agreement, including, 
// without limitation, that your use is for the sole purpose of 
// programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the 
// applicable agreement for further details.


// Generated by Quartus II Version 9.1 (Build Build 350 03/24/2010)
// Created on Sun Dec 01 15:04:04 2019

//  Module Declaration
module UA
(
	// {{ALTERA_ARGS_BEGIN}} DO NOT REMOVE THIS LINE!
	xx, z, res, mode, clk, p, y, clkout
	// {{ALTERA_ARGS_END}} DO NOT REMOVE THIS LINE!
);
// Port Declaration

	// {{ALTERA_IO_BEGIN}} DO NOT REMOVE THIS LINE!
	input xx;
	input z;
	input res;
	input [1:0] mode;
	input clk;
	input [15:0] p;
	output [26:0] y;
	output clkout;
	// {{ALTERA_IO_END}} DO NOT REMOVE THIS LINE!
integer pc=1;
reg [26:0] y;
wire clkout;
assign clkout=!clk;
always @(posedge clk)
begin
y=26'b0000000000000000000000000000;
if (res) begin pc=1; end;
case (pc)
1:begin
if (xx) begin y[0]=1; y[1]=1; y[2]=1; y[5]=1; y[16]=1; y[10]=1; pc=2; end;
end
2:begin
if ((mode[0] == 0) && (mode[1] == 0)) begin pc=3; end;
if ((mode[0] == 1) && (mode[1] == 0)) begin pc=50; end;
if (mode[1] == 1) begin pc=100; end;
end
3:begin
if (p[0]) begin pc=0; end;
if (!p[0]) begin y[17]=1; y[21]=1; pc=4; end;
end

4:begin
if (xx) begin y[2]=1; y[5]=1; pc=5; end;
end

5:begin
if (p[0]) begin pc=0; end;
if (!p[0]) begin y[17]=1; pc=6; end; 
end

6:begin
if (p[8]) begin pc=12; end;
if (!p[8]) begin
	if (p[9]) begin pc=0; end;
	if (!p[9]) begin pc=7;  end;
end;
end

7:begin
if (p[14]) begin y[11]=1; y[14]=1; end;
pc=8; 
end

8:begin
if (p[3]) begin pc=9; end;
if (!p[3]) begin y[3]=1; y[13]=1; y[22]=1; pc=7; end;
end

9:begin			//Norm
if (p[1]) begin y[19]=1; y[12]=1; pc=10; end
if (!p[1]) begin pc=11; end;			
end

10:begin
if (p[9]) begin pc=0; end;		
if (!p[9]) begin
	if (p[13]) begin pc=0; end;	
	if (!p[13]) begin pc=9; end; 
end;
end

11:begin		//Fin
if (z) begin y[22]=1; y[26]=1; pc=1; end;
end

0:begin			//ZF
y[10]=1; y[16]=1; y[24]=1; pc=11;
end

12:begin		//PRS
y[23]=1; pc=1;
end

50:begin
y[11]=1; y[14]=1; y[17]=1; pc=51;
end

51:begin
if (xx) begin y[0]=1; y[2]=1; y[5]=1; pc=52; end;
end

52:begin
if (p[0]) begin pc=11; end;
if (!p[0]) begin 
	if (p[1]) begin pc=54; end;	
	if (!p[1]) begin 
		if (!p[4]) begin y[20]=1; end;
		y[17]=1; pc=53;
	end; 
end;
end

53:begin
if (p[6]) begin 
	if (p[4]) begin pc=12; end;
	if (!p[4]) begin pc=0; end;
end;
if (!p[6]) begin 
	if (p[7]) begin 
		if (p[12]) begin pc=55; end;
		if (!p[12]) begin y[10]=1; y[16]=1; pc=54; end;
	end;
	if (!p[7]) begin 
		if (p[12]) begin y[17]=1; pc=11; end;
		if (!p[12]) begin pc=55; end;
	end; 
end;
end

54:begin	//two
y[11]=1; y[17]=1; pc=11;
end

55:begin	//Equ
if (p[10]) begin y[4]=1; y[7]=1; y[13]=1; pc=57; end;
if (!p[10]) begin 
	if (p[7]) begin y[4]=1; y[18]=1; end;
	if (!p[7]) begin 
		if (p[4]) begin y[8]=1; y[13]=1; y[19]=1; pc=56; end;
		if (!p[4]) begin y[7]=1; y[13]=1; y[19]=1; end;
	end;
end;
end

56:begin
y[6]=1; pc=55;
end

57:begin
y[11]=1; y[17]=1; pc=9; 
end

100:begin
if (xx) begin y[2]=1; y[17]=1; pc=101; end;
end

101:begin
y[11]=1; pc=9;
end

endcase;
end
endmodule
