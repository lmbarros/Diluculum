/******************************************************************************\
* LuaState                                                                     *
* A pleasant way to use a Lua state in C++.                                    *
* Leandro Motta Barros                                                         *
\******************************************************************************/


#ifndef _DILUCULUM_LUA_STATE_HPP_
#define _DILUCULUM_LUA_STATE_HPP_

extern "C"
{
#  include <lua.h>
#  include <lualib.h>
#  include <lauxlib.h>
}
#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>
#include "LuaValue.hpp"
#include "LuaVariable.hpp"


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



   /** A type representing the return value of a Lua function call. The
    *  first return value is stored at the 0th \c vector position, the second
    *  return value at the 1st \c vector position and so on.
    */
   typedef std::vector<LuaValue> LuaRetVal;



   /** \c LuaState: The Next Generation. The pleasant way to do perform relevant
    *  operations on a Lua state.
    *  <p>(My previous implementation of a class named \c LuaState was pretty
    *  low-level. It was essentially a C++ wrapper around a <tt>lua_State*</tt>,
    *  without higher level operations. This is an attempt to allow me to do
    *  the kind of things I do most of the time without much effort.)
    *  @todo There is a good amount of code replicated in \c doStringMultRet()
    *        and \c doFileMultRet(). And that's not a pretty thing.
    */
   class LuaState
   {
      public:
         /** Constructs a \c LuaState.
          *  @param loadStdLib If \c true (the default), makes all
          *         the Lua standard libraries available.
          *  @throw LuaError If something goes wrong.
          */
         explicit LuaState (bool loadStdLib = true);

         /** Destructs a \c LuaState. This calls \c lua_close()
          *  on the underlying \c lua_State*.
          */
         virtual ~LuaState();

         /** Executes the file passed as parameter and returns all the values
          *  returned by this execution.
          *  @param fileName The file to be executed.
          *  @return All the values returned by the file execution.
          *  @throw LuaError \c LuaError or any of its subclasses can be thrown.
          *         In particular, \c LuaTypeError will be thrown if the
          *         execution returns a type not supported by \c LuaType.
          */
         LuaRetVal doFileMultRet (const boost::filesystem::path& fileName);

         /** Executes the file passed as parameter and returns only the first
          *  value returned by this execution.
          *  @param fileName The file to be executed.
          *  @return The first value returned by the file execution. If the
          *          execution does not return anything, returns \c Nil.
          *  @throw LuaError \c LuaError or any of its subclasses can be thrown.
          *         In particular, \c LuaTypeError will be thrown if the
          *         execution returns a type not supported by \c LuaType.
          *  @todo The current implementation is based on \c doFileMultRet().
          *        An implementation "from scratch" can be more efficient.
          */
         LuaValue doFile (const boost::filesystem::path& fileName);

         /** Executes the string passed as parameter and returns all the values
          *  returned by this execution.
          *  @param what The string to be interpreted.
          *  @return All the values returned by the execution of \c what.
          *  @throw LuaError \c LuaError or any of its subclasses can be thrown.
          *         In particular, \c LuaTypeError will be thrown if the
          *         execution returns a type not supported by \c LuaType.
          */
         LuaRetVal doStringMultRet (const std::string& what);

         /** Executes the string passed as parameter and returns only the first
          *  value returned by this execution.
          *  @param what The string to be executed.
          *  @return The first value returned by the execution of \c what. If
          *          the execution does not return anything, returns \c Nil.
          *  @throw LuaError \c LuaError or any of its subclasses can be thrown.
          *         In particular, \c LuaTypeError will be thrown if the
          *         execution returns a type not supported by \c LuaType.
          *  @todo The current implementation is based on \c doStringMultRet().
          *        An implementation "from scratch" can be more efficient.
          */
         LuaValue doString (const std::string& what);

         // <--- TODO: document after we have something working.
         LuaVariable operator[] (const std::string& key);

      private:
         /// The underlying \c lua_State*.
         lua_State* state_;

         /** Converts and returns the element at index \c index on the stack to
          *  a \c LuaValue. This keeps the Lua stack untouched. Oh, yes, and it
          *  accepts both positive and negative indices, just like the standard
          *  functions on the Lua C API.
          *  @throw LuaTypeError If the element at \c index cannot be converted
          *         to a \c LuaValue. This can happen if the value at that
          *         position is, for example, a "Lua Thread" that is not
          *         supported by \c LuaValue.
          */
         LuaValue toLuaValue (int index);

         /** Throws an exception if the number passed as parameter corresponds
          *  to an error code from a function from the Lua API.  The exception
          *  thrown is of the proper type, that is, of the subclass of \c
          *  LuaError that best describes the error.  Furthermore, the string
          *  associated with the exception (the \c what parameter) is set to
          *  whatever Lua returned as the error message.
          *  <p>If the number passed as parameter is 0 (which is the code for
          *  "no error"), the function does nothing.
          *  @param retCode The return code for some Lua API function;
          *         that's the value we want to check for "errorness".
          *  @throw LuaRunTimeError If <tt>retCode == LUA_ERRRUN</tt>.
          *  @throw LuaFileError If <tt>retCode == LUA_ERRFILE</tt>.
          *  @throw LuaRunTimeError If <tt>retCode == LUA_ERRRUN</tt>.
          *  @throw LuaSyntaxError If <tt>retCode == LUA_ERRSYNTAX</tt>.
          *  @throw LuaMemoryError If <tt>retCode == LUA_ERRMEM</tt>.
          *  @throw LuaError If \c retCode is not a recognized Lua error code.
          */
         void throwOnLuaError (int retCode);
   };

} // namespace Diluculum

#endif // #ifndef _DILUCULUM_LUA_STATE_HPP_
