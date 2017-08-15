#include "pch.h"
#include "ZenUnit/Console/Console.h"
#include "ZenUnit/Utils/Random.h"
#include "ZenUnitTests/Console/Mock/ConsoleColorerMock.h"

namespace ZenUnit
{
   TESTS(ConsoleTests)
   FACT(Constructor_NewsConsoleColorer_SetsFunctionPointers)
   FACT(Write_CallsWriteColorWithWhite)
   FACTS(NonMinimalWrite_CallsWriteColorIfPrintModeNotMinimal)
   FACTS(WriteColor_WritesMessageInSpecifiedColor)
   FACTS(NonMinimalWriteColor_CallsWriteColorIfPrintModeNotMinimal)
   FACT(WriteLine_CallsWriteLineWithWhite)
   FACTS(NonMinimalWriteNewLine_CallsWriteLineWithWhite)
   FACTS(NonMinimalWriteLine_CallsWriteLineIfPrintModeNotMinimal)
   FACTS(WriteLineColor_WritesMessageInSpecifiedColorThenNewLine)
   FACTS(WriteLineAndExit_CallsWriteLineAndExit)
   FACTS(NonMinimalWriteStringsCommaSeparated_PrintModeNotMinimal_CallsDoWriteStringsCommaSeparated)
   FACTS(DoWriteStringsCommaSeparated_PrintsCommaSeparatedLengthNumberOfVectorValuesAtSpecifiedOffset)
   FACTS(PauseForAnyKeyIfDebuggerIsPresent_WritesPressAnyKeyAndGetsLineIfDebuggerIsPresent)
#ifdef _WIN32
   FACTS(DebuggerIsPresent_ReturnsTrueIfIsDebuggerPresentFunctionReturns1)
#endif
   EVIDENCE

   Console _console;
   ConsoleColorerMock* _consoleColorerMock;
   const string Message = Random<string>();

   struct ConsoleSelfMocked : public Zen::Mock<Console>
   {
      ZENMOCK_VOID2_CONST(WriteColor, const string&, Color)
      ZENMOCK_VOID2_CONST(WriteLineColor, const string&, Color)
      ZENMOCK_NONVOID0_CONST(bool, DebuggerIsPresent)
      ZENMOCK_VOID0_CONST(GetLine)
      ZENMOCK_VOID0_CONST(WriteNewLine)
   } _consoleSelfMocked;

   STARTUP
   {
      _console._consoleColorer.reset(_consoleColorerMock = new ConsoleColorerMock);
   }

   TEST(Constructor_NewsConsoleColorer_SetsFunctionPointers)
   {
      Console console;
      POINTER_WAS_NEWED(console._consoleColorer);
      STD_FUNCTION_TARGETS(::exit, console._exit_ZenMockable);
   #ifdef _WIN32
      STD_FUNCTION_TARGETS(::IsDebuggerPresent, console._IsDebuggerPresent_ZenMockable);
   #endif
   }

   TEST(Write_CallsWriteColorWithWhite)
   {
      _consoleSelfMocked.WriteColorMock.Expect();
      //
      _consoleSelfMocked.Write(Message);
      //
      ZEN(_consoleSelfMocked.WriteColorMock.AssertCalledOnceWith(Message, Color::White));
   }

   TEST2X2(NonMinimalWrite_CallsWriteColorIfPrintModeNotMinimal,
      PrintMode printMode, bool expectWriteCall,
      PrintMode::Minimal, false,
      PrintMode::Normal, true,
      PrintMode::Detailed, true)
   {
      struct ConsoleSelfMock_Write : public Zen::Mock<Console>
      {
         ZENMOCK_VOID1_CONST(Write, const string&)
      } consoleSelfMock_Write;
      if (expectWriteCall)
      {
         consoleSelfMock_Write.WriteMock.Expect();
      }
      //
      consoleSelfMock_Write.NonMinimalWrite(Message, printMode);
      //
      if (expectWriteCall)
      {
         ZEN(consoleSelfMock_Write.WriteMock.AssertCalledOnceWith(Message));
      }
   }

   TEST2X2(WriteColor_WritesMessageInSpecifiedColor,
      Color color, bool setColorReturnValue,
      Color::Green, false,
      Color::Green, true,
      Color::Red, false,
      Color::Red, true)
   {
      _consoleColorerMock->SetColorMock.ExpectAndReturn(setColorReturnValue);
      _consoleColorerMock->UnsetColorMock.Expect();
      //
      _console.WriteColor(Message, color);
      //
      ZEN(_consoleColorerMock->SetColorMock.AssertCalledOnceWith(color));
      ZEN(_consoleColorerMock->UnsetColorMock.AssertCalledOnceWith(setColorReturnValue));
   }

