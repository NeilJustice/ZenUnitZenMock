#include "pch.h"

namespace ZenMock
{
   template<typename ExceptionType>
   TEMPLATETESTS(TemplateThrowableTests, ExceptionType)
   AFACT(New_ReturnsNewInstanceOfSelfAsThrowablePointerWithExceptionCreatedFromExceptionArgs)
   AFACT(Throw_ExceptionIsNullptr_DoesNothing)
   AFACT(Throw_ExceptionIsNotNullptr_ThrowsTheException)
   EVIDENCE

   TemplateThrowable<ExceptionType> _templateThrowable;
   const string ExceptionWhat = "ExceptionWhat";

   TEST(New_ReturnsNewInstanceOfSelfAsThrowablePointerWithExceptionCreatedFromExceptionArgs)
   {
      const Throwable* const throwable = TemplateThrowable<ExceptionType>::New(ExceptionWhat);
      THROWS(throwable->Throw(), ExceptionType, ExceptionWhat);
      delete throwable;
   }

   TEST(Throw_ExceptionIsNullptr_DoesNothing)
   {
      _templateThrowable.Throw();
   }

   TEST(Throw_ExceptionIsNotNullptr_ThrowsTheException)
   {
      _templateThrowable._exception = make_unique<ExceptionType>(ExceptionWhat);
      THROWS(_templateThrowable.Throw(), ExceptionType, ExceptionWhat);
   }

   };
   RUNTEMPLATETESTS(TemplateThrowableTests, runtime_error)
   RUNTEMPLATETESTS(TemplateThrowableTests, logic_error)
}
