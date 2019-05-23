#pragma once

namespace Djn
{
    template<typename T>
    struct tuple2
    {
        union
        {
            T x;
            T a;
            T r;
        };
        union
        {
            T y;
            T b;
            T g;
        };
        tuple2& operator+(const tuple2& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
        tuple2& operator-(const tuple2& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }
        tuple2& operator*(T rhs)
        {
            x *= rhs;
            y *= rhs;
            return *this;
        }
        tuple2& operator/(T rhs)
        {
            x /= rhs;
            y /= rhs;
            return *this;
        }
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
            T a;
            T r;
        };
        union
        {
            T y;
            T b;
            T g;
        };
        union
        {
            T z;
            T c;
            T b;
        };
        tuple3& operator+(const tuple3& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        tuple3& operator-(const tuple3& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        tuple3& operator*(T rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }
        tuple3& operator/(T rhs)
        {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            return *this;
        }
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
            T a;
            T r;
        };
        union
        {
            T y;
            T b;
            T g;
        };
        union
        {
            T z;
            T c;
            T b;
        };
        union
        {
            T w;
            T d;
            T a;
        };
        tuple4& operator+(const tuple4& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }
        tuple4& operator-(const tuple4& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }
        tuple4& operator*(T rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;
            return *this;
        }
        tuple4& operator/(T rhs)
        {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            w /= rhs;
            return *this;
        }
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

