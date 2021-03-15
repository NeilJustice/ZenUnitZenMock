#include "pch.h"

namespace ZenUnit
{
   TESTS(DOUBLES_ARE_NEARTests)
   AFACT(DoublesDifferenceIsLessThan1EMinus16_DoesNotThrowException)
   AFACT(DoublesDifferenceIsGreaterThanOrEqualTo1EMinus16_ThrowsAnomaly)
   EVIDENCE

   TEST(DoublesDifferenceIsLessThan1EMinus16_DoesNotThrowException)
   {
      DOUBLES_ARE_NEAR(0.0, 0.0);

      const double d1 = 1e-16;
      const double d2 = 1.1e-16;
      DOUBLES_ARE_NEAR(d1, d2);

      const double d3 = -1e-16;
      const double d4 = -1.1e-16;
      DOUBLES_ARE_NEAR(d3, d4);
   }

   TEST(DoublesDifferenceIsGreaterThanOrEqualTo1EMinus16_ThrowsAnomaly)
   {
      const double d1 = 1e-16;
      const double d2 = 2e-16;
      THROWS_EXCEPTION(DOUBLES_ARE_NEAR(d1, d2), Anomaly, TestUtil::NewlineConcat("",
"  Failed: DOUBLES_ARE_NEAR(d1, d2)",
"Expected: 1e-16",
"  Actual: 2e-16",
"File.cpp(1)"));

      const double d3 = -1e-16;
      const double d4 = -2e-16;
      THROWS_EXCEPTION(DOUBLES_ARE_NEAR(d3, d4), Anomaly, TestUtil::NewlineConcat("",
"  Failed: DOUBLES_ARE_NEAR(d3, d4)",
"Expected: -1e-16",
"  Actual: -2e-16",
"File.cpp(1)"));
   }

   RUN_TESTS(DOUBLES_ARE_NEARTests)
}
