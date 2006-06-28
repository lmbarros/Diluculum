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

   DILUCULUM_WRAP_METHOD (Account, deposit);
   DILUCULUM_WRAP_METHOD (Account, withdraw);
   DILUCULUM_WRAP_METHOD (Account, balance);

   DILUCULUM_BEGIN_CLASS (Account);
      DILUCULUM_CLASS_METHOD (Account, deposit);
      DILUCULUM_CLASS_METHOD (Account, withdraw);
      DILUCULUM_CLASS_METHOD (Account, balance);
   DILUCULUM_END_CLASS (Account);


   /// A quite ridiculous class, but OK for testing...
   class NumberProperties
   {
      public:
         NumberProperties (const LuaValueList& params)
         {
            number_ = static_cast<unsigned>(params[0].asNumber());
         }

         LuaValueList isEven (const LuaValueList& params)
         {
            LuaValueList ret;
            ret.push_back (number_ % 2 == 0);
            return ret;
         }

         LuaValueList isOdd (const LuaValueList& params)
         {
            LuaValueList ret;
            ret.push_back (number_ % 2 != 0);
            return ret;
         }

         LuaValueList isBig (const LuaValueList& params)
         {
            LuaValueList ret;
            ret.push_back (number_ > 1000);
            return ret;
         }

      private:
         unsigned number_;
   };

   DILUCULUM_WRAP_METHOD (NumberProperties, isEven);
   DILUCULUM_WRAP_METHOD (NumberProperties, isOdd);
   DILUCULUM_WRAP_METHOD (NumberProperties, isBig);

   DILUCULUM_BEGIN_CLASS (NumberProperties);
      DILUCULUM_CLASS_METHOD (NumberProperties, isEven);
      DILUCULUM_CLASS_METHOD (NumberProperties, isOdd);
      DILUCULUM_CLASS_METHOD (NumberProperties, isBig);
   DILUCULUM_END_CLASS (NumberProperties);



   /** A simple class, created just to test if the destructor is really being
    *  called when it is expected to be called.
    */
   class DestructorTester
   {
      public:
         /** A flag that is set to \c true in the destructor. \c static, so that
          *  it can be accessed after the object destruction.
          */
         static bool aFlag;

         /// A constructor with this signature is always necessary...
         DestructorTester (const LuaValueList&) { }

         /// The destructor. Sets \c aFlag to \c true.
         ~DestructorTester() { aFlag = true; }
   };

   bool DestructorTester::aFlag = false;

   DILUCULUM_BEGIN_CLASS (DestructorTester);
   DILUCULUM_END_CLASS (DestructorTester);

} // (anonymous) namespace

#endif // _DILUCULUM_TESTS_WRAPPED_CLASSES_HPP_
