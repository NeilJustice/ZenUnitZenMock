#include "pch.h"
#include "ZenUnitTests/Console/Mock/ConsoleMock.h"
#include "ZenUnitTests/Results/Mock/TestClassResultMock.h"
#include "ZenUnitTests/Testing/RandomPrintMode.h"
#include "ZenUnitTests/Tests/Mock/TestMock.h"
#include "ZenUnitTests/Utils/Iteration/Mock/ExtraArgMemberForEacherMock.h"

namespace ZenUnit
{
   TESTS(SpecificTestClassRunnerTests)
   AFACT(Constructor_NewsComponents_SetsTestClassName_SetsTestsVectorFromCallToTestClassTypeGetTests)
   AFACT(TestClassName_ReturnsTestClassName)
   AFACT(HasTestNameThatCaseInsensitiveMatchesPattern_ZeroTests_ReturnsFalse)
   AFACT(HasTestNameThatCaseInsensitiveMatchesPattern_OneNonMatchingTest_ReturnsFalse)
   FACTS(HasTestNameThatCaseInsensitiveMatchesPattern_OneMatchingTest_ReturnsTrue)
   AFACT(HasTestNameThatCaseInsensitiveMatchesPattern_FourTestsWhereThirdOneMatches_DoesNotCallFourthTestName_ReturnsTrue)
   AFACT(NumberOfTestCases_ReturnsSumOfNumberOfTestCases)
   FACTS(RunTests_PrintsTestClassNameAndNumberOfNamedTests_CallsDoRunTests_PrintsTestClassResultLine_MoveReturnsTestClassResult)
   FACTS(DoRunTests_RandomlyRunsTestsIfRandomOtherwiseSequentiallyRunsTests)
   FACTS(NonMinimalPrintTestClassNameAndNumberOfNamedTests_WritesTestClassNameVerticalBarNumberOfTests)
   FACTS(ConfirmTestClassIsNewableAndDeletableAndRegisterNXNTests_RunsNewDeleteTest_AddsResultToResults_ReturnsTrueIfNewableAndDeletable)
   AFACT(RunTest_NonMinimalWritesVerticalBarTestName_RunsTest_AddsTestResultsToTestClassResult_NonMinimalWriteTestOutcome)
   AFACT(NonMinimalPrintResultLine_CallsTestClassResultPrintResultLine)
   EVIDENCE

   class TestingTestClass
   {
   public:
      static vector<unique_ptr<Test>> GetTests(const char*)
      {
         vector<unique_ptr<Test>> tests;
         // emplace one Test pointer to make tests vector non-default sized
         tests.emplace_back(nullptr);
         return tests;
      }
   };

   unique_ptr<SpecificTestClassRunner<TestingTestClass>> _specificTestClassRunner;
   ConsoleMock* _consoleMock = nullptr;
   ZENMOCK_NONVOID0_STATIC(const ZenUnitArgs&, ZenUnit::TestRunner, GetArgs)
   const char* const TestClassName = "TestClassName";

   using ExtraArgMemberForEacherMockType = ExtraArgMemberForEacherMock<
      unique_ptr<Test>, SpecificTestClassRunner<TestingTestClass>,
      void (SpecificTestClassRunner<TestingTestClass>::*)(
         const unique_ptr<Test>& test, TestClassResult*) const, TestClassResult*>;
   const ExtraArgMemberForEacherMockType* _extraArgMemberForEacherMock;

   class SpecificTestClassRunnerSelfMocked : public Zen::Mock<ZenUnit::SpecificTestClassRunner<TestingTestClass>>
   {
   public:
      ZENMOCK_VOID0_CONST(NonMinimalPrintTestClassNameAndNumberOfNamedTests)
      ZENMOCK_NONVOID2_CONST(bool, ConfirmTestClassIsNewableAndDeletableAndRegisterNXNTests, Test*, TestClassResult*)
      ZENMOCK_VOID2_CONST(NonMinimalPrintResultLine, const TestClassResult*, PrintMode)
      ZENMOCK_NONVOID0_STATIC(const ZenUnitArgs&, ZenUnit::TestRunner, GetArgs)
      ZENMOCK_VOID0(DoRunTests)

      const ConsoleMock* consoleMock;

      SpecificTestClassRunnerSelfMocked()
         : Zen::Mock<ZenUnit::SpecificTestClassRunner<TestingTestClass>>("")
      {
         _console.reset(consoleMock = new ConsoleMock);
         call_TestRunner_GetArgs = ZENMOCK_BIND0(GetArgs_ZenMock);
      }
   };
   unique_ptr<SpecificTestClassRunnerSelfMocked> _specificTestClassRunnerSelfMocked;

