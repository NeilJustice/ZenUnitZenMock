#include "pch.h"
#include "ZenUnit/TestRunners/MultiTestClassRunner.h"
#include "ZenUnit/TestRunners/NoOpTestClassRunner.h"
#include "ZenUnitTests/TestRunners/Mock/TestClassRunnerMock.h"
#include "ZenUnitTests/Utils/Iteration/Mock/ExtraArgAnyerMock.h"
#include "ZenUnitTests/Utils/Iteration/Mock/ExtraArgMemberForEacherMock.h"
#include "ZenUnitTests/Utils/Iteration/Mock/TransformerMock.h"
#include "ZenUnitTests/Utils/Mock/SorterMock.h"
#include "ZenUnitTests/Utils/Time/Mock/WatchMock.h"

namespace ZenUnit
{
   TESTS(MultiTestClassRunnerTests)
   AFACT(Constructor_NewsComponents)
   AFACT(NumberOfTestCases_ReturnsSumOfAllTestClassNumberOfTests)
   AFACT(AddTestClassRunner_EmplacesBackTestClassRunner_MakesNumberOfTestClassesToBeRunReturnAnIncreasingNumber)
   AFACT(ApplyRunFiltersIfAny_RunFiltersEmpty_DoesNothing)
   AFACT(ApplyRunFiltersIfAny_RunFiltersNotEmpty_ResetsWithNoOpTestClassesThoseTestClassesThatMatchRunFilters)
   AFACT(ResetTestClassRunnerWithNoOpIfNameDoesNotMatchRunFilter_TestClassNameMatchesAtLeastOneRunFilter_DoesNotResetTestClassRunnerWithNoOp)
   AFACT(ResetTestClassRunnerWithNoOpIfNameDoesNotMatchRunFilter_TestClassNameDoesNotMatchAnyRunFilter_ResetsTestClassRunnerWithNoOp)
   FACTS(TestClassMatchesRunFilter_ReturnsTrueIfTestClassNameCaseInsensitiveEqualsRunFilter)
   AFACT(RunTestClasses_NonRandomMode_SortsTestClassRunnersByName_RunsTestClassesSequentially_ReturnsTestClassResults)
   FACTS(RunTestClasses_RandomMode_SetsRandomSeedIfNotSetByUser_RunsTestClassesRandomly_ReturnsTestClassResults)
   AFACT(RunTestClassRunner_ReturnsCallToTestClassRunnerRunTests)
   EVIDENCE

   MultiTestClassRunner _multiTestClassRunner;

   using ExtraArgMemberForEacherMockType = ExtraArgMemberForEacherMock<
      std::unique_ptr<TestClassRunner>,
      MultiTestClassRunner,
      void(MultiTestClassRunner::*)(std::unique_ptr<TestClassRunner>&, const std::vector<RunFilter>&),
      const std::vector<RunFilter>&>;
   ExtraArgMemberForEacherMockType* _extraArgMemberForEacherMock;

   using ExtraArgAnyerMockType = ExtraArgAnyerMock<
      std::vector<RunFilter>,
      bool(*)(const RunFilter&, const std::unique_ptr<TestClassRunner>*),
      const std::unique_ptr<TestClassRunner>*>;
   ExtraArgAnyerMockType* _extraArgAnyerMock;

   SorterMock<vector<unique_ptr<TestClassRunner>>>* _sorterMock;
   using TransformerMockType = TransformerMock<unique_ptr<TestClassRunner>, TestClassResult>;
   TransformerMockType* _transformerMock;
   WatchMock* _watchMock;

   STARTUP
   {
      _multiTestClassRunner._extraArgMemberForEacher.reset(_extraArgMemberForEacherMock = new ExtraArgMemberForEacherMockType);
      _multiTestClassRunner._extraArgAnyer.reset(_extraArgAnyerMock = new ExtraArgAnyerMockType);
      _multiTestClassRunner._sorter.reset(_sorterMock = new SorterMock<vector<unique_ptr<TestClassRunner>>>);
      _multiTestClassRunner._transformer.reset(_transformerMock = new TransformerMockType);
      _multiTestClassRunner._watch.reset(_watchMock = new WatchMock);
   }

