/******************************************************************************\
* TestLuaFunction.cpp                                                          *
* Unit tests for things declared in 'LuaFunction.hpp'.                         *
*                                                                              *
*                                                                              *
* Copyright (C) 2005-2007 by Leandro Motta Barros.                             *
*                                                                              *
* Permission is hereby granted, free of charge, to any person obtaining a copy *
* of this software and associated documentation files (the "Software"), to     *
* deal in the Software without restriction, including without limitation the   *
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or  *
* sell copies of the Software, and to permit persons to whom the Software is   *
* furnished to do so, subject to the following conditions:                     *
*                                                                              *
* The above copyright notice and this permission notice shall be included in   *
* all copies or substantial portions of the Software.                          *
*                                                                              *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS *
* IN THE SOFTWARE.                                                             *
\******************************************************************************/

#define BOOST_TEST_MODULE LuaFunction

#include <cstring>
#include <boost/test/unit_test.hpp>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaFunction.hpp>


// - TestPureLuaFunctionCallViaVariableArity0 ----------------------------------
BOOST_AUTO_TEST_CASE(TestPureLuaFunctionCallViaVariableArity0)
{
   using namespace Diluculum;

   LuaState ls;
   ls.doString("function func() return 171 end");
   LuaVariable luaFunc = ls["func"];
   LuaValueList ret = luaFunc();
   BOOST_REQUIRE_EQUAL (ret.size(), 1u);
   BOOST_CHECK_EQUAL (ret[0].asNumber(), 171);
}



// - TestPureLuaFunctionCallViaVariableArity1 ----------------------------------
BOOST_AUTO_TEST_CASE(TestPureLuaFunctionCallViaVariableArity1)
{
   using namespace Diluculum;

   LuaState ls;
   ls.doString("function func(p) return p*2 end");
   LuaVariable luaFunc = ls["func"];
   LuaValueList ret = luaFunc(222);
   BOOST_REQUIRE_EQUAL (ret.size(), 1u);
   BOOST_CHECK_EQUAL (ret[0].asNumber(), 444);
}



// - TestCallAnonymousLuaFunction ----------------------------------------------
BOOST_AUTO_TEST_CASE(TestCallAnonymousLuaFunction)
{
   using namespace Diluculum;

   LuaState ls;
   LuaValueList doStringRet =
      ls.doString("return function(p1, p2) return p1 + p2, p1 * p2 end");
   BOOST_REQUIRE_EQUAL (doStringRet.size(), 1u);
   BOOST_REQUIRE_EQUAL (doStringRet[0].type(), LUA_TFUNCTION);

   LuaFunction func = doStringRet[0].asFunction();

   LuaValueList params;
   params.push_back(3);
   params.push_back(-4);

   LuaValueList ret = ls.call (func, params);

   BOOST_REQUIRE_EQUAL (ret.size(), 2u);
   BOOST_REQUIRE_EQUAL (ret[0].type(), LUA_TNUMBER);
   BOOST_REQUIRE_EQUAL (ret[1].type(), LUA_TNUMBER);
   BOOST_CHECK_EQUAL (ret[0].asNumber(), -1);
   BOOST_CHECK_EQUAL (ret[1].asNumber(), -12);
}



// - TestCallAnonymousLuaFunctionNested ----------------------------------------
BOOST_AUTO_TEST_CASE(TestCallAnonymousLuaFunctionNested)
{
   using namespace Diluculum;

   LuaState ls;
   LuaValueList doStringRet =
      ls.doString(
         "return function(p) return '<emph>'..tostring(p)..'</emph>' end");
   BOOST_REQUIRE_EQUAL (doStringRet.size(), 1u);
   BOOST_REQUIRE_EQUAL (doStringRet[0].type(), LUA_TFUNCTION);

   LuaValue emphFunc = doStringRet[0];

   doStringRet =
      ls.doString("return function(t, f) return t..' and '..f(t) end");

   BOOST_REQUIRE_EQUAL (doStringRet.size(), 1u);
   BOOST_REQUIRE_EQUAL (doStringRet[0].type(), LUA_TFUNCTION);

   LuaFunction f = doStringRet[0].asFunction();

   LuaValueList params;
   params.push_back("blah");
   params.push_back(emphFunc);

   LuaValueList ret = ls.call (f, params);

   BOOST_REQUIRE_EQUAL (ret.size(), 1u);
   BOOST_REQUIRE_EQUAL (ret[0].type(), LUA_TSTRING);
   BOOST_CHECK_EQUAL (ret[0].asString(), "blah and <emph>blah</emph>.");
}


// // - TestUserDataConstruction --------------------------------------------------
// BOOST_AUTO_TEST_CASE(TestUserDataConstruction)
// {
//    using namespace Diluculum;

//    // Create three 'LuaUserData's, of different sizes
//    LuaUserData ud1 (4);
//    const LuaUserData ud2 (1200);
//    LuaUserData ud3 (8765);

//    // Ensure that the size returned by 'getSize()' matches the expected
//    BOOST_CHECK (ud1.getSize() == 4);
//    BOOST_CHECK (ud2.getSize() == 1200);
//    BOOST_CHECK (ud3.getSize() == 8765);

