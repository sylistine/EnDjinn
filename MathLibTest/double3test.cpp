#include "stdafx.h"
#include "CppUnitTest.h"

#include "../MathLib/Math.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Djn;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    static std::wstring ToString<double3>(const double3& d2)
    {
        return L"(" +
            std::to_wstring(d2.x) + L", " + std::to_wstring(d2.y) +
            L")";
    }
}

namespace Djn::MathLibTest
{

    TEST_CLASS(double3test)
    {
    public:
        TEST_METHOD(EqualityTest)
        {
            double3 a(1.2, 3.4, 5.6);
            double3 b(1.2, 3.4, 5.6);
            Assert::IsTrue(a == b);
            Assert::AreEqual(a, b);
        }
        TEST_METHOD(BasicCTorTest)
        {
            double3 a;
            double3 b(0, 0);
            double3 c(0, 0, 0);
            Assert::AreEqual(a, b);
            Assert::AreEqual(b, c);
        }
        TEST_METHOD(SpecialCopyCtorTest)
        {
            double2 point2d(1, 2);
            double3 from2d(point2d);
            Assert::AreEqual(from2d, double3(1, 2, 0));
        }
        TEST_METHOD(SpecialAssignmentTest)
        {
            double2 point2d(1, 2);
            double3 from2d = point2d;
            Assert::AreEqual(from2d, double3(1, 2, 0));
        }
        TEST_METHOD(AccessorTest)
        {
            double3 point3d;
            point3d.x = 1.0;
            point3d.y = 2.0;
            point3d.z = 3.0;
            Assert::AreEqual(point3d.r, 1.0);
            Assert::AreEqual(point3d.g, 2.0);
            Assert::AreEqual(point3d.b, 3.0);
        }
        // Arithmetic Tests.
        TEST_METHOD(AdditionTest)
        {
            double3 a(1.0, 2.0, 3.0);
            double3 b(4.0, 5.0, 6.0);
            Assert::AreEqual(a + b, double3(5.0, 7.0, 9.0));
        }
        TEST_METHOD(SubtractionTest)
        {
            double3 a(1.0, 2.0, 3.0);
            double3 b(4.0, 5.0, 6.0);
            double3 c(1.0 - 4.0, 2.0 - 5.0, 3.0 - 6.0);
            Assert::AreEqual(a - b, c);
        }
        TEST_METHOD(MultiplicationTest)
        {
            double3 a(1.0, 2.0, 3.0);
            double k(2.0);
            Assert::AreEqual(a * k, double3(2.0, 4.0, 6.0));
        }
        TEST_METHOD(DivisionTest)
        {
            double3 a(1.0, 2.0, 3.0);
            double k = 2.0;
            Assert::AreEqual(a / k, double3(0.5, 1.0, 1.5));
        }
        // Assignment operator tests.
        TEST_METHOD(AddAssignmentTest)
        {
            double3 a(1.0, 2.0, 3.0);
            double3 b(4.0, 5.0, 6.0);
            a += b;
            Assert::AreEqual(a, double3(5.0, 7.0, 9.0));
        }
        // Assignment operator tests.
        TEST_METHOD(SubAssignmentTest)
        {
            double3 a(1.0, 2.0, 3.0);
            double3 b(4.0, 5.0, 6.0);
            a -= b;
            Assert::AreEqual(a, double3(-3.0, -3.0, -3.0));
        }
        // Assignment operator tests.
        TEST_METHOD(MulAssignmentTest)
        {
            double3 a(1.0, 2.0, 3.0);
            double k(2.0);
            a *= k;
            Assert::AreEqual(a, double3(2.0, 4.0, 6.0));
        }
        // Assignment operator tests.
        TEST_METHOD(DivAssignmentTest)
        {
            double3 a(1.0, 2.0, 3.0);
            double k(2.0);
            a /= k;
            Assert::AreEqual(a, double3(0.5, 1.0, 1.5));
        }
    };
}