/******************************************************************************\
* LuaVariable.hpp                                                              *
* A variable living in a Lua interpreter.                                      *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _LUA_VARIABLE_HPP_
#define _LUA_VARIABLE_HPP_

#include "LuaValue.hpp"


namespace Diluculum
{
   class LuaVariable
   {
      friend class LuaState;

      public:
         LuaVariable& operator= (const LuaValue& rhs);

         LuaValue value();

         LuaVariable operator[] (const LuaValue& rhs);

         bool operator== (const LuaValue& rhs);

      private:
         LuaVariable();
         LuaVariable (const LuaVariable&);
         static LuaVariable build() { return LuaVariable(); }
   };

} // namespace Diluculum

#endif // _LUA_VARIABLE_HPP_
