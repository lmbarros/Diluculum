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



   // - LuaState::doFile -------------------------------------------------------
   LuaValueList LuaState::doFile (const boost::filesystem::path& fileName)
   {
      const int stackSizeAtBeginning = lua_gettop (state_);

      throwOnLuaError (luaL_loadfile (state_,
                                      fileName.native_file_string().c_str()));
      throwOnLuaError (lua_pcall (state_, 0, LUA_MULTRET, 0));

      const int numResults = lua_gettop (state_) - stackSizeAtBeginning;

      LuaValueList ret;

      for (int i = 1; i <= numResults; ++i)
      {
         ret.push_back (ToLuaValue (state_, -1));
         lua_pop (state_, 1);
      }

      std::reverse (ret.begin(), ret.end());

      return ret;
   }



   // - LuaState::doString -----------------------------------------------------
   LuaValueList LuaState::doString (const std::string& what)
   {
      const int stackSizeAtBeginning = lua_gettop (state_);

      throwOnLuaError (luaL_loadbuffer (state_, what.c_str(),
                                        what.length(), "line"));

      throwOnLuaError (lua_pcall (state_, 0, LUA_MULTRET, 0));

      const int numResults = lua_gettop (state_) - stackSizeAtBeginning;

      LuaValueList ret;

      for (int i = 1; i <= numResults; ++i)
      {
         ret.push_back (ToLuaValue (state_, -1));
         lua_pop (state_, 1);
      }

      std::reverse (ret.begin(), ret.end());

      return ret;
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
