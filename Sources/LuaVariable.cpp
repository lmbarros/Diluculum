/******************************************************************************\
* LuaVariable.cpp                                                              *
* A variable living in a Lua interpreter.                                      *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <cassert>
#include "LuaState.hpp"
#include "LuaUtils.hpp"
#include "LuaVariable.hpp"


namespace Diluculum
{
   // - LuaVariable::LuaVariable -----------------------------------------------
   LuaVariable::LuaVariable (lua_State* state, const LuaValue& key,
                             const KeyList& predKeys)
      : state_(state), keys_(predKeys)
   {
      keys_.push_back (key);
   }



   // - LuaVariable::operator= -------------------------------------------------
   LuaVariable& LuaVariable::operator= (const LuaValue& rhs)
   {
      // Push the globals table onto the stack
      lua_pushstring (state_, "_G");
      lua_gettable (state_, LUA_GLOBALSINDEX);

      // Reach the "final" table (and leave it at the stack top)
      typedef std::vector<LuaValue>::const_iterator iter_t;

      assert (keys_.size() > 0 && "At least one key should be present here.");

      iter_t end = keys_.end();
      --end;

      for (iter_t p = keys_.begin(); p != end; ++p)
      {
         PushLuaValue (state_, *p);
         lua_gettable (state_, -2);
         if (!lua_istable(state_, -1))
         {
            throw LuaError ("Duh.");
         }

         lua_remove (state_, -2);
      }

      // Write the new value
      PushLuaValue (state_, keys_.back());
      PushLuaValue (state_, rhs);
      lua_settable (state_, -3);
      lua_pop (state_, 1);

      return *this;
   }



   // - LuaVariable::value -----------------------------------------------------
   LuaValue LuaVariable::value() const
   {
      int index = LUA_GLOBALSINDEX;

      typedef std::vector<LuaValue>::const_iterator iter_t;
      for (iter_t p = keys_.begin(); p != keys_.end(); ++p)
      {
         PushLuaValue (state_, *p);
         lua_gettable (state_, index);
//          if (!lua_istable(state_, -1))
//          {
//             throw LuaError ("Duh.");
//          }

         if (index != LUA_GLOBALSINDEX)
            lua_remove (state_, -2);
         else
            index = -2;
      }

      LuaValue ret = ToLuaValue (state_, -1);
      lua_pop (state_, 1);
      return ret;
   }



   // - LuaVariable::operator[] ------------------------------------------------
   LuaVariable LuaVariable::operator[] (const LuaValue& key)
   {
      return LuaVariable (state_, key, keys_);
   }


} // namespace Diluculum
