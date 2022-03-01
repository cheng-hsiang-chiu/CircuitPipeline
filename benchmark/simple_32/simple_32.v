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
wire n6;
wire n7;
wire n8;
wire n9;
wire n10;
wire n11;
wire n12;
wire n13;
wire n14;
wire n15;
wire n16;
wire n17;
wire n18;
wire n19;
wire n20;
wire n21;
wire n22;
wire n23;
wire n24;
wire n25;
wire n26;
wire n27;
wire n28;
wire n29;
wire n30;
wire n31;
wire n32;
wire n33;
wire n34;
wire n35;
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
INV_X3 u5 ( .a(n5), .o(n6) );
INV_X4 u6 ( .a(n6), .o(n7) );
INV_X3 u7 ( .a(n7), .o(n8) );
INV_X3 u8 ( .a(n8), .o(n9) );
INV_X4 u9 ( .a(n9), .o(n10) );
INV_X3 u10 ( .a(n10), .o(n11) );
INV_X3 u11 ( .a(n11), .o(n12) );
INV_X3 u12 ( .a(n12), .o(n13) );
INV_X4 u13 ( .a(n13), .o(n14) );
INV_X3 u14 ( .a(n14), .o(n15) );
INV_X3 u15 ( .a(n15), .o(n16) );
INV_X4 u16 ( .a(n16), .o(n17) );
INV_X3 u17 ( .a(n17), .o(n18) );
INV_X1 u18 ( .a(n18), .o(n19) );
INV_X3 u19 ( .a(n19), .o(n20) );
INV_X2 u20 ( .a(n20), .o(n21) );
INV_X3 u21 ( .a(n21), .o(n22) );
INV_X3 u22 ( .a(n22), .o(n23) );
INV_X4 u23 ( .a(n23), .o(n24) );
INV_X3 u24 ( .a(n24), .o(n25) );
INV_X3 u25 ( .a(n25), .o(n26) );
INV_X4 u26 ( .a(n26), .o(n27) );
INV_X3 u27 ( .a(n27), .o(n28) );
INV_X3 u28 ( .a(n28), .o(n29) );
INV_X3 u29 ( .a(n29), .o(n30) );
INV_X3 u30 ( .a(n30), .o(n31) );
INV_X1 u31 ( .a(n31), .o(n32) );
INV_X3 u32 ( .a(n32), .o(n33) );
INV_X4 u33 ( .a(n33), .o(n34) );
INV_X2 u34 ( .a(n34), .o(n35) );
INV_X3 u35 ( .a(n35), .o(out) );

endmodule
