module simple (
inp1,
inp2,
tau2015_clk,
out
);

// Start PIs
input inp1;
input inp2;
input tau2015_clk;

// Start POs
output out;

// Start wires
wire n1;
wire n2;
wire n3;
wire n4;
wire n5;
wire inp1;
wire inp2;
wire tau2015_clk;
wire out;

// Start cells
NAND2_X1 u1 ( .a(inp1), .b(inp2), .o(n1) );
DFF_X80 f1 ( .d(n2), .ck(tau2015_clk), .q(n3) );
INV_X1 u2 ( .a(n3), .o(n4) );
INV_X2 u3 ( .a(n4), .o(n5) );
NOR2_X1 u4 ( .a(n1), .b(n3), .o(n2) );
INV_X3 u5 ( .a(n5), .o(out) );

endmodule
