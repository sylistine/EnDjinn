#include "stdafx.h"
#include "CppUnitTest.h"

#include "../MathLib/Math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Djn;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    static std::wstring ToString<float4>(const float4& d2)
    {
        return L"(" +
            std::to_wstring(d2.x) + L", " + std::to_wstring(d2.y) +
            L")";
    }
}

namespace Djn::MathLibTest
{
    TEST_CLASS(float4test)
    {
    public:
        TEST_METHOD(EqualityTest)
        {
            float4 a(1.2, 3.4, 5.6, 7.8);
            float4 b(1.2, 3.4, 5.6, 7.8);
            Assert::IsTrue(a == b);
            Assert::AreEqual(a, b);
        }
        TEST_METHOD(BasicCtorTest)
        {
            float4 a;
            float4 b(0.0, 0.0);
            float4 c(0.0, 0.0, 0.0);
            float4 d(0.0, 0.0, 0.0, 0.0);
            Assert::AreEqual(a, b);
            Assert::AreEqual(b, c);
            Assert::AreEqual(c, d);
        }
        TEST_METHOD(SpecialCopyCtorTest)
        {
            float2 point2d(1, 2);
            float3 point3d(1, 2, 3);
            float4 from2d(point2d);
            float4 from3d(point3d);
            Assert::AreEqual(from2d, float4(1, 2, 0, 0));
            Assert::AreEqual(from3d, float4(1, 2, 3, 0));
        }
        TEST_METHOD(SpecialAssignmentTest)
        {
            float2 point2d(1, 2);
            float3 point3d(1, 2, 3);
            float4 from2d = point2d;
            float4 from3d = point3d;
            Assert::AreEqual(from2d, float4(1, 2, 0, 0));
            Assert::AreEqual(from3d, float4(1, 2, 3, 0));
        }
        TEST_METHOD(AccessorTest)
        {
            float4 point4d;
            point4d.x = 1.0f;
            point4d.y = 2.0f;
            point4d.z = 3.0f;
            point4d.w = 4.0f;
            Assert::AreEqual(point4d.r, 1.0f);
            Assert::AreEqual(point4d.g, 2.0f);
            Assert::AreEqual(point4d.b, 3.0f);
            Assert::AreEqual(point4d.a, 4.0f);
        }
        // Arithmetic Tests.
        TEST_METHOD(AdditionTest)
        {
            float4 a(1.0, 2.0, 3.0, 4.0);
            float4 b(5.0, 6.0, 7.0, 8.0);
            float4 c(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
            Assert::AreEqual(a + b, c);
        }
        TEST_METHOD(SubtractionTest)
        {
            float4 a(1.0, 2.0, 3.0, 4.0);
            float4 b(5.0, 6.0, 7.0, 8.0);
            float4 c(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
            Assert::AreEqual(a - b, c);
        }
        TEST_METHOD(MultiplicationTest)
        {
            float4 a(1.0, 2.0, 3.0, 4.0);
            float k = 2.0;
            float4 b(a.x * k, a.y * k, a.z * k, a.w * k);
            Assert::AreEqual(a * k, b);
        }
        TEST_METHOD(DivisionTest)
        {
            float4 a(1.0, 2.0, 3.0, 4.0);
            float k = 2.0;
            float4 b(a.x / k, a.y / k, a.z / k, a.w / k);
            Assert::AreEqual(a / k, b);
        }
        // Assignment operator tests.
        TEST_METHOD(AddAssignmentTest)
        {
            float4 a(1.0, 2.0, 3.0, 4.0);
            float4 b(5.0, 6.0, 7.0, 8.0);
            float4 c = a + b;
            a += b;
            Assert::AreEqual(a, c);
        }
        // Assignment operator tests.
        TEST_METHOD(SubAssignmentTest)
        {
            float4 a(1.0, 2.0, 3.0, 4.0);
            float4 b(5.0, 6.0, 7.0, 8.0);
            float4 c = a - b;
            a -= b;
            Assert::AreEqual(a, c);
        }
        // Assignment operator tests.
        TEST_METHOD(MulAssignmentTest)
        {
            float4 a(1.0, 2.0, 3.0, 4.0);
            float k = 2.0;
            float4 b = a * k;
            a *= k;
            Assert::AreEqual(a, b);
        }
        // Assignment operator tests.
        TEST_METHOD(DivAssignmentTest)
        {
            float4 a(1.0, 2.0, 3.0, 4.0);
            float k = 2.0;
            float4 b = a / k;
            a /= k;
            Assert::AreEqual(a, b);
        }
    };
}
