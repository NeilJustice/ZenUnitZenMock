#pragma once

class TestFunctionClass
{
public:
   void TestFunction() {}
};

class TestingTestClass : public Zen::Mock<TestClass<TestingTestClass>>, public TestFunctionClass
{
public:
   static bool s_destructorCalled;
   static bool s_allNXNTestsRegistered;

   TestingTestClass() noexcept
   {
      s_destructorCalled = false;
   }

   ~TestingTestClass() override
   {
      s_destructorCalled = true;
   }

   ZENMOCK_VOID0(Startup)
   ZENMOCK_VOID0(Cleanup)
   ZENMOCK_VOID0_NONVIRTUAL(TestFunction)
};
