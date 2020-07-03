#include "pch.h"
#include "ZenMockTests/ZenMock/ExceptionThrowerMock.h"

namespace ZenMock
{
   TESTS(SixArgumentZenMockerTests)
   AFACT(Constructor_SetsFields)
   AFACT(ThrowException_CallsExceptionThrowerThrow_SetsExpectedTrue)
   AFACT(ZenMockIt_ExpectedFalse_Throws)
   AFACT(ZenMockIt_ExpectedTrue_IncrementsNumberOfCalls_CallsZenMockThrowIfExceptionSet)
   EVIDENCE

   using ZenMockerType = SixArgumentZenMocker<int, int, int, int, int, int, ExceptionThrowerMock>;
   unique_ptr<ZenMockerType> _zenMocker;
   string _zenMockedFunctionSignature;

   STARTUP
   {
      _zenMockedFunctionSignature = ZenUnit::Random<string>();
      _zenMocker = make_unique<ZenMockerType>(_zenMockedFunctionSignature);
   }

   void SetAssertedTrueToNotFailDueToExpectedButNotAsserted()
   {
      _zenMocker->_wasAsserted = true;
   }

   TEST(Constructor_SetsFields)
   {
      const ZenMockerType mocker(_zenMockedFunctionSignature);
      //
      ARE_EQUAL(_zenMockedFunctionSignature, mocker.ZenMockedFunctionSignature);
      IS_FALSE(mocker._wasExpected);
      IS_FALSE(mocker._wasAsserted);
      IS_EMPTY(mocker.zenMockedFunctionCallHistory);
   }

   TEST(ThrowException_CallsExceptionThrowerThrow_SetsExpectedTrue)
   {
      IS_FALSE(_zenMocker->_wasExpected);
      _zenMocker->_exceptionThrower.ExpectCallToExpectAndThrowException();
      //
      _zenMocker->ThrowException<TestingException>("argument", 100);
      //
      _zenMocker->_exceptionThrower.AssertExpectAndThrowExceptionCalledOnceWith("ZenMock::TestingException", 2, "argument100");
      IS_TRUE(_zenMocker->_wasExpected);
      SetAssertedTrueToNotFailDueToExpectedButNotAsserted();
   }

   TEST(ZenMockIt_ExpectedFalse_Throws)
   {
      IS_FALSE(_zenMocker->_wasExpected);
      THROWS_EXCEPTION(_zenMocker->ZenMockIt(1, 2, 3, 4, 5, 6), UnexpectedCallException,
         UnexpectedCallException::MakeWhat(_zenMockedFunctionSignature, 1, 2, 3, 4, 5, 6));
   }

   TEST(ZenMockIt_ExpectedTrue_IncrementsNumberOfCalls_CallsZenMockThrowIfExceptionSet)
   {
      _zenMocker->_wasExpected = true;
      _zenMocker->_exceptionThrower.ExpectCallToZenMockThrowExceptionIfExceptionSet();
      IS_EMPTY(_zenMocker->zenMockedFunctionCallHistory);
      //
      _zenMocker->ZenMockIt(1, 2, 3, 4, 5, 6);
      //
      using CallType = SixArgumentFunctionCall<int, int, int, int, int, int>;
      const vector<CallType> expectedCalls
      {
         CallType(1, 2, 3, 4, 5, 6)
      };
      INDEXABLES_ARE_EQUAL(expectedCalls, _zenMocker->zenMockedFunctionCallHistory);
      ZENMOCK(_zenMocker->_exceptionThrower.AssertZenMockThrowExceptionIfExceptionSetCalledOnce());
      DOES_NOT_THROW(_zenMocker->CalledOnceWith(1, 2, 3, 4, 5, 6));
      SetAssertedTrueToNotFailDueToExpectedButNotAsserted();
   }

   RUN_TESTS(SixArgumentZenMockerTests)
}
