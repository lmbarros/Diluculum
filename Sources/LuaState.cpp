/******************************************************************************\
* LuaState                                                                     *
* A pleasant way to use a Lua state in C++.                                    *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include "LuaState.hpp"
#include <cassert>
#include <typeinfo>


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



   // - LuaState::doStringMultRet ----------------------------------------------
   LuaRetVal LuaState::doStringMultRet (const std::string& what)
   {
      const int stackSizeAtBeginning = lua_gettop (state_);

      throwOnLuaError (luaL_loadbuffer (state_, what.c_str(),
                                        what.length(), "line"));

      throwOnLuaError (lua_pcall (state_, 0, LUA_MULTRET, 0));

      const int numResults = lua_gettop (state_) - stackSizeAtBeginning;

      LuaRetVal ret;

      for (int i = 1; i <= numResults; ++i)
      {
         ret.push_back (toLuaValue (-1));
         lua_pop (state_, 1);
      }

      std::reverse (ret.begin(), ret.end());

      return ret;
   }


   // - LuaState::doFile -------------------------------------------------------
   LuaValue LuaState::doFile (const boost::filesystem::path& fileName)
   {
      return LuaValue(); // <--- stub!
   }



   // - LuaState::doString -----------------------------------------------------
   LuaValue LuaState::doString (const std::string& what)
   {
//       const int stackSizeAtBeginning = lua_gettop (state_);
//       throwOnLuaError (lua_pcall (state_, 0, 1, 0));
      return Nil;
   }



   // - LuaState::toLuaValue ---------------------------------------------------
   LuaValue LuaState::toLuaValue (int index)
   {
         switch (lua_type (state_, index))
         {
            case LUA_TNIL:
               return Nil;
            case LUA_TNUMBER:
               return lua_tonumber (state_, index);
            case LUA_TBOOLEAN:
               return static_cast<bool>(lua_toboolean (state_, index));
            case LUA_TSTRING:
               return lua_tostring (state_, index);
            case LUA_TTABLE:
               return EmptyTable; // <--- TODO: Whoa! This will be fun!
            default:
               throw "ARGH!"; // <--- TODO: throw something reasonable!
         }
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
