#ifndef _ECC_H_
#define _ECC_H_

// --- ELLIPTIC CURVE MATH ------------------------------------------------------
//
//   curve definition:   y^2 = x^3 - p*x - q
//   over finite field:  Z/nZ* (prime residue classes modulo a prime number n)
//
//
//   COPYRIGHT (c) 2017 Liarod.v.RomanGol
// ------------------------------------------------------------------------------

#include <iostream>
#include "../bn/bn_boost.h"


/*
Module for elliptic curve arithmetic over a prime field GF(n).
E(GF(n)) takes the form y**2 == x**3 - p*x - q (mod n) for a prime n.

0. Structures used by this module

    PARAMETERS and SCALARS are non-negative (long) integers.

    A POINT (x, y), usually denoted p1, p2, ...
    is a pair of (long) integers where 0 <= x < n and 0 <= y < n

    A POINT in PROJECTIVE COORDINATES, usually denoted jp1, jp2, ...
    takes the form (X, Y, Z, Z**2, Z**3) where x = X / Z**2
    and y = Y / z**3. This form is called Jacobian coordinates.

    The NEUTRAL element "0" or "O" is represented by None
    in both coordinate systems.

1. Basic Functions

    find_param_b()      Finds the curve parameter b (mod n)
                        when p and a point are given.
    on_the_curve()      Tests whether a point (x, y) is on the curve.

2. Point transformations

    to_projective()     Converts a point (x, y) to projective coordinates.
    from_projective()   Converts a point from projective coordinates
                        to (x, y) using the transformation described above.
    neg()               Computes the inverse point -P in both coordinate
                        systems.

3. Slow point arithmetic

    These algorithms make use of basic geometry and modular arithmetic
    thus being suitable for small numbers and academic study.

    add()               Computes the sum of two (x, y)-points
    mul()               Perform scalar multiplication using "double & add"

4. Fast point arithmetic

    These algorithms make use of projective coordinates, signed binary
    expansion and a JSP-like approach (joint sparse form).

    The following functions consume and return projective coordinates:

    addf()              Optimized point addition.
    doublef()           Optimized point doubling.
    mulf()              Highly optimized scalar multiplication.
    muladdf()           Highly optimized addition of two products.
    
    The following functions use the optimized ones above but consume
    and output (x, y)-coordinates for a more convenient usage:

    mulp()              Encapsulates mulf()
    muladdp()           Encapsulates muladdf()

    For single additions add() is generally faster than an encapsulation of
    addf() which would involve expensive coordinate transformations.
    Hence there is no addp() and doublep().
*/


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
	size_t blockLen;
	cpp_int a;
	cpp_int b;
	cpp_int p;
	cpp_int n;
	EPoint G; 
};

bool is_null		( const EPoint & point );
bool is_null		( const JPoint & point );
bool on_the_curve	(const EPoint & point, const Curve & curve);

cpp_int		find_param_b	( const EPoint & point, const cpp_int & a, const cpp_int & n);
JPoint		to_projective	( const EPoint & point);
EPoint		from_projective	( const JPoint & jpoint, const cpp_int & n );

EPoint		neg				( const EPoint & point, const cpp_int & n);
EPoint		add				( const EPoint & p, const EPoint & q, const Curve & curve );
EPoint 		mul				( cpp_int k, EPoint p, const Curve & curve );



#endif
