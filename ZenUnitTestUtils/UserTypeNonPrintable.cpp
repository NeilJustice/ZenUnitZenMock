#include "pch.h"
#include "ZenUnitTestUtils/UserTypeNonPrintable.h"

UserTypeNonPrintable::UserTypeNonPrintable(int x)
   : x(x)
{
}

bool operator<(const UserTypeNonPrintable& left, const UserTypeNonPrintable& right)
{
   const bool isLessThan = left.x < right.x;
   return isLessThan;
}

bool operator==(const UserTypeNonPrintable& left, const UserTypeNonPrintable& right)
{
   const bool isEqual = left.x == right.x;
   return isEqual;
}