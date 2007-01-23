/******************************************************************************\
* TestLuaWrappers.cpp                                                          *
* Unit tests for things declared in 'LuaWrappers.hpp'.                         *
*                                                                              *
*                                                                              *
* Copyright (C) 2005-2006 by Leandro Motta Barros.                             *
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
   BOOST_CHECK (res[0] == "one um eins");

   res = ls.doString ("return FibonacciSequence (2)");
   BOOST_REQUIRE (res.size() == 2);
   BOOST_CHECK (res[0] == 1);
   BOOST_CHECK (res[1] == 1);

   res = ls.doString ("return FibonacciSequence (10)");
   BOOST_REQUIRE (res.size() == 10);
   BOOST_CHECK (res[0] == 1);
   BOOST_CHECK (res[1] == 1);
   BOOST_CHECK (res[2] == 2);
   BOOST_CHECK (res[3] == 3);
   BOOST_CHECK (res[4] == 5);
   BOOST_CHECK (res[5] == 8);
   BOOST_CHECK (res[6] == 13);
   BOOST_CHECK (res[7] == 21);
   BOOST_CHECK (res[8] == 34);
   BOOST_CHECK (res[9] == 55);

   res = ls.doString ("return FibonacciSequence (6)");
   BOOST_REQUIRE (res.size() == 6);
   BOOST_CHECK (res[0] == 1);
   BOOST_CHECK (res[1] == 1);
   BOOST_CHECK (res[2] == 2);
   BOOST_CHECK (res[3] == 3);
   BOOST_CHECK (res[4] == 5);
   BOOST_CHECK (res[5] == 8);

   res = ls.doString ("return ToOrFromString (1)");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0] == "one");

   res = ls.doString ("return ToOrFromString (3)");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0] == "three");

   res = ls.doString ("return ToOrFromString ('one')");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0] == 1);

   res = ls.doString ("return ToOrFromString ('two')");
   BOOST_REQUIRE (res.size() == 1);
   BOOST_CHECK (res[0] == 2);
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
   BOOST_CHECK (ret[0] == 0.0);

   ret = ls.doString ("return a2:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 123.45);

   ls.doString ("a1:deposit (55.66)");
   ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 55.66);

   ls.doString ("a1:withdraw (15.66)");
   ret = ls.doString ("return a1:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 40.0);

   ls.doString ("a2:withdraw (0.45)");
   ret = ls.doString ("return a2:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_REQUIRE (ret[0].type() == LUA_TNUMBER);
   BOOST_CHECK (ret[0] == 123.0);

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
   BOOST_CHECK (ret[0] == -10.0);
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
   BOOST_CHECK (ret[0] == 1000.01);

   ls.doString ("a:deposit (0.09)");
   ls.doString ("a:withdraw (500.0)");
   ret = ls.doString ("return a:balance()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == 500.10);

   // And now, some 'NumberProperties' methods
   ret = ls.doString ("return n:isEven()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == true);

   ret = ls.doString ("return n:isOdd()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == false);

   ret = ls.doString ("return n:isBig()");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == true);
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

   // Third case: the object is instantiated in Lua, and its destructor is
   // explicitly called.
   DestructorTester::aFlag = false;

   {
      LuaState ls;
      DILUCULUM_REGISTER_CLASS (ls, DestructorTester);
      ls.doString ("foo = DestructorTester.new()");

      // Just to be paranoid, ensure that 'aFlag' is still false
      BOOST_REQUIRE (DestructorTester::aFlag == false);

      // Call the destructor explicitly
      ls.doString ("foo:delete()");

      // Check if the destructor was really called
      BOOST_CHECK (DestructorTester::aFlag == true);
   }

   // Just to be even more paranoid, ensure that 'aFlag' is still true
   BOOST_REQUIRE (DestructorTester::aFlag == true);
}



// - TestDynamicModule ---------------------------------------------------------
void TestDynamicModule()
{
   using namespace Diluculum;
   LuaState ls;

   LuaValueList ret;

   BOOST_REQUIRE_NO_THROW (ls.doString ("require 'ATestModule'"));

   BOOST_REQUIRE_NO_THROW (ls.doString ("obj = ATestModule.AClass.new (123)"));

   BOOST_REQUIRE_NO_THROW (ret = ls.doString ("obj:aMethod()"));
   BOOST_CHECK (ret.size() == 1);
   BOOST_CHECK (ret[0] == 123);

   BOOST_REQUIRE_NO_THROW (ret = ls.doString ("ATestModule.AFunction()"));
   BOOST_CHECK (ret.size() == 2);
   BOOST_CHECK (ret[0] == true);
   BOOST_CHECK (ret[1] == "Ahey!");
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
   test->add (BOOST_TEST_CASE (&TestDynamicModule));

   return test;
}
