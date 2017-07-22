#include "pch.h"
#include "ZenUnit/Tests/NormalTest.h"
#include "ZenUnit/Utils/TestRandom.h"
#include "ZenUnitTests/Console/Mock/ConsoleMock.h"
#include "ZenUnitTests/Results/Mock/TestResultMock.h"
#include "ZenUnitTests/Tests/TestingTestClass.h"

namespace ZenUnit
{
   TESTS(NormalTestTests)
   SPEC(NumberOfTestCases_Returns1)
   SPECX(OptionallyWritePostTestNameMessage_WritesSpaceArrowSpace)
   SPECX(OptionallyWritePostTestCompletionMessage_CallsTestResultPrintOKIfTestPassedAndDoWriteMessageTrue)
   SPEC(Constructor_SetsTestClassNameAndTestName_SetsTestBodyPointer)
   SPEC(NewTestClass_NewsTestClass)
   SPEC(Startup_CallsStartupOnTestClass)
   SPEC(TestBody_CallsMemberTestFunctionBoundToTestClassPointer)
   SPEC(Cleanup_CallsCleanupOnTestClass)
   SPEC(DeleteTestClass_DeletesTestClass)
   SPECEND

   unique_ptr<NormalTest<TestingTestClass>> _normalTest;
   const string TestClassName = TestRandom<string>();
   const string TestName = TestRandom<string>();

   STARTUP
   {
      _normalTest = make_unique<NormalTest<TestingTestClass>>(
         TestClassName.c_str(), TestName.c_str(), &TestingTestClass::Test);
   }

   TEST(NumberOfTestCases_Returns1)
   {
      ARE_EQUAL(1, _normalTest->NumberOfTestCases());
   }

   TEST2X2(OptionallyWritePostTestNameMessage_WritesSpaceArrowSpace,
      bool doWriteMessage, bool expectWriteCall,
      false, false,
      true, true)
   {
      ConsoleMock consoleMock;
      if (expectWriteCall)
      {
         consoleMock.WriteMock.Expect();
      }
      //
      _normalTest->OptionallyWritePostTestNameMessage(&consoleMock, doWriteMessage);
      //
      if (expectWriteCall)
      {
         ZEN(consoleMock.WriteMock.AssertCalledOnceWith(" -> "));
      }
   }

   TEST1X1(OptionallyWritePostTestCompletionMessage_CallsTestResultPrintOKIfTestPassedAndDoWriteMessageTrue,
      bool doWriteMessage,
      false,
      true)
   {
      ConsoleMock consoleMock;
      TestResultMock testResultMock;
      testResultMock.OptionallyWriteOKIfTestPassedMock.Expect();
      //
      _normalTest->OptionallyWritePostTestCompletionMessage(&consoleMock, testResultMock, doWriteMessage);
      //
      ZEN(testResultMock.OptionallyWriteOKIfTestPassedMock.AssertCalledOnceWith(&consoleMock, doWriteMessage));
   }

   TEST(Constructor_SetsTestClassNameAndTestName_SetsTestBodyPointer)
   {
      const NormalTest<TestingTestClass> normalTest(TestClassName.c_str(), TestName.c_str(), &TestingTestClass::Test);
      ARE_EQUAL(TestName, normalTest.Name());
      ARE_EQUAL("TESTS(" + TestClassName + ")\nTEST(" + TestName + ")", normalTest.FullTestNameValue());
      ARE_EQUAL("(0)", normalTest.FileLineString());
      ARE_EQUAL(&TestingTestClass::Test, normalTest._testMemberFunction);
      IS_NULL(normalTest._testClass);
   }

   TEST(NewTestClass_NewsTestClass)
   {
      IS_NULL(_normalTest->_testClass);
      //
      _normalTest->NewTestClass();
      //
      IS_NOT_NULL(_normalTest->_testClass);
      _normalTest->_testClass.reset();
   }

   TEST(Startup_CallsStartupOnTestClass)
   {
      _normalTest->_testClass = make_unique<TestingTestClass>();
      _normalTest->_testClass->StartupMock.Expect();
      //
      _normalTest->Startup();
      //
      ZEN(_normalTest->_testClass->StartupMock.AssertCalledOnce());
   }

   TEST(TestBody_CallsMemberTestFunctionBoundToTestClassPointer)
   {
      _normalTest->_testMemberFunction = &TestingTestClass::Test;
      _normalTest->_testClass = make_unique<TestingTestClass>();
      _normalTest->_testClass->TestMock.Expect();
      //
      _normalTest->TestBody();
      //
      ZEN(_normalTest->_testClass->TestMock.AssertCalledOnce());
   }

   TEST(Cleanup_CallsCleanupOnTestClass)
   {
      _normalTest->_testClass = make_unique<TestingTestClass>();
      _normalTest->_testClass->CleanupMock.Expect();
      //
      _normalTest->Cleanup();
      //
      ZEN(_normalTest->_testClass->CleanupMock.AssertCalledOnce());
   }

   TEST(DeleteTestClass_DeletesTestClass)
   {
      _normalTest->_testClass = make_unique<TestingTestClass>();
      IS_FALSE(TestingTestClass::s_destructorCalled);
      //
      _normalTest->DeleteTestClass();
      //
      IS_TRUE(TestingTestClass::s_destructorCalled);
   }

   }; RUN(NormalTestTests)
}
