#include "pch.h"
#include "ZenUnit/Macros/MacroUtils.h"
#include "ZenUnit/Utils/AssertTrue.h"
#include "ZenUnit/Utils/FileLine.h"
#include "ZenUnit/Utils/StringUtil.h"

namespace ZenUnit
{
   INLINE void ThrowLogicError(
      const char* predicateText,
      const FileLine& fileLine,
      const char* functionName)
   {
      const std::string what = String::Concat(
         "assert_true(", predicateText, ") failed in ",
         functionName, "()\n", fileLine.filePath, "(", fileLine.lineNumber, ")");
      throw std::logic_error(what);
   }

   INLINE void AssertTrue(
      bool predicateResult,
      const char* predicateText,
      const FileLine& fileLine,
      const char* functionName)
   {
      if (!predicateResult)
      {
         ThrowLogicError(predicateText, fileLine, functionName);
      }
   }
}
