#include "stdafx.h"
#include "CppUnitTest.h"

#include "../MathLib/Math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Djn;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    static std::wstring ToString<double2>(const double2& d2)
    {
        return L"(" +
            std::to_wstring(d2.x) + L", " +
            std::to_wstring(d2.y) + L")";
    }
}

namespace Djn::MathLibTest
{
    TEST_CLASS(double2test)
    {
    public:
        TEST_METHOD(AccessorTest)
        {
            double2 point2d;
            point2d.x = 1.0;
            point2d.y = 2.0;
            Assert::AreEqual(point2d.r, 1.0);
            Assert::AreEqual(point2d.g, 2.0);
        }
        TEST_METHOD(EqualityTest)
        {
            double2 a(12.34, 56.78);
            double2 b(12.34, 56.78);
            Assert::IsTrue(a == b);
            Assert::AreEqual(a, b);
        }
        // Arithmetic Tests.
        TEST_METHOD(AdditionTest)
        {
            double2 a(1.0, 2.0);
            double2 b(3.0, 4.0);
            Assert::AreEqual(a + b, double2(4.0, 6.0));
        }
        TEST_METHOD(SubtractionTest)
        {
            double2 a(1.0, 2.0);
            double2 b(3.0, 4.0);
            Assert::AreEqual(a - b, double2(1.0 - 3.0, 2.0 - 4.0));
        }
        TEST_METHOD(MultiplicationTest)
        {
            double2 a(1.0, 2.0);
            double k(2.0);
            Assert::AreEqual(a * k, double2(2.0, 4.0));
        }
        TEST_METHOD(DivisionTest)
        {
            double2 a(1.0, 2.0);
            double k = 2.0;
            Assert::AreEqual(a / k, double2(0.5, 1.0));
        }
        // Assignment operator tests.
        TEST_METHOD(AddAssignmentTest)
        {
            double2 a(1.0, 2.0);
            double2 b(3.0, 4.0);
            a += b;
            Assert::AreEqual(a, double2(4.0, 6.0));
        }
        // Assignment operator tests.
        TEST_METHOD(SubAssignmentTest)
        {
            double2 a(1.0, 2.0);
            double2 b(3.0, 4.0);
            a -= b;
            Assert::AreEqual(a, double2(-2.0, -2.0));
        }
        // Assignment operator tests.
        TEST_METHOD(MulAssignmentTest)
        {
            double2 a(1.0, 2.0);
            double k(2.0);
            a *= k;
            Assert::AreEqual(a, double2(2.0, 4.0));
        }
        // Assignment operator tests.
        TEST_METHOD(DivAssignmentTest)
        {
            double2 a(1.0, 2.0);
            double k(2.0);
            a /= k;
            Assert::AreEqual(a, double2(0.5, 1.0));
        }
    };
}
