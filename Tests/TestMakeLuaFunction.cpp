/******************************************************************************\
* TestMakeLuaFunction.cpp                                                      *
* Unit tests for things declared in 'MakeLuaFunction.hpp'.                     *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include "MakeLuaFunction.hpp"
#include "LuaState.hpp"
#include <iostream>


namespace
{
   /** A global variable used in some "C Lua Functions" defined below. This
    *  global is changed as the side effect of calling these functions, so that
    *  it is possible to check whether a returnless function is being really
    *  executed.
    */
   int TheGlobal;


   // - LuaC_ZeroTheGlobal -----------------------------------------------------
   void LuaC_ZeroTheGlobal()
   {
      TheGlobal = 0;
   }

   DILUCULUM_MAKE_LUA_FUNCTION_0_VOID (LuaC_ZeroTheGlobal, ZeroTheGlobal,
                                       "ZeroTheGlobal");


   // - LuaC_SetTheGlobalDirectly ----------------------------------------------
   void LuaC_SetTheGlobalDirectly (const Diluculum::LuaValue& value)
   {
      lua_Number n = value.asNumber();
      TheGlobal = static_cast<int>(n);
   }

   DILUCULUM_MAKE_LUA_FUNCTION_1_VOID (LuaC_SetTheGlobalDirectly,
                                       SetTheGlobalDirectly,
                                       "SetTheGlobalDirectly");


   // - LuaC_SetTheGlobalAddingAString -----------------------------------------
   void LuaC_SetTheGlobalAddingAString (const Diluculum::LuaValue& intValue,
                                        const Diluculum::LuaValue& strValue)
   {
      int v = 0;
      if (strValue == "one")
         v = 1;
      else if (strValue == "two")
         v = 2;

      TheGlobal = static_cast<int>(intValue.asNumber()) + v;
   }

   DILUCULUM_MAKE_LUA_FUNCTION_2_VOID (LuaC_SetTheGlobalAddingAString,
                                       SetTheGlobalAddingAString,
                                       "SetTheGlobalAddingAString");


   // - LuaC_SetTheGlobalMultiplying -------------------------------------------
   void LuaC_SetTheGlobalMultiplying (const Diluculum::LuaValue& v1,
                                      const Diluculum::LuaValue& v2,
                                      const Diluculum::LuaValue& v3)
   {
      TheGlobal = static_cast<int>(v1.asNumber())
         * static_cast<int>(v2.asNumber())
         * static_cast<int>(v3.asNumber());
   }

   DILUCULUM_MAKE_LUA_FUNCTION_3_VOID (LuaC_SetTheGlobalMultiplying,
                                       SetTheGlobalMultiplying,
                                       "SetTheGlobalMultiplying");


   // - LuaC_SetTheGlobalFirstParamDefinesOperation ----------------------------
   void LuaC_SetTheGlobalFirstParamDefinesOperation(
      const Diluculum::LuaValue& v1, const Diluculum::LuaValue& v2,
      const Diluculum::LuaValue& v3, const Diluculum::LuaValue& v4)
   {
      if (v1.asBoolean())
         TheGlobal = static_cast<int>(v2.asNumber())
            + static_cast<int>(v3.asNumber())
            + static_cast<int>(v4.asNumber());
      else
         TheGlobal = static_cast<int>(v2.asNumber())
            * static_cast<int>(v3.asNumber())
            * static_cast<int>(v4.asNumber());
   }

   DILUCULUM_MAKE_LUA_FUNCTION_4_VOID(
      LuaC_SetTheGlobalFirstParamDefinesOperation,
      SetTheGlobalFirstParamDefinesOperation,
      "SetTheGlobalFirstParamDefinesOperation");


   // - LuaC_SetTheGlobalLastParamSelectsValue ---------------------------------
   void LuaC_SetTheGlobalLastParamSelectsValue (const Diluculum::LuaValue& v1,
                                                const Diluculum::LuaValue& v2,
                                                const Diluculum::LuaValue& v3,
                                                const Diluculum::LuaValue& v4,
                                                const Diluculum::LuaValue& v5)
   {
      switch (static_cast<int>(v5.asNumber()))
      {
         case 1:
            TheGlobal = static_cast<int>(v1.asNumber());
            break;
         case 2:
            TheGlobal = static_cast<int>(v2.asNumber());
            break;
         case 3:
            TheGlobal = static_cast<int>(v3.asNumber());
            break;
         case 4:
            TheGlobal = static_cast<int>(v4.asNumber());
            break;
         default:
            TheGlobal = 0;
            break;
      }
   }

   DILUCULUM_MAKE_LUA_FUNCTION_5_VOID (LuaC_SetTheGlobalLastParamSelectsValue,
                                       SetTheGlobalLastParamSelectsValue,
                                       "SetTheGlobalLastParamSelectsValue");


   // - LuaC_WhatIsYourFavoriteColor -------------------------------------------
   Diluculum::LuaValue LuaC_WhatIsYourFavoriteColor()
   {
      return "Blue";
   }

   DILUCULUM_MAKE_LUA_FUNCTION_0_RET (LuaC_WhatIsYourFavoriteColor,
                                      WhatIsYourFavoriteColor,
                                      "WhatIsYourFavoriteColor");


   // - LuaC_GetTableSize ------------------------------------------------------
   Diluculum::LuaValue LuaC_GetTableSize (const Diluculum::LuaValue& table)
   {
      return table.asTable().size();
   }

   DILUCULUM_MAKE_LUA_FUNCTION_1_RET (LuaC_GetTableSize, GetTableSize,
                                      "GetTableSize");


   // - LuaC_Mod ---------------------------------------------------------------
   Diluculum::LuaValue LuaC_Mod (const Diluculum::LuaValue& v1,
                                 const Diluculum::LuaValue& v2)
   {
      return static_cast<int>(v1.asNumber()) % static_cast<int>(v2.asNumber());
   }

   DILUCULUM_MAKE_LUA_FUNCTION_2_RET (LuaC_Mod, Mod, "Mod");


   // - LuaC_Mean --------------------------------------------------------------
   Diluculum::LuaValue LuaC_Mean (const Diluculum::LuaValue& v1,
                                  const Diluculum::LuaValue& v2,
                                  const Diluculum::LuaValue& v3)
   {
      return (v1.asNumber() + v2.asNumber() + v3.asNumber()) / 3.0;
   }

   DILUCULUM_MAKE_LUA_FUNCTION_3_RET (LuaC_Mean, Mean, "Mean");


   // - LuaC_CalcEvenParityBit -------------------------------------------------
   Diluculum::LuaValue LuaC_CalcEvenParityBit(
      const Diluculum::LuaValue& v1, const Diluculum::LuaValue& v2,
      const Diluculum::LuaValue& v3, const Diluculum::LuaValue& v4)
   {
      int n = v1.asBoolean() + v2.asBoolean() + v3.asBoolean() + v4.asBoolean();
      return n % 2 == 1;
   }

   DILUCULUM_MAKE_LUA_FUNCTION_4_RET (LuaC_CalcEvenParityBit, CalcEvenParityBit,
                                      "CalcEvenParityBit");


   // - LuaC_Concatenate -------------------------------------------------------
   Diluculum::LuaValue LuaC_Concatenate (const Diluculum::LuaValue& v1,
                                         const Diluculum::LuaValue& v2,
                                         const Diluculum::LuaValue& v3,
                                         const Diluculum::LuaValue& v4,
                                         const Diluculum::LuaValue& v5)
   {
      return v1.asString() + v2.asString() + v3.asString() + v4.asString()
         + v5.asString();
   }

   DILUCULUM_MAKE_LUA_FUNCTION_5_RET (LuaC_Concatenate, Concatenate,
                                      "Concatenate");

} // (anonymous) namespace



