/******************************************************************************\
* MakeLuaFunction.hpp                                                          *
* Macros (yes, macros) to create 'lua_CFunctions' from slightly more friendly  *
* functions taking (and possibly returning) 'LuaValue's.                       *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <Diluculum/MakeLuaFunction.hpp>

namespace Diluculum
{
   namespace Impl
   {
      // - MLF_IssueLuaErrorIf  ------------------------------------------------
      void MLF_IssueLuaErrorIf (lua_State* ls, bool condition,
                                const std::string& function,
                                const std::string& message)
      {
         if (condition)
         {
            lua_pushstring (ls, ("When calling '" + function + "()': "
                                 + message).c_str());
            lua_error (ls);
         }
      }



      // - MLF_ReadParameters -----------------------------------------------------
      void MLF_ReadParameters (lua_State* ls, Diluculum::LuaValue params[], int n)
      {
         for (int i = 0; i < n; ++i)
            params[i] = Diluculum::ToLuaValue (ls, i+1);
      }

   } // namespace Impl

} // namespace Diluculum
