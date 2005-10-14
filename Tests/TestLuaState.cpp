/******************************************************************************\
* TestLuaState.cpp                                                             *
* Unit tests for things declared in 'LuaState.hpp'.                            *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include "LuaState.hpp"


// - TestLuaStateDoStringMultiRet ----------------------------------------------
void TestLuaStateDoStringMultiRet()
{
   using namespace Diluculum;

   LuaState ls;

   const LuaRetVal nothing = ls.doStringMultRet ("a = 1 + 1 - 1");
   const LuaRetVal one = ls.doStringMultRet ("return a");
   const LuaRetVal oneTwo = ls.doStringMultRet ("return a, a*2");
   const LuaRetVal oneTwoThree = ls.doStringMultRet ("return a, a+1, 'three'");
   const LuaRetVal oneTwoThreeFalse = ls.doStringMultRet(
      "return a, a+a, 'three', a == 10");

   // Check the size of the returned data
   BOOST_CHECK (nothing.size() == 0);
   BOOST_CHECK (one.size() == 1);
   BOOST_CHECK (oneTwo.size() == 2);
   BOOST_CHECK (oneTwoThree.size() == 3);

   // Check the returned data
   BOOST_CHECK (one[0].asNumber() == 1.0);

   BOOST_CHECK (oneTwo[0].asNumber() == 1.0);
   BOOST_CHECK (oneTwo[1].asNumber() == 2.0);

   BOOST_CHECK (oneTwoThree[0].asNumber() == 1.0);
   BOOST_CHECK (oneTwoThree[1].asNumber() == 2.0);
   BOOST_CHECK (oneTwoThree[2].asString() == "three");

   BOOST_CHECK (oneTwoThreeFalse[0].asNumber() == 1.0);
   BOOST_CHECK (oneTwoThreeFalse[1].asNumber() == 2.0);
   BOOST_CHECK (oneTwoThreeFalse[2].asString() == "three");
   BOOST_CHECK (oneTwoThreeFalse[3].asBoolean() == false);
}



// - TestLuaStateDoExceptions --------------------------------------------------
void TestLuaStateDoExceptions()
{
   using namespace Diluculum;

   LuaState ls;

   BOOST_CHECK_THROW (ls.doStringMultRet ("@#$%#"), LuaSyntaxError);
}



using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'LuaState' tests");
   test->add (BOOST_TEST_CASE (&TestLuaStateDoStringMultiRet));
   test->add (BOOST_TEST_CASE (&TestLuaStateDoExceptions));

   return test;
}