   STARTUP
   {
      _specificTestClassRunner = make_unique<SpecificTestClassRunner<TestingTestClass>>(TestClassName);
      _specificTestClassRunner->_console.reset(_consoleMock = new ConsoleMock);
      _specificTestClassRunner->call_TestRunner_GetArgs = ZENMOCK_BIND0(GetArgs_ZenMock);
      _specificTestClassRunner->_extraArgMemberForEacher.reset(_extraArgMemberForEacherMock = new ExtraArgMemberForEacherMockType);
      _specificTestClassRunnerSelfMocked = make_unique<SpecificTestClassRunnerSelfMocked>();
   }

   TEST(Constructor_NewsComponents_SetsTestClassName_SetsTestsVectorFromCallToTestClassTypeGetTests)
   {
      SpecificTestClassRunner<TestingTestClass> specificTestClassRunner(TestClassName);
      //
      POINTER_WAS_NEWED(specificTestClassRunner._console);
      POINTER_WAS_NEWED(specificTestClassRunner._extraArgMemberForEacher);
      ARE_EQUAL(TestClassName, specificTestClassRunner._testClassName);
      STD_FUNCTION_TARGETS(TestRunner::GetArgs, specificTestClassRunner.call_TestRunner_GetArgs);

      vector<unique_ptr<Test>> expectedTests;
      expectedTests.emplace_back(nullptr);
      VECTORS_EQUAL(expectedTests, specificTestClassRunner._tests);
   }

   TEST(TestClassName_ReturnsTestClassName)
   {
      const char* const testClassName = _specificTestClassRunner->TestClassName();
      ARE_EQUAL(TestClassName, testClassName);
   }

   TEST(HasTestNameThatCaseInsensitiveMatchesPattern_ZeroTests_ReturnsFalse)
   {
      _specificTestClassRunner->_tests.resize(0);
      //
      const bool hasMatchingTestName
         = _specificTestClassRunner->HasTestNameThatCaseInsensitiveMatchesPattern("TestName");
      //
      IS_FALSE(hasMatchingTestName);
   }

   TEST(HasTestNameThatCaseInsensitiveMatchesPattern_OneNonMatchingTest_ReturnsFalse)
   {
      TestMock* const testMock = new TestMock;
      testMock->NameMock.ExpectAndReturn("TestName");
      _specificTestClassRunner->_tests.resize(0);
      _specificTestClassRunner->_tests.emplace_back(testMock);
      //
      const bool hasMatchingTestName
         = _specificTestClassRunner->HasTestNameThatCaseInsensitiveMatchesPattern("NotTestName");
      //
      ZEN(testMock->NameMock.CalledOnce());
      IS_FALSE(hasMatchingTestName);
   }

   TEST1X1(HasTestNameThatCaseInsensitiveMatchesPattern_OneMatchingTest_ReturnsTrue,
      const char* testNamePattern,
      "TestName",
      "testname")
   {
      TestMock* const testMock = new TestMock;
      testMock->NameMock.ExpectAndReturn("TestName");
      _specificTestClassRunner->_tests.resize(0);
      _specificTestClassRunner->_tests.emplace_back(testMock);
      //
      const bool hasMatchingTestName = _specificTestClassRunner->
         HasTestNameThatCaseInsensitiveMatchesPattern(testNamePattern);
      //
      ZEN(testMock->NameMock.CalledOnce());
      IS_TRUE(hasMatchingTestName);
   }

   TEST(HasTestNameThatCaseInsensitiveMatchesPattern_FourTestsWhereThirdOneMatches_DoesNotCallFourthTestName_ReturnsTrue)
   {
      TestMock* const testMock1 = new TestMock;
      TestMock* const testMock2 = new TestMock;
      TestMock* const testMock3 = new TestMock;
      TestMock* const testMock4 = new TestMock;
      testMock1->NameMock.ExpectAndReturn("Prefix_TestName");
      testMock2->NameMock.ExpectAndReturn("TestName_Suffix");
      testMock3->NameMock.ExpectAndReturn("TestName");
      _specificTestClassRunner->_tests.resize(0);
      _specificTestClassRunner->_tests.emplace_back(testMock1);
      _specificTestClassRunner->_tests.emplace_back(testMock2);
      _specificTestClassRunner->_tests.emplace_back(testMock3);
      _specificTestClassRunner->_tests.emplace_back(testMock4);
      const string testNamePattern = ZenUnit::Random<string>();
      //
      const bool hasMatchingTestName
         = _specificTestClassRunner->HasTestNameThatCaseInsensitiveMatchesPattern("TestName");
      //
      ZEN(testMock1->NameMock.CalledOnce());
      ZEN(testMock2->NameMock.CalledOnce());
      ZEN(testMock3->NameMock.CalledOnce());
      IS_TRUE(hasMatchingTestName);
   }

