#include "pch.h"
#include "ZenMockTests/ZenMock/T.h"

namespace ZenMock
{
   const T<1> DV1;
   const T<1> V1 = T<1>(true);

   TESTS(OneArgumentCallTests)
   AFACT(DefaultConstructor_DefaultInitializesArg)
   AFACT(Constructor_CopiesArgToDecayedTypeArgField)
   AFACT(ZenUnitEqualizer_ThrowsIfArg1NotEqual)
   EVIDENCE

   using Call1 = OneArgumentCall<T<1>>;

   TEST(DefaultConstructor_DefaultInitializesArg)
   {
      const Call1 intOneArgumentCall;
      ARE_EQUAL(DV1, intOneArgumentCall.argument.value);
   }

   TEST(Constructor_CopiesArgToDecayedTypeArgField)
   {
      const OneArgumentCall<const T<1>&> call(V1);
      //
      ARE_COPIES(V1, call.argument.value);
      IS_TRUE((is_same<T<1>, decltype(call.argument.value)>::value));
   }

   TEST(ZenUnitEqualizer_ThrowsIfArg1NotEqual)
   {
      SETUP_EQUALIZER_THROWS_TEST(Call1);
      EQUALIZER_THROWS(Call1, argument, V1);
   }

   RUN_TESTS(OneArgumentCallTests)


   TESTS(OneArgumentCallRefTests)
   AFACT(Constructor_SetsReferences)
   AFACT(CallConstructor_SetsReferencesToOneArgumentCallArg)
   AFACT(ZenUnitPrinter_WritesToStringedArg)
   AFACT(ZenUnitEqualizer_ThrowsIfArgFieldNotEqual)
   EVIDENCE

   using Call1 = OneArgumentCall<T<1>>;
   using CallRef1 = OneArgumentCallRef<T<1>>;
   using CallRefString1 = OneArgumentCallRef<string>;

   TEST(Constructor_SetsReferences)
   {
      const CallRef1 callRef(V1);
      ARE_SAME(V1, callRef.argumentReference.value);
   }

   TEST(CallConstructor_SetsReferencesToOneArgumentCallArg)
   {
      const Call1 call(V1);
      //
      const CallRef1 callRef(call);
      //
      ARE_SAME(call.argument.value, callRef.argumentReference.value);
   }

   TEST(ZenUnitPrinter_WritesToStringedArg)
   {
      const CallRefString1 callRef(S1);
      ostringstream oss;
      //
      ZenUnit::Printer<CallRefString1>::Print(oss, callRef);
      //
      const string zenUnitPrintResult = oss.str();
      const string expected = "ZenMock::OneArgumentCall:\n"
"Argument: \"1\"";
      ARE_EQUAL(expected, zenUnitPrintResult);
   }

   TEST(ZenUnitEqualizer_ThrowsIfArgFieldNotEqual)
   {
      const T<1> VF1 = T<1>(false);

      const CallRef1 callRef(V1);
      const CallRef1 callRef_arg1(VF1);
      AssertARE_EQUALThrowsAnomalyContaining([&] { ARE_EQUAL(callRef, callRef_arg1); }, "argument");
   }

   RUN_TESTS(OneArgumentCallRefTests)
}