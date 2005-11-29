/******************************************************************************\
* MakeLuaFunction.hpp                                                          *
* Macros (yes, macros) to create 'lua_CFunctions' from slightly more friendly  *
* functions taking (and possibly returning) 'LuaValue's.                       *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _DILUCULUM_MAKE_LUA_FUNCTION_HPP_
#define _DILUCULUM_MAKE_LUA_FUNCTION_HPP_

#include <Diluculum/LuaValue.hpp>
#include <Diluculum/LuaUtils.hpp>

namespace Diluculum
{
   namespace Impl
   {
      /** Issues a Lua error (by calling \c lua_error()) if a given condition
       *  is true.
       *  @param ls The Lua state on which \c lua_error() will possibly be
       *         called.
       *  @param condition The error will be raised only if this is \c true.
       *  @param function The function name as available in Lua. This is used
       *         only to give some additional information to the error message.
       *  @param message An error message that will be associated with the
       *         generated error
       */
      void MLF_IssueLuaErrorIf (lua_State* ls, bool condition,
                                const std::string& function,
                                const std::string& message);

      /** Reads the parameters passed to a function. The parameters are read
       *  from the stack of a Lua state and written to an array.
       *  @param ls The Lua state from whose stack the parameters will be read.
       *  @param params The array to which the parameters will be written.
       *         Please, ensure that it is large enough to hold all parameters.
       *  @param n The number of parameters to be read.
       */
      void MLF_ReadParameters (lua_State* ls, Diluculum::LuaValue params[], int n);

   }// namespace Impl

} // namespace Diluculum



