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
   // - LuaState::doFile -------------------------------------------------------
   LuaValue LuaState::doFile (const boost::filesystem::path& fileName)
   {
      return LuaValue(); // <--- stub!
   }



   // - LuaState::doString -----------------------------------------------------
   LuaValue LuaState::doString (const std::string& what)
   {
      const int stackSizeAtBeginning = lua_gettop (state_);
      throwOnLuaError (lua_pcall (state_, 0, 1, 0));
   }

} // namespace Diluculum
