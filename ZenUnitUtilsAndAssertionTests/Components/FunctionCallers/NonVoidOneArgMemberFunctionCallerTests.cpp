#include "pch.h"

namespace ZenUnit
{
   template<typename ReturnType, typename Arg1Type>
   TEMPLATE_TESTS(NonVoidOneArgMemberFunctionCallerTests, ReturnType, Arg1Type)
   AFACT(ConstCall_CallsConstMemberFunctionOnce_ReturnsReturnValue)
   AFACT(NonConstCall_CallsNonConstMemberFunctionOnce_ReturnsReturnValue)
   EVIDENCE

   class Class
   {
   public:
      mutable vector<Arg1Type> calls;
      mutable ReturnType returnValue = ReturnType{};

      ReturnType NonVoidOneArgConstFunction(Arg1Type arg1) const
      {
         calls.emplace_back(arg1);
         returnValue = ZenUnit::Random<ReturnType>();
         return returnValue;
      }

      ReturnType NonVoidOneArgFunction(Arg1Type arg1)
      {
         calls.emplace_back(arg1);
         returnValue = ZenUnit::Random<ReturnType>();
         return returnValue;
      }
   };

   OneArgMemberFunctionCaller<ReturnType, Class, Arg1Type> _oneArgMemberFunctionCaller;
   Class _classInstance;

   TEST(ConstCall_CallsConstMemberFunctionOnce_ReturnsReturnValue)
   {
      const Arg1Type arg1 = ZenUnit::Random<Arg1Type>();
      //
      const ReturnType returnValueA = _oneArgMemberFunctionCaller.ConstCall(
         &_classInstance, &Class::NonVoidOneArgConstFunction, arg1);
      //
      const vector<Arg1Type> expectedCallsA =
      {
         arg1
      };
      INDEXABLES_ARE_EQUAL(expectedCallsA, _classInstance.calls);
      ARE_EQUAL(_classInstance.returnValue, returnValueA);


      const Arg1Type arg2 = ZenUnit::Random<Arg1Type>();
      //
      const ReturnType returnValueB = _oneArgMemberFunctionCaller.ConstCall(
         &_classInstance, &Class::NonVoidOneArgConstFunction, arg2);
      //
      const vector<Arg1Type> expectedCallsB =
      {
         arg1, arg2
      };
      INDEXABLES_ARE_EQUAL(expectedCallsB, _classInstance.calls);
      ARE_EQUAL(_classInstance.returnValue, returnValueB);
   }

   TEST(NonConstCall_CallsNonConstMemberFunctionOnce_ReturnsReturnValue)
   {
      const Arg1Type arg1 = ZenUnit::Random<Arg1Type>();
      //
      const ReturnType returnValueA = _oneArgMemberFunctionCaller.NonConstCall(
         &_classInstance, &Class::NonVoidOneArgFunction, arg1);
      //
      const vector<Arg1Type> expectedCallsA =
      {
         arg1
      };
      INDEXABLES_ARE_EQUAL(expectedCallsA, _classInstance.calls);
      ARE_EQUAL(_classInstance.returnValue, returnValueA);


      const Arg1Type arg2 = ZenUnit::Random<Arg1Type>();
      //
      const ReturnType returnValueB = _oneArgMemberFunctionCaller.
         NonConstCall(&_classInstance, &Class::NonVoidOneArgFunction, arg2);
      //
      const vector<Arg1Type> expectedCallsB =
      {
         arg1, arg2
      };
      INDEXABLES_ARE_EQUAL(expectedCallsB, _classInstance.calls);
      ARE_EQUAL(_classInstance.returnValue, returnValueB);
   }

   RUN_TEMPLATE_TESTS(NonVoidOneArgMemberFunctionCallerTests, char, int)
   THEN_RUN_TEMPLATE_TESTS(NonVoidOneArgMemberFunctionCallerTests, int, double)
}