/** Creates a wrapper \c lua_CFunction around a function taking no parameters
 *  and returning \c void.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 *  @todo This macro (and all other \c DILUCULUM_MAKE_LUA_FUNCTION* macros)
 *        raises a Lua error if the wrong number of parameters is passed from
 *        Lua. I wonder if this is the best way to go. Perhaps the ideal is to
 *        pass all absent parameters as 'nil's, so that \c FUNC can do whatever
 *        it wants (for example, "simulating" default parameters).
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_0_VOID(FUNC,NEW_FUNC,LUA_NAME)         \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 0, LUA_NAME,                               \
         "this function does not expect any parameter");                   \
      FUNC();                                                              \
      return 0;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 0;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking no parameters
 *  and returning a \c LuaValue.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_0_RET(FUNC,NEW_FUNC,LUA_NAME)          \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 0, LUA_NAME,                               \
         "this function does not expect any parameter");                   \
                                                                           \
      Diluculum::LuaValue ret = FUNC();                                    \
      Diluculum::PushLuaValue (ls, ret);                                   \
      return 1;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 1;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 1;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking one \c LuaValue
 *  parameter and returning \c void.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_1_VOID(FUNC,NEW_FUNC,LUA_NAME)         \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 1, LUA_NAME,                               \
         "this function expects exactly one parameter");                   \
                                                                           \
      Diluculum::LuaValue param;                                           \
      Diluculum::Impl::MLF_ReadParameters (ls, &param, 1);                 \
                                                                           \
      FUNC (param);                                                        \
      return 0;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 0;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking one \c LuaValue
 *  parameter and returning another \c LuaValue.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_1_RET(FUNC,NEW_FUNC,LUA_NAME)          \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 1, LUA_NAME,                               \
         "this function expects exactly one parameter");                   \
                                                                           \
      Diluculum::LuaValue param;                                           \
      Diluculum::Impl::MLF_ReadParameters (ls, &param, 1);                 \
                                                                           \
      Diluculum::LuaValue ret = FUNC (param);                              \
      Diluculum::PushLuaValue (ls, ret);                                   \
      return 1;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 1;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 1;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking two \c LuaValue
 *  parameters and returning \c void.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_2_VOID(FUNC,NEW_FUNC,LUA_NAME)         \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 2, LUA_NAME,                               \
         "this function expects exactly two parameters");                  \
                                                                           \
      Diluculum::LuaValue params[2];                                       \
      Diluculum::Impl::MLF_ReadParameters (ls, params, 2);                 \
                                                                           \
      FUNC (params[0], params[1]);                                         \
      return 0;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 0;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking two \c LuaValue
 *  parameters and returning another \c LuaValue.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_2_RET(FUNC,NEW_FUNC,LUA_NAME)          \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 2, LUA_NAME,                               \
         "this function expects exactly two parameters");                  \
                                                                           \
      Diluculum::LuaValue params[2];                                       \
      Diluculum::Impl::MLF_ReadParameters (ls, params, 2);                 \
                                                                           \
      Diluculum::LuaValue ret = FUNC (params[0], params[1]);               \
      Diluculum::PushLuaValue (ls, ret);                                   \
      return 1;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 1;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 1;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking three
 *  \c LuaValue parameters and returning \c void.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_3_VOID(FUNC,NEW_FUNC,LUA_NAME)         \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 3, LUA_NAME,                               \
         "this function expects exactly three parameters");                \
                                                                           \
      Diluculum::LuaValue params[3];                                       \
      Diluculum::Impl::MLF_ReadParameters (ls, params, 3);                 \
                                                                           \
      FUNC (params[0], params[1], params[2]);                              \
      return 0;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 0;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking three
 *  \c LuaValue parameters and returning another \c LuaValue.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_3_RET(FUNC,NEW_FUNC,LUA_NAME)          \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 3, LUA_NAME,                               \
         "this function expects exactly three parameters");                \
                                                                           \
      Diluculum::LuaValue params[3];                                       \
      Diluculum::Impl::MLF_ReadParameters (ls, params, 3);                 \
                                                                           \
      Diluculum::LuaValue ret = FUNC (params[0], params[1], params[2]);    \
      Diluculum::PushLuaValue (ls, ret);                                   \
      return 1;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 1;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 1;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking four \c LuaValue
 *  parameters and returning \c void.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_4_VOID(FUNC,NEW_FUNC,LUA_NAME)         \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 4, LUA_NAME,                               \
         "this function expects exactly four parameters");                 \
                                                                           \
      Diluculum::LuaValue params[4];                                       \
      Diluculum::Impl::MLF_ReadParameters (ls, params, 4);                 \
                                                                           \
      FUNC (params[0], params[1], params[2], params[3]);                   \
      return 0;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 0;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking four \c LuaValue
 *  parameters and returning another \c LuaValue.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_4_RET(FUNC,NEW_FUNC,LUA_NAME)          \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 4, LUA_NAME,                               \
         "this function expects exactly four parameters");                 \
                                                                           \
      Diluculum::LuaValue params[4];                                       \
      Diluculum::Impl::MLF_ReadParameters (ls, params, 4);                 \
                                                                           \
      Diluculum::LuaValue ret = FUNC (params[0], params[1], params[2],     \
                                   params[3]);                             \
      Diluculum::PushLuaValue (ls, ret);                                   \
      return 1;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 1;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 1;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking five \c LuaValue
 *  parameters and returning \c void.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_5_VOID(FUNC,NEW_FUNC,LUA_NAME)         \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 5, LUA_NAME,                               \
         "this function expects exactly five parameters");                 \
                                                                           \
      Diluculum::LuaValue params[5];                                       \
      Diluculum::Impl::MLF_ReadParameters (ls, params, 5);                 \
                                                                           \
      FUNC (params[0], params[1], params[2], params[3], params[4]);        \
      return 0;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 0;                                                            \
   }                                                                       \
}



/** Creates a wrapper \c lua_CFunction around a function taking five \c LuaValue
 *  parameters and returning another \c LuaValue.
 *  @param FUNC The function to be wrapped.
 *  @param NEW_FUNC The name to be given to the wrapper function that will be
 *         created.
 *  @param LUA_NAME A string with the function name, as it is expected to be
 *         called from Lua. This is used for error reporting.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_5_RET(FUNC,NEW_FUNC,LUA_NAME)          \
int NEW_FUNC (lua_State* ls)                                               \
{                                                                          \
   try                                                                     \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 5, LUA_NAME,                               \
         "this function expects exactly five parameters");                 \
                                                                           \
      Diluculum::LuaValue params[5];                                       \
      Diluculum::Impl::MLF_ReadParameters (ls, params, 5);                 \
                                                                           \
      Diluculum::LuaValue ret = FUNC (params[0], params[1], params[2],     \
                                      params[3], params[4]);               \
      Diluculum::PushLuaValue (ls, ret);                                   \
      return 1;                                                            \
   }                                                                       \
   catch (Diluculum::LuaError& e)                                          \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 1;                                                            \
   }                                                                       \
   catch(...)                                                              \
   {                                                                       \
      Diluculum::Impl::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");        \
      return 1;                                                            \
   }                                                                       \
}

#endif // _DILUCULUM_MAKE_LUA_FUNCTION_HPP_
