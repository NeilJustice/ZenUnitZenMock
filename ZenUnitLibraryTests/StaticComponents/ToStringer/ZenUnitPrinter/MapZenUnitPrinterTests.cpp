#include "pch.h"
#include "ZenUnitTestUtils/CustomPolicyClasses.h"

namespace ZenUnit
{
   template<typename TKey, typename TValue>
   TEMPLATE_TESTS(MapZenUnitPrinterTests, TKey, TValue)
   AFACT(Print_Map_PrintsPlaceholder)
   AFACT(Print_UnorderedMap_PrintsPlaceholder)
   EVIDENCE

   TEST(Print_Map_PrintsPlaceholder)
   {
      ostringstream oss;

      ZenUnit::Printer<map<TKey, TValue>>::Print(oss, {});
      ARE_EQUAL("std::map<TKey, TValue>", oss.str());
      oss = ostringstream();

      ZenUnit::Printer<map<TKey, TValue, CustomLessComparator<TKey>>>::Print(oss, {});
      ARE_EQUAL("std::map<TKey, TValue>", oss.str());
      oss = ostringstream();

#ifdef _WIN32
      ZenUnit::Printer<map<TKey, TValue, CustomLessComparator<TKey>, CustomAllocator<pair<const TKey, TValue>>>>::Print(oss, {});
      ARE_EQUAL("std::map<TKey, TValue>", oss.str());
#endif
   }

   TEST(Print_UnorderedMap_PrintsPlaceholder)
   {
      ostringstream oss;

      ZenUnit::Printer<unordered_map<TKey, TValue>>::Print(oss, {});
      ARE_EQUAL("std::unordered_map<TKey, TValue>", oss.str());
      oss = ostringstream();

      ZenUnit::Printer<unordered_map<TKey, TValue, CustomHasher<TKey>>>::Print(oss, {});
      ARE_EQUAL("std::unordered_map<TKey, TValue>", oss.str());
      oss = ostringstream();

      ZenUnit::Printer<unordered_map<TKey, TValue, CustomHasher<TKey>, CustomEqualityComparator<TKey>>>::Print(oss, {});
      ARE_EQUAL("std::unordered_map<TKey, TValue>", oss.str());
      oss = ostringstream();

#ifdef _WIN32
      ZenUnit::Printer<unordered_map<TKey, TValue, CustomHasher<TKey>, CustomEqualityComparator<TKey>, CustomAllocator<pair<const TKey, TValue>>>>::Print(oss, {});
      ARE_EQUAL("std::unordered_map<TKey, TValue>", oss.str());
#endif
   }

   RUN_TEMPLATE_TESTS(MapZenUnitPrinterTests, int, int)
   THEN_RUN_TEMPLATE_TESTS(MapZenUnitPrinterTests, char, unsigned long long)
}
