#pragma once
#include "ZenUnit/Equalizers/ZenUnitEqualizer.h"

namespace ZenUnit
{
   struct ZenUnitArgs
   {
      std::string commandLine;
      bool minimal = false;
      bool exit0 = false;
      bool failskips = false;
      unsigned testruns = 1;
      unsigned maxtestmilliseconds = 0;
      unsigned maxtotalseconds = 0;

      static const size_t NumberOfValidArgs;
   };

   #ifdef __linux__
      #if __clang_major__ == 3 && __clang_minor__ == 9
         static_assert(sizeof(ZenUnitArgs) == 48);
      #endif
   #elif _MSC_FULL_VER == 191025019 && defined(_WIN64) // VS2017 15.2
      #ifdef _DEBUG
         static_assert(sizeof(ZenUnitArgs) == 56);
      #elif NDEBUG
         static_assert(sizeof(ZenUnitArgs) == 48);
      #endif
   #endif
}

template<>
struct ZenUnitEqualizer<ZenUnit::ZenUnitArgs>
{
   static void AssertEqual(
      const ZenUnit::ZenUnitArgs& expectedArgs,
      const ZenUnit::ZenUnitArgs& actualArgs);
};
