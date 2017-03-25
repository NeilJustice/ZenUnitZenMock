#include "pch.h"
#include "ZenUnit/ToStringer/ZenUnitPrinter/VectorZenUnitPrinter.h"
#include "Testing/CustomPolicyClasses.h"

namespace ZenUnit
{
   ZENUNIT(ZenUnitPrinter_Print_PrintsPlaceholderText)
   {
      ostringstream oss;
      ZenUnitPrinter<vector<int>>::Print(oss, {});
      ARE_EQUAL("vector<T>", oss.str());
      oss = ostringstream();

      ZenUnitPrinter<vector<int, CustomAllocator<int>>>::Print(oss, {});
      ARE_EQUAL("vector<T>", oss.str());
   }
}
