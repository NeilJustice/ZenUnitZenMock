#include "pch.h"
#include "ZenUnit/ToStringer/SFINAE/has_ostream_left_shift.h"
#include "ZenUnitTests/Testing/UserTypeInsOpAndZenUnitPrintable.h"
#include "ZenUnitTests/Testing/UserTypeOnlyZenUnitPrintable.h"

namespace ZenUnit
{
   TESTS(has_ostream_left_shiftTests)
   FACT(value_IsFalseIfFreeFunctionOStreamInsertionOperatorNotDefinedForType)
   FACT(value_IsTrueIfFreeFunctionOStreamInsertionOperatorIsDefinedForType)
   EVIDENCE

   TEST(value_IsFalseIfFreeFunctionOStreamInsertionOperatorNotDefinedForType)
   {
      IS_FALSE(has_ostream_left_shift<vector<int>>::value);
      IS_FALSE(has_ostream_left_shift<UserTypeNonPrintable>::value);

      IS_FALSE(has_ostream_left_shift<UserType>::value);
      IS_FALSE(has_ostream_left_shift<UserType&>::value);
      IS_FALSE(has_ostream_left_shift<const UserType&>::value);
      IS_FALSE(has_ostream_left_shift<UserType&&>::value);

      IS_FALSE(has_ostream_left_shift<UserTypeNonPrintable>::value);
      IS_FALSE(has_ostream_left_shift<UserTypeNonPrintable&>::value);
      IS_FALSE(has_ostream_left_shift<const UserTypeNonPrintable&>::value);
      IS_FALSE(has_ostream_left_shift<UserTypeNonPrintable&&>::value);

      IS_FALSE(has_ostream_left_shift<UserTypeOnlyZenUnitPrintable>::value);
      IS_FALSE(has_ostream_left_shift<UserTypeOnlyZenUnitPrintable&>::value);
      IS_FALSE(has_ostream_left_shift<const UserTypeOnlyZenUnitPrintable&>::value);
      IS_FALSE(has_ostream_left_shift<UserTypeOnlyZenUnitPrintable&&>::value);
   }

   TEST(value_IsTrueIfFreeFunctionOStreamInsertionOperatorIsDefinedForType)
   {
      IS_TRUE(has_ostream_left_shift<int>::value);
      IS_TRUE(has_ostream_left_shift<int*>::value);

      IS_TRUE(has_ostream_left_shift<UserTypeInsOpAndZenUnitPrintable>::value);
      IS_TRUE(has_ostream_left_shift<UserTypeInsOpAndZenUnitPrintable&>::value);
      IS_TRUE(has_ostream_left_shift<const UserTypeInsOpAndZenUnitPrintable&>::value);
      IS_TRUE(has_ostream_left_shift<UserTypeInsOpAndZenUnitPrintable&&>::value);
   }

   RUNTESTS(has_ostream_left_shiftTests)
}
