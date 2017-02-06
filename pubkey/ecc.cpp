#include <iostream>
#include "bn/bn_boost.h"

using namespace boost::multiprecision;

struct EPoint
{
	cpp_int x;
	cpp_int y;
};

struct JPoint
{
	cpp_int x;
	cpp_int y;
	cpp_int z0;
	cpp_int z1;
	cpp_int z2;
};

struct Curve
{
	cpp_int p;
	cpp_int q;
	cpp_int n;
};

// Find curve parameter q mod n having point (x, y) and parameter p
cpp_int curve_q( const & cpp_int x, const & y, const & p, const & n)
{
    return ((x * x - p) * x - y * y) % n;
}

// Test, whether the given point is on the curve (p, q, n)'''
bool on_the_curve(const & EPoint point, const Curve & curve)
{
	if ( point.x != 0 || point.y != 0 )
	{
		return (point.x * point.x * point.x - curve.p * point.x - curve.q) % curve.n == (point.y * point.y) % curve.n;
	}
	
	return true;
}

// Transform point p given as (x, y) to projective coordinates
JPoint to_projective( const EPoint & p)
{
	JPoint jp;
	jp.x = p.x;
	jp.y = p.y;
	
	if ( p.x == 0 && p.y== 0 )
	{
		jp.z0 = jp.z1 = jp.z2 = 0;
		return jp;
	}

	jp.z0 = jp.z1 = jp.z2 = 1;
	return jp;
}

// Transform a point from projective coordinates to (x, y) mod n
EPoint from_projective( const JPoint & jp, const cpp_int & n )
{
	EPoint p;
    if ( jp.x == 0 && jp.y == 0 )
	{
		p.x = p.y = 0;
	}
    else
    {
		p.x = (jp.x * inv_mod(jp.z1, n)) % n;
		p.y = (jp.y * inv(jp.z2, n)) % n;
    }
	
	return p; 
}

// Compute the inverse point to p in affine coordinate system
EPoint neg( const EPoint & p, const cpp_int & n)
{
	EPoint pn;
	pn.x = p.x;
	pn.y = (n - p.y) % n;
    
    return pn;
}
