/******************************************************************************\
* TestLuaState.cpp                                                             *
* Unit tests for things declared in 'LuaState.hpp'.                            *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include "LuaState.hpp"


// - TestLuaStateDoFileMultRet -------------------------------------------------
void TestLuaStateDoFileMultRet()
{
   using namespace Diluculum;

   LuaState ls;
   const LuaRetVal ret = ls.doFileMultRet ("Tests/TestLuaStateDoFile.lua");

   BOOST_REQUIRE (ret.size() == 5);
   BOOST_CHECK (ret[0] == "foo");
   BOOST_CHECK (ret[1] == 43.21);
   BOOST_CHECK (ret[2][1] == true);
   BOOST_CHECK (ret[2][2] == "bar");
   BOOST_CHECK (ret[3] == "baz");
   BOOST_CHECK (ret[4] == 4.5);

   const LuaRetVal empty = ls.doFileMultRet(
      "Tests/TestLuaStateDoFileNoReturn.lua");
   BOOST_CHECK (empty.size() == 0);
}



// - TestLuaStateDoFile --------------------------------------------------------
void TestLuaStateDoFile()
{
   using namespace Diluculum;

   LuaState ls;
   const LuaValue ret = ls.doFile ("Tests/TestLuaStateDoFile.lua");

   BOOST_CHECK (ret == "foo");

   const LuaValue empty = ls.doFile("Tests/TestLuaStateDoFileNoReturn.lua");
   BOOST_CHECK (empty == Nil);
}



// - TestLuaStateDoStringMultRet -----------------------------------------------
void TestLuaStateDoStringMultRet()
{
   using namespace Diluculum;

   LuaState ls;

   const LuaRetVal nothing = ls.doStringMultRet ("a = 1 + 1 - 1");
   const LuaRetVal one = ls.doStringMultRet ("return a");
   const LuaRetVal oneTwo = ls.doStringMultRet ("return a, a*2");
   const LuaRetVal oneTwoThree = ls.doStringMultRet ("return a, a+1, 'three'");
   const LuaRetVal oneTwoThreeFalse = ls.doStringMultRet(
      "return a, a+a, 'three', a == 10");
   const LuaRetVal nestedTables = ls.doStringMultRet(
      "return { 'one', 2, { [1] = 'one', two = 2, [true] = 'foo' }, false }");

   // Check the size of the returned data
   BOOST_CHECK (nothing.size() == 0);
   BOOST_REQUIRE (one.size() == 1);
   BOOST_REQUIRE (oneTwo.size() == 2);
   BOOST_REQUIRE (oneTwoThree.size() == 3);
   BOOST_REQUIRE (nestedTables.size() == 1);

   // Check the returned data; first the easy cases...
   BOOST_CHECK (one[0] == 1);

   BOOST_CHECK (oneTwo[0] == 1);
   BOOST_CHECK (oneTwo[1] == 2);

   BOOST_CHECK (oneTwoThree[0] == 1);
   BOOST_CHECK (oneTwoThree[1] == 2);
   BOOST_CHECK (oneTwoThree[2] == "three");

   BOOST_CHECK (oneTwoThreeFalse[0] == 1);
   BOOST_CHECK (oneTwoThreeFalse[1] == 2);
   BOOST_CHECK (oneTwoThreeFalse[2] == "three");
   BOOST_CHECK (oneTwoThreeFalse[3] == false);

   // ...and now that more complicated table
   BOOST_CHECK (nestedTables[0][1] == "one");
   BOOST_CHECK (nestedTables[0][2] == 2);
   BOOST_CHECK (nestedTables[0][3][1.0].asString() == "one");
   BOOST_CHECK (nestedTables[0][3]["two"].asNumber() == 2);
   BOOST_CHECK (nestedTables[0][3][true].asString() == "foo");
   BOOST_CHECK (nestedTables[0][4] == false);
}



// - TestLuaStateDoString ------------------------------------------------------
void TestLuaStateDoString()
{
   using namespace Diluculum;

   LuaState ls;

   const LuaValue nothing = ls.doString ("a = math.cos(0)");
   const LuaValue one = ls.doString ("return a");
   const LuaValue two = ls.doString ("return 2*a, { }");
   const LuaValue three = ls.doString ("return 'three', true, 3.0");

   // Check the returned data
   BOOST_CHECK (nothing == Nil);
   BOOST_CHECK (one == 1);
   BOOST_CHECK (two == 2);
   BOOST_CHECK (three == "three");
}



// - TestLuaStateDoExceptions --------------------------------------------------
void TestLuaStateDoExceptions()
{
   using namespace Diluculum;

   LuaState ls;

   // Force a syntax error
   BOOST_CHECK_THROW (ls.doStringMultRet ("@#$%#"), LuaSyntaxError);
   BOOST_CHECK_THROW (ls.doString ("@#$%#"), LuaSyntaxError);

   BOOST_CHECK_THROW (ls.doFileMultRet ("Tests/SyntaxError.lua"),
                      LuaSyntaxError);
   BOOST_CHECK_THROW (ls.doFile ("Tests/SyntaxError.lua"), LuaSyntaxError);

   // "Lua threads" (coroutines) are not supported by 'LuaValue'. Trying to
   // return one must generate an error.
   BOOST_CHECK_THROW (ls.doStringMultRet(
                         "return coroutine.create(function() end)"),
                      LuaTypeError);

   BOOST_CHECK_THROW (ls.doString ("return coroutine.create(function() end)"),
                      LuaTypeError);

   BOOST_CHECK_THROW (ls.doFileMultRet ("Tests/ReturnThread.lua"),
                      LuaTypeError);

   BOOST_CHECK_THROW (ls.doFile ("Tests/ReturnThread.lua"), LuaTypeError);

   // We are expected to throw in "file not found" situations
   BOOST_CHECK_THROW (ls.doFileMultRet ("__THiis_fi1e.doeSNt--exIst.lua"),
                      LuaFileError);

   BOOST_CHECK_THROW (ls.doFile ("__THiis_fi1e.doeSNt--exIst.lua"),
                      LuaFileError);

}



// - TestLuaStateSubscriptOperator ---------------------------------------------
void TestLuaStateSubscriptOperator()
{
   // Some related, but more stressing tests are done in 'TestLuaVariable.cpp'.

   using namespace Diluculum;

   LuaState ls;
   ls.doString ("a = 1; b = 'two'; c = true; d = { 'foo', 3.1415, a = 'baz' }");

   LuaValueMap lvm;
   lvm[1] = "foo";
   lvm[2] = 3.1415;
   lvm["a"] = "baz";

   // Check if the values can be read
   BOOST_CHECK (ls["a"] == 1);
   BOOST_CHECK (ls["b"] == "two");
   BOOST_CHECK (ls["c"] == true);
   BOOST_CHECK (ls["d"] == lvm);

   // Change some values and verify if they were really changed
   ls["a"] = ls["a"].value().asNumber() + 1;
   ls["b"] = ls["d"]["a"].value();

   BOOST_CHECK (ls["a"] == 2);
   BOOST_CHECK (ls["b"] == "baz");

   // Add new values, and read them
   ls["d"][4] = 12.34;
   ls["z"] = 56.78;

   BOOST_CHECK (ls["d"][4] == 12.34);
   BOOST_CHECK (ls["z"] == 56.78);
}



using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'LuaState' tests");
   test->add (BOOST_TEST_CASE (&TestLuaStateDoFileMultRet));
   test->add (BOOST_TEST_CASE (&TestLuaStateDoFile));
   test->add (BOOST_TEST_CASE (&TestLuaStateDoStringMultRet));
   test->add (BOOST_TEST_CASE (&TestLuaStateDoString));
   test->add (BOOST_TEST_CASE (&TestLuaStateDoExceptions));
   test->add (BOOST_TEST_CASE (&TestLuaStateSubscriptOperator));

   return test;
}
