#pragma once

class StopwatchMock : public Metal::Mock<ZenUnit::Stopwatch>
{
public:
   METALMOCK_VOID0(Start)
   METALMOCK_NONVOID0(long long, GetElapsedMicrosecondsThenResetStopwatch)
   METALMOCK_NONVOID0(string, StopAndGetElapsedSeconds)
};
