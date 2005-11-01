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
          *  @return \c *this, so that a sequence of assignments, like
          *          <tt>a = b = c = 1;</tt> works.
          *  @todo Considering that <tt>LuaVariable</tt>s shouldn't be copyable
          *        by the current design, perhaps it would make more sense to
          *        return a \c LuaValue (<tt>this->value()</tt>).
          */
         LuaVariable& operator= (const LuaValue& rhs);

         /** Assigns the value of a \c LuaVariable to this \c LuaVariable.
          *  @todo The assignment is based on the value of \c rhs. Recall that
          *        "real" copy of <tt>LuaVariable</tt>s is forbidden by design,
          *        so this makes some sense. But, the design is objectionable,
          *        and I expect to rethink it soon. If I decide that copying
          *        <tt>LuaVariable</tt>s is OK, then I must change the
          *        documentation accordingly. If not, then add this discussion
          *        as a note.
          */
         LuaVariable& operator= (const LuaVariable& rhs)
         { *this = rhs.value(); return *this; }

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
         LuaVariable operator[] (const LuaValue& key);

         /** Checks whether the value stored in this variable is equal to the
          *  value at \c rhs.
          *  @param rhs The value against which the comparison will be done.
          *  @return \c true if this variable's value is equal to \c rhs.
          *          \c false otherwise.
          */
         bool operator== (const LuaValue& rhs)
         { return value() == rhs; }

      private:
         /// A sequence of keys, used to access nested tables.
         typedef std::vector<LuaValue> KeyList;

         /** Constructs a \c LuaVariable. Note that this is private, so that no
          *  one is expected to construct a \c LuaVariable directly. Use
          *  <tt>LuaState</tt>'s subscript operator instead.
          */
         LuaVariable (lua_State* state, const LuaValue& key,
                      const KeyList& predKeys = KeyList());

         /** <tt>LuaVariable</tt>'s copy constructor.
          *  @note This is just declared, not not defined. In other words,
          *        trying to copy a \c LuaVariable will result in a link-time
          *        error, and this is so by design.
          *  @todo I firstly disallowed copying because it sounded hard to keep
          *        multiple copies of the save variable in sync (for example, if
          *        a new value is assigned to a variable, its copies should
          *        refer to the new value if their \c value() method is called).
          *        Now, it sounds like it is not that hard to allow copies. Must
          *        check this.
          */
         LuaVariable (const LuaVariable&);


         /// The Lua state in which this \c LuaVariable lives.
         lua_State* state_;

         /** The sequence of keys used to get to this variable. For a global
          *  variable, this will consist of a single key; for variables inside
          *  nested tables, this sequence can be arbitrarily long.
          */
         std::vector<LuaValue> keys_;
   };

} // namespace Diluculum

#endif // _DILUCULUM_LUA_VARIABLE_HPP_
