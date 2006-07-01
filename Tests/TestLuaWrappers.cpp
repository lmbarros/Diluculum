/******************************************************************************\
* TestLuaWrappers.cpp                                                          *
* Unit tests for things declared in 'LuaWrappers.hpp'.                         *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaWrappers.hpp>
#include "WrappedClasses.hpp"
#include "WrappedFunctions.hpp"


// - TestFunctionWrapping ------------------------------------------------------
void TestFunctionWrapping()
{
   using namespace Diluculum;
   LuaState ls;

   // The functions used below are defined in 'CLuaFunctions.hpp', which was
   // included in this file. So, we just have to make them available in the
   // 'LuaState' used in this test.
   ls["ZeroTheGlobal"] = DILUCULUM_WRAPPER_FUNCTION (ZeroTheGlobal);
   ls["SetTheGlobal"] = DILUCULUM_WRAPPER_FUNCTION (SetTheGlobal);
   ls["SetTheGlobalToSum"] = DILUCULUM_WRAPPER_FUNCTION (SetTheGlobalToSum);
   ls["ConcatenateThree"] = DILUCULUM_WRAPPER_FUNCTION (ConcatenateThree);
   ls["FibonacciSequence"] = DILUCULUM_WRAPPER_FUNCTION (FibonacciSequence);
   ls["ToOrFromString"] = DILUCULUM_WRAPPER_FUNCTION (ToOrFromString);

   // Here we go...
   Diluculum::LuaValueList res;
   TheGlobal = 171;

   ls.doString ("ZeroTheGlobal()");
   BOOST_CHECK (res.size() == 0);
   BOOST_CHECK (TheGlobal == 0);

   ls.doString ("SetTheGlobal (345)");
   BOOST_CHECK (res.size() == 0);
   BOOST_CHECK (TheGlobal == 345);

   ls.doString ("SetTheGlobalToSum()");
   BOOST_CHECK (res.size() == 0);
   BOOST_CHECK (TheGlobal == 0);

   ls.doString ("SetTheGlobalToSum (3, 8, -5, 22, -11, 7)");
   BOOST_CHECK (res.size() == 0);
   BOOST_CHECK (TheGlobal == 24);

   ls.doString ("SetTheGlobalToSum (171)");
   BOOST_CHECK (res.size() == 0);
   BOOST_CHECK (TheGlobal == 171);

   res = ls.doString ("return ConcatenateThree ('one ', 'um ', 'eins')");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0].asString() == "one um eins");

   res = ls.doString ("return FibonacciSequence (2)");
   BOOST_REQUIRE (res.size() == 2);
   BOOST_CHECK (res[0].asNumber() == 1);
   BOOST_CHECK (res[1].asNumber() == 1);

   res = ls.doString ("return FibonacciSequence (10)");
   BOOST_REQUIRE (res.size() == 10);
   BOOST_CHECK (res[0].asNumber() == 1);
   BOOST_CHECK (res[1].asNumber() == 1);
   BOOST_CHECK (res[2].asNumber() == 2);
   BOOST_CHECK (res[3].asNumber() == 3);
   BOOST_CHECK (res[4].asNumber() == 5);
   BOOST_CHECK (res[5].asNumber() == 8);
   BOOST_CHECK (res[6].asNumber() == 13);
   BOOST_CHECK (res[7].asNumber() == 21);
   BOOST_CHECK (res[8].asNumber() == 34);
   BOOST_CHECK (res[9].asNumber() == 55);

   res = ls.doString ("return FibonacciSequence (6)");
   BOOST_REQUIRE (res.size() == 6);
   BOOST_CHECK (res[0].asNumber() == 1);
   BOOST_CHECK (res[1].asNumber() == 1);
   BOOST_CHECK (res[2].asNumber() == 2);
   BOOST_CHECK (res[3].asNumber() == 3);
   BOOST_CHECK (res[4].asNumber() == 5);
   BOOST_CHECK (res[5].asNumber() == 8);

   res = ls.doString ("return ToOrFromString (1)");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0].asString() == "one");

   res = ls.doString ("return ToOrFromString (3)");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0].asString() == "three");

   res = ls.doString ("return ToOrFromString ('one')");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0].asNumber() == 1);

   res = ls.doString ("return ToOrFromString ('two')");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0].asNumber() == 2);
}



// - TestClassWrapping ---------------------------------------------------------
void TestClassWrapping()
{
   using namespace Diluculum;
   LuaState ls;

   // First, make the desired class available in our 'LuaState'
   DILUCULUM_REGISTER_CLASS (ls, Account);

   // In Lua, create two instances of the class and call some methods
   ls.doString ("a1 = Account.new()");
   ls.doString ("a2 = Account.new(123.45)");

   Diluculum::LuaValueList ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0].asNumber() == 0.0);

   ret = ls.doString ("return a2:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0].asNumber() == 123.45);

   ls.doString ("a1:deposit (55.66)");
   ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0].asNumber() == 55.66);

   ls.doString ("a1:withdraw (15.66)");
   ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0].asNumber() == 40.0);

   ls.doString ("a2:withdraw (0.45)");
   ret = ls.doString ("return a2:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0].asNumber() == 123.0);

   // Now, create an instance in C++, make it available in our 'LuaState', call
   // its methods in Lua and C++, and check if the things really change on the
   // other side
   LuaValueList params;
   params.push_back (50.0);
   Account aCppAccount (params);

   DILUCULUM_REGISTER_OBJECT (ls["a3"], Account, aCppAccount);

   ls.doString ("a3:deposit (50.0)");
   BOOST_CHECK (aCppAccount.balance (LuaValueList())[0] == 100.0);

   params.clear();
   params.push_back (110.0);
   aCppAccount.withdraw (params);
   ret = ls.doString ("return a3:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0].asNumber() == -10.0);
}



// - TestTwoClasses ------------------------------------------------------------
void TestTwoClasses()
{
   using namespace Diluculum;

   LuaState ls;

   // Register two classes here, to ensure that no conflicts arise
   DILUCULUM_REGISTER_CLASS (ls, Account);
   DILUCULUM_REGISTER_CLASS (ls, NumberProperties);

   ls.doString ("a = Account.new (1000.01)");
   ls.doString ("n = NumberProperties.new (1234)");

   // Try calling some 'Account' methods
   LuaValueList ret = ls.doString ("return a:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0].asNumber() == 1000.01);

   ls.doString ("a:deposit (0.09)");
   ls.doString ("a:withdraw (500.0)");
   ret = ls.doString ("return a:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0].asNumber() == 500.10);

   // And now, some 'NumberProperties' methods
   ret = ls.doString ("return n:isEven()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0].asBoolean() == true);

   ret = ls.doString ("return n:isOdd()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0].asBoolean() == false);

   ret = ls.doString ("return n:isBig()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0].asBoolean() == true);
}



// - TestClassDestructor -------------------------------------------------------
void TestClassDestructor()
{
   using namespace Diluculum;

   // First case: an object is instantiated from Lua. After the 'LuaState' in
   // which it lives is destroyed, the object is expected to have been
   // garbage-collected and its destructor called.
   DestructorTester::aFlag = false;

   {
      LuaState ls;
      DILUCULUM_REGISTER_CLASS (ls, DestructorTester);
      ls.doString ("foo = DestructorTester.new()");

      // Just to be paranoid, ensure that 'aFlag' is still false
      BOOST_REQUIRE (DestructorTester::aFlag == false);
   }

   BOOST_CHECK (DestructorTester::aFlag == true);

   // Second case: the object is instantiated in C++ and exported to a
   // 'LuaState'. In this case, the object's destruction is responsibility of
   // the programmer on the C++ side. So, the destructor should not be called
   // after the object is garbage-collected in Lua.
   DestructorTester::aFlag = false;
   LuaValueList params;
   DestructorTester dt (params);

   {
      LuaState ls;
      DILUCULUM_REGISTER_CLASS (ls, DestructorTester);

      DILUCULUM_REGISTER_OBJECT (ls["dt"], DestructorTester, dt);

      // Do something to assert that the object is there
      LuaValueList ret = ls.doString ("return type (dt)");
      BOOST_REQUIRE (ret.size() == 1);
      BOOST_REQUIRE (ret[0].asString() == "userdata");

      // Just to be paranoid, ensure that 'aFlag' is still false
      BOOST_REQUIRE (DestructorTester::aFlag == false);
   }

   BOOST_CHECK (DestructorTester::aFlag == false);
}



using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'LuaWrappers' tests");
   test->add (BOOST_TEST_CASE (&TestFunctionWrapping));
   test->add (BOOST_TEST_CASE (&TestClassWrapping));
   test->add (BOOST_TEST_CASE (&TestTwoClasses));
   test->add (BOOST_TEST_CASE (&TestClassDestructor));

   return test;
}
