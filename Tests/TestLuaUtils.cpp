/******************************************************************************\
* TestLuaUtils.cpp                                                             *
* Unit tests for things declared in 'LuaUtils.hpp'.                            *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include <cstring>
#include "LuaExceptions.hpp"
#include "LuaUtils.hpp"


int CLuaFunctionExample (lua_State* ls)
{
   return 0;
}


// - TestToLuaValue ------------------------------------------------------------
void TestToLuaValue()
{
   using namespace Diluculum;

   lua_State* ls = lua_open();

   lua_pushboolean (ls, true);
   lua_pushnumber (ls, 171.171);
   lua_pushstring (ls, "The book is on the table.");
   lua_pushnil (ls);
   lua_pushcfunction (ls, CLuaFunctionExample);

   LuaValue lvTrue (true);
   LuaValue lv171 (171.171);
   LuaValue lvTBIOTT ("The book is on the table.");
   LuaValue lvCLua (CLuaFunctionExample);

   // Try reading using positive (absolute) indexes
   BOOST_CHECK (ToLuaValue (ls, 1) == lvTrue);
   BOOST_CHECK (ToLuaValue (ls, 2) == lv171);
   BOOST_CHECK (ToLuaValue (ls, 3) == lvTBIOTT);
   BOOST_CHECK (ToLuaValue (ls, 4) == Nil);
   BOOST_CHECK (ToLuaValue (ls, 5) == lvCLua);

   // And now, try with negative indexes
   BOOST_CHECK (ToLuaValue (ls, -1) == lvCLua);
   BOOST_CHECK (ToLuaValue (ls, -2) == Nil);
   BOOST_CHECK (ToLuaValue (ls, -3) == lvTBIOTT);
   BOOST_CHECK (ToLuaValue (ls, -4) == lv171);
   BOOST_CHECK (ToLuaValue (ls, -5) == lvTrue);

   // Just to be complete, do everything once more using "constants"
   BOOST_CHECK (ToLuaValue (ls, 1) == true);
   BOOST_CHECK (ToLuaValue (ls, 2) == 171.171);
   BOOST_CHECK (ToLuaValue (ls, 3) == "The book is on the table.");
   BOOST_CHECK (ToLuaValue (ls, 5) == CLuaFunctionExample);
   BOOST_CHECK (ToLuaValue (ls, -1) == CLuaFunctionExample);
   BOOST_CHECK (ToLuaValue (ls, -3) == "The book is on the table.");
   BOOST_CHECK (ToLuaValue (ls, -4) == 171.171);
   BOOST_CHECK (ToLuaValue (ls, -5) == true);

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
   PushLuaValue (ls, CLuaFunctionExample);

   // Check if the values were properly pushed
   BOOST_CHECK (lua_isnil (ls, 1));

   BOOST_REQUIRE (lua_isboolean (ls, 2));
   BOOST_CHECK (lua_toboolean (ls, 2) == false);

   BOOST_REQUIRE (lua_isstring (ls, 3));
   BOOST_CHECK (std::strcmp (lua_tostring (ls, 3), "The sky is blue.") == 0);

   BOOST_REQUIRE (lua_isnumber (ls, 4));
   BOOST_CHECK (lua_tonumber (ls, 4) == 2.7183);

   BOOST_REQUIRE (lua_iscfunction (ls, 5));
   BOOST_CHECK (lua_tocfunction (ls, 5) == CLuaFunctionExample);

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
