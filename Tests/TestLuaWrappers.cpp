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

   DILUCULUM_REGISTER_CLASS (ls, Account);

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
}



// - TestClassDestructor -------------------------------------------------------
void TestClassDestructor()
{
   using namespace Diluculum;

   DestructorTester::aFlag = false;

   {
      LuaState ls;
      DILUCULUM_REGISTER_CLASS (ls, DestructorTester);
      ls.doString ("foo = DestructorTester.new()");

      // Just to be paranoid, ensure that 'aFlag' is still false
      BOOST_REQUIRE (DestructorTester::aFlag == false);
   }

   // Here, 'ls' no longer exists. The Lua variable 'foo' should have been
   // garbage collected, and its C++ destructor called.
   BOOST_CHECK (DestructorTester::aFlag == true);
}



using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'LuaWrappers' tests");
   test->add (BOOST_TEST_CASE (&TestFunctionWrapping));
   test->add (BOOST_TEST_CASE (&TestClassWrapping));
   test->add (BOOST_TEST_CASE (&TestClassDestructor));

   return test;
}
