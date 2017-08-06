#pragma once
#include "ZenUnit/Utils/Time/Watch.h"

struct WatchMock : public Zen::Mock<ZenUnit::Watch>
{
   ZENMOCK_NONVOID0_CONST(string, TimeZoneDateTimeNow)
   ZENMOCK_NONVOID0_CONST(tm, TMNow)
   ZENMOCK_NONVOID0_CONST(unsigned short, SecondsSince1970CastToAnUnsignedShort)
};
