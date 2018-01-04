#include "pch.h"
#include "ZenUnitTests/Testing/UserType.h"

namespace ZenUnit
{
   TESTS(VectorEqualizerTests)
   AFACT(AssertEqual_IntVectors_CallsVECTORS_EQUAL)
   AFACT(AssertEqual_StringVectors_CallsVECTORS_EQUAL)
   AFACT(AssertEqual_UserTypeVectors_CallsVECTORS_EQUAL)
   EVIDENCE

   TEST(AssertEqual_IntVectors_CallsVECTORS_EQUAL)
   {
      vector<int> expectedIntVector;
      const vector<int> actualIntVector;
      Equalizer<vector<int>>::AssertEqual(expectedIntVector, actualIntVector);
      expectedIntVector.push_back(1);
      THROWS(Equalizer<vector<int>>::AssertEqual(expectedIntVector, actualIntVector), Anomaly, R"(
  Failed: VECTORS_EQUAL(expectedVector, actualVector)
Expected: vector<T>
  Actual: vector<T>
 Because: ARE_EQUAL(expectedVector.size(), actualVector.size()) failed
Expected: 1
  Actual: 0
File.cpp(1)
File.cpp(1))");
   }

   TEST(AssertEqual_StringVectors_CallsVECTORS_EQUAL)
   {
      vector<string> expectedStringVector;
      const vector<string> actualStringVector;
      Equalizer<vector<string>>::AssertEqual(expectedStringVector, actualStringVector);
      expectedStringVector.emplace_back();
      THROWS(Equalizer<vector<string>>::AssertEqual(
         expectedStringVector Comma actualStringVector), Anomaly, R"(
  Failed: VECTORS_EQUAL(expectedVector, actualVector)
Expected: vector<T>
  Actual: vector<T>
 Because: ARE_EQUAL(expectedVector.size(), actualVector.size()) failed
Expected: 1
  Actual: 0
File.cpp(1)
File.cpp(1))");
   }

   TEST(AssertEqual_UserTypeVectors_CallsVECTORS_EQUAL)
   {
      vector<UserType> expectedUserTypeVector;
      const vector<UserType> actualUserTypeVector;
      Equalizer<vector<UserType>>::AssertEqual(expectedUserTypeVector, actualUserTypeVector);
      expectedUserTypeVector.emplace_back();
      THROWS(Equalizer<vector<UserType>>::AssertEqual(
         expectedUserTypeVector Comma actualUserTypeVector), Anomaly, R"(
  Failed: VECTORS_EQUAL(expectedVector, actualVector)
Expected: vector<T>
  Actual: vector<T>
 Because: ARE_EQUAL(expectedVector.size(), actualVector.size()) failed
Expected: 1
  Actual: 0
File.cpp(1)
File.cpp(1))");
   }

   }; RUNTESTS(VectorEqualizerTests)
}