   TEST2X2(NonMinimalWriteColor_CallsWriteColorIfPrintModeNotMinimal,
      PrintMode printMode, bool expectWriteColorCall,
      PrintMode::Minimal, false,
      PrintMode::Normal, true,
      PrintMode::Detailed, true)
   {
      struct ConsoleSelfMock_WriteColor : public Zen::Mock<Console>
      {
         ZENMOCK_VOID2_CONST(WriteColor, const string&, Color)
      } consoleSelfMock_WriteColor;
      if (expectWriteColorCall)
      {
         consoleSelfMock_WriteColor.WriteColorMock.Expect();
      }
      //
      consoleSelfMock_WriteColor.NonMinimalWriteColor(Message, Color::Green, printMode);
      //
      if (expectWriteColorCall)
      {
         ZEN(consoleSelfMock_WriteColor.WriteColorMock.AssertCalledOnceWith(Message, Color::Green));
      }
   }

   TEST(WriteLine_CallsWriteLineWithWhite)
   {
      _consoleSelfMocked.WriteLineColorMock.Expect();
      //
      _consoleSelfMocked.WriteLine(Message);
      //
      ZEN(_consoleSelfMocked.WriteLineColorMock.AssertCalledOnceWith(Message, Color::White));
   }

   TEST2X2(NonMinimalWriteNewLine_CallsWriteLineWithWhite,
      PrintMode printMode, bool expectWriteNewLineCall,
      PrintMode::Minimal, false,
      PrintMode::Normal, true,
      PrintMode::Detailed, true)
   {
      if (expectWriteNewLineCall)
      {
         _consoleSelfMocked.WriteNewLineMock.Expect();
      }
      //
      _consoleSelfMocked.NonMinimalWriteNewLine(printMode);
      //
      if (expectWriteNewLineCall)
      {
         ZEN(_consoleSelfMocked.WriteNewLineMock.AssertCalledOnce());
      }
   }

   TEST2X2(NonMinimalWriteLine_CallsWriteLineIfPrintModeNotMinimal,
      PrintMode printMode, bool expectWriteLineCall,
      PrintMode::Minimal, false,
      PrintMode::Normal, true,
      PrintMode::Detailed, true)
   {
      struct ConsoleSelfMock_WriteColor : public Zen::Mock<Console>
      {
         ZENMOCK_VOID1_CONST(WriteLine, const string&)
      } consoleSelfMock_WriteLine;
      if (expectWriteLineCall)
      {
         consoleSelfMock_WriteLine.WriteLineMock.Expect();
      }
      //
      consoleSelfMock_WriteLine.NonMinimalWriteLine(Message, printMode);
      //
      if (expectWriteLineCall)
      {
         ZEN(consoleSelfMock_WriteLine.WriteLineMock.AssertCalledOnceWith(Message));
      }
   }

   TEST2X2(WriteLineColor_WritesMessageInSpecifiedColorThenNewLine,
      Color color, bool setColorReturnValue,
      Color::Green, false,
      Color::Green, true,
      Color::Red, false,
      Color::Red, true)
   {
      _consoleColorerMock->SetColorMock.ExpectAndReturn(setColorReturnValue);
      _consoleColorerMock->UnsetColorMock.Expect();
      //
      _console.WriteLineColor(Message, color);
      //
      ZEN(_consoleColorerMock->SetColorMock.AssertCalledOnceWith(color));
      ZEN(_consoleColorerMock->UnsetColorMock.AssertCalledOnceWith(setColorReturnValue));
   }

   TEST1X1(WriteLineAndExit_CallsWriteLineAndExit,
      int exitCode,
      0,
      1)
   {
      ZENMOCK_VOID1_FREE(exit, int);
      exit_ZenMock.Expect();
      _console._exit_ZenMockable = ZENBIND1(exit_ZenMock);
      //
      _console.WriteLineAndExit(Message, exitCode);
      //
      ZEN(exit_ZenMock.AssertCalledOnceWith(exitCode));
   }

