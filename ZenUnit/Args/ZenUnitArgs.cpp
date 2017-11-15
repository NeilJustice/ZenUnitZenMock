//#ifdef __linux__
//#include "ZenUnit/pch.h"
//#elif defined(_WIN64)
#include "pch.h"
//#endif
#include "ZenUnit/Args/ZenUnitArgs.h"
#include "ZenUnit/Macros/ARE_EQUAL.h"

INLINE void ZenUnitEqualizer<ZenUnit::ZenUnitArgs>::
AssertEqual(const ZenUnit::ZenUnitArgs& expectedArguments, const ZenUnit::ZenUnitArgs& actualArgs)
{
   ARE_EQUAL(expectedArguments.commandLine, actualArgs.commandLine);
   ARE_EQUAL(expectedArguments.printMode, actualArgs.printMode);
   VECTORS_EQUAL(expectedArguments.runFilters, actualArgs.runFilters);
   ARE_EQUAL(expectedArguments.pause, actualArgs.pause);
   ARE_EQUAL(expectedArguments.wait, actualArgs.wait);
   ARE_EQUAL(expectedArguments.exit0, actualArgs.exit0);
   ARE_EQUAL(expectedArguments.failskips, actualArgs.failskips);
   ARE_EQUAL(expectedArguments.testruns, actualArgs.testruns);
   ARE_EQUAL(expectedArguments.random, actualArgs.random);
   ARE_EQUAL(expectedArguments.randomseed, actualArgs.randomseed);
   ARE_EQUAL(expectedArguments.randomseedsetbyuser, actualArgs.randomseedsetbyuser);
   ARE_EQUAL(expectedArguments.maxtestmilliseconds, actualArgs.maxtestmilliseconds);
   ARE_EQUAL(expectedArguments.maxtotalseconds, actualArgs.maxtotalseconds);
}
