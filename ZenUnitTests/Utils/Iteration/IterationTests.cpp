#include "pch.h"
// Windows-only tests until Travis CI no longer runs out of memory
#if _WIN32

namespace ZenUnit
{
   template<
      template<typename...>
   typename CollectionType,
   typename T, typename Arg2Type>
   TEMPLATE_TESTS(TwoArgMemberAnyerTests, CollectionType, T, Arg2Type)
   AFACT(TwoArgAny_CollectionEmpty_ReturnsFalse)
   AFACT(TwoArgAny_1ElementCollection_ElementDoesNotMatchPredicate_ReturnsFalse)
   AFACT(TwoArgAny_2ElementCollection_FirstOfTwoElementsMatchesPredicate_CallsPredicateOnce_ReturnsTrue)
   AFACT(TwoArgAny_2ElementCollection_SecondOfTwoElementsMatchesPredicate_CallsPredicateTwice_ReturnsTrue)
   AFACT(TwoArgAny_2ElementCollection_NeitherElementMatchesPredicate_CallsPredicateTwice_ReturnsFalse)
   EVIDENCE

   class Class
   {
   public:
      mutable vector<pair<T, Arg2Type>> predicateArgs;
   public:
      [[noreturn]]
      bool PredicateThatThrowsIfCalled(const T&, const Arg2Type&) const
      {
         throw invalid_argument("unexpected call");
      };

      bool AlwaysFalsePredicate(const T& element, const Arg2Type& arg2) const
      {
         predicateArgs.emplace_back(element, arg2);
         return false;
      };

      bool ElementValueOneReturnsTruePredicate(const T& element, const Arg2Type& arg2) const
      {
         predicateArgs.emplace_back(element, arg2);
         if (element == 1)
         {
            return true;
         }
         return false;
      };

      bool ElementValueTwoReturnsTruePredicate(const T& element, const Arg2Type& arg2) const
      {
         predicateArgs.emplace_back(element, arg2);
         if (element == 2)
         {
            return true;
         }
         return false;
      };
   };

   Class _classInstance;
   CollectionType<T> _twoCollection = { 1, 2 };
   Arg2Type _arg2;

   STARTUP
   {
      _arg2 = ZenUnit::Random<Arg2Type>();
   }

   TEST(TwoArgAny_CollectionEmpty_ReturnsFalse)
   {
      const CollectionType<T> emptyCollection;
      const TwoArgMemberAnyer<CollectionType<T>, Class,
         decltype(&Class::PredicateThatThrowsIfCalled), const Arg2Type&> twoArgMemberAnyer;
      //
      const bool anyElementMatchesPredicate = twoArgMemberAnyer.TwoArgAny(
         emptyCollection, &_classInstance, &Class::PredicateThatThrowsIfCalled, Arg2Type{});
      //
      IS_EMPTY(_classInstance.predicateArgs);
      IS_FALSE(anyElementMatchesPredicate);
   }

   TEST(TwoArgAny_1ElementCollection_ElementDoesNotMatchPredicate_ReturnsFalse)
   {
      const CollectionType<T> collection = { 1 };
      const TwoArgMemberAnyer<CollectionType<T>, Class,
         decltype(&Class::AlwaysFalsePredicate), const Arg2Type&> twoArgMemberAnyer;
      //
      const bool anyElementMatchesPredicate = twoArgMemberAnyer.TwoArgAny(
         collection, &_classInstance, &Class::AlwaysFalsePredicate, _arg2);
      //
      const vector<pair<T, Arg2Type>> expectedPredicateArgs = { { T(1), _arg2 } };
      VECTORS_EQUAL(expectedPredicateArgs, _classInstance.predicateArgs);
      IS_FALSE(anyElementMatchesPredicate);
   }

