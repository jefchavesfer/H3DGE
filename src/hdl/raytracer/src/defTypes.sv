package definitions_pack;

parameter _WIDTH = 32;
parameter BI = 4;
parameter BF = _WIDTH - BI;

parameter _WIDTH_CM = 21;  //width for materials and colors proprieties
parameter BI_CM = 5;
parameter BF_CM = _WIDTH_CM - BI_CM;   

parameter _WIDTH_BP = BF + BI + 4;	//precision for refraction calculations	(2.5*2.5*-8) [diamond refraction index] ^ 2 * (1 - maxVetorialProductValue)
parameter BI_BP = BI + 4;
parameter BF_BP = BF;

parameter DEC_BP = BF_BP - BF;        //for equalizing the decalage in mRefr function
parameter DEC_CM_BP = BF_BP - BF_CM; 
parameter DEC_CM = BF - BF_CM;

`define m(A , B) ( ( ( A +64'b0 ) * ( B + 64'b0 ) ) >> BF )
`define fixed( A ) ( A*(2**BF) )  
`define fixed2( A, B ) ( A*(2**BF) + B )  

`define mCM(A , B) ( ( ( A +64'b0 ) * ( B + 64'b0 ) ) >> BF_CM )
`define fixedCM( A ) ( A*(2**BF_CM) )  
`define fixed2CM( A, B ) ( A*(2**BF_CM) + B )  

`define mBP(A , B) ( ( ( A + 72'b0 ) * ( B + 72'b0 ) ) >> BF_BP )
`define fixedBP( A ) ( A*(2**BF_BP) )
`define fixed2BP( A, B ) ( A*(2**BF_BP) + B ) 

`define fCMp0p003 21'h0000c4
`define fCMp0p5 21'h008000

`define fBP0p001 36'h000041893

`define fp0p1 32'h01999999a
`define fp0p001  32'h00041893
`define fn0p001  32'hfffbe76d	  
`define fp0p002  32'h00083126
`define fn0p2  32'hfccccccc		
`define fp0p5  32'h08000000
`define fn0p5  32'hf8000000

//`define fp0p0001 32'h0000d1b7


typedef struct packed 
{
	logic [_WIDTH - 1:0] value;
} fixed;

typedef struct packed 
{
	logic [_WIDTH_CM - 1:0] value;
} fixedCM;

typedef struct packed 
{
	logic [_WIDTH_BP - 1:0] value;
} fixedBP;

typedef struct packed
{
	fixed x;
	fixed y;
	fixed z;
} point;

typedef struct packed
{
	fixed x;
	fixed y;
	fixed z;
} vector;

typedef struct packed
{
	point start;
	vector dir;
} ray;

typedef struct packed
{
	point v1;
	point v2;
	point v3;
} triangle;	 

typedef struct packed
{
    fixedCM reflection;
	fixedCM red, green, blue;
	fixedCM transmission;
	fixedCM nrefraction;
} material;

endpackage 

