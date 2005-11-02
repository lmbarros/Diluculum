/******************************************************************************\
* LuaVariable.hpp                                                              *
* A variable living in a Lua interpreter.                                      *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _DILUCULUM_LUA_VARIABLE_HPP_
#define _DILUCULUM_LUA_VARIABLE_HPP_

#include <vector>
#include "LuaValue.hpp"


namespace Diluculum
{
   /** A variable living in a Lua state. Notice the crucial difference: unlike a
    *  \c LuaValue, a \c LuaVariable necessarily has a real counterpart in a Lua
    *  state. Thus, when something is assigned to a \c LuaVariable, the value of
    *  the corresponding variable in the Lua state is changed, too.
    *  <p><tt>LuaVariable</tt>s cannot be directly constructed. They are
    *  designed to be returned by <tt>LuaState</tt>'s subscript operator.
    *  @todo Add tests for expected exceptions.
    */
   class LuaVariable
   {
      friend class LuaState;

      public:
         /** Assigns a new value to this \c LuaVariable. The corresponding
          *  variable in the Lua state is updated accordingly.
          *  @param rhs The new value for the variable.
          *  @return \c rhs, so that a sequence of assignments, like
          *          <tt>a = b = c = 1;</tt> works. Actually, returning
          *          \c rhs is the same as returning <tt>this->value()</tt>,
          *          that perhaps make more sense.
          */
         const LuaValue& operator= (const LuaValue& rhs);

         /** Returns the value associated with this variable.
          *  @todo Throw a more specific exception (instead \c LuaError), and a
          *        more adequate message (instead of "Duh"). When doing this,
          *        add some tests to ensure that the proper exception is thrown
          *        when calling \c value() and \c operator[].
          *  @bug In fact, no exception is currently being thrown. Must add test
          *       cases, check the code and document.
          */
         LuaValue value() const;

         /** Assuming that this \c LuaVariable holds a table, returns the value
          *  whose index is \c key.
          *  @param key The key whose value is desired.
          *  @return The value whose index is \c key.
          *  @todo What about exceptions? Must add test cases, check the code
          *        and document.
          */
         LuaVariable operator[] (const LuaValue& key) const;

         /** Checks whether the value stored in this variable is equal to the
          *  value at \c rhs.
          *  @param rhs The value against which the comparison will be done.
          *  @return \c true if this variable's value is equal to \c rhs.
          *          \c false otherwise.
          */
         bool operator== (const LuaValue& rhs) const
         { return value() == rhs; }

         /** Checks whether this \c LuaVariable refers to the same variable as
          *  \c rhs.
          *  @param rhs The variable against which the comparison will be done.
          *  @return \c true if both involved <tt>LuaVariable</tt>s refer to the
          *          same variable. \c false otherwise.
          */
         bool operator== (const LuaVariable& rhs) const
         { return state_ == rhs.state_ && keys_ == rhs.keys_; }

         /** Checks whether this \c LuaVariable refers to a different variable
          *  than \c rhs.
          *  @param rhs The variable against which the comparison will be done.
          *  @return \c false if both involved <tt>LuaVariable</tt>s refer to the
          *          same variable. \c true otherwise.
          */
         bool operator!= (const LuaVariable& rhs) const
         { return state_ != rhs.state_ || keys_ != rhs.keys_; }

      private:
         /// A sequence of keys, used to access nested tables.
         typedef std::vector<LuaValue> KeyList;

         /** Constructs a \c LuaVariable.
          *   @note This is private because no one is expected to construct a
          *         \c LuaVariable directly. For Diluculum users, the only
          *         allowed way to create a new \c LuaVariable is by using
          *         <tt>LuaState</tt>'s subscript operator.
          *   @param state The <tt>lua_State*</tt> in which the variable
          *          represented by this \c LuaVariable resides.
          *   @param key The "key" used to access the variable represented by
          *          this \c LuaVariable. This can be variable name (for the case
          *          of a global variable) or a "real" key (when it is a field in
          *          a possibly nested table).

          *   @param predKeys The sequence of keys used to access the table that
          *          contains the variable represented by this \c LuaVariable.
          *          For a global variable, this list will be empty. For a
          *          variable stored in a possibly nested table, the list can be
          *          arbitrarily long.
          */
         LuaVariable (lua_State* state, const LuaValue& key,
                      const KeyList& predKeys = KeyList());

         /// The Lua state in which this \c LuaVariable lives.
         lua_State* state_;

         /** The sequence of keys used to get to this variable. For a global
          *  variable, this will consist of a single key; for variables inside
          *  nested tables, this sequence can be arbitrarily long.
          *  <p>Just to relate this with the parameters passed to the
          *  constructor: this sequence equals to \c predKeys parameter, with
          *  the \c key parameter appended to it.
          */
         std::vector<LuaValue> keys_;
   };

} // namespace Diluculum

#endif // _DILUCULUM_LUA_VARIABLE_HPP_
