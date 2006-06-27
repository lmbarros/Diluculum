/******************************************************************************\
* WrappedClasses.hpp                                                           *
* C++ classes wrapped for use with Lua for testing purposes.                   *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _DILUCULUM_TESTS_WRAPPED_CLASSES_HPP_
#define _DILUCULUM_TESTS_WRAPPED_CLASSES_HPP_

#include <Diluculum/LuaWrappers.hpp>

namespace
{
   using Diluculum::LuaValueList;

   /// The good and old \c Account class...
   class Account
   {
      public:
         Account (const LuaValueList& params)
         {
            if (params.size() == 0)
               balance_ = 0.0;
            else if (params.size() == 1 && params[0].type() == LUA_TNUMBER)
               balance_ = params[0].asNumber();
            else
               throw Diluculum::LuaError ("Bad parameters!");
         }

         LuaValueList deposit (const LuaValueList& params)
         {
            if (params.size() != 1 || params[0].type() != LUA_TNUMBER)
               throw Diluculum::LuaError ("Bad parameters!");

            balance_ += params[0].asNumber();

            return LuaValueList();
         }

         LuaValueList withdraw (const LuaValueList& params)
         {
            if (params.size() != 1 || params[0].type() != LUA_TNUMBER)
               throw Diluculum::LuaError ("Bad parameters!");

            balance_ -= params[0].asNumber();

            return LuaValueList();
         }

         LuaValueList balance (const LuaValueList& params)
         {
            LuaValueList ret;
            ret.push_back (balance_);
            return ret;
         }

      private:
         double balance_;
   };



   // Call the macros necessary to create the wrappers.
   DILUCULUM_WRAP_METHOD (Account, deposit);
   DILUCULUM_WRAP_METHOD (Account, withdraw);
   DILUCULUM_WRAP_METHOD (Account, balance);

   DILUCULUM_BEGIN_CLASS (Account);
      DILUCULUM_CLASS_METHOD (Account, deposit);
      DILUCULUM_CLASS_METHOD (Account, withdraw);
      DILUCULUM_CLASS_METHOD (Account, balance);
   DILUCULUM_END_CLASS (Account);

} // (anonymous) namespace

#endif // _DILUCULUM_TESTS_WRAPPED_CLASSES_HPP_
