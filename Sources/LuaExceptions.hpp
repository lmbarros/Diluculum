/******************************************************************************\
* LuaExceptions.hpp                                                            *
* Exceptions related to Lua errors.                                            *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _DILUCULUM_LUA_EXCEPTIONS_HPP_
#define _DILUCULUM_LUA_EXCEPTIONS_HPP_

#include <stdexcept>

namespace Diluculum
{
   /// A generic Lua-related error.
   class LuaError: public std::runtime_error
   {
      public:
         /** Constructs a \c LuaError object.
          *  @param what The message associated with the error.
          */
         LuaError (const char* what)
            : std::runtime_error (what)
         { }
   };



   /// A Lua run-time error.
   class LuaRunTimeError: public LuaError
   {
      public:
         /** Constructs a \c LuaRunTimeError object.
          *  @param what The message associated with the error. Typically will
          *              be the error message issued by the Lua interpreter.
          */
         LuaRunTimeError (const char* what)
            : LuaError (what)
         { }
   };



   /// A Lua file-related error.
   class LuaFileError: public LuaError
   {
      public:
         /** Constructs a \c LuaFileError object.
          *  @param what The message associated with the error.
          */
         LuaFileError (const char* what)
            : LuaError (what)
         { }
   };



   /// An error while interpreting a chunk of Lua code.
   class LuaSyntaxError: public LuaError
   {
      public:
         /** Constructs a \c LuaSyntaxError object.
          *  @param what The message associated with the error. Typically will be
          *              the error message issued by the Lua interpreter.
          */
         LuaSyntaxError (const char* what)
            : LuaError (what)
         { }
   };



   /// A memory error reported by Lua.
   class LuaMemoryError: public LuaError
   {
      public:
         /** Constructs a \c LuaMemoryError object.
          *  @param what The message associated with the error.
          */
         LuaMemoryError (const char* what)
            : LuaError (what)
         { }
   };



   /// An error while executing the Lua error handler.
   class LuaErrorError: public LuaError
   {
      public:
         /** Constructs a \c LuaErrorError object.
          *  @param what The message associated with the error.
          */
         LuaErrorError (const char* what)
            : LuaError (what)
         { }
   };



   /// An error related to types when doing some Lua-related processing.
   class LuaTypeError: public LuaError
   {
      public:
         /** Constructs a \c LuaTypeError object.
          *  @param what The message associated with the error.
          */
         LuaTypeError (const char* what)
            : LuaError (what)
         { }
   };

} // namespace Diluculum


#endif // _DILUCULUM_LUA_EXCEPTIONS_HPP_
