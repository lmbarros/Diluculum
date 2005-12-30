/******************************************************************************\
* LuaState                                                                     *
* A pleasant way to use a Lua state in C++.                                    *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <cassert>
#include <typeinfo>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaUtils.hpp>


namespace Diluculum
{
   // - LuaState::LuaState -----------------------------------------------------
   LuaState::LuaState (bool loadStdLib)
      : state_(0)
   {
      state_ = lua_open();
      if (state_ == 0)
         throw LuaError ("Error opening Lua state.");

      if (loadStdLib)
      {
         luaopen_base (state_);
         luaopen_table (state_);
         luaopen_io (state_);
         luaopen_string (state_);
         luaopen_math (state_);
         luaopen_debug (state_);
         luaopen_loadlib (state_);
      }
   }



   // - LuaState::~LuaState ----------------------------------------------------
   LuaState::~LuaState()
   {
      if (state_ != 0)
         lua_close (state_);
   }



   // - LuaState::doStringOrFile -----------------------------------------------
   LuaValueList LuaState::doStringOrFile (bool isString, const std::string& str)
   {
      const int stackSizeAtBeginning = lua_gettop (state_);

      if (isString)
      {
         throwOnLuaError (luaL_loadbuffer (state_, str.c_str(),
                                           str.length(), "line"));
      }
      else
      {
         throwOnLuaError (luaL_loadfile (state_, str.c_str()));
      }

      throwOnLuaError (lua_pcall (state_, 0, LUA_MULTRET, 0));

      const int numResults = lua_gettop (state_) - stackSizeAtBeginning;

      LuaValueList results;

      for (int i = numResults; i > 0; --i)
         results.push_back (ToLuaValue (state_, -i));

      lua_pop (state_, numResults);

      return results;
   }



   // - LuaState::operator[] ---------------------------------------------------
   LuaVariable LuaState::operator[] (const std::string& variable)
   {
      return LuaVariable (state_, variable);
   }



   // - LuaState::throwOnLuaError ----------------------------------------------
   void LuaState::throwOnLuaError (int retCode)
   {
      if (retCode != 0)
      {
         std::string errorMessage;
         if (lua_isstring (state_, -1))
         {
            errorMessage = lua_tostring (state_, -1);
            lua_pop (state_, 1);
         }
         else
         {
            errorMessage =
               "Sorry, there is no additional information about this error.";
         }

         switch (retCode)
         {
            case LUA_ERRRUN:
               throw LuaRunTimeError (errorMessage.c_str());
            case LUA_ERRFILE:
               throw LuaFileError (errorMessage.c_str());
            case LUA_ERRSYNTAX:
               throw LuaSyntaxError (errorMessage.c_str());
            case LUA_ERRMEM:
               throw LuaMemoryError (errorMessage.c_str());
            case LUA_ERRERR:
               throw LuaErrorError (errorMessage.c_str());
            default:
               throw LuaError ("Unknown Lua return code passed "
                               "to 'LuaState::throwOnLuaError'.");
         }
      }
   }

} // namespace Diluculum
