#include "pch.h"

namespace Namespace
{
   struct TestStruct
   {
      int fieldA;
      int fieldB;
      TestStruct()
         : fieldA(0)
         , fieldB(0)
      {
      }
   };
}

namespace ZenUnit
{
   template<>
   struct Equalizer<Namespace::TestStruct>
   {
      static void AssertEqual(const Namespace::TestStruct& expected, const Namespace::TestStruct& actual)
      {
         ARE_EQUAL(expected.fieldA, actual.fieldA);
         // Absence of line ARE_EQUAL(expected.fieldB, actual.fieldB);
      }
   };
}

struct TestStructB
{
   int field;
   TestStructB() : field(0) {}
   static const int ThrowWithoutFieldNameInWhatText;
   static const int ThrowWithFieldNameInWhatText;
};

const int TestStructB::ThrowWithoutFieldNameInWhatText = 1;
const int TestStructB::ThrowWithFieldNameInWhatText = 2;

namespace ZenUnit
{
   template<>
   struct Equalizer<TestStructB>
   {
      static void AssertEqual(const TestStructB& expected, const TestStructB&)
      {
         if (expected.field == TestStructB::ThrowWithoutFieldNameInWhatText)
         {
            throw Anomaly();
         }
         if (expected.field == TestStructB::ThrowWithFieldNameInWhatText)
         {
            Anomaly anomaly;
            anomaly.expected = "field";
            throw anomaly;
         }
      }
   };

   TESTS(EQUALIZER_THROWSTests)
   AFACT(EqualizerDoesNotAssertSpecifiedFieldEqual_Throws)
   AFACT(EqualizerThrowsAnomalyThatDoesNotContainFieldName_Throws)
   AFACT(EqualizerThrowsAnomalyThatContainsFieldName_ButStillThrowsThatAnomalyWhenAllFieldsEqual_Throws)
   AFACT(EqualizerAssertsSpecifiedFieldEqual_DoesNotThrow)
   EVIDENCE

   TEST(EqualizerDoesNotAssertSpecifiedFieldEqual_Throws)
   {
      SETUP_EQUALIZER_THROWS_TEST(Namespace::TestStruct);
      THROWS(EQUALIZER_THROWS_FOR_FIELD(Namespace::TestStruct, fieldB, 1), Anomaly, R"(
  Failed: EQUALIZER_THROWS_FOR_FIELD(Namespace::TestStruct, fieldB, 1)
Expected: Function ZenUnit::Equalizer<Namespace::TestStruct>::AssertEqual(expected, actual)
          to throw a ZenUnit::Anomaly from an
          ARE_EQUAL(expected.fieldB, actual.fieldB) assert statement.
  Actual: No ZenUnit::Anomaly thrown despite field 'fieldB'
          differing between objects expected and actual.
File.cpp(1))");
   }

   TEST(EqualizerThrowsAnomalyThatDoesNotContainFieldName_Throws)
   {
      SETUP_EQUALIZER_THROWS_TEST(TestStructB);
      THROWS(EQUALIZER_THROWS_FOR_FIELD(TestStructB, field, TestStructB::ThrowWithoutFieldNameInWhatText), Anomaly, R"(
  Failed: EQUALIZER_THROWS_FOR_FIELD(TestStructB, field, TestStructB::ThrowWithoutFieldNameInWhatText)
Expected: N/A
  Actual: N/A
 Because: IS_TRUE(String::Contains(anomalyWhat, fieldName)) failed
Expected: true
  Actual: false
File.cpp(1)
File.cpp(1))");
   }

   TEST(EqualizerThrowsAnomalyThatContainsFieldName_ButStillThrowsThatAnomalyWhenAllFieldsEqual_Throws)
   {
      SETUP_EQUALIZER_THROWS_TEST(TestStructB);
      THROWS(EQUALIZER_THROWS_FOR_FIELD(TestStructB, field, TestStructB::ThrowWithFieldNameInWhatText), Anomaly, R"(
  Failed: EQUALIZER_THROWS_FOR_FIELD(TestStructB, field, TestStructB::ThrowWithFieldNameInWhatText)
Expected: N/A
  Actual: N/A
 Because: ARE_EQUAL(equalizerTestObjectA, equalizerTestObjectB) failed
Expected: <TestStructB>
  Actual: <TestStructB>
File.cpp(1)
File.cpp(1))");
   }

   TEST(EqualizerAssertsSpecifiedFieldEqual_DoesNotThrow)
   {
      SETUP_EQUALIZER_THROWS_TEST(Namespace::TestStruct);
      EQUALIZER_THROWS_FOR_FIELD(Namespace::TestStruct, fieldA, 1);
   }

   RUN_TESTS(EQUALIZER_THROWSTests)
}