// - TestReturnless ------------------------------------------------------------
void TestReturnless()
{
   using namespace Diluculum;

   // First, create a Lua state and register the functions
   LuaState ls;

   ls["ZeroTheGlobal"] = ZeroTheGlobal;
   ls["SetTheGlobalDirectly"] = SetTheGlobalDirectly;
   ls["SetTheGlobalAddingAString"] = SetTheGlobalAddingAString;
   ls["SetTheGlobalMultiplying"] = SetTheGlobalMultiplying;
   ls["SetTheGlobalFirstParamDefinesOperation"] =
      SetTheGlobalFirstParamDefinesOperation;
   ls["SetTheGlobalLastParamSelectsValue"] = SetTheGlobalLastParamSelectsValue;

   // Also, put something uninteresting in 'TheGlobal'
   TheGlobal = -1;

   // Finally, call the functions in Lua and check if 'TheGlobal' was
   // properly modified.
   ls.doString ("ZeroTheGlobal()");
   BOOST_CHECK (TheGlobal == 0);

   ls.doString ("SetTheGlobalDirectly (171)");
   BOOST_CHECK (TheGlobal == 171);

   ls.doString ("SetTheGlobalAddingAString (99, 'one')");
   BOOST_CHECK (TheGlobal == 100);
   ls.doString ("SetTheGlobalAddingAString (-2, 'two')");
   BOOST_CHECK (TheGlobal == 0);

   ls.doString ("SetTheGlobalMultiplying (1, 2, 3)");
   BOOST_CHECK (TheGlobal == 6);
   ls.doString ("SetTheGlobalMultiplying (2, 3, 4)");
   BOOST_CHECK (TheGlobal == 24);

   ls.doString ("SetTheGlobalFirstParamDefinesOperation (true, 2, 3, 4)");
   BOOST_CHECK (TheGlobal == 9);
   ls.doString ("SetTheGlobalFirstParamDefinesOperation (false, 2, 3, 4)");
   BOOST_CHECK (TheGlobal == 24);

   ls.doString ("SetTheGlobalLastParamSelectsValue (11, 22, 33, 44, 3)");
   BOOST_CHECK (TheGlobal == 33);
   ls.doString ("SetTheGlobalLastParamSelectsValue (11, 22, 33, 44, 1)");
   BOOST_CHECK (TheGlobal == 11);
}



