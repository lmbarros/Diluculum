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


namespace Diluculum
{
   // Just a forward declaration.
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

         /** Returns one of the <tt>LUA_T*</tt> constants from <tt>lua.h</tt>,
          *  representing the type stored in this \c LuaValue.
          */
         int type() const;

         /** Returns the type of this \c LuaValue as a string, just like the Lua
          *  built-in function \c type().
          *  @return One of the following strings: <tt>"nil"</tt>,
          *          <tt>"boolean"</tt>, <tt>"number"</tt>, <tt>"string"</tt>,
          *          <tt>"table"</tt>. If the stored value is different than
          *          those for some reason, returns an empty string
          *          (<tt>""</tt>).
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
          *  (assuming it is a table).
          *  @throw TypeMismatchError If this \c LuaValue does not hold a table.
          *  @throw NoSuchKeyError If there is no value associated with the key
          *         passed as parameter.
          */
         const LuaValue& operator[] (const LuaValue& key) const;

      private:
         /// Almost dummy class; simply represents the type of \c nil.
         class NilType { };

         /// Stores the value (and the type) stored in this \c LuaValue.
         boost::variant <NilType, lua_Number, std::string, bool, LuaValueMap>
         value_;
   };



   /// A generic <tt>LuaValue</tt>-related error.
   class LuaValueError: public std::runtime_error
   {
      public:
         /** Constructs a \c LuaValueError object.
          *  @param what The message associated with the error.
          */
         LuaValueError (const char* what)
            : std::runtime_error (what)
         { }
   };



   /** An error in a \c LuaValue that happens when a certain type is expected
    *  but another one is found.
    */
   class TypeMismatchError: public LuaValueError
   {
      public:
         /** Constructs a \c TypeMismatchError object.
          *  @param expectedType The type that was expected.
          *  @param foundType The type that was actually found.
          */
         TypeMismatchError (const std::string& expectedType,
                            const std::string& foundType);

         /** Destroys a \c TypeMismatchError object.
          *  @note This was defined just to pretend that the destructor does not
          *        throw any exception. While this is something that I cannot
          *        guarantee (at least with this implementation), I believe this
          *        not a very dangerous lie.
          */
         ~TypeMismatchError() throw() { };

         /// Returns the type that was expected.
         std::string getExpectedType() { return expectedType_; }

         /// Returns the type that was actually found.
         std::string getFoundType() { return foundType_; }

      private:
         /// The type that was expected.
         std::string expectedType_;

         /// The type that was actually found.
         std::string foundType_;
   };



   /** An error in a table-typed \c LuaValue that happens when trying to access
    *  a key that does not exist.
    */
   class NoSuchKeyError: public LuaValueError
   {
      public:
         /** Constructs a \c NoSuchKeyError object.
          *  @param badKey The key that was not found.
          */
         NoSuchKeyError (const LuaValue& badKey);

         /** Destroys a \c NoSuchKeyError object.
          *  @note This was defined just to pretend that the destructor does not
          *        throw any exception. While this is something that I cannot
          *        guarantee (at least with this implementation), I believe this
          *        not a very dangerous lie.
          */
         ~NoSuchKeyError() throw() { };

         /// Returns the key that was not found.
         const LuaValue& getBadKey() { return badKey_; }

      private:
         /// The key that was not found.
         LuaValue badKey_;
   };



   /// A constant with the value of \c nil.
   const LuaValue Nil;

   /// A constant with value of an empty table.
   const LuaValueMap EmptyLuaValueMap;
   const LuaValue EmptyTable (EmptyLuaValueMap);

} // namespace Diluculum


#endif // _DILUCULUM_LUA_VALUE_HPP_
