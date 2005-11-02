/******************************************************************************\
* LuaExceptions.hpp                                                            *
* Exceptions related to Lua errors.                                            *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include "LuaExceptions.hpp"

namespace Diluculum
{
   // - TypeMismatchError::TypeMismatchError -----------------------------------
   TypeMismatchError::TypeMismatchError (const std::string& expectedType,
                                         const std::string& foundType)
      : LuaError (("Type mismatch: '" + expectedType
                   + "' was expected but '" + foundType
                   + "' was found.").c_str()),
        expectedType_ (expectedType), foundType_(foundType)
   { }

} // namespace Diluculum
