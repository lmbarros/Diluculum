/******************************************************************************\
* LuaState                                                                     *
* A pleasant way to use a Lua state in C++.                                    *
*                                                                              *
*                                                                              *
* Copyright (C) 2005-2007 by Leandro Motta Barros.                             *
*                                                                              *
* Permission is hereby granted, free of charge, to any person obtaining a copy *
* of this software and associated documentation files (the "Software"), to     *
* deal in the Software without restriction, including without limitation the   *
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or  *
* sell copies of the Software, and to permit persons to whom the Software is   *
* furnished to do so, subject to the following conditions:                     *
*                                                                              *
* The above copyright notice and this permission notice shall be included in   *
* all copies or substantial portions of the Software.                          *
*                                                                              *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS *
* IN THE SOFTWARE.                                                             *
\******************************************************************************/

#include <cassert>
#include <typeinfo>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaUtils.hpp>


namespace Diluculum
{
   // - LuaState::LuaState -----------------------------------------------------
   LuaState::LuaState (bool loadStdLib)
      : state_(0), ownsState_(true)
   {
      state_ = luaL_newstate();
      if (state_ == 0)
         throw LuaError ("Error opening Lua state.");

      if (loadStdLib)
         luaL_openlibs (state_);
   }


   LuaState::LuaState (lua_State* state, bool loadStdLib)
      : state_(state), ownsState_(false)
   {
      if (state_ == 0)
         throw LuaError ("Constructor of 'LuaState' got a NULL pointer.");

      if (loadStdLib)
         luaL_openlibs (state_);
   }



   // - LuaState::~LuaState ----------------------------------------------------
   LuaState::~LuaState()
   {
      if (ownsState_ && state_ != 0)
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
