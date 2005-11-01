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
          *  @todo Add tests for multiple assignments, like
          *        <tt>x = y = z = 1</tt>.
          */
         LuaVariable& operator= (const LuaValue& rhs);

         /** Returns the value associated with this variable.
          *  @todo Throw a more specific exception (instead \c LuaError), and a
          *        more adequate message (instead of "Duh"). When doing this,
          *        add some tests to ensure that the proper exception is thrown
          *        when calling \c value() and \c operator[]. It is probably a
          *        good idea to put all exceptions in a new file
          *        (<tt>LuaExceptions.hpp</tt>?).
          *  @bug In fact, no exception is currently being thrown. Must add test
          *       cases, check the code and document.
          */
         LuaValue value();

         /** Assuming that this \c LuaVariable holds a table, returns the value
          *  whose index is \c key.
          *  @param key The key whose value is desired.
          *  @return The value whose index is \c key.
          *  @todo What about exceptions? Must add test cases, check the code
          *        and document.
          */
         LuaVariable operator[] (const LuaValue& key);

         bool operator== (const LuaValue& rhs);

      private:
         /// A sequence of keys, used to access nested tables.
         typedef std::vector<LuaValue> KeyList;

         /** Constructs a \c LuaVariable. Note that this is private, so that no
          *  one is expected to construct a \c LuaVariable directly. Use
          *  <tt>LuaState</tt>'s subscript operator instead.
          */
         LuaVariable (lua_State* state, const LuaValue& key,
                      const KeyList& predKeys);

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

         /** @todo Documentation!
          *  @note Curiously, GCC 3.4.3 don't allow me to put this function
          *        definition on the \c .cpp file: I get "sorry, this method is
          *        private" errors when trying to call it -- even from methods
          *        from this very same class. I haven't tried other compilers
          *        and really don't know what the standard says about private
          *        static member functions (this is a conceptual monstrosity, I
          *        admit).
          */
         static LuaVariable build (lua_State* state, const LuaValue& key,
                                   const KeyList& predKeys = KeyList())
         {
            return LuaVariable (state, key, predKeys);
         }

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
