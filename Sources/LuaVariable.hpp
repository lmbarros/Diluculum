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
   /** @todo Add tests for expected exceptions.
    */
   class LuaVariable
   {
      friend class LuaState;

      public:
         LuaVariable& operator= (const LuaValue& rhs);

         /** @todo Throw a more specific exception (instead \c LuaError), and a
          *        more adequate message (instead of "Duh"). When doing this,
          *        add some tests to ensure that the proper exception is thrown
          *        when calling \c value() and \c operator[]. It is probably a
          *        good idea to put all exceptions in a new file
          *        (<tt>LuaExceptions.hpp</tt>?)
          */
         LuaValue value();

         LuaVariable operator[] (const LuaValue& rhs);

         bool operator== (const LuaValue& rhs);

      private:
         typedef std::vector<LuaValue> KeyList;

         LuaVariable (lua_State* state, const LuaValue& key,
                      const KeyList& predKeys);

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

         lua_State* state_;

         std::vector<LuaValue> keys_; // sequence of keys to reach the variable,
                                      // starting from the 'LUA_GLOBALSINDEX'.
   };

} // namespace Diluculum

#endif // _DILUCULUM_LUA_VARIABLE_HPP_
