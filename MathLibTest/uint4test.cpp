#include "stdafx.h"
#include "CppUnitTest.h"

#include "../MathLib/Math.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Djn;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    static std::wstring ToString<uint4>(const uint4& d2)
    {
        return L"(" +
            std::to_wstring(d2.x) + L", " + std::to_wstring(d2.y) +
            L")";
    }
}

namespace Djn::MathLibTest
{

    TEST_CLASS(uint4test)
    {
    public:
        TEST_METHOD(EqualityTest)
        {
            uint4 a(1, 2, 3, 4);
            uint4 b(1, 2, 3, 4);
            Assert::IsTrue(a == b);
            Assert::AreEqual(a, b);
        }
        TEST_METHOD(BasicCtorTest)
        {
            uint4 a;
            uint4 b(0, 0);
            uint4 c(0, 0, 0);
            uint4 d(0, 0, 0, 0);
            Assert::AreEqual(a, b);
            Assert::AreEqual(b, c);
            Assert::AreEqual(c, d);
        }
        TEST_METHOD(SpecialCopyCtorTest)
        {
            uint2 point2d(1, 2);
            uint3 point3d(1, 2, 3);
            uint4 from2d(point2d);
            uint4 from3d(point3d);
            Assert::AreEqual(from2d, uint4(1, 2, 0, 0));
            Assert::AreEqual(from3d, uint4(1, 2, 3, 0));
        }
        TEST_METHOD(SpecialAssignmentTest)
        {
            uint2 point2d(1, 2);
            uint3 point3d(1, 2, 3);
            uint4 from2d = point2d;
            uint4 from3d = point3d;
            Assert::AreEqual(from2d, uint4(1, 2, 0, 0));
            Assert::AreEqual(from3d, uint4(1, 2, 3, 0));
        }
        TEST_METHOD(AccessorTest)
        {
            uint4 point4d;
            point4d.x = 1;
            point4d.y = 2;
            point4d.z = 3;
            point4d.w = 4;
            Assert::AreEqual(point4d.r, 1u);
            Assert::AreEqual(point4d.g, 2u);
            Assert::AreEqual(point4d.b, 3u);
            Assert::AreEqual(point4d.a, 4u);
        }
        // Arithmetic Tests.
        TEST_METHOD(AdditionTest)
        {
            uint4 a(1, 2, 3, 4);
            uint4 b(5, 6, 7, 8);
            uint4 c(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
            Assert::AreEqual(a + b, c);
        }
        TEST_METHOD(SubtractionTest)
        {
            uint4 a(1, 2, 3, 4);
            uint4 b(5, 6, 7, 8);
            uint4 c(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
            Assert::AreEqual(a - b, c);
        }
        TEST_METHOD(MultiplicationTest)
        {
            uint4 a(1, 2, 3, 4);
            unsigned int k = 2;
            uint4 b(a.x * k, a.y * k, a.z * k, a.w * k);
            Assert::AreEqual(a * k, b);
        }
        TEST_METHOD(DivisionTest)
        {
            uint4 a(1, 2, 3, 4);
            unsigned int k = 2;
            uint4 b(a.x / k, a.y / k, a.z / k, a.w / k);
            Assert::AreEqual(a / k, b);
        }
        // Assignment operator tests.
        TEST_METHOD(AddAssignmentTest)
        {
            uint4 a(1, 2, 3, 4);
            uint4 b(5, 6, 7, 8);
            uint4 c = a + b;
            a += b;
            Assert::AreEqual(a, c);
        }
        // Assignment operator tests.
        TEST_METHOD(SubAssignmentTest)
        {
            uint4 a(1, 2, 3, 4);
            uint4 b(5, 6, 7, 8);
            uint4 c = a - b;
            a -= b;
            Assert::AreEqual(a, c);
        }
        // Assignment operator tests.
        TEST_METHOD(MulAssignmentTest)
        {
            uint4 a(1, 2, 3, 4);
            unsigned int k = 2;
            uint4 b = a * k;
            a *= k;
            Assert::AreEqual(a, b);
        }
        // Assignment operator tests.
        TEST_METHOD(DivAssignmentTest)
        {
            uint4 a(1, 2, 3, 4);
            unsigned int k = 2;
            uint4 b = a / k;
            a /= k;
            Assert::AreEqual(a, b);
        }
    };
}