   TEST(Constructor_NewsComponents)
   {
      POINTER_WAS_NEWED(_multiTestClassRunner._extraArgMemberForEacher);
      POINTER_WAS_NEWED(_multiTestClassRunner._extraArgAnyer);
      POINTER_WAS_NEWED(_multiTestClassRunner._sorter);
      POINTER_WAS_NEWED(_multiTestClassRunner._transformer);
      POINTER_WAS_NEWED(_multiTestClassRunner._watch);
      IS_EMPTY(_multiTestClassRunner._testClassRunners);
   }

   TEST(AddTestClassRunner_EmplacesBackTestClassRunner_MakesNumberOfTestClassesToBeRunReturnAnIncreasingNumber)
   {
      ARE_EQUAL(0, _multiTestClassRunner.NumberOfTestClassesToBeRun());
      TestClassRunnerMock* testClassRunnerA = new TestClassRunnerMock;
      //
      _multiTestClassRunner.AddTestClassRunner(testClassRunnerA);
      //
      ARE_EQUAL(1, _multiTestClassRunner._testClassRunners.size());
      ARE_EQUAL(testClassRunnerA, _multiTestClassRunner._testClassRunners[0].get());
      testClassRunnerA->TestClassNameMock.ExpectAndReturn("WidgetATests");
      ARE_EQUAL(1, _multiTestClassRunner.NumberOfTestClassesToBeRun());
      ZEN(testClassRunnerA->TestClassNameMock.AssertCalledOnce());


      NoOpTestClassRunner* noOpTestClassRunner = new NoOpTestClassRunner;
      //
      _multiTestClassRunner.AddTestClassRunner(noOpTestClassRunner);
      //
      ARE_EQUAL(2, _multiTestClassRunner._testClassRunners.size());
      ARE_EQUAL(testClassRunnerA, _multiTestClassRunner._testClassRunners[0].get());
      ARE_EQUAL(noOpTestClassRunner, _multiTestClassRunner._testClassRunners[1].get());
      ARE_EQUAL(1, _multiTestClassRunner.NumberOfTestClassesToBeRun());


      TestClassRunnerMock* testClassRunnerB = new TestClassRunnerMock;
      //
      _multiTestClassRunner.AddTestClassRunner(testClassRunnerB);
      //
      ARE_EQUAL(3, _multiTestClassRunner._testClassRunners.size());
      ARE_EQUAL(testClassRunnerA, _multiTestClassRunner._testClassRunners[0].get());
      ARE_EQUAL(noOpTestClassRunner, _multiTestClassRunner._testClassRunners[1].get());
      ARE_EQUAL(testClassRunnerB, _multiTestClassRunner._testClassRunners[2].get());
      testClassRunnerB->TestClassNameMock.ExpectAndReturn("WidgetBTests");
      ARE_EQUAL(2, _multiTestClassRunner.NumberOfTestClassesToBeRun());
      ZEN(testClassRunnerB->TestClassNameMock.AssertCalledOnce());
   }

   TEST(ApplyRunFiltersIfAny_RunFiltersEmpty_DoesNothing)
   {
      _multiTestClassRunner.ApplyRunFiltersIfAny({});
   }

   TEST(ApplyRunFiltersIfAny_RunFiltersNotEmpty_ResetsWithNoOpTestClassesThoseTestClassesThatMatchRunFilters)
   {
      _extraArgMemberForEacherMock->ExtraArgMemberForEachMock.Expect();
      const vector<RunFilter> runFilters = { ZenUnit::Random<RunFilter>() };
      //
      _multiTestClassRunner.ApplyRunFiltersIfAny(runFilters);
      //
      ZEN(_extraArgMemberForEacherMock->ExtraArgMemberForEachMock.AssertCalledOnceWith(
         &_multiTestClassRunner._testClassRunners,
         &_multiTestClassRunner,
         &MultiTestClassRunner::ResetTestClassRunnerWithNoOpIfNameDoesNotMatchRunFilter,
         runFilters));
   }

   TEST(ResetTestClassRunnerWithNoOpIfNameDoesNotMatchRunFilter_TestClassNameMatchesAtLeastOneRunFilter_DoesNotResetTestClassRunnerWithNoOp)
   {
      _extraArgAnyerMock->ExtraArgAnyMock.ExpectAndReturn(true);
      unique_ptr<TestClassRunner> testClassRunner{};
      const vector<RunFilter> runFilters = { ZenUnit::Random<RunFilter>() };
      //
      _multiTestClassRunner.ResetTestClassRunnerWithNoOpIfNameDoesNotMatchRunFilter(testClassRunner, runFilters);
      //
      ZEN(_extraArgAnyerMock->ExtraArgAnyMock.AssertCalledOnceWith(
         runFilters, MultiTestClassRunner::TestClassNameMatchesRunFilter, &testClassRunner));
      IS_TRUE(dynamic_cast<NoOpTestClassRunner*>(testClassRunner.get()) == nullptr);
   }