   TEST(NumberOfTestCases_ReturnsSumOfNumberOfTestCases)
   {
      _specificTestClassRunner->_tests.resize(3);
      TestMock* const testMockA = new TestMock;
      testMockA->NumberOfTestCasesMock.ExpectAndReturn(10);
      TestMock* const testMockB = new TestMock;
      testMockB->NumberOfTestCasesMock.ExpectAndReturn(0);
      TestMock* const testMockC = new TestMock;
      testMockC->NumberOfTestCasesMock.ExpectAndReturn(20);
      _specificTestClassRunner->_tests[0].reset(testMockA);
      _specificTestClassRunner->_tests[1].reset(testMockB);
      _specificTestClassRunner->_tests[2].reset(testMockC);
      //
      const size_t numberOfTestCases = _specificTestClassRunner->NumberOfTestCases();
      //
      ZEN(testMockA->NumberOfTestCasesMock.CalledOnce());
      ZEN(testMockB->NumberOfTestCasesMock.CalledOnce());
      ZEN(testMockC->NumberOfTestCasesMock.CalledOnce());
      ARE_EQUAL(30, numberOfTestCases);
   }

   TEST2X2(RunTests_PrintsTestClassNameAndNumberOfNamedTests_CallsDoRunTests_PrintsTestClassResultLine_MoveReturnsTestClassResult,
      bool testClassTypeNewableAndDeletable, bool expectDoRunTestsCall,
      false, false,
      true, true)
   {
      _specificTestClassRunnerSelfMocked->NonMinimalPrintTestClassNameAndNumberOfNamedTestsMock.Expect();
      _specificTestClassRunnerSelfMocked->ConfirmTestClassIsNewableAndDeletableAndRegisterNXNTestsMock
         .ExpectAndReturn(testClassTypeNewableAndDeletable);
      if (expectDoRunTestsCall)
      {
         _specificTestClassRunnerSelfMocked->DoRunTestsMock.Expect();
      }
      _specificTestClassRunnerSelfMocked->NonMinimalPrintResultLineMock.Expect();
      _specificTestClassRunnerSelfMocked->consoleMock->NonMinimalWriteNewLineMock.Expect();
      _specificTestClassRunnerSelfMocked->_testClassResult = TestClassResult::TestingNonDefault();

      const ZenUnitArgs zenUnitArgs = []
      {
         ZenUnitArgs zenUnitArgs;
         zenUnitArgs.printMode = RandomPrintMode();
         return zenUnitArgs;
      }();
      _specificTestClassRunnerSelfMocked->GetArgs_ZenMock.ExpectAndReturn(zenUnitArgs);
      //
      const TestClassResult testClassResult = _specificTestClassRunnerSelfMocked->RunTests();
      //
      ZEN(_specificTestClassRunnerSelfMocked->NonMinimalPrintTestClassNameAndNumberOfNamedTestsMock.CalledOnce());
      ZEN(_specificTestClassRunnerSelfMocked->ConfirmTestClassIsNewableAndDeletableAndRegisterNXNTestsMock.CalledOnceWith(
          &_specificTestClassRunnerSelfMocked->_newDeleteTest, &_specificTestClassRunnerSelfMocked->_testClassResult));
      if (expectDoRunTestsCall)
      {
         ZEN(_specificTestClassRunnerSelfMocked->DoRunTestsMock.CalledOnce());
      }
      ZEN(_specificTestClassRunnerSelfMocked->GetArgs_ZenMock.CalledOnce());
      ZEN(_specificTestClassRunnerSelfMocked->NonMinimalPrintResultLineMock.
         CalledOnceWith(&_specificTestClassRunnerSelfMocked->_testClassResult, zenUnitArgs.printMode));
      ZEN(_specificTestClassRunnerSelfMocked->consoleMock->
         NonMinimalWriteNewLineMock.CalledOnceWith(zenUnitArgs.printMode));
      ARE_EQUAL(TestClassResult::TestingNonDefault(), testClassResult);
      ARE_EQUAL(TestClassResult(), _specificTestClassRunnerSelfMocked->_testClassResult);
   }

