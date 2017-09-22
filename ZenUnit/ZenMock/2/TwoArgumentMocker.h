#pragma once
#include "ZenUnit/Macros/VECTORS_EQUAL.h"
#include "ZenUnit/ZenMock/2/TwoArgumentCall.h"
#include "ZenUnit/ZenMock/2/TwoArgumentCallRef.h"
#include "ZenUnit/ZenMock/Exceptions/UnexpectedCallException.h"
#include "ZenUnit/ZenMock/ZenMocker.h"

#define ZENBIND2(FunctionName_ZenMock) \
   std::bind(&decltype(FunctionName_ZenMock)::ZenMockItFunctionPointer, &(FunctionName_ZenMock), \
   std::placeholders::_1, std::placeholders::_2)

namespace ZenMock
{
   template<
      typename Arg1Type,
      typename Arg2Type,
      typename MockableExceptionThrowerType = ExceptionThrower>
   class TwoArgumentMocker : public ZenMocker<MockableExceptionThrowerType>
   {
      friend class TwoArgumentMockerTests;
   private:
      std::vector<TwoArgumentCall<Arg1Type, Arg2Type>> twoArgumentCalls;
   public:
      explicit TwoArgumentMocker(const std::string& zenMockedFunctionSignature)
         : ZenMocker<MockableExceptionThrowerType>(zenMockedFunctionSignature)
      {
      }

      void ZenMockIt(const Arg1Type& argument1, const Arg2Type& argument2)
      {
         this->ZenMockThrowIfNotExpected(argument1, argument2);
         twoArgumentCalls.emplace_back(argument1, argument2);
         this->ZenMockThrowIfExceptionSet();
      }

      void AssertCalledOnceWith(
         const Arg1Type& expectedArgument1,
         const Arg2Type& expectedArgument2)
      {
         this->ZenMockSetAsserted();
         const size_t expectedNumberOfCalls = 1;
         ARE_EQUAL(expectedNumberOfCalls, twoArgumentCalls.size(), this->ZenMockedFunctionSignature);
         ARE_EQUAL(expectedArgument1, twoArgumentCalls[0].argument1, this->ZenMockedFunctionSignature);
         ARE_EQUAL(expectedArgument2, twoArgumentCalls[0].argument2, this->ZenMockedFunctionSignature);
      }

      void AssertCalledNTimesWith(
         size_t expectedNumberOfCalls,
         const Arg1Type& expectedArgument1,
         const Arg2Type& expectedArgument2)
      {
         this->ZenMockThrowIfExpectedNumberOfCalls0(expectedNumberOfCalls);
         this->ZenMockSetAsserted();
         ARE_EQUAL(expectedNumberOfCalls, twoArgumentCalls.size(), this->ZenMockedFunctionSignature);
         for (size_t i = 0; i < expectedNumberOfCalls; ++i)
         {
            const std::string zenMockedFunctionSignatureAndCallIndex
               = ZenUnit::String::Concat(this->ZenMockedFunctionSignature, ", at i=", i);
            ARE_EQUAL(expectedArgument1, twoArgumentCalls[i].argument1, zenMockedFunctionSignatureAndCallIndex);
            ARE_EQUAL(expectedArgument2, twoArgumentCalls[i].argument2, zenMockedFunctionSignatureAndCallIndex);
         }
      }

      void AssertCalls(const std::vector<TwoArgumentCallRef<Arg1Type, Arg2Type>>& expectedTwoArgumentCalls)
      {
         this->ZenMockThrowIfExpectedCallsSizeIsZero(expectedTwoArgumentCalls.size());
         this->ZenMockSetAsserted();
         const std::vector<TwoArgumentCallRef<Arg1Type, Arg2Type>>
            actualTwoArgumentCalls = PrivateCallsToCallRefs(twoArgumentCalls);
         VECTORS_EQUAL(expectedTwoArgumentCalls, actualTwoArgumentCalls, this->ZenMockedFunctionSignature);
      }
   private:
      static std::vector<TwoArgumentCallRef<Arg1Type, Arg2Type>>
         PrivateCallsToCallRefs(const std::vector<TwoArgumentCall<Arg1Type, Arg2Type>>& twoArgumentCalls)
      {
         std::vector<TwoArgumentCallRef<Arg1Type, Arg2Type>> twoArgumentCallRefs;
         twoArgumentCallRefs.reserve(twoArgumentCalls.size());
         std::for_each(twoArgumentCalls.cbegin(), twoArgumentCalls.cend(),
            [&](const TwoArgumentCall<Arg1Type, Arg2Type>& twoArgumentCall)
            {
               twoArgumentCallRefs.emplace_back(twoArgumentCall);
            });
         return twoArgumentCallRefs;
      }
   };
}