   TEST(TwoArgAny_2ElementCollection_FirstOfTwoElementsMatchesPredicate_CallsPredicateOnce_ReturnsTrue)
   {
      const TwoArgMemberAnyer<CollectionType<T>, Class,
         decltype(&Class::ElementValueOneReturnsTruePredicate), const Arg2Type&> twoArgMemberAnyer;
      //
      const bool anyElementMatchesPredicate = twoArgMemberAnyer.TwoArgAny(
         _twoCollection, &_classInstance, &Class::ElementValueOneReturnsTruePredicate, _arg2);
      //
      const vector<pair<T, Arg2Type>> expectedPredicateArgs = { { T(1), _arg2 } };
      VECTORS_EQUAL(expectedPredicateArgs, _classInstance.predicateArgs);
      IS_TRUE(anyElementMatchesPredicate);
   }

   TEST(TwoArgAny_2ElementCollection_SecondOfTwoElementsMatchesPredicate_CallsPredicateTwice_ReturnsTrue)
   {
      const TwoArgMemberAnyer<CollectionType<T>, Class,
         decltype(&Class::ElementValueTwoReturnsTruePredicate), const Arg2Type&> twoArgMemberAnyer;
      //
      const bool anyElementMatchesPredicate = twoArgMemberAnyer.TwoArgAny(
         _twoCollection, &_classInstance, &Class::ElementValueTwoReturnsTruePredicate, _arg2);
      ////
      const vector<pair<T, Arg2Type>> expectedPredicateArgs = { { T(1), _arg2 },{ T(2), _arg2 } };
      VECTORS_EQUAL(expectedPredicateArgs, _classInstance.predicateArgs);
      IS_TRUE(anyElementMatchesPredicate);
   }

