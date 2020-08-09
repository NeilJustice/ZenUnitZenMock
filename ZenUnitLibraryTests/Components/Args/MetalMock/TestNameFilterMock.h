#pragma once

class TestNameFilterMock : public Zen::Mock<TestNameFilter>
{
public:
   METALMOCK_NONVOID1_CONST(bool, MatchesTestName, std::string_view)
   METALMOCK_NONVOID1_CONST(bool, MatchesTestClassName, std::string_view)
   METALMOCK_NONVOID3_CONST(bool, MatchesTestCase, std::string_view, std::string_view, size_t)
};
