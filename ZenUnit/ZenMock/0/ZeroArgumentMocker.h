#pragma once
#include "ZenUnit/Macros/ARE_EQUAL.h"
#include "ZenUnit/ZenMock/Exceptions/FunctionAlreadyExpectedException.h"
#include "ZenUnit/ZenMock/Exceptions/UnexpectedCallException.h"
#include "ZenUnit/ZenMock/Exceptions/UnsupportedAssertCalledZeroTimesException.h"
#include "ZenUnit/ZenMock/ZenMocker.h"

#define ZENBIND0(FunctionName_ZenMock) \
   std::bind(&decltype(FunctionName_ZenMock)::ZenMockItFunctionPointer, &(FunctionName_ZenMock))

namespace ZenMock
{
   class ZeroArgumentMockerTests;

   template<typename MockableExceptionThrowerType = ExceptionThrower>
   class ZeroArgumentMocker : public ZenMocker<MockableExceptionThrowerType>
   {
      friend class ZeroArgumentMockerTests;
   private:
      size_t _numberOfCalls;
   public:
      explicit ZeroArgumentMocker(const std::string& zenMockedFunctionSignature)
         : ZenMocker<MockableExceptionThrowerType>(zenMockedFunctionSignature)
         , _numberOfCalls(0)
      {
      }

      void ZenMockIt()
      {
         this->ZenMockThrowIfNotExpected();
         ++_numberOfCalls;
         this->ZenMockThrowIfExceptionSet();
      }

      void AssertCalledOnce()
      {
         this->ZenMockSetAsserted();
         const size_t expectedNumberOfCalls = 1;
         ARE_EQUAL(expectedNumberOfCalls, _numberOfCalls, this->ZenMockedFunctionSignature);
      }

      void AssertCalledNTimes(size_t expectedNumberOfCalls)
      {
         this->ZenMockThrowIfExpectedNumberOfCalls0(expectedNumberOfCalls);
         this->ZenMockSetAsserted();
         ARE_EQUAL(expectedNumberOfCalls, _numberOfCalls, this->ZenMockedFunctionSignature);
      }
   };
}