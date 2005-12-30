/******************************************************************************\
* TestLuaVariable.cpp                                                          *
* Unit tests for things declared in 'LuaVariable.hpp'.                         *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include <Diluculum/LuaState.hpp>
#include "CLuaFunctions.hpp"


// - CLuaFunctionTimesTwo ------------------------------------------------------
int CLuaFunctionTimesTwo (lua_State* ls)
{
   int numArgs = lua_gettop (ls);
   if (numArgs != 1)
   {
      lua_pushstring (ls, "expected one parameter in call to 'TimesTwo()'");
      lua_error (ls);
   }

   if (!lua_isnumber (ls, 1))
   {
      lua_pushstring(ls, "argument to function 'TimesTwo()' should be a number.");
      lua_error(ls);
   }

   lua_Number num = lua_tonumber (ls, 1);

   lua_pushnumber (ls, num * 2);
   return 1;
}



// - CLuaFunctionTimesTwoThreeFour -----------------------------------
int CLuaFunctionTimesTwoThreeFour (lua_State* ls)
{
   int numArgs = lua_gettop (ls);
   if (numArgs != 1)
   {
      lua_pushstring (ls, "expected one parameter in call "
                      "to 'TimesTwoThreeFour()'");
      lua_error (ls);
   }

   if (!lua_isnumber (ls, 1))
   {
      lua_pushstring(ls, "argument to function 'TimesTwoThreeFour()' "
                     "should be a number.");
      lua_error(ls);
   }

   lua_Number num = lua_tonumber (ls, 1);

   lua_pushnumber (ls, num * 2);
   lua_pushnumber (ls, num * 3);
   lua_pushnumber (ls, num * 4);
   return 3;
}



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
   ls["d"][3] = ls["b"].value();

   BOOST_CHECK (ls["a"] == false);
   BOOST_CHECK (ls["b"] == 21);
   BOOST_CHECK (ls["d"][2] == "two");
   BOOST_CHECK (ls["d"][3] == 21);

   // Now, change some values in Lua and verify if they were really changed
   ls.doString ("a = 'So I\\'m crazy!'");
   ls.doString ("b = 'What-what-what can I do?'");
   ls.doString ("c = 'So are you!'");
   ls.doString ("d[3] = 123.456");

   BOOST_CHECK (ls["a"] == "So I'm crazy!");
   BOOST_CHECK (ls["b"] == "What-what-what can I do?");
   BOOST_CHECK (ls["c"] == "So are you!");
   BOOST_CHECK (ls["d"][3] == 123.456);

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

   // Now in Lua: add values and check if they can be read from C++
   ls.doString ("e = true");
   ls.doString ("f = { 11.22, 33.44, yup = 'nope' }");
   ls.doString ("d[171] = 99.88");

   BOOST_CHECK (ls["e"] == true);
   BOOST_CHECK (ls["f"][1] == 11.22);
   BOOST_CHECK (ls["f"][2] == 33.44);
   BOOST_CHECK (ls["f"]["yup"] == "nope");
   BOOST_CHECK (ls["d"][171] == 99.88);
}



// - TestLuaVariableAssignmentOperatorValue ------------------------------------
void TestLuaVariableAssignmentOperatorValue()
{
   using namespace Diluculum;

   LuaState ls;
   ls.doString ("a = 1; b = 'two'; c = true; d = { 'foo', 'bar', [5] = 'baz' }");

   // "Multiple" assignment
   ls["newField"] = ls["a"] = ls["c"] = ls["d"][5] = 123.456;
   BOOST_CHECK (ls["newField"] == 123.456);
   BOOST_CHECK (ls["a"] == 123.456);
   BOOST_CHECK (ls["c"] == 123.456);
   BOOST_CHECK (ls["d"][5] == 123.456);

   // The other fields shall be untouched
   BOOST_CHECK (ls["b"] == "two");
   BOOST_CHECK (ls["d"][1] == "foo");
   BOOST_CHECK (ls["d"][2] == "bar");

   // Try with other types, too
   ls["a"] = "Boo!";
   BOOST_CHECK (ls["a"] == "Boo!");

   ls["a"] = false;
   BOOST_CHECK (ls["a"] == false);

   ls["a"] = Nil;
   BOOST_CHECK (ls["a"] == Nil);

   ls["a"] = CLuaFunctionTimesTwo;
   BOOST_CHECK (ls["a"] == CLuaFunctionTimesTwo);

   // Subscripting a non-table shall throw
   BOOST_CHECK_THROW (ls["a"]["a"] = "Ugh!", TypeMismatchError);
}



// - TestLuaVariableAssignmentOperatorVariable ---------------------------------
void TestLuaVariableAssignmentOperatorVariable()
{
   using namespace Diluculum;

   LuaState ls;
   ls.doString ("a = 1; b = 'two'; c = { 1.1, 2.2, 3.3 }");

   // Create two 'LuaVariable's and, just in case, check if they have the proper
   // values
   LuaVariable lv1 = ls["a"];
   LuaVariable lv2 = ls["b"];
   BOOST_CHECK (lv1 == 1);
   BOOST_CHECK (lv2 == "two");

   // Test for self assignment
   lv1 = lv1;
   lv2 = lv2;
   BOOST_CHECK (lv1 == 1);
   BOOST_CHECK (lv2 == "two");

   // Assign one to another. Check if they now refer to the same variable
   lv2 = lv1;
   BOOST_CHECK (lv1 == 1);
   BOOST_CHECK (lv2 == 1);

   lv2 = false;
   BOOST_CHECK (lv1 == false);
   BOOST_CHECK (lv2 == false);

   // "Multiple" assignment
   LuaVariable lv3 = ls["c"];
   lv1 = lv2 = lv3;
   BOOST_CHECK (lv1[1] == 1.1);
   BOOST_CHECK (lv1[2] == 2.2);
   BOOST_CHECK (lv1[3] == 3.3);
   BOOST_CHECK (lv2[1] == 1.1);
   BOOST_CHECK (lv2[2] == 2.2);
   BOOST_CHECK (lv2[3] == 3.3);
   BOOST_CHECK (lv3[1] == 1.1);
   BOOST_CHECK (lv3[2] == 2.2);
   BOOST_CHECK (lv3[3] == 3.3);
}



// - TestLuaVariableCopyConstructor --------------------------------------------
void TestLuaVariableCopyConstructor()
{
   using namespace Diluculum;

   LuaState ls;
   ls.doString ("a = 1");

   // Create two 'LuaVariable's referring to the same variable in the same
   // 'LuaState'. Then chack if changing one also changes the another one.
   LuaVariable lv = ls["a"];
   BOOST_CHECK (lv == 1); // just to be sure, this is already tested elsewhere

   LuaVariable lvc (lv);
   BOOST_CHECK (lvc == 1);

   lvc = "a string";
   BOOST_CHECK (lvc == "a string");
   BOOST_CHECK (lv == "a string");

   lv = true;
   BOOST_CHECK (lv == true);
   BOOST_CHECK (lvc == true);
}



// - TestLuaVariableEqualityOperator -------------------------------------------
void TestLuaVariableEqualityOperator()
{
   using namespace Diluculum;

   LuaState ls1;
   ls1.doString ("a = 1; b = 'two'; c = 3; d = 3");

   LuaState ls2;
   ls2.doString ("a = 1; b = 'two'");

   LuaVariable lv1 = ls1["a"];
   LuaVariable lv2 = ls1["b"];
   LuaVariable lv3 = ls2["a"];
   LuaVariable lv4 = ls2["b"];

   LuaVariable lv5 = ls1["a"];
   LuaVariable lv6 = ls1["b"];
   LuaVariable lv7 = ls2["a"];
   LuaVariable lv8 = ls2["b"];

   LuaVariable lv9 = ls1["c"];
   LuaVariable lv10 = ls1["d"];

   // Each variable is expected to be equal to itself
   BOOST_CHECK (lv1 == lv1);
   BOOST_CHECK (lv2 == lv2);
   BOOST_CHECK (lv3 == lv3);
   BOOST_CHECK (lv4 == lv4);
   BOOST_CHECK (lv5 == lv5);
   BOOST_CHECK (lv6 == lv6);
   BOOST_CHECK (lv7 == lv7);
   BOOST_CHECK (lv8 == lv8);

   // Each variable is expected to be no different than itself
   BOOST_CHECK (!(lv1 != lv1));
   BOOST_CHECK (!(lv2 != lv2));
   BOOST_CHECK (!(lv3 != lv3));
   BOOST_CHECK (!(lv4 != lv4));
   BOOST_CHECK (!(lv5 != lv5));
   BOOST_CHECK (!(lv6 != lv6));
   BOOST_CHECK (!(lv7 != lv7));
   BOOST_CHECK (!(lv8 != lv8));

   // 'LuaVariable's created as referring to the same variables should be equal
   BOOST_CHECK (lv1 == lv5);
   BOOST_CHECK (lv2 == lv6);
   BOOST_CHECK (lv3 == lv7);
   BOOST_CHECK (lv4 == lv8);

   BOOST_CHECK (!(lv1 != lv5));
   BOOST_CHECK (!(lv2 != lv6));
   BOOST_CHECK (!(lv3 != lv7));
   BOOST_CHECK (!(lv4 != lv8));

   // The same as the previous, but inverting the operands
   BOOST_CHECK (lv5 == lv1);
   BOOST_CHECK (lv6 == lv2);
   BOOST_CHECK (lv7 == lv3);
   BOOST_CHECK (lv8 == lv4);

   BOOST_CHECK (!(lv5 != lv1));
   BOOST_CHECK (!(lv6 != lv2));
   BOOST_CHECK (!(lv7 != lv3));
   BOOST_CHECK (!(lv8 != lv4));

   // If the variables differ "just" by the 'LuaState', they are still different
   BOOST_CHECK (lv1 != lv3);
   BOOST_CHECK (lv2 != lv4);

   BOOST_CHECK (!(lv1 == lv3));
   BOOST_CHECK (!(lv2 == lv4));

   // Try some other assorted comparisons
   BOOST_CHECK (lv1 != lv2);
   BOOST_CHECK (lv1 != lv3);
   BOOST_CHECK (lv1 != lv4);

   BOOST_CHECK (!(lv1 == lv2));
   BOOST_CHECK (!(lv1 == lv3));
   BOOST_CHECK (!(lv1 == lv4));

   // Ensure that variables are equal after one is assigned to another
   lv1 = lv2;
   lv3 = lv4 = lv5 = lv6;

   BOOST_CHECK (lv1 == lv1);
   BOOST_CHECK (lv1 == lv2);
   BOOST_CHECK (lv2 == lv2);

   BOOST_CHECK (lv3 == lv3);
   BOOST_CHECK (lv3 == lv4);
   BOOST_CHECK (lv3 == lv5);
   BOOST_CHECK (lv3 == lv6);

   BOOST_CHECK (lv4 == lv3);
   BOOST_CHECK (lv4 == lv4);
   BOOST_CHECK (lv4 == lv5);
   BOOST_CHECK (lv4 == lv6);

   BOOST_CHECK (lv5 == lv3);
   BOOST_CHECK (lv5 == lv4);
   BOOST_CHECK (lv5 == lv5);
   BOOST_CHECK (lv5 == lv6);

   BOOST_CHECK (lv6 == lv3);
   BOOST_CHECK (lv6 == lv4);
   BOOST_CHECK (lv6 == lv5);
   BOOST_CHECK (lv6 == lv6);

   // Finally, verify that two distinct 'LuaVariable's are different, even if
   // they have the same value.
   BOOST_REQUIRE (lv9.value() == lv10.value());
   BOOST_CHECK (lv9 != lv10);
   BOOST_CHECK (!(lv9 == lv10));
}



// - TestLuaVariableExceptions -------------------------------------------------
void TestLuaVariableExceptions()
{
   using namespace Diluculum;

   LuaState ls;
   ls.doString ("t = { a = 1, b = 'b', c = true }");

   // Accessing a valid global variable shall not throw.
   LuaVariable lv1 = ls["t"];
   BOOST_CHECK_NO_THROW (lv1.value());

   // Accessing an invalid global variable shall not throw ('Nil' is returned)
   LuaVariable lv2 = ls["x"];
   BOOST_CHECK_NO_THROW (lv2.value());
   BOOST_CHECK (lv2.value() == Nil);

   // Accessing a valid table key shall not throw
   LuaVariable lv3 = ls["t"]["a"];
   BOOST_CHECK_NO_THROW (lv3.value());

   // Accessing an invalid table key shall not throw ('Nil' is returned)
   LuaVariable lv4 = ls["t"]["x"];
   BOOST_CHECK_NO_THROW (lv4.value());
   BOOST_CHECK (lv4.value() == Nil);

   // Accessing an (obviously) invalid key of an invalid table shall throw
   BOOST_CHECK_THROW (ls["x"]["x"].value(), TypeMismatchError);

   // Subscripting a non-table shall throw, too
   BOOST_CHECK_THROW (ls["a"]["x"].value(), TypeMismatchError);
}



// - TestLuaVariableFunctionCall -----------------------------------------------
void TestLuaVariableFunctionCall()
{
   using namespace Diluculum;

   LuaState ls;

   // Call some C functions registered in Lua through a 'LuaVariable'
   ls["TimesTwo"] = CLuaFunctionTimesTwo;
   ls["TimesTwoThreeFour"] = CLuaFunctionTimesTwoThreeFour;
   LuaVariable funcTimesTwo = ls["TimesTwo"];
   LuaVariable funcTimesTwoThreeFour = ls["TimesTwoThreeFour"];

   LuaValueList ret = funcTimesTwo (2);
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == 4);

   ret = funcTimesTwoThreeFour (3);
   BOOST_REQUIRE (ret.size() == 3);
   BOOST_CHECK (ret[0] == 6);
   BOOST_CHECK (ret[1] == 9);
   BOOST_CHECK (ret[2] == 12);

   // Once those "pure Lua C functions" are tested, try all the
   // Diluculum-wrapped functions defined in 'CLuaFunctions.hpp'.
   ls["ZeroTheGlobal"] = ZeroTheGlobal;
   TheGlobal = 171;
   LuaVariable fZeroTheGlobal = ls["ZeroTheGlobal"];
   ret = fZeroTheGlobal();
   BOOST_REQUIRE (ret.size() == 0);
   BOOST_CHECK (TheGlobal == 0);

   ls["SetTheGlobalDirectly"] = SetTheGlobalDirectly;
   LuaVariable fSetTheGlobalDirectly = ls["SetTheGlobalDirectly"];
   ret = fSetTheGlobalDirectly (-55);
   BOOST_REQUIRE (ret.size() == 0);
   BOOST_CHECK (TheGlobal == -55);

   ls["SetTheGlobalAddingAString"] = SetTheGlobalAddingAString;
   LuaVariable fSetTheGlobalAddingAString = ls["SetTheGlobalAddingAString"];
   ret = fSetTheGlobalAddingAString (77, "two");
   BOOST_REQUIRE (ret.size() == 0);
   BOOST_CHECK (TheGlobal == 79);

   ls["SetTheGlobalMultiplying"] = SetTheGlobalMultiplying;
   LuaVariable fSetTheGlobalMultiplying = ls["SetTheGlobalMultiplying"];
   ret = fSetTheGlobalMultiplying (11, 2, -3);
   BOOST_REQUIRE (ret.size() == 0);
   BOOST_CHECK (TheGlobal == -66);

   ls["SetTheGlobalFirstParamDefinesOperation"] =
      SetTheGlobalFirstParamDefinesOperation;
   LuaVariable fSetTheGlobalFirstParamDefinesOperation =
      ls["SetTheGlobalFirstParamDefinesOperation"];
   ret = fSetTheGlobalFirstParamDefinesOperation (true, 44, -1, -3);
   BOOST_REQUIRE (ret.size() == 0);
   BOOST_CHECK (TheGlobal == 40);


   ls["SetTheGlobalLastParamSelectsValue"] = SetTheGlobalLastParamSelectsValue;
   LuaVariable fSetTheGlobalLastParamSelectsValue =
      ls["SetTheGlobalLastParamSelectsValue"];
   ret = fSetTheGlobalLastParamSelectsValue (11, -22, 33, -44, 2);
   BOOST_REQUIRE (ret.size() == 0);
   BOOST_CHECK (TheGlobal == -22);

   ls["WhatIsYourFavoriteColor"] = WhatIsYourFavoriteColor;
   LuaVariable fWhatIsYourFavoriteColor =
      ls["WhatIsYourFavoriteColor"];
   ret = fWhatIsYourFavoriteColor();
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == "Blue");

   ls["GetTableSize"] = GetTableSize;
   LuaVariable fGetTableSize = ls["GetTableSize"];
   ls.doString ("GetTableSize_TestTable = { 'abc', 4.2, true, { }, -7 }");
   ret = fGetTableSize (ls["GetTableSize_TestTable"].value());
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == 5);

   ls["Mod"] = Mod;
   LuaVariable fMod = ls["Mod"];
   ret = fMod (8, 6);
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == 2);

   ls["Mean"] = Mean;
   LuaVariable fMean = ls["Mean"];
   ret = fMean (10, 3, 20);
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == 11);

   ls["CalcEvenParityBit"] = CalcEvenParityBit;
   LuaVariable fCalcEvenParityBit = ls["CalcEvenParityBit"];
   ret = fCalcEvenParityBit (true, true, true, true);
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == false);

   ls["Concatenate"] = Concatenate;
   LuaVariable fConcatenate = ls["Concatenate"];
   ret = fConcatenate ("As armas ", "e os barões ", "assinalados que ",
                       "da ocidental ", "praia lusitana...");
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == "As armas e os barões assinalados que "
                "da ocidental praia lusitana...");


   // Now, so the same with some "authentic" Lua functions
   ls.doString ("function Square (x) return x*x end");
   ret = ls["Square"](7);
   BOOST_REQUIRE (ret.size() == 1);
   BOOST_CHECK (ret[0] == 49);

   ls.doString ("function SumProd (x) return x+x, x*x end");
   ret = ls["SumProd"](5);
   BOOST_REQUIRE (ret.size() == 2);
   BOOST_CHECK (ret[0] == 10);
   BOOST_CHECK (ret[1] == 25);

   ls.doString ("function PartialSums (x, y, z) return 0, x, x+y, x+y+z end");
   ret = ls["PartialSums"](4, 10, 3);
   BOOST_REQUIRE (ret.size() == 4);
   BOOST_CHECK (ret[0] == 0);
   BOOST_CHECK (ret[1] == 4);
   BOOST_CHECK (ret[2] == 14);
   BOOST_CHECK (ret[3] == 17);

   // Finally, check if the proper exceptions are thrown
   ls["noFunc"] = 123.456;
   LuaVariable noFunc = ls["noFunc"];
   BOOST_CHECK_THROW (noFunc(), TypeMismatchError);
}



// - TestLuaVariableCFunction --------------------------------------------------
void TestLuaVariableCFunction()
{
   using namespace Diluculum;

   LuaState ls;

   // Register a C function in Lua, call it from Lua and check the return value.
   ls["TimesTwo"] = CLuaFunctionTimesTwo;
   LuaValue val = ls.doString ("return TimesTwo (50)");
   BOOST_CHECK (val == 100);

   // Forcing a call to 'lua_error()' in the C function shall translate to
   // an exception.
   BOOST_CHECK_THROW (ls.doString ("return TimesTwo (1, 2)"), LuaRunTimeError);
}


using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'LuaVariable' tests");
   test->add (BOOST_TEST_CASE (&TestLuaVariableSubscriptOperator));
   test->add (BOOST_TEST_CASE (&TestLuaVariableAssignmentOperatorValue));
   test->add (BOOST_TEST_CASE (&TestLuaVariableAssignmentOperatorVariable));
   test->add (BOOST_TEST_CASE (&TestLuaVariableCopyConstructor));
   test->add (BOOST_TEST_CASE (&TestLuaVariableEqualityOperator));
   test->add (BOOST_TEST_CASE (&TestLuaVariableExceptions));
   test->add (BOOST_TEST_CASE (&TestLuaVariableFunctionCall));
   test->add (BOOST_TEST_CASE (&TestLuaVariableCFunction));

   return test;
}
