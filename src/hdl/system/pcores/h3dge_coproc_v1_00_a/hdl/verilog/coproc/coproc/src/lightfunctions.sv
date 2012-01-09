import definitions_pack::*;
import math_pack::*;

package light_pack;

function logic[_WIDTH+1:0] functionIntersectionTest( ray r, triangle trig, integer of );
	integer of;
	vector e1, e2, t1, p, q;
	fixed t, u, v, det;				
	fixed teste1, teste2, teste3;
	logic[_WIDTH+1:0] resultado;								  
	
	$fdisplay(of, "starting calculations");
	
	$fdisplay(of, "v1: %x %x %x", trig.v1.x, trig.v1.y, trig.v1.z );
	$fdisplay(of, "v2: %x %x %x", trig.v2.x, trig.v2.y, trig.v2.z );
	$fdisplay(of, "v3: %x %x %x", trig.v3.x, trig.v3.y, trig.v3.z );
	
	e1.x = trig.v2.x - trig.v1.x;
	e1.y = trig.v2.y - trig.v1.y;
	e1.z = trig.v2.z - trig.v1.z; 
	
	$fdisplay(of, "e1: %x %x %x", e1.x, e1.y, e1.z );
	
	e2.x = trig.v3.x - trig.v1.x;
	e2.y = trig.v3.y - trig.v1.y;
	e2.z = trig.v3.z - trig.v1.z;				
	
	$fdisplay(of, "e2: %x %x %x", e2.x, e2.y, e2.z );
	
	t1.x = r.start.x - trig.v1.x;
	t1.y = r.start.y - trig.v1.y;
	t1.z = r.start.z - trig.v1.z;
	
	$fdisplay(of, "r.start: %x %x %x", r.start.x, r.start.y, r.start.z );
	$fdisplay(of, "r.dir: %x %x %x", r.dir.x, r.dir.y, r.dir.z );
	$fdisplay(of, "trig.v1: %x %x %x", trig.v1.x, trig.v1.y, trig.v1.z );
	$fdisplay(of, "t1: %x %x %x", t1.x, t1.y, t1.z );
	
	p.x = fMul( r.dir.y, e2.z ) - fMul( r.dir.z, e2.y ); 
	p.y = fMul( e2.x, r.dir.z ) - fMul( r.dir.x, e2.z ); 
	p.z = fMul( r.dir.x, e2.y ) - fMul( e2.x, r.dir.y);
	
	$fdisplay( of, "p: %x %x %x", p.x, p.y, p.z );
	
	q.x = fMul( t1.y, e1.z ) - fMul( t1.z, e1.y ); 
	q.y = fMul( e1.x, t1.z ) - fMul( t1.x, e1.z ); 
	q.z = fMul( t1.x, e1.y ) - fMul( e1.x, t1.y );			  
	
	$fdisplay( of, "q: %x %x %x", q.x, q.y, q.z );
	
	det = fMul( p.x, e1.x ) + fMul( p.y, e1.y ) + fMul( p.z, e1.z );	  	
	
	$fdisplay( of, "det: %x", det );
	
	if( det >= `fixed(`fn0p001) || det <= `fixed2(0, `fp0p001 ) )
	begin
		resultado = 0;		 
		$fdisplay(of, "case det == 0");
	end
	else			  
	begin
		u = fDiv( fMul( t1.x, p.x ) + fMul( t1.y, p.y ) + fMul( t1.z, p.z ) , det );			 
		$fdisplay(of, "u %x", u);
		if( u < 0 || u > `fixed2(1, `fp0p001) )							  
		begin  
			$fdisplay(of, "parametric value u outside limits : %x", u );
			resultado = 0;
		end
		else
		begin
			v = fMul(r.dir.z, q.z);
			$fdisplay(of,"testv: %x", v);
			v = fDiv ( fMul(r.dir.x, q.x) + fMul(r.dir.y, q.y) + fMul(r.dir.z, q.z), det );
			
			$fdisplay(of, "v: %x", v);
			
			if( v < 0 || ( u + v ) > `fixed2(1, `fp0p001) )
			begin
				$fdisplay(of, "parametric value v outside limits %x, %x", u, v);
				resultado = 0;
			end
			else
			begin
				t = fMul(e2.x, q.x) + fMul( e2.y, q.y ) + fMul( e2.z, q.z ); 
				t = fDiv( t, det );
				$display(of, "t: %x", t);
				if( t > `fixed2( 0, `fp0p001 ) && t < `fixed( 2 )  )
				begin  
					$fdisplay(of, "value t inside limits");
					$fdisplay(of, "e2: %x, %x, %x, e1: %x, %x, %x t1: %x, %x, %x", e2.x, e2.y, e2.z, e1.x, e1.y, e1.z, t1.x, t1.y, t1.z);
					$fdisplay(of, "q %x, %x, %x, det %x, u %x, v %x", q.x, q.y, q.z, det, u, v );
					$fdisplay(of, "t: %x", t);
					resultado[ _WIDTH + 1 : _WIDTH ] = 'b10;
					resultado[ _WIDTH - 1 : 0     ] = t;	
				end																													  
				else
				begin
					$fdisplay(of, "value t outside limits: %x", t);
					resultado = 0;
				end
			end
		end	  			  
	end					  
	
	$fdisplay(of, "saindo da funcao");
	return resultado; 
	
endfunction

function vector generateNormalVector(triangle trig, integer of);
	vector v1, v2, normal;
	fixed norm, modNorm, temp1;
	v1.x = trig.v2.x - trig.v1.x;
	v1.y = trig.v2.y - trig.v1.y;
	v1.z = trig.v2.z - trig.v1.z;
	
	v2.x = trig.v3.x - trig.v1.x;
	v2.y = trig.v3.y - trig.v1.y;
	v2.z = trig.v3.z - trig.v1.z;
	
	//$fdisplay( of,"v1 %x %x %x v2 %x %x %x", v1.x, v1.y , v1.z, v2.x, v2.y, v2.z );
	//provisorio normalizacao de vetores do triangulo (triangulos geralmente pequenos - nao precisa)
	
	temp1 = fMul(v1.x, v1.x) + fMul(v1.y, v1.y) + fMul(v1.z, v1.z);
	temp1 = squareRoot(temp1/*, of*/);
	v1.x = fDiv( v1.x, temp1 );
	v1.y = fDiv( v1.y, temp1 );
	v1.z = fDiv( v1.z, temp1 );	 
	
	temp1 = fMul(v2.x, v2.x) + fMul(v2.y, v2.y) + fMul(v2.z, v2.z);
	temp1 = squareRoot(temp1/*, of*/);
	v2.x = fDiv( v2.x, temp1 );
	v2.y = fDiv( v2.y, temp1 );
	v2.z = fDiv( v2.z, temp1 );
	
	//$fdisplay( of,"v1 %x %x %x v2 %x %x %x", v1.x, v1.y , v1.z, v2.x, v2.y, v2.z );
	
	normal.x = fMul(v1.y, v2.z) - fMul(v1.z, v2.y);
	normal.y = fMul(v1.z, v2.x) - fMul(v1.x, v2.z);
	normal.z = fMul(v1.x, v2.y) - fMul(v2.x, v1.y);
	
	$fdisplay( of,"normal %x %x %x", normal.x, normal.y , normal.z );
	
	norm = fMul(normal.x, normal.x) + fMul(normal.y, normal.y) + fMul(normal.z, normal.z);
	modNorm = squareRoot ( norm/*, of */);
	temp1  = fDiv( `fixed(1) , modNorm );
	normal.x = fMul( normal.x, temp1 );
	normal.y = fMul( normal.y, temp1 );
	normal.z = fMul( normal.z, temp1 );	   
	
	$fdisplay( of,"normal %x %x %x", normal.x, normal.y , normal.z );
	return normal;
