#pragma once

class TestResultMock : public Metal::Mock<TestResult>
{
public:
   METALMOCK_VOID1_CONST(WriteLineOKIfSuccess, const Console*)
   METALMOCK_VOID2_CONST(PrintIfFailure, const Console*, TestFailureNumberer*)
};