// - TestReturnful -------------------------------------------------------------
void TestReturnful()
{
   using namespace Diluculum;

   // First, create a Lua state and register the functions
   LuaState ls;

   ls["WhatIsYourFavoriteColor"] = WhatIsYourFavoriteColor;
   ls["GetTableSize"] = GetTableSize;
   ls["Mod"] = Mod;
   ls["Mean"] = Mean;
   ls["CalcEvenParityBit"] = CalcEvenParityBit;
   ls["Concatenate"] = Concatenate;

   // Then, call the functions in Lua and check their return values
   LuaValue favoriteColor = ls.doString ("return WhatIsYourFavoriteColor()");
   BOOST_CHECK (favoriteColor == "Blue");

   LuaValue tableSize = ls.doString ("return GetTableSize{1, 'two', true, x = 5}");
   BOOST_CHECK (tableSize == 4);
   tableSize = ls.doString ("return GetTableSize{1, 'two'}");
   BOOST_CHECK (tableSize == 2);

   LuaValue mod = ls.doString ("return Mod (5, 3)");
   BOOST_CHECK (mod == 2);
   mod = ls.doString ("return Mod (4, 2)");
   BOOST_CHECK (mod == 0);

   LuaValue mean = ls.doString ("return Mean (1, 2, 3)");
   BOOST_CHECK (mean == 2);
   mean = ls.doString ("return Mean (1, 2, 6)");
   BOOST_CHECK (mean == 3);

   LuaValue parity =
      ls.doString ("return CalcEvenParityBit (true, true, false, true)");
   BOOST_CHECK (parity == true);
   parity = ls.doString ("return CalcEvenParityBit (false, true, true, false)");
   BOOST_CHECK (parity == false);

   LuaValue concatenated =
      ls.doString ("return Concatenate ('a', 'b', 'c', 'd', 'ef')");
   BOOST_CHECK (concatenated == "abcdef");
}



// - TestExceptions ------------------------------------------------------------
void TestExceptions()
{
   using namespace Diluculum;

   // First, create a Lua state and register the functions
   LuaState ls;

   ls["ZeroTheGlobal"] = ZeroTheGlobal;
   ls["SetTheGlobalDirectly"] = SetTheGlobalDirectly;
   ls["SetTheGlobalAddingAString"] = SetTheGlobalAddingAString;
   ls["SetTheGlobalMultiplying"] = SetTheGlobalMultiplying;
   ls["SetTheGlobalFirstParamDefinesOperation"] =
      SetTheGlobalFirstParamDefinesOperation;
   ls["SetTheGlobalLastParamSelectsValue"] = SetTheGlobalLastParamSelectsValue;
   ls["WhatIsYourFavoriteColor"] = WhatIsYourFavoriteColor;
   ls["GetTableSize"] = GetTableSize;
   ls["Mod"] = Mod;
   ls["Mean"] = Mean;
   ls["CalcEvenParityBit"] = CalcEvenParityBit;
   ls["Concatenate"] = Concatenate;

   // Then, make some invalid calls and ensure that the proper exception is
   // thrown. First, try passing the wrong number of parameters.
   BOOST_CHECK_THROW (ls.doString ("ZeroTheGlobal (2)"), LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("SetTheGlobalDirectly()"), LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("SetTheGlobalAddingAString()"),
                      LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("SetTheGlobalMultiplying()"),
                      LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("SetTheGlobalFirstParamDefinesOperation()"),
                      LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("SetTheGlobalLastParamSelectsValue()"),
                      LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("WhatIsYourFavoriteColor('Lancelot')"),
                      LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("GetTableSize()"), LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("Mod()"), LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("Mean()"), LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("CalcEvenParityBit()"), LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("Concatenate()"), LuaRunTimeError);

   // Now, some arguments of the wrong type
   BOOST_CHECK_THROW (ls.doString ("SetTheGlobalDirectly ('one')"),
                      LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("SetTheGlobalAddingAString (true, false)"),
                      LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("SetTheGlobalMultiplying (nil, 1, 'augh!')"),
                      LuaRunTimeError);
   BOOST_CHECK_THROW (
      ls.doString ("SetTheGlobalFirstParamDefinesOperation ('uh?', 2, 3, 4)"),
      LuaRunTimeError);
   BOOST_CHECK_THROW (
      ls.doString (
         "SetTheGlobalLastParamSelectsValue (true, 'false', 2, nil, 1.1)"),
      LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("GetTableSize ('not a table')"),
                      LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("Mod (true, { 4, 3 })"), LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("Mean (1, '2', true)"), LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString (
                         "CalcEvenParityBit (0, 1, { false }, '0101')"),
                      LuaRunTimeError);
   BOOST_CHECK_THROW (ls.doString ("Concatenate (1, 2, 3, 4, 5)"),
                      LuaRunTimeError);
}


using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'MakeLuaFunction' tests");
   test->add (BOOST_TEST_CASE (&TestReturnless));
   test->add (BOOST_TEST_CASE (&TestReturnful));
   test->add (BOOST_TEST_CASE (&TestExceptions));

   return test;
}