   TEST2X2(NonMinimalWriteStringsCommaSeparated_PrintModeNotMinimal_CallsDoWriteStringsCommaSeparated,
      PrintMode printMode, bool expectDoWriteCall,
      PrintMode::Minimal, false,
      PrintMode::Normal, true,
      PrintMode::Detailed, true)
   {
      struct ConsoleSelfMocked : public Zen::Mock<Console>
      {
         ZENMOCK_VOID3_CONST(DoWriteStringsCommaSeparated, const std::vector<std::string>&, size_t, size_t)
      } consoleSelfMocked;

      vector<string> strings = { Random<string>() };
      size_t startIndex = Random<size_t>();
      size_t numberOfElements = Random<size_t>();
      if (expectDoWriteCall)
      {
         consoleSelfMocked.DoWriteStringsCommaSeparatedMock.Expect();
      }
      //
      consoleSelfMocked.NonMinimalWriteStringsCommaSeparated(strings, startIndex, numberOfElements, printMode);
      //
      if (expectDoWriteCall)
      {
         consoleSelfMocked.DoWriteStringsCommaSeparatedMock.AssertCalledOnceWith(strings, startIndex, numberOfElements);
      }
   }

   TEST4X4(DoWriteStringsCommaSeparated_PrintsCommaSeparatedLengthNumberOfVectorValuesAtSpecifiedOffset,
      const vector<string>& strings, size_t startIndex, size_t numberOfElements, const vector<string>& expectedConsoleWrites,
      vector<string>{ "Arg1" }, size_t(0), size_t(1), vector<string>{ "Arg1" },
      vector<string>{ "Arg1", "Arg2" }, size_t(0), size_t(2), vector<string>{ "Arg1", ", ", "Arg2" },
      vector<string>{ "Arg1", "Arg2" }, size_t(1), size_t(1), vector<string>{ "Arg2" },
      vector<string>{ "Arg1", "Arg2", "Arg3", "Arg4" }, size_t(2), size_t(2), vector<string>{ "Arg3", ", ", "Arg4" })
   {
      struct ConsoleSelfMocked : public Zen::Mock<Console>
      {
         ZENMOCK_VOID1_CONST(Write, const string&)
      } consoleSelfMocked;
      consoleSelfMocked.WriteMock.Expect();
      //
      consoleSelfMocked.DoWriteStringsCommaSeparated(strings, startIndex, numberOfElements);
      //
      vector<ZenMock::OneArgCallRef<const string&>> expectedConsoleWriteCalls;
      for (const string& expectedConsoleWrite : expectedConsoleWrites)
      {
         expectedConsoleWriteCalls.emplace_back(expectedConsoleWrite);
      }
      ZEN(consoleSelfMocked.WriteMock.AssertCalls(expectedConsoleWriteCalls));
   }

   TEST2X2(PauseForAnyKeyIfDebuggerIsPresent_WritesPressAnyKeyAndGetsLineIfDebuggerIsPresent,
      bool debuggerIsPresentReturnValue, bool expectPressAnyKeyAndGetLine,
      false, false,
      true, true)
   {
      _consoleSelfMocked.DebuggerIsPresentMock.ExpectAndReturn(debuggerIsPresentReturnValue);
      if (expectPressAnyKeyAndGetLine)
      {
         _consoleSelfMocked.WriteLineColorMock.Expect();
         _consoleSelfMocked.GetLineMock.Expect();
      }
      //
      _consoleSelfMocked.PauseForAnyKeyIfDebuggerIsPresent();
      //
      ZEN(_consoleSelfMocked.DebuggerIsPresentMock.AssertCalledOnce());
      if (expectPressAnyKeyAndGetLine)
      {
         ZEN(_consoleSelfMocked.WriteLineColorMock.AssertCalledOnceWith("Press any key to continue...", Color::White));
         ZEN(_consoleSelfMocked.GetLineMock.AssertCalledOnce());
      }
   }

#ifdef _WIN32
   TEST2X2(DebuggerIsPresent_ReturnsTrueIfIsDebuggerPresentFunctionReturns1,
      int isDebuggerPresentReturnValue, bool expectedReturnValue,
      0, false,
      1, true,
      2, false)
   {
      ZENMOCK_NONVOID0_FREE(int, IsDebuggerPresent);
      IsDebuggerPresent_ZenMock.ExpectAndReturn(isDebuggerPresentReturnValue);
      _console._IsDebuggerPresent_ZenMockable = ZENBIND0(IsDebuggerPresent_ZenMock);
      //
      const bool debuggerIsPresent = _console.DebuggerIsPresent();
      //
      ZEN(IsDebuggerPresent_ZenMock.AssertCalledOnce());
      ARE_EQUAL(expectedReturnValue, debuggerIsPresent);
   }
#endif

   RUNTESTS(ConsoleTests)
}
