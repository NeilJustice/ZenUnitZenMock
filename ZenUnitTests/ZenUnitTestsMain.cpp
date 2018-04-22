#include "pch.h"

const char* const Reason = "Test of skip feature";

TESTS(SkippedTestsTestClass)
AFACT(NonSkippedTest)
FACTS(NonSkipped1X1Test)
SKIPAFACT(SkippedNormalTest, Reason)
SKIPFACTS(SkippedTest1X1, Reason)
SKIPFACTS(SkippedTest2X2, Reason)
EVIDENCE
TEST(NonSkippedTest) {}
TEST1X1(NonSkipped1X1Test, bool, true, false) {}
TEST(SkippedNormalTest) {}
TEST1X1(SkippedTest1X1, int, 0) {}
TEST2X2(SkippedTest2X2, int, int, 0, 0) {}
RUN_TESTS(SkippedTestsTestClass)

TESTS(SkippedTestClassA)
EVIDENCE
SKIP_TESTS(SkippedTestClassA, Reason)

TESTS(SkippedTestClassB)
EVIDENCE
SKIP_TESTS(SkippedTestClassB, Reason)

template<typename T>
TEMPLATE_TESTS(SkippedTemplateTestClassA, T)
EVIDENCE
SKIP_TEMPLATE_TESTS(SkippedTemplateTestClassA, Reason, int)
THEN_SKIP_TEMPLATE_TESTS(SkippedTemplateTestClassA, Reason, double)

template<template<typename...> class MapType>
TEMPLATE_TESTS(SkippedTemplateTestClassB, MapType)
EVIDENCE
RUN_TEMPLATE_TESTS(SkippedTemplateTestClassB, map)
THEN_SKIP_TEMPLATE_TESTS(SkippedTemplateTestClassB, Reason, unordered_map)

TESTS(Tests)
AFACT(VoidTest)
FACTS(NXNTestA)
FACTS(NXNTestB)
EVIDENCE

TEST(VoidTest)
{
}

TEST1X1(NXNTestA,
   int,
   0,
   1)
{
}

TEST1X1(NXNTestB,
   int,
   0,
   1)
{
}

RUN_TESTS(Tests)

int main(int argc, char* argv[])
{
   FileLiner::File(nullptr); // 100% code coverage
   FileLiner::Line(0); // 100% code coverage
   FileLiner::zenUnitTestingMode = true;
   const int exitCode = ZenUnit::RunTests(argc, argv);
   return exitCode;
}