   TEST(TwoArgAny_2ElementCollection_NeitherElementMatchesPredicate_CallsPredicateTwice_ReturnsFalse)
   {
      const TwoArgMemberAnyer<CollectionType<T>, Class,
         decltype(&Class::AlwaysFalsePredicate), const Arg2Type&> twoArgMemberAnyer;
      //
      const bool anyElementMatchesPredicate = twoArgMemberAnyer.TwoArgAny(
         _twoCollection, &_classInstance, &Class::AlwaysFalsePredicate, _arg2);
      //
      const vector<pair<T, Arg2Type>> expectedPredicateArgs = { { T(1), _arg2 },{ T(2), _arg2 } };
      VECTORS_EQUAL(expectedPredicateArgs, _classInstance.predicateArgs);
      IS_FALSE(anyElementMatchesPredicate);
   }

   };
   RUN_TEMPLATE_TESTS(TwoArgMemberAnyerTests, vector, int, int)
   RUN_TEMPLATE_TESTS(TwoArgMemberAnyerTests, set, long long, unsigned char)


   template<typename ElementType, typename Arg2Type>
   TEMPLATE_TESTS(TwoArgMemberForEacherTests, ElementType, Arg2Type)
   AFACT(TwoArgMemberForEach_EmptyCollection_DoesNotCallFunc)
   AFACT(TwoArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
   AFACT(TwoArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
   AFACT(RandomTwoArgMemberForEach_EmptyCollection_DoesNotCallFunc)
   AFACT(RandomTwoArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
   AFACT(RandomTwoArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwiceInRandomOrder)
   AFACT(CodeCoverage_ClassTypeTwoArgFunction)
   EVIDENCE

   class ClassType
   {
   public:
      virtual void TwoArgFunction(ElementType, Arg2Type) {}
      virtual ~ClassType() = default;
   };

   class ClassTypeMock : public Zen::Mock<ClassType>
   {
   public:
      vector<ElementType> vec;
      ZENMOCK_VOID2(TwoArgFunction, ElementType, Arg2Type)
   };

   using TwoArgMemberForEacherType = TwoArgMemberForEacher<
      ElementType, ClassType, void (ClassType::*)(ElementType, Arg2Type), Arg2Type>;

   TwoArgMemberForEacherType _twoArgMemberForEacher;

   TEST(TwoArgMemberForEach_EmptyCollection_DoesNotCallFunc)
   {
      ClassTypeMock classInstance{};
      _twoArgMemberForEacher.TwoArgMemberForEach(
         &classInstance.vec, &classInstance, &ClassType::TwoArgFunction, 0);
   }

   TEST(TwoArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
   {
      ClassTypeMock classInstance;
      classInstance.vec = { 1 };
      classInstance.TwoArgFunctionMock.Expect();
      //
      _twoArgMemberForEacher.TwoArgMemberForEach(
         &classInstance.vec, &classInstance, &ClassType::TwoArgFunction, 10);
      //
      classInstance.TwoArgFunctionMock.CalledOnceWith(1, 10);
   }

   TEST(TwoArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
   {
      ClassTypeMock classInstance;
      classInstance.vec = { 1, 2 };
      classInstance.TwoArgFunctionMock.Expect();
      //
      _twoArgMemberForEacher.TwoArgMemberForEach(
         &classInstance.vec, &classInstance, &ClassType::TwoArgFunction, 20);
      //
      classInstance.TwoArgFunctionMock.CalledAsFollows(
         {
            { 1, 20 },
         { 2, 20 }
         });
   }

   TEST(RandomTwoArgMemberForEach_EmptyCollection_DoesNotCallFunc)
   {
      ClassTypeMock classInstance{};
      _twoArgMemberForEacher.RandomTwoArgMemberForEach(
         &classInstance.vec, &classInstance, &ClassType::TwoArgFunction, 0, 0);
   }

   TEST(RandomTwoArgMemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
   {
      ClassTypeMock classInstance;
      classInstance.vec = { 1 };
      classInstance.TwoArgFunctionMock.Expect();
      //
      _twoArgMemberForEacher.RandomTwoArgMemberForEach(
         &classInstance.vec, &classInstance, &ClassType::TwoArgFunction, 10, 0);
      //
      classInstance.TwoArgFunctionMock.CalledOnceWith(1, 10);
   }

   TEST(RandomTwoArgMemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwiceInRandomOrder)
   {
      ClassTypeMock classInstance;
      classInstance.vec = { 1, 2 };
      classInstance.TwoArgFunctionMock.Expect();
      //
      _twoArgMemberForEacher.RandomTwoArgMemberForEach(
         &classInstance.vec, &classInstance, &ClassType::TwoArgFunction, 20,
         static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));
      //
      try
      {
         classInstance.TwoArgFunctionMock.CalledAsFollows(
            {
               { 1, 20 },
            { 2, 20 }
            });
      }
      catch (const exception&)
      {
         classInstance.TwoArgFunctionMock.CalledAsFollows(
            {
               { 2, 20 },
            { 1, 20 }
            });
      }
   }

   TEST(CodeCoverage_ClassTypeTwoArgFunction)
   {
      ClassType classType;
      classType.TwoArgFunction(ElementType{}, Arg2Type{});
   };

   };
   RUN_TEMPLATE_TESTS(TwoArgMemberForEacherTests, int, int)
   RUN_TEMPLATE_TESTS(TwoArgMemberForEacherTests, double, char)


   template<
      template<typename T, typename...>
   class CollectionType, typename ElementType>
   TEMPLATE_TESTS(MemberForEacherTests, CollectionType, ElementType)
   AFACT(MemberForEach_EmptyCollection_DoesNotCallFunc)
   AFACT(MemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
   AFACT(MemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
   AFACT(CodeCoverage_ClassTypeFunc)
   EVIDENCE

   class ClassType
   {
   public:
      virtual void Func(ElementType) const {}
      virtual ~ClassType() = default;
   };

   class ClassTypeMock : public Zen::Mock<ClassType>
   {
   public:
      CollectionType<ElementType> collection;
      ZENMOCK_VOID1_CONST(Func, ElementType)
   };

   using MemberForEacherType = MemberForEacher<
      CollectionType<ElementType>, ClassType, void (ClassType::*)(ElementType) const>;
   MemberForEacherType _memberForEacher;

   TEST(MemberForEach_EmptyCollection_DoesNotCallFunc)
   {
      const ClassTypeMock classInstance{};
      _memberForEacher.MemberForEach(&classInstance.collection, &classInstance, &ClassType::Func);
   }

   TEST(MemberForEach_OneItemCollection_CallsThisPointerBoundFuncOnce)
   {
      ClassTypeMock classInstance;
      classInstance.collection = { 1 };
      classInstance.FuncMock.Expect();
      //
      _memberForEacher.MemberForEach(
         &classInstance.collection, &classInstance, &ClassType::Func);
      //
      classInstance.FuncMock.CalledOnceWith(1);
   }

   TEST(MemberForEach_TwoItemCollection_CallsThisPointerBoundFuncTwice)
   {
      ClassTypeMock classInstance;
      classInstance.collection = { 1, 2 };
      classInstance.FuncMock.Expect();
      //
      _memberForEacher.MemberForEach(
         &classInstance.collection, &classInstance, &ClassType::Func);
      //
      classInstance.FuncMock.CalledAsFollows(
         {
            1, 2
         });
   }

   TEST(CodeCoverage_ClassTypeFunc)
   {
      ClassType classType;
      classType.Func(ElementType{});
   }

   };
   RUN_TEMPLATE_TESTS(MemberForEacherTests, vector, int)
   RUN_TEMPLATE_TESTS(MemberForEacherTests, vector, double)
   RUN_TEMPLATE_TESTS(MemberForEacherTests, set, int)
   RUN_TEMPLATE_TESTS(MemberForEacherTests, set, double)


   template<typename T, typename TransformedT>
   TEMPLATE_TESTS(TransformerTests, T, TransformedT)
   AFACT(Transform_EmptyRange_DoesNothing)
   AFACT(Transform_OneItemRange_CallsTransformerOnce)
   AFACT(Transform_TwoItemRange_CallsTransformerTwice)
   AFACT(RandomTransform_EmptyRange_DoesNothing)
   AFACT(RandomTransform_OneItemRange_CallsTransformerOnce)
   AFACT(RandomTransform_ThreeItemRange_CallsTransformerThreeTimesInRandomOrder)
   EVIDENCE

   using TransformerType = Transformer<T, TransformedT>;
   TransformerType _transformer;

   static TransformedT PlusOne(const T& element)
   {
      return element + 1;
   }

   TEST(Transform_EmptyRange_DoesNothing)
   {
      const vector<T> source;
      //
      const vector<TransformedT> dest = _transformer.Transform(&source, PlusOne);
      //
      IS_EMPTY(dest);
   }

   TEST(Transform_OneItemRange_CallsTransformerOnce)
   {
      const vector<T> source{ 1 };
      //
      const vector<TransformedT> dest = _transformer.Transform(&source, PlusOne);
      //
      VECTORS_EQUAL(vector<TransformedT>{ 2 }, dest);
   }

   TEST(Transform_TwoItemRange_CallsTransformerTwice)
   {
      const vector<T> source{ 1, 2 };
      //
      const vector<TransformedT> dest = _transformer.Transform(&source, PlusOne);
      //
      VECTORS_EQUAL((vector<TransformedT>{ 2, 3 }), dest);
   }

   TEST(RandomTransform_EmptyRange_DoesNothing)
   {
      vector<T> source;
      //
      const vector<TransformedT> dest = _transformer.RandomTransform(&source, PlusOne, 0);
      //
      IS_EMPTY(dest);
   }

   TEST(RandomTransform_OneItemRange_CallsTransformerOnce)
   {
      vector<T> source{ 1 };
      //
      const vector<TransformedT> dest = _transformer.RandomTransform(&source, PlusOne, 0);
      //
      VECTORS_EQUAL(vector<TransformedT>{ 2 }, dest);
   }

   TEST(RandomTransform_ThreeItemRange_CallsTransformerThreeTimesInRandomOrder)
   {
      vector<T> source{ 1, 2, 3 };
      //
      const vector<TransformedT> dest = _transformer.RandomTransform(&source, PlusOne,
         static_cast<unsigned short>(chrono::system_clock::now().time_since_epoch().count()));
      //
      ARE_EQUAL(3, dest.size());
      IS_TRUE(
         (dest == vector<TransformedT>{2, 3, 4}) ||
         (dest == vector<TransformedT>{2, 4, 3}) ||
         (dest == vector<TransformedT>{3, 2, 4}) ||
         (dest == vector<TransformedT>{3, 4, 2}) ||
         (dest == vector<TransformedT>{4, 2, 3}) ||
         (dest == vector<TransformedT>{4, 3, 2}));
   }

   };
   RUN_TEMPLATE_TESTS(TransformerTests, int, long long)
   RUN_TEMPLATE_TESTS(TransformerTests, unsigned long long, unsigned long long)


   template<typename ElementType, typename TransformedElementType>
   TEMPLATE_TESTS(MemberFunctionTransformerTests, ElementType, TransformedElementType)
   AFACT(Transform_ElementsEmpty_DoesNotCallTransformFunction_ReturnsEmptyVector)
   AFACT(Transform_TwoElements_CallsTransformFunctionOnEachElement_ReturnsTransformedElements)
   EVIDENCE

   class Class
   {
   public:
      mutable vector<ElementType> calls;

      TransformedElementType ConstMemberFunctionThatThrowsIfCalled(const ElementType&) const
      {
         throw runtime_error("Expected call to ConstMemberFunctionThatThrows()");
      }

      TransformedElementType ConstMemberFunction(const ElementType& element) const
      {
         calls.emplace_back(element);
         const TransformedElementType transformedElement = static_cast<TransformedElementType>(element + ElementType{ 1 });
         return transformedElement;
      }
   };

   MemberFunctionTransformer<Class, ElementType, TransformedElementType> _memberFunctionTransformer;
   const Class _classInstance;

   TEST(Transform_ElementsEmpty_DoesNotCallTransformFunction_ReturnsEmptyVector)
   {
      vector<ElementType> emptyElements;
      //
      const vector<TransformedElementType> transformedElements = _memberFunctionTransformer.Transform(
         emptyElements, &_classInstance, &Class::ConstMemberFunctionThatThrowsIfCalled);
      //
      IS_EMPTY(transformedElements);

      THROWS(_classInstance.ConstMemberFunctionThatThrowsIfCalled(ElementType{}), runtime_error,
         "Expected call to ConstMemberFunctionThatThrows()"); // 100% code coverage
   }

   TEST(Transform_TwoElements_CallsTransformFunctionOnEachElement_ReturnsTransformedElements)
   {
      vector<ElementType> elements = { ElementType{ 1 }, ElementType{ 2 } };
      //
      const vector<TransformedElementType> transformedElements = _memberFunctionTransformer.Transform(
         elements, &_classInstance, &Class::ConstMemberFunction);
      //
      vector<ElementType> expectedCalls =
      {
         ElementType{ 1 },
         ElementType{ 2 }
      };
      VECTORS_EQUAL(expectedCalls, _classInstance.calls);

      vector<TransformedElementType> expectedTransformedElements =
      {
         static_cast<TransformedElementType>(elements[0] + 1),
         static_cast<TransformedElementType>(elements[1] + 1)
      };
      VECTORS_EQUAL(expectedTransformedElements, transformedElements);
   }

   };
   RUN_TEMPLATE_TESTS(MemberFunctionTransformerTests, int, int)
   RUN_TEMPLATE_TESTS(MemberFunctionTransformerTests, unsigned, double)
}

#endif
