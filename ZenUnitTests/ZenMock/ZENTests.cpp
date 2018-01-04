#include "pch.h"
#include "ZenUnitTests/Console/Mock/ConsoleMock.h"

namespace ZenMock
{
   TESTS(ZENTests)
   AFACT(ZENWrappedAssertionDoesNotThrow_NothingHappens)
   AFACT(ZENWrappedAssertionThrows_RethrowsZENWrappedAnomaly)
   EVIDENCE

   TEST(ZENWrappedAssertionDoesNotThrow_NothingHappens)
   {
      ConsoleMock consoleMock;
      consoleMock.WriteLineMock.Expect();
      consoleMock.WriteLine("message");
      ZEN(consoleMock.WriteLineMock.CalledOnceWith("message"));
   }

   TEST(ZENWrappedAssertionThrows_RethrowsZENWrappedAnomaly)
   {
      ConsoleMock consoleMock;
      consoleMock.WriteLineMock.Expect();
      consoleMock.WriteLine("message");
      const string expectedMessage = "Message";
      THROWS(ZEN(consoleMock.WriteLineMock.CalledOnceWith(expectedMessage)), Anomaly, R"(
  Failed: ZEN(consoleMock.WriteLineMock.CalledOnceWith(expectedMessage))
 Because: ARE_EQUAL(expectedArgument, callHistory[0].argument, this->ZenMockedFunctionSignature) failed
Expected: "Message"
  Actual: "message"
 Message: "virtual void ZenUnit::Console::WriteLine(const string&) const"
File.cpp(1))");
   }

   }; RUNTESTS(ZENTests)
}
