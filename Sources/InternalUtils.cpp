/******************************************************************************\
* InternalUtils.cpp                                                            *
* Miscellaneous utilities used internally by Diluculum.                        *
*                                                                              *
*                                                                              *
* Copyright (C) 2005-2009 by Leandro Motta Barros.                             *
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

#include "InternalUtils.hpp"
#include <Diluculum/LuaUtils.hpp>
#include <boost/lexical_cast.hpp>

namespace Diluculum
{
   namespace Impl
   {
      LuaValueList CallFunctionOnTop (lua_State* ls, const LuaValueList& params)
      {
         int topBefore = lua_gettop (ls);

         if (lua_type (ls, -1) != LUA_TFUNCTION)
            throw TypeMismatchError ("function", luaL_typename (ls, -1));

         typedef LuaValueList::const_iterator iter_t;
         for (iter_t p = params.begin(); p != params.end(); ++p)
            PushLuaValue (ls, *p);

         int status = lua_pcall (ls, params.size(), LUA_MULTRET, 0);

         if (status != 0)
         {
            std::string errMessage = lua_tostring (ls, -1);
            lua_pop (ls, 1);

            switch (status)
            {
               case LUA_ERRRUN:
                  throw LuaRunTimeError(
                     ("'LUA_ERRRUN' returned while calling function from Lua. "
                      "Additional error message: '" + errMessage
                      + "'.").c_str());

               case LUA_ERRMEM:
                  throw LuaRunTimeError(
                     ("'LUA_ERRMEM' returned while calling function from Lua. "
                      "Additional error message: '" + errMessage
                      + "'.").c_str());

               case LUA_ERRERR:
                  throw LuaRunTimeError(
                     ("'LUA_ERR' returned while calling function from Lua. "
                      "Additional error message: '" + errMessage
                      + "'.").c_str());

               default:
                  throw LuaRunTimeError(
                     ("Unknown error code ("
                      + boost::lexical_cast<std::string>(status)
                      + ") returned while calling function from Lua. "
                      + "Additional error message: '" + errMessage
                      + "'.").c_str());
            }
         }

         int numResults = lua_gettop (ls) - topBefore + 1;

         LuaValueList results;

         for (int i = numResults; i > 0; --i)
            results.push_back (ToLuaValue (ls, -i));

         lua_pop (ls, numResults);

         return results;
      }
   }

} // namespace Diluculum