   TEST2X2(DoRunTests_RandomlyRunsTestsIfRandomOtherwiseSequentiallyRunsTests,
      bool random, bool expectRandomForEach,
      false, false,
      true, true)
   {
      ZenUnitArgs zenUnitArgs;
      zenUnitArgs.random = random;
      zenUnitArgs.randomseed = Random<unsigned short>();
      GetArgs_ZenMock.ExpectAndReturn(zenUnitArgs);
      if (expectRandomForEach)
      {
         _extraArgMemberForEacherMock->RandomExtraArgMemberForEachMock.Expect();
      }
      else
      {
         _extraArgMemberForEacherMock->ExtraArgMemberForEachMock.Expect();
      }
      //
      _specificTestClassRunner->DoRunTests();
      //
      ZEN(GetArgs_ZenMock.CalledOnce());
      if (expectRandomForEach)
      {
         ZEN(_extraArgMemberForEacherMock->RandomExtraArgMemberForEachMock.CalledOnceWith(
             &_specificTestClassRunner->_tests, _specificTestClassRunner.get(),
             &SpecificTestClassRunner<TestingTestClass>::RunTest,
             &_specificTestClassRunner->_testClassResult,
             zenUnitArgs.randomseed));
      }
      else
      {
         ZEN(_extraArgMemberForEacherMock->ExtraArgMemberForEachMock.CalledOnceWith(
             &_specificTestClassRunner->_tests, _specificTestClassRunner.get(),
             &SpecificTestClassRunner<TestingTestClass>::RunTest,
             &_specificTestClassRunner->_testClassResult));
      }
   }

   TEST2X2(NonMinimalPrintTestClassNameAndNumberOfNamedTests_WritesTestClassNameVerticalBarNumberOfTests,
      size_t numberOfTests, bool expectTestsPlural,
      size_t(0), true,
      size_t(1), false,
      size_t(2), true,
      size_t(3), true)
   {
      _consoleMock->NonMinimalWriteColorMock.Expect();
      _consoleMock->NonMinimalWriteLineMock.Expect();
      _specificTestClassRunner->_testClassName = TestClassName;
      _specificTestClassRunner->_tests.resize(numberOfTests);
      const ZenUnitArgs zenUnitArgs = []
      {
         ZenUnitArgs zenUnitArgs;
         zenUnitArgs.printMode = RandomPrintMode();
         return zenUnitArgs;
      }();
      GetArgs_ZenMock.ExpectAndReturn(zenUnitArgs);
      //
      _specificTestClassRunner->NonMinimalPrintTestClassNameAndNumberOfNamedTests();
      //
      ZEN(GetArgs_ZenMock.CalledOnce());
      ZEN(_consoleMock->NonMinimalWriteColorMock.CalledAsFollows(
      {
         { "@", Color::Green, zenUnitArgs.printMode },
         { TestClassName, Color::Green, zenUnitArgs.printMode }
      }));
      if (expectTestsPlural)
      {
         ZEN(_consoleMock->NonMinimalWriteLineMock.CalledOnceWith(
            String::Concat(" | ", numberOfTests, " named tests"), zenUnitArgs.printMode));
      }
      else
      {
         ZEN(_consoleMock->NonMinimalWriteLineMock.CalledOnceWith(
            String::Concat(" | ", numberOfTests, " named test"), zenUnitArgs.printMode));
      }
   }

