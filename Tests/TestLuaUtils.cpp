/******************************************************************************\
* TestLuaUtils.cpp                                                             *
* Unit tests for things declared in 'LuaUtils.hpp'.                            *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include <cstring>
#include "LuaState.hpp"
#include "LuaUtils.hpp"


// - TestToLuaValue ------------------------------------------------------------
void TestToLuaValue()
{
   using namespace Diluculum;

   lua_State* ls = lua_open();

   lua_pushboolean (ls, true);
   lua_pushnumber (ls, 171.171);
   lua_pushstring (ls, "The book is on the table.");
   lua_pushnil (ls);

   LuaValue lvTrue (true);
   LuaValue lv171 (171.171);
   LuaValue lvTBIOTT ("The book is on the table.");

   // Try reading using positive (absolute) indexes
   BOOST_CHECK (ToLuaValue (ls, 1) == lvTrue);
   BOOST_CHECK (ToLuaValue (ls, 2) == lv171);
   BOOST_CHECK (ToLuaValue (ls, 3) == lvTBIOTT);
   BOOST_CHECK (ToLuaValue (ls, 4) == Nil);

   // And now, try with negative indexes
   BOOST_CHECK (ToLuaValue (ls, -1) == Nil);
   BOOST_CHECK (ToLuaValue (ls, -2) == lvTBIOTT);
   BOOST_CHECK (ToLuaValue (ls, -3) == lv171);
   BOOST_CHECK (ToLuaValue (ls, -4) == lvTrue);

   // Just to be complete, do everything once more using "constants"
   BOOST_CHECK (ToLuaValue (ls, 1) == true);
   BOOST_CHECK (ToLuaValue (ls, 2) == 171.171);
   BOOST_CHECK (ToLuaValue (ls, 3) == "The book is on the table.");
   BOOST_CHECK (ToLuaValue (ls, -2) == "The book is on the table.");
   BOOST_CHECK (ToLuaValue (ls, -3) == 171.171);
   BOOST_CHECK (ToLuaValue (ls, -4) == true);

   // Ensure that trying to convert unsupported types throws an exception
   lua_newthread (ls);
   BOOST_CHECK_THROW (ToLuaValue (ls, -1), LuaTypeError);

   // Close the Lua state used in this test
   lua_close (ls);
}



// - TestPushLuaValue ----------------------------------------------------------
void TestPushLuaValue()
{
   using namespace Diluculum;

   lua_State* ls = lua_open();

   PushLuaValue (ls, Nil);
   PushLuaValue (ls, false);
   PushLuaValue (ls, "The sky is blue.");
   PushLuaValue (ls, 2.7183);

   // Check if the values were properly pushed
   BOOST_CHECK (lua_isnil (ls, 1));

   BOOST_CHECK (lua_isboolean (ls, 2));
   BOOST_CHECK (lua_toboolean (ls, 2) == false);

   BOOST_CHECK (lua_isstring (ls, 3));
   BOOST_CHECK (std::strcmp (lua_tostring (ls, 3), "The sky is blue.") == 0);

   BOOST_CHECK (lua_isnumber (ls, 4));
   BOOST_CHECK (lua_tonumber (ls, 4) == 2.7183);

   // Close the Lua state used in this test
   lua_close (ls);
}



using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'LuaUtils' tests");
   test->add (BOOST_TEST_CASE (&TestToLuaValue));
   test->add (BOOST_TEST_CASE (&TestPushLuaValue));

   return test;
}