   TEST(ResetTestClassRunnerWithNoOpIfNameDoesNotMatchRunFilter_TestClassNameDoesNotMatchAnyRunFilter_ResetsTestClassRunnerWithNoOp)
   {
      _extraArgAnyerMock->ExtraArgAnyMock.ExpectAndReturn(false);
      unique_ptr<TestClassRunner> testClassRunner{};
      const vector<RunFilter> runFilters = { ZenUnit::Random<RunFilter>() };
      //
      _multiTestClassRunner.ResetTestClassRunnerWithNoOpIfNameDoesNotMatchRunFilter(testClassRunner, runFilters);
      //
      ZEN(_extraArgAnyerMock->ExtraArgAnyMock.AssertCalledOnceWith(
         runFilters, MultiTestClassRunner::TestClassNameMatchesRunFilter, &testClassRunner));
      IS_TRUE(dynamic_cast<NoOpTestClassRunner*>(testClassRunner.get()) != nullptr);
   }

   TEST(NumberOfTestCases_ReturnsSumOfAllTestClassNumberOfTests)
   {
      TestClassRunnerMock* testClassRunnerAMock = new TestClassRunnerMock;
      TestClassRunnerMock* testClassRunnerBMock = new TestClassRunnerMock;
      TestClassRunnerMock* testClassRunnerCMock = new TestClassRunnerMock;
      testClassRunnerAMock->NumberOfTestCasesMock.ExpectAndReturn(10);
      testClassRunnerBMock->NumberOfTestCasesMock.ExpectAndReturn(0);
      testClassRunnerCMock->NumberOfTestCasesMock.ExpectAndReturn(20);
      _multiTestClassRunner.AddTestClassRunner(testClassRunnerAMock);
      _multiTestClassRunner.AddTestClassRunner(testClassRunnerBMock);
      _multiTestClassRunner.AddTestClassRunner(testClassRunnerCMock);
      //
      const size_t totalNumberOfTestCases = _multiTestClassRunner.NumberOfTestCases();
      //
      ZEN(testClassRunnerAMock->NumberOfTestCasesMock.AssertCalledOnce());
      ZEN(testClassRunnerBMock->NumberOfTestCasesMock.AssertCalledOnce());
      ZEN(testClassRunnerCMock->NumberOfTestCasesMock.AssertCalledOnce());
      ARE_EQUAL(30, totalNumberOfTestCases);
   }

   TEST3X3(TestClassMatchesRunFilter_ReturnsTrueIfTestClassNameCaseInsensitiveEqualsRunFilter,
      const string& testClassNameRunFilter, const char* testClassName, bool expectedReturnValue,
      "", "", true,
      "", "WidgetTests", false,
      "WidgetTests", "WidgetTests", true,
      "widgettests", "WidgetTests", true,
      "WidgetTests", "widgettests", true,
      "Widget", "WidgetTests", false,
      "WidgetTests", "WidgetTestsABC", false,
      "WidgetTests", "ABCWidgetTests", false)
   {
      TestClassRunnerMock* const testClassRunnerMock = new TestClassRunnerMock;
      testClassRunnerMock->TestClassNameMock.ExpectAndReturn(testClassName);
      const std::unique_ptr<TestClassRunner> testClassRunner(testClassRunnerMock);

      RunFilter runFilter;
      runFilter.testClassName = testClassNameRunFilter;
      //
      const bool testClassMatchesRunFilter = MultiTestClassRunner::TestClassNameMatchesRunFilter(runFilter, &testClassRunner);
      //
      ZEN(testClassRunnerMock->TestClassNameMock.AssertCalledOnce());
      ARE_EQUAL(expectedReturnValue, testClassMatchesRunFilter);
   }