   TEST3X3(ConfirmTestClassIsNewableAndDeletableAndRegisterNXNTests_RunsNewDeleteTest_AddsResultToResults_ReturnsTrueIfNewableAndDeletable,
      bool expectedReturnValue, TestOutcome newDeleteTestOutcome, bool expectWriteLineOK,
      false, TestOutcome::Anomaly, false,
      false, TestOutcome::Exception, false,
      true, TestOutcome::Success, true)
   {
      const ZenUnitArgs zenUnitArgs = []
      {
         ZenUnitArgs zenUnitArgs;
         zenUnitArgs.printMode = static_cast<PrintMode>(Random<underlying_type_t<PrintMode>>(
            static_cast<underlying_type_t<PrintMode>>(PrintMode::Unset),
            static_cast<underlying_type_t<PrintMode>>(PrintMode::MaxValue)));
         return zenUnitArgs;
      }();
      GetArgs_ZenMock.ExpectAndReturn(zenUnitArgs);

      _consoleMock->NonMinimalWriteColorMock.Expect();
      _consoleMock->NonMinimalWriteMock.Expect();
      if (expectWriteLineOK)
      {
         _consoleMock->NonMinimalWriteLineMock.Expect();
      }
      TestMock testMock;

      TestResult testResult;
      testResult.testOutcome = newDeleteTestOutcome;
      const vector<TestResult> testResults { testResult };
      testMock.RunMock.ExpectAndReturn(testResults);

      TestClassResultMock testClassResultMock;
      testClassResultMock.AddTestResultsMock.Expect();
      //
      const bool testClassTypeIsNewableAndDeletable = _specificTestClassRunner->
         ConfirmTestClassIsNewableAndDeletableAndRegisterNXNTests(&testMock, &testClassResultMock);
      //
      ZEN(GetArgs_ZenMock.CalledOnce());
      ZEN(_consoleMock->NonMinimalWriteColorMock.CalledOnceWith("|", Color::Green, zenUnitArgs.printMode));
      ZEN(_consoleMock->NonMinimalWriteMock.CalledOnceWith("TestClassIsNewableAndDeletable -> ", zenUnitArgs.printMode));
      if (expectWriteLineOK)
      {
         ZEN(_consoleMock->NonMinimalWriteLineMock.CalledOnceWith("OK", zenUnitArgs.printMode));
      }
      ZEN(testMock.RunMock.CalledOnce());
      ZEN(testClassResultMock.AddTestResultsMock.CalledOnceWith(testResults));
      ARE_EQUAL(expectedReturnValue, testClassTypeIsNewableAndDeletable);
   }

   TEST(RunTest_NonMinimalWritesVerticalBarTestName_RunsTest_AddsTestResultsToTestClassResult_NonMinimalWriteTestOutcome)
   {
      const ZenUnitArgs zenUnitArgs = []
      {
         ZenUnitArgs zenUnitArgs;
         zenUnitArgs.printMode = RandomPrintMode();
         return zenUnitArgs;
      }();
      _specificTestClassRunnerSelfMocked->GetArgs_ZenMock.ExpectAndReturn(zenUnitArgs);

      _specificTestClassRunnerSelfMocked->consoleMock->NonMinimalWriteColorMock.Expect();
      _specificTestClassRunnerSelfMocked->consoleMock->NonMinimalWriteMock.Expect();

      TestMock* const testMock = new TestMock;
      const string TestName = Random<string>();
      testMock->NameMock.ExpectAndReturn(TestName.c_str());
      testMock->NonMinimalWritePostTestNameMessageMock.Expect();
      TestResult test0;
      test0.fullTestName = FullTestName("", "Test0", 0);
      const vector<TestResult> TestResults { test0, TestResult() };
      testMock->RunMock.ExpectAndReturn(TestResults);
      testMock->NonMinimalWritePostTestCompletionMessageMock.Expect();
      const unique_ptr<Test> test(testMock);

      TestClassResultMock testClassResultMock;
      testClassResultMock.AddTestResultsMock.Expect();
      //
      _specificTestClassRunnerSelfMocked->RunTest(test, &testClassResultMock);
      //
      ZEN(_specificTestClassRunnerSelfMocked->GetArgs_ZenMock.CalledOnce());
      ZEN(_specificTestClassRunnerSelfMocked->consoleMock->
         NonMinimalWriteColorMock.CalledOnceWith("|", Color::Green, zenUnitArgs.printMode));
      ZEN(_specificTestClassRunnerSelfMocked->consoleMock->
         NonMinimalWriteMock.CalledOnceWith(TestName, zenUnitArgs.printMode));
      ZEN(testMock->NameMock.CalledOnce());
      ZEN(testMock->NonMinimalWritePostTestNameMessageMock.CalledOnceWith(
         _specificTestClassRunnerSelfMocked->_console.get(), zenUnitArgs.printMode));
      ZEN(testMock->RunMock.CalledOnce());
      ZEN(testClassResultMock.AddTestResultsMock.CalledOnceWith(TestResults));
      ZEN(testMock->NonMinimalWritePostTestCompletionMessageMock.CalledOnceWith(
         _specificTestClassRunnerSelfMocked->_console.get(), test0, zenUnitArgs.printMode));
   }

   TEST(NonMinimalPrintResultLine_CallsTestClassResultPrintResultLine)
   {
      TestClassResultMock testClassResultMock;
      testClassResultMock.NonMinimalPrintResultLineMock.Expect();
      const PrintMode printMode = RandomPrintMode();
      //
      _specificTestClassRunner->NonMinimalPrintResultLine(&testClassResultMock, printMode);
      //
      ZEN(testClassResultMock.NonMinimalPrintResultLineMock.CalledOnceWith(_consoleMock, printMode));
   }

   }; RUNTESTS(SpecificTestClassRunnerTests)
}
