#include "ecc.h"

bool is_null ( const EPoint & point )
{
	return ( point.x == 0 && point.y == 0 );
}

bool is_null ( const JPoint & jpoint )
{
	return ( jpoint.x == 0 && jpoint.y == 0 );
}


// Find curve parameter b mod p having point (x, y) and parameter a
cpp_int find_param_b( const EPoint & point, const cpp_int & a, const cpp_int & n)
{
    return mod( ((point.x * point.x - a) * point.x - point.y * point.y), n);
}

// Test, whether the given point is on the curve (a, b, n)'''
bool on_the_curve(const EPoint & point, const Curve & curve)
{
	if ( point.x != 0 || point.y != 0 )
	{
		return mod( (point.x * point.x * point.x - curve.a * point.x - curve.b), curve.n ) == mod( (point.y * point.y), curve.n);
	}
	
	return true;
}

// Reconstruct the y-coordinate when curve, x and the sign-bit of
// the y coordinate are given:
// sign value: 1, or -1
// TODO: simple case for prime mod sqrt ,curve.n%4==3
cpp_int y_from_x(const cpp_int& x, const Curve& curve,int sign)
{
	if( mod( curve.n ,cpp_int(4) ) == cpp_int(3) )
	{
		cpp_int res = mod( x*x*3 - curve.a*x - curve.b, curve.n);
		cpp_int t = pow_mod(res, (curve.n+1)/4 , curve.n);
		if(sign==1)
			return t;
		else
			return curve.n -t ;

	}
	else
	{
		return 0;
	}
}




// Transform point p given as (x, y) to projective coordinates
JPoint to_projective( const EPoint & point)
{
	JPoint jpoint;
	jpoint.x = point.x;
	jpoint.y = point.y;
	
	if ( is_null( point ) )
	{
		jpoint.z0 = jpoint.z1 = jpoint.z2 = 0;
		return jpoint;
	}

	jpoint.z0 = jpoint.z1 = jpoint.z2 = 1;
	return jpoint;
}

// Transform a point from projective coordinates to (x, y) mod n
EPoint from_projective( const JPoint & jpoint, const cpp_int & n )
{
	EPoint point;
    if ( is_null(jpoint) )
	{
		point.x = point.y = 0;
	}
    else
    {
		point.x = mod( (jpoint.x * inv_mod(jpoint.z1, n)), n );
		point.y = mod( (jpoint.y * inv_mod(jpoint.z2, n)), n );
    }
	
	return point;
}

// Compute the inverse point to p in affine coordinate system
EPoint neg( const EPoint & point, const cpp_int & n)
{
	EPoint pn;
	pn.x = point.x;
	pn.y = mod( (n - point.y), n );
    
    return pn;
}


// '''Add points p and q over curve'''
EPoint add( const EPoint & p, const EPoint & q, const Curve & curve )
{
	if ( !is_null(p) && !is_null(q) )
	{
		EPoint r;
		cpp_int slope;
		cpp_int x;

		if ( mod( (p.x - q.x), curve.n ) )
	    {
            slope = (p.y - q.y) * inv_mod(p.x - q.x, curve.n);
            slope = mod(slope, curve.n);
            x = slope * slope - p.x - q.x;
            x = mod(x, curve.n);				// intersection with curve
        }
        else
        {
            if ( mod(p.y + q.y, curve.n) )							// slope s calculated by derivation
            {
                slope = (3 * p.x * p.x - curve.a) * inv_mod(2 * p.y, curve.n);
                slope = mod( slope, curve.n );
                x = slope * slope - 2 * p.x;            			// intersection with curve
                x = mod(x, curve.n);
            }
		}
		r.x = x;
		r.y = p.y + slope * (x - p.x);
		r.y = curve.n - mod(r.y, curve.n);

		return r;
    }
   

	if ( is_null(q) )
    	return p;
	return q;
}

// scalar multiplication k * p = p + p + ... + p (k times) in O(log(n))
// '''multiply point p by scalar k over curve (p, q, n)'''
EPoint mul( cpp_int k, EPoint p, const Curve & curve )
{
    EPoint r;
    r.x = r.y = 0;
    
	while ( k > 0 )
	{
		if ( k & 1 )
		{
			r = add(r, p, curve);
		}
		k >>= 1;
		p = add(p, p, curve);
	}

	return r;
}


