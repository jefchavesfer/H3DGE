import definitions_pack::*;

package math_pack;

function fixed fDiv2( fixed a, fixed b );
	logic[ _WIDTH + BF : 0 ] aux;
	aux = a << BF;
	aux = aux / b;					
	return aux;
endfunction		

function fixed compl2( fixed x );
	logic[5:0] i;
	for( i = 0; i < _WIDTH; i++ )
	begin
		x[i] = ~x[i];
	end
	x = x + 1;	
	return x;
endfunction

function fixedCM compl2BP( fixedBP x );
	logic[5:0] i;
	for( i = 0; i < _WIDTH_BP; i++ )
	begin
		x[i] = ~x[i];
	end
	x = x + 1;	
	return x;
endfunction

function fixedCM compl2CM( fixedCM x );
	logic[5:0] i;
	for( i = 0; i < _WIDTH_CM; i++ )
	begin
		x[i] = ~x[i];
	end
	x = x + 1;	
	return x;
endfunction

function fixed fDiv( fixed a, fixed b /*, integer of*/ );
	logic[ _WIDTH + BF - 1 : 0 ] aux;
	logic signal;  
	fixed c;
	signal = a[_WIDTH - 1] ^ b[_WIDTH - 1];	
	//$fdisplay(of, "sa: %x, sb: %x, sc %x", a[_WIDTH - 1], b[_WIDTH - 1], signal);
	if( a[_WIDTH - 1] == 1 )
	begin
	  a = compl2( a );
	end
	if( b[_WIDTH - 1] == 1 )
	begin
	  b = compl2( b );
	end
	aux = a << BF;
	//$fdisplay( of, "a: %x aux: %x, b:  %x", a, aux, b );
	c = aux / b;				
	if( signal == 1 )
	begin
	  c = compl2( c );
	end
	//$fdisplay( of, "res: %x", c ); 
	return c;
endfunction	

function fixedCM fDivCM( fixedCM a, fixedCM b /*, integer of*/ );
	logic[ _WIDTH_CM + BF_CM - 1 : 0 ] aux;
	logic signal;  
	fixedCM c;
	signal = a[_WIDTH_CM - 1] ^ b[_WIDTH_CM - 1];	
	//$fdisplay(of, "sa: %x, sb: %x, sc %x", a[_WIDTH - 1], b[_WIDTH - 1], signal);
	if( a[_WIDTH_CM - 1] == 1 )
	begin
	  a = compl2CM( a );
	end
	if( b[_WIDTH_CM - 1] == 1 )
	begin
	  b = compl2CM( b );
	end
	aux = a << BF_CM;
	//$fdisplay( of, "a: %x aux: %x, b:  %x", a, aux, b );
	c = aux / b;				
	if( signal == 1 )
	begin
	  c = compl2CM( c );
	end
	//$fdisplay( of, "res: %x", c ); 
	return c;
endfunction

function fixed fMul( fixed a, fixed b /*, integer of*/ );
	logic signal;  
	fixed c;
	signal = a[_WIDTH - 1] ^ b[_WIDTH - 1];	
	//$fdisplay(of, "sa: %x, sb: %x, sc %x", a[_WIDTH - 1], b[_WIDTH - 1], signal);
	if( a[_WIDTH - 1] == 1 )
	begin
	  a = compl2( a );
	end
	if( b[_WIDTH - 1] == 1 )
	begin
	  b = compl2( b );
	end
	c = `m(a,b);
	//$fdisplay( of, "a: %x aux: %x, b:  %x", a, aux, b );				
	if( signal == 1 )
	begin
	  c = compl2( c );
	end
	//$fdisplay( of, "res: %x", c ); 
	return c;
endfunction	 

function fixedCM fMulCM( fixedCM a, fixedCM b /*, integer of*/ );
	logic signal;  
	fixedCM c;
	signal = a[_WIDTH_CM - 1] ^ b[_WIDTH_CM - 1];	
	//$fdisplay(of, "sa: %x, sb: %x, sc %x", a[_WIDTH_CM - 1], b[_WIDTH_CM - 1], signal);
	if( a[_WIDTH_CM - 1] == 1 )
	begin
	  a = compl2CM( a );
	end
	if( b[_WIDTH_CM - 1] == 1 )
	begin
	  b = compl2CM( b );
	end
	c = `mCM(a,b);
	//$fdisplay( of, "a: %x aux: %x, b:  %x", a, aux, b );				
	if( signal == 1 )
	begin
	  c = compl2CM( c );
	end
	//$fdisplay( of, "res: %x", c ); 
	return c;
endfunction

function fixedBP fMulRefr( fixedCM a, fixed b /*, integer of*/ );

	logic signal;  
	fixedBP ax, bx;
	fixedBP c;
	signal = a[_WIDTH_CM - 1] ^ b[_WIDTH - 1];	
	//$fdisplay(of, "sa: %x, sb: %x, sc %x", a[_WIDTH_CM - 1], b[_WIDTH_CM - 1], signal);
	if( a[_WIDTH_CM - 1] == 1 )
	begin
	  a = compl2CM( a );
	end
	if( b[_WIDTH - 1] == 1 )
	begin
	  b = compl2( b );
	end
	
	ax = a << DEC_CM_BP;
	bx = b << DEC_BP;
	
	c = `mBP(ax,bx);
	//$fdisplay( of, "a: %x aux: %x, b:  %x", a, aux, b );				
	if( signal == 1 )
	begin
	  c = compl2BP( c );
	end
	//$fdisplay( of, "res: %x", c ); 
	return c;
endfunction

function fixed squareRoot( fixed x, integer of ); 
	fixed number, n, r, temp1, temp2, temp3, aux;
	logic[8:0] count, maxcount;
	
	number = x;
	//$fdisplay(of, "number: %x", number);
	maxcount = _WIDTH - BI/2 - 1;

	r = 0;
	n = 0;
	for ( count=0; count <= maxcount; count = count + 1 )
	begin
		temp1 = n << 2;
		//$fdisplay(of, "temp1 %x",temp1 );
	    temp2 = number >> ( _WIDTH - 2 );
	    //$fdisplay(of, "temp2 %x",temp2 );
		temp3 = 3;
	    n = temp1 | ( temp2 & temp3 );
		//$fdisplay(of, "n %x temp3: %x",n, temp3 );
	    number = number << 2;
		//$fdisplay(of, "number %x",number );
	    temp1 = r << 2;	
		//$fdisplay(of, "temp1 %x r %x",temp1, r );
	    temp1 = temp1 + 1;	   
		//$fdisplay(of, "temp1 %x",temp1 );
	    aux = n - temp1;
		if( aux[_WIDTH - 1] == 0 )
	    begin
	       n = n - temp1;
		   //$fdisplay(of, "n %x",n );
	       r = ( r << 1 ) + 1; 
		   //$fdisplay(of, "r %x",r );
	    end
	    else
	    begin
			r = r << 1;
			//$fdisplay(of, "r %x",r );
	    end
	end
	x = r;
	return x;
endfunction

endpackage