endfunction

function ray functionReflectedRay( ray r, triangle trig, point intersectionPoint, vector normal, integer of );
	ray refl;
	fixed aux;
	
	$fdisplay( of, "intersection point: %x %x %x", intersectionPoint.x, intersectionPoint.y, intersectionPoint.z );
	
	aux = fMul( normal.x, r.dir.x ) + fMul( normal.y, r.dir.y ) + fMul( normal.z, r.dir.z );
	
	$fdisplay( of, "aux: %x", aux );	   
	
	if ( aux[_WIDTH - 1] == 0 )
	begin										
		//TODO: change this for optimized implementation
		normal.x= fMul( `fixed(-1), normal.x );
		normal.y= fMul( `fixed(-1), normal.y );
		normal.z= fMul( `fixed(-1), normal.z );
		aux = fMul( normal.x, r.dir.x ) + fMul( normal.y, r.dir.y ) + fMul( normal.z, r.dir.z );
		$fdisplay( of, "aux after normal change: %x", aux );
	end
	
	$fdisplay( of, "normal: %x %x %x", normal.x, normal.y, normal.z );
	
	refl.start.x = intersectionPoint.x;
	refl.start.y = intersectionPoint.y;
	refl.start.z = intersectionPoint.z; 	  
	
	aux = fMul( `fixed(2), aux ); 
	
	$fdisplay( of, "2 * aux: %x", aux ); 
	
	refl.dir.x = r.dir.x - fMul( aux, normal.x );
	refl.dir.y = r.dir.y - fMul( aux, normal.y );
	refl.dir.z = r.dir.z - fMul( aux, normal.z );
	
	$fdisplay( of, "reflected ray: %x %x %x", refl.dir.x, refl.dir.y, refl.dir.z );
	
	aux = fMul( refl.dir.x, refl.dir.x) + fMul( refl.dir.y, refl.dir.y ) + fMul( refl.dir.z, refl.dir.z );
	aux = squareRoot( aux/*, of*/ );
	aux = fDiv( `fixed( 1 ), aux );
	
	refl.dir.x = fMul( aux, refl.dir.x );
	refl.dir.y = fMul( aux, refl.dir.y );
	refl.dir.z = fMul( aux, refl.dir.z );
	
	$fdisplay( of, "reflected ray norm: start %x %x %x dir %x %x %x", refl.start.x, refl.start.y, refl.start.z, refl.dir.x, refl.dir.y, refl.dir.z );
	
	return refl;
endfunction

function logic[ 6 * _WIDTH + 1: 0] functionRefractedRay( ray r, triangle trig, point intersectionPoint, vector normal, integer of );
	ray refr;
	logic[1:0] code; 
	logic[ 6 * _WIDTH + 1: 0] exit;
	fixed aux, aux2, aux3, aux4; 
	fixedCM n1, n2, nq;
	vector v1, v2;
	fixedBP auxRefr;
	n2 = `fixed2CM( 2, `fCMp0p5 );
	n1 = `fixed2CM( 1, `fCMp0p003 );
	
	$fdisplay( of, "refr: intersection point: %x %x %x", intersectionPoint.x, intersectionPoint.y, intersectionPoint.z );
	
	aux = fMul( normal.x, r.dir.x ) + fMul( normal.y, r.dir.y ) + fMul( normal.z, r.dir.z );
	
	$fdisplay( of, "aux: %x", aux );	   
	
	if ( aux[_WIDTH - 1] == 0 )
	begin										
		//TODO: change this for optimized implementation
		normal.x= fMul( `fixed(-1), normal.x );
		normal.y= fMul( `fixed(-1), normal.y );
		normal.z= fMul( `fixed(-1), normal.z );
		aux = fMul( normal.x, r.dir.x ) + fMul( normal.y, r.dir.y ) + fMul( normal.z, r.dir.z );
		$fdisplay( of, "aux after normal change: %x", aux );
	end
	
	$fdisplay( of, "normal: %x %x %x", normal.x, normal.y, normal.z );
	
	refr.start.x = intersectionPoint.x;
	refr.start.y = intersectionPoint.y;
	refr.start.z = intersectionPoint.z; 	
	
	nq = fDivCM( fMulCM( n1, n1 ), fMulCM( n2, n2 ) );
	$fdisplay( of, "nq: %x", nq );
	aux2 = fMul( aux, aux );
	$fdisplay( of, "n.i^2: %x", aux2 );
	aux2 = `fixed(1) - aux2;	 
	$fdisplay( of, "1 - n.i^2: %x", aux2 );
	auxRefr = fMulRefr( nq, aux2 );	
	$fdisplay( of, "refrCond: %x", auxRefr );
	//auxRefr > 1
	if( auxRefr[ _WIDTH_BP - 1 ] == 0 && ( ( auxRefr[ _WIDTH_BP - 2 : BF_BP + 1 ] != 0 ) ||  ( auxRefr[ BF_BP ] == 1 && auxRefr[ BF_BP - 1 : 0 ] != 0 ) ) )
	begin
		code = 0;
		refr = 0; 
		$fdisplay( of, "total reflection - no refraction ray" );
	end
	else
	begin
		aux2 = auxRefr >> BF_BP - BF;
		$fdisplay( of, "auxRefr: %x", aux2 );  
		aux2 = `fixed(1) - aux2;
		$fdisplay( of, "1 - auxRefr: %x", aux2 ); 
		aux2 = squareRoot( aux2/*, of */);
		$fdisplay( of, "squrt(auxRefr): %x", aux2 );
		
		//returning to normal precision;
		nq = fDivCM( n1, n2 );
		$fdisplay( of, "nq: %x", nq );
		aux3 = nq << DEC_CM;   
		$fdisplay( of, "nqdec: %x", aux3 );
		aux4 = fMul( aux3, aux ) + aux2;
		$fdisplay( of, "normal coeff: %x", aux4 );
		
		v1.x = fMul( aux3, r.dir.x );
		v1.y = fMul( aux3, r.dir.y );
		v1.z = fMul( aux3, r.dir.z );
		$fdisplay( of, "v1: %x %x %x, aux3 %x, r.dir %x %x %x", v1.x, v1.y, v1.z, aux3, r.dir.x, r.dir.y, r.dir.z );
		
		v2.x = fMul( aux4, normal.x );
		v2.y = fMul( aux4, normal.y );
		v2.z = fMul( aux4, normal.z );	
		$fdisplay( of, "v2: %x %x %x, aux4 %x, normal %x %x %x", v2.x, v2.y, v2.z, aux4, normal.x, normal.y, normal.z );
		
		refr.start.x = intersectionPoint.x;
		refr.start.y = intersectionPoint.y;
		refr.start.z = intersectionPoint.z;	
		
		refr.dir.x = v1.x - v2.x;
		refr.dir.y = v1.y - v2.y;
		refr.dir.z = v1.z - v2.z;	
		$fdisplay( of, "refr.dir: %x %x %x", refr.dir.x, refr.dir.y, refr.dir.z );
		code = 2;
		$fdisplay( of, "successful refraction calculation" );
	end	  
	exit[ 6 * _WIDTH + 1 : 6 * _WIDTH ] = code;
	exit[ 6 * _WIDTH - 1 : 0          ] = refr;
	return exit;
endfunction

endpackage
