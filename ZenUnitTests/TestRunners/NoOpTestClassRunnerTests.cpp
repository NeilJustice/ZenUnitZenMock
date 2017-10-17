#include "pch.h"
#include "ZenUnit/TestRunners/NoOpTestClassRunner.h"

TESTS(NoOpTestClassRunnerTests)
AFACT(TestClassName_ReturnsNoOpTestClassRunner)
AFACT(NumberOfTestCases_Returns0)
AFACT(RunTests_ReturnsDefaultTestClassResult)
AFACT(HasTestNameThatCaseInsensitiveMatchesPattern_ReturnsFalse)
EVIDENCE

NoOpTestClassRunner _noOpTestClassRunner;

TEST(TestClassName_ReturnsNoOpTestClassRunner)
{
   IS_NULL(_noOpTestClassRunner.TestClassName());
}

TEST(NumberOfTestCases_Returns0)
{
   ARE_EQUAL(0, _noOpTestClassRunner.NumberOfTestCases());
}

TEST(RunTests_ReturnsDefaultTestClassResult)
{
   ARE_EQUAL(TestClassResult(), _noOpTestClassRunner.RunTests());
}

TEST(HasTestNameThatCaseInsensitiveMatchesPattern_ReturnsFalse)
{
   IS_FALSE(_noOpTestClassRunner.HasTestNameThatCaseInsensitiveMatchesPattern(ZenUnit::Random<string>()));
}

}; RUNTESTS(NoOpTestClassRunnerTests)
