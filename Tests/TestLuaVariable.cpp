/******************************************************************************\
* TestLuaVariable.cpp                                                          *
* Unit tests for things declared in 'LuaVariable.hpp'.                         *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include "LuaState.hpp"


// - TestLuaVariableSubscriptOperator ------------------------------------------
void TestLuaVariableSubscriptOperator()
{
   using namespace Diluculum;

   LuaState ls;
   ls.doString ("a = 1; b = 'two'; c = true; d = { 'foo', 'bar', 'baz' }");

   // Check if the values can be read
   BOOST_CHECK (ls["a"] == 1);
   BOOST_CHECK (ls["b"] == "two");
   BOOST_CHECK (ls["c"] == true);
   BOOST_CHECK (ls["d"][1] == "foo");
   BOOST_CHECK (ls["d"][2] == "bar");
   BOOST_CHECK (ls["d"][3] == "baz");

   // Change some values and verify if they were really changed
   ls["a"] = false;
   ls["b"] = 3 * 7;
   ls["d"][2] = "two";
   ls["d"][3] = ls["b"];

   BOOST_CHECK (ls["a"] == false);
   BOOST_CHECK (ls["b"] == 21);
   BOOST_CHECK (ls["d"][2] == "two");
   BOOST_CHECK (ls["d"][3] == 21);

   // Add some new values, and read them
   LuaValueMap nestedLVM;
   nestedLVM[1] = "I";
   nestedLVM["six"] = "VI";
   nestedLVM[false] = 0;

   LuaValueMap lvm;
   lvm[1] = "eins";
   lvm[2] = "deux";
   lvm[3] = "três";
   lvm[4] = "cuatro";
   lvm[5] = "five";
   lvm[6] = nestedLVM;

   ls["d"][4] = 12.34;
   ls["d"][true] = lvm;
   ls["z"] = 56.78;

   BOOST_CHECK (ls["d"][4] == 12.34);
   BOOST_CHECK (ls["d"][true][1] == "eins");
   BOOST_CHECK (ls["d"][true][2] == "deux");
   BOOST_CHECK (ls["d"][true][3] == "três");
   BOOST_CHECK (ls["d"][true][4] == "cuatro");
   BOOST_CHECK (ls["d"][true][5] == "five");
   BOOST_CHECK (ls["d"][true][6][1] == "I");
   BOOST_CHECK (ls["d"][true][6]["six"] == "VI");
   BOOST_CHECK (ls["d"][true][6][false] == 0);
   BOOST_CHECK (ls["z"] == 56.78);
}



using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'LuaVariable' tests");
   test->add (BOOST_TEST_CASE (&TestLuaVariableSubscriptOperator));

   return test;
}
