#pragma once

struct TestClassRunnerRunnerMock : public Zen::Mock<TestClassRunnerRunner>
{
   ZENMOCK_VOID1(AddTestClassRunner, TestClassRunner*)
   ZENMOCK_VOID1(ApplyRunFiltersIfAny, const vector<RunFilter>&)
   ZENMOCK_NONVOID0_CONST(size_t, NumberOfTestClassesToBeRun)
   ZENMOCK_NONVOID0_CONST(size_t, NumberOfTestCases)
   ZENMOCK_NONVOID1(vector<TestClassResult>, RunTestClasses, ZenUnitArgs&)
};