/*
# addition of points in y**2 = x**3 - p*x - q over <Z/nZ*; +>
def add(p, q, n, p1, p2):
    '''Add points p1 and p2 over curve (p, q, n)'''
    if p1 and p2:
        x1, y1 = p1
        x2, y2 = p2
        if (x1 - x2) % n:
            s = ((y1 - y2) * inv(x1 - x2, n)) % n   # slope
            x = (s * s - x1 - x2) % n               # intersection with curve
            return (x, n - (y1 + s * (x - x1)) % n)
        else:
            if (y1 + y2) % n:       # slope s calculated by derivation
                s = ((3 * x1 * x1 - p) * inv(2 * y1, n)) % n
                x = (s * s - 2 * x1) % n            # intersection with curve
                return (x, n - (y1 + s * (x - x1)) % n)
            else:
                return None
    else:   # either p1 is not none -> ret. p1, otherwiese p2, which may be
        return p1 if p1 else p2     # none too.


# faster addition: redundancy in projective coordinates eliminates
# expensive inversions mod n.
def addf(p, q, n, jp1, jp2):
    '''Add jp1 and jp2 in projective (jacobian) coordinates.'''
    if jp1 and jp2:
        
        x1, y1, z1, z1s, z1c = jp1
        x2, y2, z2, z2s, z2c = jp2

        s1 = (y1 * z2c) % n
        s2 = (y2 * z1c) % n

        u1 = (x1 * z2s) % n
        u2 = (x2 * z1s) % n

        if (u1 - u2) % n:

            h = (u2 - u1) % n
            r = (s2 - s1) % n

            hs = (h * h) % n
            hc = (hs * h) % n

            x3 = (-hc - 2 * u1 * hs + r * r) % n
            y3 = (-s1 * hc + r * (u1 * hs - x3)) % n
            z3 = (z1 * z2 * h) % n
            
            z3s = (z3 * z3) % n
            z3c = (z3s * z3) % n
    
            return (x3, y3, z3, z3s, z3c)
        
        else:
            if (s1 + s2) % n:
                return doublef(p, q, n, jp1)
            else:
                return None
    else:
        return jp1 if jp1 else jp2

# explicit point doubling using redundant coordinates
def doublef(p, q, n, jp):
    '''Double jp in projective (jacobian) coordinates'''
    if not jp:
        return None
    x1, y1, z1, z1p2, z1p3 = jp
    
    y1p2 = (y1 * y1) % n
    a = (4 * x1 * y1p2) % n
    b = (3 * x1 * x1 - p * z1p3 * z1) % n
    x3 = (b * b - 2 * a) % n
    y3 = (b * (a - x3) - 8 * y1p2 * y1p2) % n
    z3 = (2 * y1 * z1) % n
    z3p2 = (z3 * z3) % n
    
    return x3, y3, z3, z3p2, (z3p2 * z3) % n


# SCALAR MULTIPLICATION --------------------------------------------------------

# scalar multiplication p1 * c = p1 + p1 + ... + p1 (c times) in O(log(n))
def mul(p, q, n, p1, c):
    '''multiply point p1 by scalar c over curve (p, q, n)'''
    res = None
    while c > 0:
        if c & 1:
            res = add(p, q, n, res, p1)
        c >>= 1                     # c = c / 2
        p1 = add(p, q, n, p1, p1)   # p1 = p1 * 2
    return res


# this method allows _signed_bin() to choose between 1 and -1. It will select
# the sign which leaves the higher number of zeroes in the binary
# representation (the higher GDB).
def _gbd(n):
    '''Compute second greatest base-2 divisor'''
    i = 1
    if n <= 0: return 0
    while not n % i:
        i <<= 1
    return i >> 2


# This method transforms n into a binary representation having signed bits.
# A signed binary expansion contains more zero-bits hence reducing the number
# of additions required by a multiplication algorithm.
#
# Example:  15 ( 0b1111 ) can be written as 16 - 1, resulting in (1,0,0,0,-1)
#           and saving 2 additions. Subtraction can be performed as
#           efficiently as addition.
def _signed_bin(n):
    '''Transform n into an optimized signed binary representation'''
    r = []
    while n > 1:
        if n & 1:
            cp = _gbd(n + 1) 
            cn = _gbd(n - 1)
            if cp > cn:         # -1 leaves more zeroes -> subtract -1 (= +1)
                r.append(-1)
                n += 1
            else:               # +1 leaves more zeroes -> subtract +1 (= -1)
                r.append(+1)
                n -= 1
        else:
            r.append(0)         # be glad about one more zero
        n >>= 1
    r.append(n)
    return r[::-1]


# This multiplication algorithm combines signed binary expansion and
# fast addition using projective coordinates resulting in 5 to 10 times
# faster multiplication.
def mulf(p, q, n, jp1, c):
    '''Multiply point jp1 by c in projective coordinates'''
    sb = _signed_bin(c)
    res = None
    jp0 = neg(jp1, n)  # additive inverse of jp1 to be used fot bit -1
    for s in sb:
        res = doublef(p, q, n, res)
        if s:
            res = addf(p, q, n, res, jp1) if s > 0 else \
                  addf(p, q, n, res, jp0)
    return res

# Encapsulates mulf() in order to enable flat coordinates (x, y)
def mulp(p, q, n, p1, c):
    '''Multiply point p by c using fast multiplication'''
    return from_projective(mulf(p, q, n, to_projective(p1), c), n)


# Sum of two products using Shamir's trick and signed binary expansion
def muladdf(p, q, n, jp1, c1, jp2, c2):
    '''Efficiently compute c1 * jp1 + c2 * jp2 in projective coordinates'''
    s1 = _signed_bin(c1)
    s2 = _signed_bin(c2)
    diff = len(s2) - len(s1)
    if diff > 0:
        s1 = [0] * diff + s1
    elif diff < 0:
        s2 = [0] * -diff + s2

    jp1p2 = addf(p, q, n, jp1, jp2)
    jp1n2 = addf(p, q, n, jp1, neg(jp2, n))

    precomp = ((None,           jp2,            neg(jp2, n)),
               (jp1,            jp1p2,          jp1n2),
               (neg(jp1, n),    neg(jp1n2, n),  neg(jp1p2, n)))
    res = None

    for i, j in zip(s1, s2):
        res = doublef(p, q, n, res)
        if i or j:
            res = addf(p, q, n, res, precomp[i][j])
    return res

# Encapsulate muladdf()
def muladdp(p, q, n, p1, c1, p2, c2):
    '''Efficiently compute c1 * p1 + c2 * p2 in (x, y)-coordinates'''
    return from_projective(muladdf(p, q, n,
                                   to_projective(p1), c1,
                                   to_projective(p2), c2), n)

# POINT COMPRESSION ------------------------------------------------------------

# Compute the square root modulo n


# Determine the sign-bit of a point allowing to reconstruct y-coordinates
# when x and the sign-bit are given:
def sign_bit(p1):
    '''Return the signedness of a point p1'''
    return p1[1] % 2 if p1 else 0

# Reconstruct the y-coordinate when curve parameters, x and the sign-bit of
# the y coordinate are given:
def y_from_x(x, p, q, n, sign):
    '''Return the y coordinate over curve (p, q, n) for given (x, sign)'''

    # optimized form of (x**3 - p*x - q) % n
    a = (((x * x) % n - p) * x - q) % n

*/        


#ifdef DEBUG
using namespace std;

//test vector
//http://point-at-infinity.org/ecc/nisttv
int main()
{
	EPoint pp;
	for(int i=1;i<20;i++)
	{
	cpp_int k(i);
	cout<<dec<<"k = "<<k<<endl;

	pp = mul(k,secp256r1.G,secp256r1);

	cout << "x = "<<hex << pp.x <<endl;
	cout << "y = "<<hex << pp.y <<endl;
	cout<<endl;
	}


}

#endif //DEBUG
        
