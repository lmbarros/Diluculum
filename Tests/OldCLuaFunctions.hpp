/******************************************************************************\
* OldCLuaFunctions.hpp                                                         *
* Some Lua functions implemented in C for use in unit tests. (These functions  *
* use the old, deprecated binding system.)                                     *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _DILUCULUM_TESTS_OLD_CLUA_FUNCTIONS_HPP_
#define _DILUCULUM_TESTS_OLD_CLUA_FUNCTIONS_HPP_

#include <Diluculum/MakeLuaFunction.hpp>

namespace
{
   /** A global variable used in some "C Lua Functions" defined below. This
    *  global is changed as the side effect of calling these functions, so that
    *  it is possible to check whether a returnless function is being really
    *  executed.
    */
   int TheGlobal;


   /// Sets \c TheGlobal to zero.
   void LuaC_ZeroTheGlobal()
   {
      TheGlobal = 0;
   }

   DILUCULUM_MAKE_LUA_FUNCTION_0_VOID (LuaC_ZeroTheGlobal, ZeroTheGlobal,
                                       "ZeroTheGlobal");


   /// Sets \c TheGlobal to the \c value parameter.
   void LuaC_SetTheGlobalDirectly (const Diluculum::LuaValue& value)
   {
      lua_Number n = value.asNumber();
      TheGlobal = static_cast<int>(n);
   }

   DILUCULUM_MAKE_LUA_FUNCTION_1_VOID (LuaC_SetTheGlobalDirectly,
                                       SetTheGlobalDirectly,
                                       "SetTheGlobalDirectly");


   /** Sets \c TheGlobal to a number that is the sum of \c intValue and the
    *  value of \c strValue converted to a number. \c strValue must a string,
    *  either <tt>"one"</tt> or <tt>"two"</tt>.
    */
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


   /** Sets \c TheGlobal to be equal to the product of the numbers \c v1, \c v2
    *  and \c v3 passed as parameter.
    */
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


   /** Sets the value of \c TheGlobal to a number that is either the sum or the
    *  product of \c v2, \c v3 and \c v4. The selection of the operation is
    *  defined by the first parameter, \c v1: if it is \c true, a sum is
    *  performed. If it is \c false, a product is performed.
    */
   void LuaC_SetTheGlobalFirstParamDefinesOperation(
      const Diluculum::LuaValue& v1, const Diluculum::LuaValue& v2,
      const Diluculum::LuaValue& v3, const Diluculum::LuaValue& v4)
   {
      if (v1.asBoolean())
      {
         TheGlobal = static_cast<int>(v2.asNumber())
            + static_cast<int>(v3.asNumber())
            + static_cast<int>(v4.asNumber());
      }
      else
      {
         TheGlobal = static_cast<int>(v2.asNumber())
            * static_cast<int>(v3.asNumber())
            * static_cast<int>(v4.asNumber());
      }
   }

   DILUCULUM_MAKE_LUA_FUNCTION_4_VOID(
      LuaC_SetTheGlobalFirstParamDefinesOperation,
      SetTheGlobalFirstParamDefinesOperation,
      "SetTheGlobalFirstParamDefinesOperation");


   /** Sets \c TheGlobal to one of the values from \c v1 to \c v4. The fifth
    *  parameter, \c v5, selects which of them is used. If \c v5 is 1, \c v1 is
    *  used; if \c v5 is 2, \c v2 is used, and so on.
    */
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


   /// Returns the string <tt>"Blue"</tt>.
   Diluculum::LuaValue LuaC_WhatIsYourFavoriteColor()
   {
      return "Blue";
   }

   DILUCULUM_MAKE_LUA_FUNCTION_0_RET (LuaC_WhatIsYourFavoriteColor,
                                      WhatIsYourFavoriteColor,
                                      "WhatIsYourFavoriteColor");


   /// Returns the size of the table passed as parameter.
   Diluculum::LuaValue LuaC_GetTableSize (const Diluculum::LuaValue& table)
   {
      return table.asTable().size();
   }

   DILUCULUM_MAKE_LUA_FUNCTION_1_RET (LuaC_GetTableSize, GetTableSize,
                                      "GetTableSize");


   /// Returns the modulus (remainder of integer division) of \c v1 and \c v2.
   Diluculum::LuaValue LuaC_Mod (const Diluculum::LuaValue& v1,
                                 const Diluculum::LuaValue& v2)
   {
      return static_cast<int>(v1.asNumber()) % static_cast<int>(v2.asNumber());
   }

   DILUCULUM_MAKE_LUA_FUNCTION_2_RET (LuaC_Mod, Mod, "Mod");


   /// Returns the mean of \c v1, \c v2 and \c v3.
   Diluculum::LuaValue LuaC_Mean (const Diluculum::LuaValue& v1,
                                  const Diluculum::LuaValue& v2,
                                  const Diluculum::LuaValue& v3)
   {
      return (v1.asNumber() + v2.asNumber() + v3.asNumber()) / 3.0;
   }

   DILUCULUM_MAKE_LUA_FUNCTION_3_RET (LuaC_Mean, Mean, "Mean");


   /** Returns a boolean representing the even parity bit for the values passed
    *  from \c v1 to \c v4.
    */
   Diluculum::LuaValue LuaC_CalcEvenParityBit(
      const Diluculum::LuaValue& v1, const Diluculum::LuaValue& v2,
      const Diluculum::LuaValue& v3, const Diluculum::LuaValue& v4)
   {
      int n = v1.asBoolean() + v2.asBoolean() + v3.asBoolean() + v4.asBoolean();
      return n % 2 == 1;
   }

   DILUCULUM_MAKE_LUA_FUNCTION_4_RET (LuaC_CalcEvenParityBit, CalcEvenParityBit,
                                      "CalcEvenParityBit");


   /** Returns a string that is a concatenation of the five strings passed as
    *  parameters.
    */
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

#endif // _DILUCULUM_TESTS_OLD_CLUA_FUNCTIONS_HPP_
