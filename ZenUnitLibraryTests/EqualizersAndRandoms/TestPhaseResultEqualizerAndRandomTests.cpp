#include "pch.h"

namespace ZenUnit
{
   TESTS(TestPhaseResultEqualizerAndRandomTests)
   AFACT(ZenUnitEqualizer_ThrowsIfAnyFieldNotEqual)
   AFACT(TestableRandomTestPhaseResult_ReturnsTestPhaseResultWithAllRandomFields)
   AFACT(RandomTestPhaseResult_DoesNotThrowException)
   EVIDENCE

   TEST(ZenUnitEqualizer_ThrowsIfAnyFieldNotEqual)
   {
      ZENUNIT_EQUALIZER_TEST_SETUP(TestPhaseResult);
      ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(TestPhaseResult, testPhase, ZenUnit::RandomNon0Enum<TestPhase>(TestPhase::MaxValue));
      ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(TestPhaseResult, testOutcome, TestOutcome::Anomaly);
      ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(TestPhaseResult, microseconds, ZenUnit::RandomNon0<long long>());
      ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(TestPhaseResult, anomalyOrException, make_shared<AnomalyOrException>(ZenUnit::Random<AnomalyOrException>()));
   }

   TEST(TestableRandomTestPhaseResult_ReturnsTestPhaseResultWithAllRandomFields)
   {
      ZenUnit::RandomGeneratorMock randomGeneratorMock;

      const int testPhaseInt = ZenUnit::Random<int>();
      const int testOutcomeInt = ZenUnit::Random<int>();
      randomGeneratorMock.EnumMock.ReturnValues(testPhaseInt, testOutcomeInt);

      const long long microseconds = randomGeneratorMock.LongLongMock.ReturnRandom();
      //
      const TestPhaseResult randomTestPhaseResult = TestableRandomTestPhaseResult(&randomGeneratorMock);
      //
      METALMOCK(randomGeneratorMock.EnumMock.CalledAsFollows(
      {
         static_cast<int>(TestPhase::MaxValue),
         static_cast<int>(TestOutcome::MaxValue)
      }));
      METALMOCK(randomGeneratorMock.LongLongMock.CalledOnce());
      TestPhaseResult expectedRandomTestPhaseResult;
      expectedRandomTestPhaseResult.testPhase = static_cast<TestPhase>(testPhaseInt);
      expectedRandomTestPhaseResult.testOutcome = static_cast<TestOutcome>(testOutcomeInt);
      expectedRandomTestPhaseResult.microseconds = microseconds;
      expectedRandomTestPhaseResult.anomalyOrException = nullptr;
      ARE_EQUAL(expectedRandomTestPhaseResult, randomTestPhaseResult);
   }

   TEST(RandomTestPhaseResult_DoesNotThrowException)
   {
      const TestPhaseResult randomTestPhaseResult = ZenUnit::Random<TestPhaseResult>();
   }

   RUN_TESTS(TestPhaseResultEqualizerAndRandomTests)
}