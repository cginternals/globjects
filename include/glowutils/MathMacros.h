#pragma once

#include <cmath>


// NOTE: When using powers, try to use Horner's Method


#define PI 3.1415926535897932384626433832795L
#define PI2  (PI * 2.00L)
#define PI_2 (PI * 0.50L)
#define PI4  (PI * 4.00L)
#define PI_4 (PI * 0.25L)

//#define abs(v) \
//    (((v) < 0) ? -(v) : (v))

#define intcast(v) \
    (static_cast<int>(v))

#define shortcast(v) \
    (static_cast<short>(v))

//#define mod(a, m) \
//    ((a) - (m) * (intcast((a) / (m)) - ((a) < 0 ? 1 : 0)))

#define frac(x) \
    ((x)-intcast(x))

#define deg(rad) \
    static_cast<float>((rad)* 180.0L / PI)

#define rad(deg) \
    static_cast<float>((deg)* PI / 180.0L)

#define mi(a, b) \
    ((a) < (b) ? (a) : (b))

#define ma(a, b) \
    ((a) < (b) ? (b) : (a))

// same as hour
// note: if d is negative use -decimal(d, m, s) instead of decimal(-d, m, s)
#define decimal(d, m, s) \
    ((d)+((m)+(s) / 60.0L) / 60.0L)

#define sind(deg) \
    (sin(rad(deg)))

#define cosd(deg) \
    (cos(rad(deg)))

#define tand(deg) \
    (tan(rad(deg)))

#define asind(rad) \
    (deg(asin(rad)))

#define acosd(rad) \
    (deg(acos(rad)))

#define atand(rad) \
    (deg(atan(rad)))

#define atan2d(x, y) \
    (deg(atan2(x, y)))

#define adiameter(D, r) \
    (2 * atan(r / D))

// normalizes an angle to between 0 and 2PI radians
#define rev(rad) \
    ((rad)-floor((rad) / PI2) * PI2)

// normalizes an angle to between 0 and 360 degrees
#define revd(deg) \
    ((deg)-floor((deg) / 360.0L) * 360.0L)

// cube root (e.g. needed for parabolic orbits)
#define cbrt(x) \
    (((x) > 0.0) ? exp(log(x) / 3.0L) : (((x) < 0.0) ? -cbrt(-(x)) : 0.0))


#define __b02(x) (     (x) | (     (x) >>  1))
#define __b04(x) (__b02(x) | (__b02(x) >>  2))
#define __b08(x) (__b04(x) | (__b04(x) >>  4))
#define __b16(x) (__b08(x) | (__b08(x) >>  8))
#define __b32(x) (__b16(x) | (__b16(x) >> 16))

// Returns the next power of an integer.
#define nextPowerOf2(x) \
    (__b32((x)-1) + 1)

#define prevPowerOf2(x) \
    (nextPowerOf2(x) >> 1)

#define randf(l, u) \
    (static_cast<float>(rand()) / RAND_MAX * ((u)-(l)) + (l))

#define _rand(l, u) \
    (static_cast<int>(static_cast<float>(rand()) / RAND_MAX * ((u)-(l)) + (l)))


// Interpolate

#define i_linear(t)             (t)
#define i_smoothstep(t)         ((t) * (t) * (3 - 2 * (t)))
#define i_smoothstep2(t)        smoothstep(smoothstep(t))
#define i_smoothstep3(t)        smoothstep(smoothstep2(t))
#define i_smootherstep(t)       ((t) * (t) * (t) * ((t) * (6 * (t) - 15) + 10))
#define i_squared(t)            ((t) * (t))
#define i_squareroot(t)         sqrt(t)
#define i_exponential(t, v)     ((exp((t) * (v)) - 1) / (exp((v)) - 1))
#define i_logarithmic(t, v)     (log(1 + ((v) - 1) * (t)) / log(v))
#define i_cupola(t)             sin((t) * PI)
#define i_cube(t)               ((t) * (t) * (t))
#define i_roof(t)               (1 - 2 * fabs((t) - 0.5))
#define i_sin(t)                sin((t) * PI_2)
#define i_smoothstep_ext(t, l, r) \
    ((t) < (l) ? 0 : (r) < (t) ? 1 : smoothstep(((t)-(l)) / ((r)-(l))))

// Several interpolation methods in action: http://sol.gfxile.net/interpolation/

enum InterpolationType
{
    LinearInterpolation
    , SmoothStepInterpolation
    , SmoothStep2Interpolation
    , SmoothStep3Interpolation
    , SmootherStepInterpolation // Ken Perlin
    , SquaredInterpolation
    , SquarerootInterpolation
    , ExponentialInterpolation
    , LogarithmicInterpolation
    , CupolaInterpolation
    , CubeInterpolation
    , RoofInterpolation
    , SinInterpolation          // strong in, soft out
};


template<typename T>
inline const T interpolate(
    const T t
    , const InterpolationType function = LinearInterpolation
    , bool invert = false)
{
    const T v = invert ? 1 - t : t;
    T r;

    switch (function)
    {
    case SmoothStepInterpolation:
        r = i_smoothstep(t); break;
    case SmoothStep2Interpolation:
        r = i_smoothstep2(t); break;
    case SmoothStep3Interpolation:
        r = i_smoothstep3(t); break;
    case SmootherStepInterpolation:
        r = i_smootherstep(t); break;
    case SquaredInterpolation:
        r = i_squared(t); break;
    case SquarerootInterpolation:
        r = i_squareroot(t); break;
    case ExponentialInterpolation:
        r = i_exponential(t, 3); break;
    case LogarithmicInterpolation:
        r = i_logarithmic(t, 10); break;
    case CupolaInterpolation:
        r = i_cupola(t); break;
    case CubeInterpolation:
        r = i_cube(t); break;
    case RoofInterpolation:
        r = i_roof(t); break;
    case SinInterpolation:
        r = i_sin(t); break;
    case LinearInterpolation:
    default:
        r = i_linear(t); break;
    }
    return invert ? static_cast<T>(1.0) - r : r;
}
