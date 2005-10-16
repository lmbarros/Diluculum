/******************************************************************************\
* LuaVariable.cpp                                                              *
* A variable living in a Lua interpreter.                                      *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <iostream> // <--- TODO: just testing...
#include "LuaVariable.hpp"

namespace Diluculum
{

   LuaVariable& LuaVariable::operator= (const LuaValue& rhs)
   {
      std::cout << "Assigning... " << rhs.typeName() << '\n';
      return *this;
   }


   LuaValue LuaVariable::value()
   {
      return Nil;
   }

   LuaVariable LuaVariable::operator[] (const LuaValue& rhs)
   {
      return build();
   }

   bool LuaVariable::operator== (const LuaValue& rhs)
   {
      return false;
   }


   LuaVariable::LuaVariable()
   {
   }

   LuaVariable::LuaVariable (const LuaVariable&)
   {
   }

//    static LuaVariable::LuaVariable build()
//    {
//       return LuaVariable();
//    }

} // namespace Diluculum
