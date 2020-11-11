#include "pch.h"
#include "ZenUnitTestUtils/EqualizersAndRandoms/FullTestNameEqualizerAndRandom.h"

namespace ZenUnit
{
   void Equalizer<FullTestName>::AssertEqual(const FullTestName& expectedFullTestName, const FullTestName& actualFullTestName)
   {
      ARE_EQUAL(expectedFullTestName.testClassName, actualFullTestName.testClassName);
      ARE_EQUAL(expectedFullTestName.testName, actualFullTestName.testName);
      ARE_EQUAL(expectedFullTestName.arity, actualFullTestName.arity);
   }

   FullTestName Random()
   {
      FullTestName randomFullTestName;
      randomFullTestName.testClassName = ZenUnit::Random<const char*>();
      randomFullTestName.testName = ZenUnit::Random<const char*>();
      randomFullTestName.arity = ZenUnit::Random<unsigned char>();
      return randomFullTestName;
   }
};