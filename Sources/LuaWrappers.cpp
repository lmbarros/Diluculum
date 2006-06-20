/******************************************************************************\
* LuaWrappers.cpp                                                              *
* Making C++ stuff accessible from Lua.                                        *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <Diluculum/LuaWrappers.hpp>
#include <cassert>


namespace Diluculum
{
   namespace Impl
   {
      // - ReportErrorFromCFunction --------------------------------------------
      void ReportErrorFromCFunction (lua_State* ls, const::std::string& what)
      {
         lua_Debug ar;
         int ret = lua_getstack (ls, 0, &ar);

         assert (ret != 0 && "'lua_getstack()' wasn't supposed to return '0' "
                 "here. Possible error cause: 'ReportErrorFromCFunction()' "
                 "wasn't called from a Lua function implemented in C.");

         ret = lua_getinfo (ls, "n", &ar);

         assert (ret != 0 && "'lua_getinfo()' wasn't supposed to return '1' "
                 "here. *Nothing* could go wrong at this point! Oh, well...");

         const std::string msg = std::string("Error found when calling '")
            + ar.name + "': " + what;

         lua_pushstring (ls, msg.c_str());
         lua_error (ls);
      }
   }
}
