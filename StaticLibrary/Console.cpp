#include "pch.h"
#include "Console.h"

void Console::WriteLine(const std::string& message) const
{
   std::cout << message << '\n';
}

void Console::WriteNewline() const
{
   std::cout << '\n';
}

Console::~Console()
{
}