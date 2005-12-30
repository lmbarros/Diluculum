/******************************************************************************\
* LuaValue.hpp                                                                 *
* A class that somewhat mimics a Lua value.                                    *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _DILUCULUM_LUA_VALUE_HPP_
#define _DILUCULUM_LUA_VALUE_HPP_

extern "C"
{
#  include <lua.h>
}
#include <map>
#include <stdexcept>
#include <string>
#include <boost/variant.hpp>
#include <Diluculum/Types.hpp>


namespace Diluculum
{
   // Some forward declarations.
   class LuaValue;


   /** Type mapping from <tt>LuaValue</tt>s to <tt>LuaValue</tt>s. Think of it
    *  as a C++ approximation of a Lua table.
    */
   typedef std::map<LuaValue, LuaValue> LuaValueMap;



   /// A class that somewhat mimics a Lua value.
   class LuaValue
   {
      public:
         /// Constructs a \c LuaValue with a \c nil value.
         LuaValue() { };

         /// Constructs a \c LuaValue with boolean type and \c b value.
         LuaValue (bool b)
            : value_(b)
         { }

         /// Constructs a \c LuaValue with number type and \c n value.
         LuaValue (double n)
            : value_(static_cast<lua_Number>(n))
         { }

         /// Constructs a \c LuaValue with number type and \c n value.
         LuaValue (float n)
            : value_(static_cast<lua_Number>(n))
         { }

         /// Constructs a \c LuaValue with number type and \c n value.
         LuaValue (long double n)
            : value_(static_cast<lua_Number>(n))
         { }

         /// Constructs a \c LuaValue with number type and \c n value.
         LuaValue (int n)
            : value_(static_cast<lua_Number>(n))
         { }

         /// Constructs a \c LuaValue with number type and \c n value.
         LuaValue (unsigned n)
            : value_(static_cast<lua_Number>(n))
         { }

         /// Constructs a \c LuaValue with number type and \c n value.
         LuaValue (unsigned long n)
            : value_(static_cast<lua_Number>(n))
         { }

         /// Constructs a \c LuaValue with number type and \c n value.
         LuaValue (long n)
            : value_(static_cast<lua_Number>(n))
         { }

         /// Constructs a \c LuaValue with string type and \c s value.
         LuaValue (const std::string& s)
            : value_(s)
         { }

         /// Constructs a \c LuaValue with string type and \c s value.
         LuaValue (const char* s)
            : value_(std::string(s))
         { }

         /// Constructs a \c LuaValue with table type and \c t value.
         LuaValue (const LuaValueMap& t)
            : value_(t)
         { }

         /// Constructs a \c LuaValue with function type and \c f value.
         LuaValue (lua_CFunction f)
            : value_(f)
         { }

         /** @todo Not tested.
          *  @todo Problem if <tt>v.size() == 0</tt>. Set to \c Nil in this
          *        case?
          */
         LuaValue (const LuaValueList& v)
         { *this = v[0]; }

         /** @todo Not tested.
          *  @todo Problem if <tt>v.size() == 0</tt>. Set to \c Nil in this
          *        case?
          */
         const LuaValueList& operator= (const LuaValueList& rhs)
         { *this = rhs[0]; return rhs; }

         /** Returns one of the <tt>LUA_T*</tt> constants from <tt>lua.h</tt>,
          *  representing the type stored in this \c LuaValue.
          */
         int type() const;

         /** Returns the type of this \c LuaValue as a string, just like the Lua
          *  built-in function \c type().
          *  @return One of the following strings: <tt>"nil"</tt>,
          *          <tt>"boolean"</tt>, <tt>"number"</tt>, <tt>"string"</tt>,
          *          <tt>"table"</tt>, <tt>"function"</tt>.
          */
         std::string typeName() const;

         /** Return the value as a number.
          *  @throw TypeMismatchError If the value is not a number (this is a
          *         strict check; no type conversion is performed).
          */
         lua_Number asNumber() const;

         /** Return the value as a string.
          *  @throw TypeMismatchError If the value is not a string (this is a
          *         strict check; no type conversion is performed).
          */
         std::string asString() const;

         /** Return the value as a boolean.
          *  @throw TypeMismatchError If the value is not a boolean (this is a
          *         strict check; no type conversion is performed).
          */
         bool asBoolean() const;

         /** Returns the value as a table (\c LuaValueMap).
          *  @note Notice that the table is returned by value. You may strongly
          *        consider using the subscript operator (that returns a
          *        reference) for accessing the values stored in a table-typed
          *        \c LuaValue.
          *  @throw TypeMismatchError If the value is not a table (this is a
          *         strict check; no type conversion is performed).
          */
         LuaValueMap asTable() const;

         /** Return the value as a function.
          *  @throw TypeMismatchError If the value is not a function (this is a
          *         strict check; no type conversion is performed).
          */
         lua_CFunction asFunction() const;

         /** "Less than" operator for <tt>LuaValue</tt>s.
          *  @return The order relationship is quite arbitrary for
          *          <tt>LuaValue</tt>s, but this has to be defined in order to
          *          \c LuaValueMap work nicely. Anyway, here are the rules used
          *          to determine who is less than who:
          *          - First, \c typeName() is called for both
          *            <tt>LuaValue</tt>s and they are compared with the usual
          *            "less than" operator for strings.
          *          - If both type names are equal, but something different
          *            than <tt>"nil"</tt> or <tt>"table"</tt>, then the values
          *            contained in the <tt>LuaValue</tt>s are compared using
          *            the usual "less than" operator for that type.
          *          - If both type names are <tt>"nil"</tt>, the function
          *            returns \c false.
          *          - If both type names are <tt>"table"</tt>, then the number
          *            of elements in each table are compared. The shorter table
          *            is "less than" the larger table.
          *          - If both tables have the same size, then each entry is
          *            recursively compared (that is, using the rules described
          *            here). For each entry, the key is compared first, than
          *            the value. This is done until finding something "less
          *            than" the other thing.
          *          - If no differences are found, \c false is obviously
          *            returned.
          */
         bool operator< (const LuaValue& rhs) const;

         /** "Greater than" operator for <tt>LuaValue</tt>s.
          *  @return The rules for determining who is greater than who are
          *          similar to the ones described in \c operator<.
          */
         bool operator> (const LuaValue& rhs) const;

         /** "Equal" operator for <tt>LuaValue</tt>s.
          *  @return \c true if <tt>*this</tt> and \c rhs have the same value.
          *          \c false otherwise.
          *  @todo A much more efficient implementation can be done.
          */
         bool operator== (const LuaValue& rhs) const
         { return !(*this > rhs) && !(*this < rhs); }

         /** "Different" operator for <tt>LuaValue</tt>s.
          *  @return \c true if <tt>*this</tt> and \c rhs don't have the same
          *          value. \c false otherwise.
          */
         bool operator!= (const LuaValue& rhs) const
         { return !(*this == rhs); }

         /** Returns a reference to a field of this \c LuaValue (assuming it is
          *  a table). If there is no value associated with the key passed as
          *  parameter, inserts a new value (\c nil) and returns a reference to
          *  it.
          *  @throw TypeMismatchError If this \c LuaValue does not hold a table.
          */
         LuaValue& operator[] (const LuaValue& key);

         /** Returns a \c const reference to a field of this \c LuaValue
          *  (assuming it is a table). If there is no value associated with the
          *  key passed as parameter, returns \c Nil.
          *  @throw TypeMismatchError If this \c LuaValue does not hold a table.
          */
         const LuaValue& operator[] (const LuaValue& key) const;

      private:
         /// Almost dummy class; simply represents the type of \c nil.
         class NilType { };

         /// Stores the value (and the type) stored in this \c LuaValue.
         boost::variant <NilType, lua_Number, std::string, bool, LuaValueMap,
                         lua_CFunction> value_;
   };



   /// A constant with the value of \c nil.
   const LuaValue Nil;

   /// A constant that is an empty \c LuaValueMap.
   const LuaValueMap EmptyLuaValueMap;

   /// A constant with value of an empty table.
   const LuaValue EmptyTable (EmptyLuaValueMap);

} // namespace Diluculum


#endif // _DILUCULUM_LUA_VALUE_HPP_