   TEST(RunTestClasses_NonRandomMode_SortsTestClassRunnersByName_RunsTestClassesSequentially_ReturnsTestClassResults)
   {
      const size_t testClassRunnersSize = ZenUnit::Random<size_t>(0, 2);
      _multiTestClassRunner._testClassRunners.resize(testClassRunnersSize);

      ZenUnitArgs zenUnitArgs;
      IS_FALSE(zenUnitArgs.random);

      _sorterMock->SortMock.Expect();
      _transformerMock->TransformMock.Expect();
      //
      const vector<TestClassResult> testClassResults = _multiTestClassRunner.RunTestClasses(zenUnitArgs);
      //
      ZEN(_sorterMock->SortMock.AssertCalledOnceWith(&_multiTestClassRunner._testClassRunners));

      std::vector<TestClassResult> expectedTestClassResults(testClassRunnersSize);
      ZEN(_transformerMock->TransformMock.AssertCalledOnceWith(
         &_multiTestClassRunner._testClassRunners,
         expectedTestClassResults,
         &MultiTestClassRunner::RunTestClassRunner));

      const ZenUnitArgs expectedResultingZenUnitArgs;
      ARE_EQUAL(expectedResultingZenUnitArgs, zenUnitArgs);

      VECTORS_EQUAL(expectedTestClassResults, testClassResults);
   }

   TEST2X2(RunTestClasses_RandomMode_SetsRandomSeedIfNotSetByUser_RunsTestClassesRandomly_ReturnsTestClassResults,
      bool randomseedsetbyuser, bool expectRandomSeedSet,
      false, false,
      true, true)
   {
      const size_t testClassRunnersSize = ZenUnit::Random<size_t>(0, 2);
      _multiTestClassRunner._testClassRunners.resize(testClassRunnersSize);

      _transformerMock->RandomTransformMock.Expect();

      ZenUnitArgs zenUnitArgs;
      zenUnitArgs.random = true;
      zenUnitArgs.randomseedsetbyuser = randomseedsetbyuser;
      unsigned short therandomseedsetbyuser = 0;
      unsigned short randomseedsetbycode = 0;
      if (randomseedsetbyuser)
      {
         zenUnitArgs.randomseed = therandomseedsetbyuser = ZenUnit::Random<unsigned short>();
      }
      else
      {
         randomseedsetbycode = ZenUnit::Random<unsigned short>();
         _watchMock->SecondsSince1970CastToUnsignedShortMock.ExpectAndReturn(randomseedsetbycode);
      }
      //
      const vector<TestClassResult> testClassResults = _multiTestClassRunner.RunTestClasses(zenUnitArgs);
      //
      if (!randomseedsetbyuser)
      {
         _watchMock->SecondsSince1970CastToUnsignedShortMock.AssertCalledOnce();
      }

      std::vector<TestClassResult> expectedTestClassResults(testClassRunnersSize);
      ZEN(_transformerMock->RandomTransformMock.AssertCalledOnceWith(
         &_multiTestClassRunner._testClassRunners,
         expectedTestClassResults,
         &MultiTestClassRunner::RunTestClassRunner,
         zenUnitArgs.randomseed));

      ZenUnitArgs expectedResultingZenUnitArgs;
      expectedResultingZenUnitArgs.random = true;
      expectedResultingZenUnitArgs.randomseedsetbyuser = randomseedsetbyuser;
      expectedResultingZenUnitArgs.randomseed = randomseedsetbyuser ? therandomseedsetbyuser : randomseedsetbycode;
      ARE_EQUAL(expectedResultingZenUnitArgs, zenUnitArgs);

      VECTORS_EQUAL(expectedTestClassResults, testClassResults);
   }

   TEST(RunTestClassRunner_ReturnsCallToTestClassRunnerRunTests)
   {
      TestClassRunnerMock* testClassRunnerMock = new TestClassRunnerMock;
      testClassRunnerMock->RunTestsMock.ExpectAndReturn(TestClassResult::TestingNonDefault());
      const unique_ptr<TestClassRunner> testClassRunnerReallyAMock(testClassRunnerMock);
      //
      const TestClassResult testClassResult = MultiTestClassRunner::RunTestClassRunner(testClassRunnerReallyAMock);
      //
      ZEN(testClassRunnerMock->RunTestsMock.AssertCalledOnce());
      ARE_EQUAL(TestClassResult::TestingNonDefault(), testClassResult);
   }

   }; RUNTESTS(MultiTestClassRunnerTests)
}
