#pragma once

#include <type_traits>
#include <cmath>

#include <glm/gtx/constants.hpp>

#include <glowutils/glowutils.h>

namespace glowutils
{
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

    namespace interpolation
    {
        template <typename T>
        inline T linear(T t)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return t;
        }

        template <typename T>
        inline T smoothstep(T t)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return t * t * (3 - 2 * t);
        }

        template <typename T>
        inline T smoothstep2(T t)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return smoothstep(smoothstep(t));
        }

        template <typename T>
        inline T smoothstep3(T t)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return smoothstep(smoothstep2(t));
        }

        template <typename T>
        inline T smootherstep(T t)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return t * t * t * (t * (6 * t - 15) + 10);
        }

        template <typename T>
        inline T squared(T t)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return t * t;
        }

        template <typename T>
        inline T squareroot(T t)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return std::sqrt(t);
        }

        template <typename T, typename V>
        inline T exponential(T t, V v)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return (std::exp(t * v) - 1) / (std::exp(v) - 1);
        }

        template <typename T, typename V>
        inline T logarithmic(T t, V v)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return std::log(1 + (v - 1) * t) / std::log(v);
        }

        template <typename T>
        inline T cube(T t)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return t * t * t;
        }

        template <typename T>
        inline T cupola(T t)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return std::sin(t * glm::pi<T>());
        }

        template <typename T>
        inline T roof(T t)
        {
            static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

            return 1 - 2 * std::abs(t - 0.5);
        }

        template <typename T>
        inline T sin(T t)
        {
            return std::sin(t * glm::half_pi<T>());
        }
    } // namespace interpolation

    template<typename T>
    inline T interpolate(T t, InterpolationType function = LinearInterpolation, bool invert = false)
    {
        static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

        T r;

        switch (function)
        {
        case SmoothStepInterpolation:
            r = interpolation::smoothstep(t); break;
        case SmoothStep2Interpolation:
            r = interpolation::smoothstep2(t); break;
        case SmoothStep3Interpolation:
            r = interpolation::smoothstep3(t); break;
        case SmootherStepInterpolation:
            r = interpolation::smootherstep(t); break;
        case SquaredInterpolation:
            r = interpolation::squared(t); break;
        case SquarerootInterpolation:
            r = interpolation::squareroot(t); break;
        case ExponentialInterpolation:
            r = interpolation::exponential(t, 3); break;
        case LogarithmicInterpolation:
            r = interpolation::logarithmic(t, 10); break;
        case CupolaInterpolation:
            r = interpolation::cupola(t); break;
        case CubeInterpolation:
            r = interpolation::cube(t); break;
        case RoofInterpolation:
            r = interpolation::roof(t); break;
        case SinInterpolation:
            r = interpolation::sin(t); break;
        case LinearInterpolation:
        default:
            r = interpolation::linear(t); break;
        }

        return invert ? static_cast<T>(1.0) - r : r;
    }
} // namespace glowutils
