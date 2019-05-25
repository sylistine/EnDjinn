#pragma once

namespace Djn
{
    template<typename T>
    struct tuple2
    {
        union
        {
            T x;
            T r;
        };
        union
        {
            T y;
            T g;
        };
        // ctors.
        tuple2() : x(0), y(0) {}
        tuple2(T x, T y) : x(x), y(y) {}
        // Arithmetic operators.
        inline const tuple2 operator+(const tuple2& rhs)
        {
            return tuple2(x + rhs.x, y + rhs.y);
        }
        inline const tuple2 operator-(const tuple2& rhs)
        {
            return tuple2(x - rhs.x, y - rhs.y);
        }
        inline const tuple2 operator*(const T& rhs)
        {
            return tuple2(x * rhs, y * rhs);
        }
        inline const tuple2 operator/(const T& rhs)
        {
            return tuple2(x / rhs, y / rhs);
        }
        // Assignment operators.
        const tuple2& operator+=(const tuple2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
        const tuple2& operator-=(const tuple2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }
        const tuple2& operator*=(T rhs)
        {
            x *= rhs;
            y *= rhs;
            return *this;
        }
        const tuple2& operator/=(T rhs)
        {
            x /= rhs;
            y /= rhs;
            return *this;
        }
        // etc.
        bool operator==(const tuple2& rhs)
        {
            return x == rhs.x && y == rhs.y;
        }
        bool operator!=(const tuple2& rhs)
        {
            return x != rhs.x || y != rhs.y;
        }
    };

    typedef tuple2<double> double2;
    typedef tuple2<float> float2;
    typedef tuple2<int> int2;
    typedef tuple2<unsigned int> uint2;

    template<typename T>
    struct tuple3
    {
        union
        {
            T x;
            T r;
        };
        union
        {
            T y;
            T g;
        };
        union
        {
            T z;
            T b;
        };
        // ctors
        tuple3() : x(0), y(0), z(0) {}
        tuple3(T x, T y, T z) : x(x), y(y), z(z) {}
        // Arithmetic Operators
        inline const tuple3 operator+(const tuple3& rhs)
        {
            return tuple3(x + rhs.x, y + rhs.y, z + rhs.z);
        }
        inline const tuple3 operator-(const tuple3& rhs)
        {
            return tuple3(x - rhs.x, y - rhs.y, z - rhs.z);
        }
        inline const tuple3 operator*(const T& rhs)
        {
            return tuple3(x * rhs, y * rhs, z * rhs);
        }
        inline const tuple3 operator/(const T& rhs)
        {
            return tuple3(x / rhs, y / rhs, z / rhs);
        }
        // Assignment Operators
        const tuple3& operator+=(const tuple3& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        const tuple3& operator-=(const tuple3& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        const tuple3& operator*=(const T& rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }
        const tuple3& operator/=(const T& rhs)
        {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            return *this;
        }
        // etc.
        bool operator==(const tuple3& rhs)
        {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }
        bool operator!=(const tuple3& rhs)
        {
            return x != rhs.x || y != rhs.y || z != rhs.z;
        }
    };

    typedef tuple3<double> double3;
    typedef tuple3<float> float3;
    typedef tuple3<int> int3;
    typedef tuple3<unsigned int> uint3;

    template<typename T>
    struct tuple4
    {
        union
        {
            T x;
            T r;
        };
        union
        {
            T y;
            T g;
        };
        union
        {
            T z;
            T b;
        };
        union
        {
            T w;
            T a;
        };
        // ctor
        tuple4() : x(0), y(0), z(0), w(0) {}
        tuple4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        // Arithmetic operators.
        inline const tuple4 operator+(const tuple4& rhs)
        {
            return tuple4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }
        inline const tuple4 operator-(const tuple4& rhs)
        {
            return tuple4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }
        inline const tuple4 operator*(const T& rhs)
        {
            return tuple4(x * rhs, y * rhs, z * rhs, w * rhs);
        }
        inline const tuple4 operator/(const T& rhs)
        {
            return tuple4(x / rhs, y / rhs, z / rhs, w / rhs);
        }
        // Assignment operators.
        const tuple4& operator+=(const tuple4& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }
        const tuple4& operator-=(const tuple4& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }
        const tuple4& operator*=(const T& rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;
            return *this;
        }
        const tuple4& operator/=(const T& rhs)
        {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            w /= rhs;
            return *this;
        }
        // etc.
        bool operator==(const tuple4& rhs)
        {
            return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
        }
        bool operator!=(const tuple4& rhs)
        {
            return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
        }
    };

    typedef tuple4<double> double4;
    typedef tuple4<float> float4;
    typedef tuple4<int> int4;
    typedef tuple4<unsigned int> uint4;
}