//    // The address returned by 'getData()' should be valid (non-NULL)
//    BOOST_CHECK (ud1.getData() != 0);
//    BOOST_CHECK (ud2.getData() != 0);
//    BOOST_CHECK (ud3.getData() != 0);

//    // Just in case, check that the address of the various 'LuaUserData's are
//    // different.
//    BOOST_CHECK (ud1.getData() != ud2.getData());
//    BOOST_CHECK (ud1.getData() != ud3.getData());
//    BOOST_CHECK (ud2.getData() != ud3.getData());
// }



// // - TestUserDataGetData -------------------------------------------------------
// BOOST_AUTO_TEST_CASE(TestUserDataGetData)
// {
//    using namespace Diluculum;

//    CREATE_USERDATA (ud1, 33, 128, 129, 130);
//    unsigned char data1[33] = { 128, 129, 130 };

//    CREATE_USERDATA (ud2, 33, 128, 129, 130);
//    unsigned char data2[33] = { 128, 129, 130 };

//    CREATE_USERDATA (ud3, 33, 128, 129, 255);
//    unsigned char data3[33] = { 128, 129, 255 };

//    // Check every userdata with the data it is expected to contain
//    BOOST_CHECK (memcmp (ud1.getData(), data1, sizeof(data1)) == 0);
//    BOOST_CHECK (memcmp (ud1.getData(), data2, sizeof(data2)) == 0);
//    BOOST_CHECK (memcmp (ud3.getData(), data3, sizeof(data3)) == 0);

//    // Compare data among the 'LuaUserData's
//    BOOST_CHECK (memcmp (ud1.getData(), ud2.getData(), sizeof(data1)) == 0);
//    BOOST_CHECK (memcmp (ud1.getData(), ud3.getData(), sizeof(data1)) != 0);
//    BOOST_CHECK (memcmp (ud2.getData(), ud3.getData(), sizeof(data2)) != 0);
// }



// // - TestRelationalOperators ---------------------------------------------------
// BOOST_AUTO_TEST_CASE(TestRelationalOperators)
// {
//    using namespace Diluculum;

//    CREATE_USERDATA (ud1, 10, 3, 4, 5);
//    CREATE_USERDATA (ud2, 11, 3, 4, 5);
//    CREATE_USERDATA (ud3, 9,  3, 4, 5);
//    CREATE_USERDATA (ud4, 10, 3, 5, 5);
//    CREATE_USERDATA (ud5, 10, 3, 4, 5);

//    // operator<
//    BOOST_CHECK (ud1 < ud2);
//    BOOST_CHECK (ud3 < ud1);
//    BOOST_CHECK (ud1 < ud4);
//    BOOST_CHECK (!(ud1 < ud5));
//    BOOST_CHECK (!(ud2 < ud2));

//    // operator>
//    BOOST_CHECK (ud2 > ud1);
//    BOOST_CHECK (ud1 > ud3);
//    BOOST_CHECK (ud4 > ud1);
//    BOOST_CHECK (!(ud5 > ud1));
//    BOOST_CHECK (!(ud2 > ud2));

//    // operator==
//    BOOST_CHECK (ud1 == ud5);
//    BOOST_CHECK (ud5 == ud1);
//    BOOST_CHECK (!(ud2 == ud1));
//    BOOST_CHECK (!(ud1 == ud2));
//    BOOST_CHECK (!(ud1 == ud3));
//    BOOST_CHECK (!(ud3 == ud1));
//    BOOST_CHECK (!(ud4 == ud1));
//    BOOST_CHECK (!(ud1 == ud4));

//    // operator!=
//    BOOST_CHECK (ud2 != ud1);
//    BOOST_CHECK (ud1 != ud2);
//    BOOST_CHECK (ud1 != ud3);
//    BOOST_CHECK (ud3 != ud1);
//    BOOST_CHECK (ud4 != ud1);
//    BOOST_CHECK (ud1 != ud4);
//    BOOST_CHECK (!(ud1 != ud5));
//    BOOST_CHECK (!(ud5 != ud1));
// }



// // - TestUserDataCopies --------------------------------------------------------
// BOOST_AUTO_TEST_CASE(TestUserDataCopies)
// {
//    using namespace Diluculum;

//    CREATE_USERDATA (ud1, 10, 3, 4, 5);
//    unsigned char data1[10] = { 3, 4, 5 };
//    CREATE_USERDATA (ud2, 11, 3, 4, 5);
//    CREATE_USERDATA (ud3, 9,  3, 4, 5);
//    CREATE_USERDATA (ud4, 10, 3, 5, 5);
//    unsigned char data4[10] = { 3, 5, 5 };
//    CREATE_USERDATA (ud5, 10, 3, 4, 5);

//    // Try a "chained assignment"
//    BOOST_REQUIRE (ud2 != ud1);
//    BOOST_REQUIRE (ud3 != ud1);

//    ud2 = ud3 = ud1;

//    BOOST_ASSERT (memcmp (ud1.getData(), data1, sizeof(data1)) == 0);
//    BOOST_ASSERT (ud2 == ud1);
//    BOOST_ASSERT (ud3 == ud1);

//    // And a simple one...
//    BOOST_REQUIRE (ud4 != ud5);

//    ud5 = ud4;

//    BOOST_ASSERT (memcmp (ud4.getData(), data4, sizeof(data4)) == 0);
//    BOOST_ASSERT (ud5 == ud4);
// }
